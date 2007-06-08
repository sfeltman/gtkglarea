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

#include "gdkgl-x11.h"
#include "gdkglcontext-x11.h"

static XVisualInfo *get_xvisualinfo(GdkVisual *visual)
{
  Display *dpy;
  XVisualInfo vinfo_template;
  XVisualInfo *vi;
  int nitems_return;

  dpy = GDK_DISPLAY();

  /* 'GLX uses VisualInfo records because they uniquely identify
   * a (VisualID,screen,depth) tuple.'
   */
  vinfo_template.visual   = GDK_VISUAL_XVISUAL(visual);
  vinfo_template.visualid = XVisualIDFromVisual(vinfo_template.visual);
  vinfo_template.depth    = visual->depth;
  vinfo_template.screen   = DefaultScreen(dpy);
  vi = XGetVisualInfo(dpy, VisualIDMask|VisualDepthMask|VisualScreenMask,
		      &vinfo_template, &nitems_return);

  g_assert( vi!=0  && nitems_return==1 ); /* visualinfo needs to be unique */

  /* remember to XFree returned XVisualInfo !!! */
  return vi;
}

gint gdk_gl_query(void)
{
  return (glXQueryExtension(GDK_DISPLAY(),NULL,NULL) == True) ? TRUE : FALSE;
}

gchar *gdk_gl_get_info()
{
  return g_strdup_printf("VENDOR     : %s\n"
			 "VERSION    : %s\n"
			 "EXTENSIONS : %s\n",
			 glXGetClientString(GDK_DISPLAY(), GLX_VENDOR),
			 glXGetClientString(GDK_DISPLAY(), GLX_VERSION),
			 glXGetClientString(GDK_DISPLAY(), GLX_EXTENSIONS));
}

GdkVisual *gdk_gl_choose_visual(int *attrlist)
{
  Display *dpy;
  XVisualInfo *vi;
  GdkVisual  *visual;

  g_return_val_if_fail(attrlist != NULL, NULL);

  dpy = GDK_DISPLAY();
  if ((vi = glXChooseVisual(dpy,DefaultScreen(dpy), attrlist)) == NULL)
    return NULL;
  
  visual = gdkx_visual_get(vi->visualid);
  XFree(vi);
  return visual;
}

int gdk_gl_get_config(GdkVisual *visual, int attrib)
{
  Display *dpy;
  XVisualInfo *vi;
  int value;
  
  g_return_val_if_fail(visual != NULL, -1);

  dpy = GDK_DISPLAY();
 
  vi = get_xvisualinfo(visual);

  if (glXGetConfig(dpy, vi, attrib, &value) == 0)
    {
      XFree(vi);
      return value;
    }
  XFree(vi);
  return -1;
}

gint gdk_gl_make_current(GdkDrawable *drawable, GdkGLContext *context)
{

  g_return_val_if_fail(drawable != NULL, FALSE);
  g_return_val_if_fail(context  != NULL, FALSE);

  return (glXMakeCurrent(context->xdisplay, GDK_WINDOW_XWINDOW(drawable),
			 context->glxcontext) == True) ? TRUE : FALSE;

#if 0
  if (context->glxcontext != None && context->glxcontext == glXGetCurrentContext())
    {
      glFlush();
      return TRUE;
    }
  else
    {
      return (glXMakeCurrent(context->xdisplay, GDK_WINDOW_XWINDOW(drawable), context->glxcontext) == True) ? TRUE : FALSE;
    }
#endif
}

void gdk_gl_swap_buffers(GdkDrawable *drawable)
{
  g_return_if_fail(drawable != NULL);

  glXSwapBuffers(GDK_WINDOW_XDISPLAY(drawable), GDK_WINDOW_XWINDOW(drawable));
}

void gdk_gl_wait_gdk(void)
{
  glXWaitX();
}

void gdk_gl_wait_gl (void)
{
  glXWaitGL();
}

#define __GDK_GL_X11_C__
