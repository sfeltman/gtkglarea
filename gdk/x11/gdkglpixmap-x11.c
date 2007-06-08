/* 
 * Copyright (C) 1998 Janne Löf <jlof@mail.student.oulu.fi>
 * Copyright (C) 2007 C.J. Adams-Collier <cjac@colliertech.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "gdkglpixmap-x11.h"

GType
gdk_gl_pixmap_get_type (void)
{
  static GType object_type = 0;

  if (!object_type)
    {
      static const GTypeInfo object_info =
      {
        sizeof (GdkGLPixmapClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gdk_gl_pixmap_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GdkGLPixmap),
        0,              /* n_preallocs */
        (GInstanceInitFunc) NULL,
      };
      
      object_type = g_type_register_static (G_TYPE_OBJECT,
                                            "GdkGLPixmap",
                                            &object_info, 0);
    }
  return object_type;
}

static void
gdk_gl_pixmap_finalize(GObject *object)
{
  GdkGLPixmap *pixmap;

  pixmap = GDK_GL_PIXMAP(object);

  if (pixmap->glxpixmap != None) {
    glXDestroyGLXPixmap(pixmap->xdisplay, pixmap->glxpixmap);
    glXWaitGL();
  }
  pixmap->glxpixmap = None;
  if (pixmap->front_left) {
    gdk_pixmap_unref(pixmap->front_left);
    glXWaitX();
  }
  pixmap->front_left = NULL;

  (* glcontext_parent_class->finalize)(object);
}

static void
gdk_gl_pixmap_class_init(GdkGLPixmapClass *class)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS(class);
  glpixmap_parent_class = g_type_class_peek_parent(class);

  gobject_class->finalize = gdk_gl_pixmap_finalize;
}

GdkGLPixmap *
gdk_gl_pixmap_new(GdkVisual *visual, GdkPixmap *pixmap)
{
  Display *dpy;
  XVisualInfo *vi;
  Pixmap xpixmap;
  GdkGLPixmap *glpixmap;
  GLXPixmap glxpixmap;
  Window root_return;
  unsigned int x_ret, y_ret, w_ret, h_ret, bw_ret, depth_ret;

  g_return_val_if_fail(GDK_IS_VISUAL(visual), NULL);
  g_return_val_if_fail(GDK_IS_PIXMAP(pixmap), NULL);

  glpixmap = g_object_new(GDK_TYPE_GL_PIXMAP, NULL);
  if (!glpixmap) return NULL;

  dpy = GDK_DISPLAY();
  xpixmap = (Pixmap)GDK_DRAWABLE_XID(pixmap);
  
  g_return_val_if_fail(XGetGeometry(dpy, xpixmap, &root_return,
				    &x_ret, &y_ret, &w_ret, &h_ret, &bw_ret, &depth_ret), NULL);

  g_return_val_if_fail((gdk_gl_get_config(visual, GDK_GL_RED_SIZE) +
			gdk_gl_get_config(visual, GDK_GL_GREEN_SIZE) +
			gdk_gl_get_config(visual, GDK_GL_BLUE_SIZE)) == depth_ret, NULL);

  vi = get_xvisualinfo(visual);
  glxpixmap = glXCreateGLXPixmap(dpy, vi, xpixmap);
  XFree(vi);

  g_return_val_if_fail(glxpixmap != None, NULL);

  glpixmap->xdisplay   = dpy;
  glpixmap->glxpixmap  = glxpixmap;
  glpixmap->front_left = gdk_pixmap_ref(pixmap);

  return glpixmap;
}

/**
 * gdk_gl_pixmap_make_current:
 * @glpixmap: a #GdkGLPixmap.
 * @context: the #GdkGLContext to make this glpixmap current for
 * 
 * Makes the given #GdkGLPixmap ready for drawing
 **/
gint gdk_gl_pixmap_make_current(GdkGLPixmap *glpixmap, GdkGLContext *context)
{
  Display  *dpy;
  GLXPixmap glxpixmap;
  GLXContext glxcontext;

  g_return_val_if_fail(GDK_IS_GL_PIXMAP(glpixmap), FALSE);
  g_return_val_if_fail(GDK_IS_GL_CONTEXT(context), FALSE);

  dpy        = context->xdisplay;
  glxpixmap  = glpixmap->glxpixmap;
  glxcontext = context->glxcontext;

  return (glXMakeCurrent(dpy, glxpixmap, glxcontext) == True) ? TRUE : FALSE;
}

#define __GDK_GL_PIXMAP_X11_C__
