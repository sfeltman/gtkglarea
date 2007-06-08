/* 
 * Copyright (C) 1998 Janne Löf <jlof@mail.student.oulu.fi>
 * Copyright (C) 2000 Marc Flerackers <mflerackers@androme.be>
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

#include "gdkglpixmap-win32.h"

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

  glFinish ();
  SelectObject ( pixmap->hdc, pixmap->hbitmap );
  gdk_pixmap_unref ( pixmap->pixmap );

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

GdkGLPixmap *gdk_gl_pixmap_new(GdkVisual *visual, GdkPixmap *pixmap)
{
  GdkGLPixmap *glpixmap;

  g_return_val_if_fail(GDK_IS_VISUAL(visual), NULL);
  g_return_val_if_fail(GDK_IS_PIXMAP(pixmap), NULL);

  glpixmap = g_object_new(GDK_TYPE_GL_PIXMAP, NULL);
  if (!glpixmap) return NULL;

  glpixmap->initialised = FALSE;
  glpixmap->hdc = NULL;
  glpixmap->hbitmap = NULL;
  glpixmap->pixmap = gdk_pixmap_ref ( pixmap );

  return glpixmap;
}

gint gdk_gl_pixmap_make_current(GdkGLPixmap *glpixmap, GdkGLContext *context)
{
  g_return_val_if_fail (GDK_IS_GL_PIXMAP(glpixmap), FALSE );
  g_return_val_if_fail (GDK_IS_GL_CONTEXT(context), FALSE );

  if ( !context->initialised )
  {
    int pf;

    context->hdc = CreateCompatibleDC ( NULL );
    glpixmap->hdc = context->hdc;
    glpixmap->hbitmap = SelectObject ( context->hdc, (HBITMAP) gdk_win32_drawable_get_handle ( glpixmap->pixmap ) );

    pf = ChoosePixelFormat ( context->hdc, &context->pfd );

    if ( pf != 0 )
	{
	  SetPixelFormat ( context->hdc, pf, &context->pfd );
	  context->hglrc = wglCreateContext ( context->hdc );
	}

    if (context->share)
	{
	  if ( context->share->hglrc )
	    wglShareLists ( context->share->hglrc, context->hglrc );
	  gdk_gl_context_unref ( (GdkGLContext*)context->share );
	}

    context->initialised = TRUE;
  }

  g_return_val_if_fail ( context->hdc    != NULL, FALSE );
  g_return_val_if_fail ( context->hglrc  != NULL, FALSE );

  wglMakeCurrent ( context->hdc, context->hglrc );
  
  return TRUE;
}

#define __GDK_GL_PIXMAP_WIN32_C__
