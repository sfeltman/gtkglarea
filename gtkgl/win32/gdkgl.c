/* 
 * Copyright (C) 1998 Janne Löf <jlof@mail.student.oulu.fi>
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


#include "gdkgl.h"
#include <gdk/gdkx.h>
#include <GL/gl.h>
#include <stdio.h>
#if !defined(_WIN32)
#include <GL/glx.h>
#else

/* TODO: Replace with GDK_GL_XXXX definitions, thats what they are for! -- Janne */
#define GLX_USE_GL              1       /* support GLX rendering */
#define GLX_BUFFER_SIZE         2       /* depth of the color buffer */
#define GLX_LEVEL               3       /* level in plane stacking */
#define GLX_RGBA                4       /* true if RGBA mode */
#define GLX_DOUBLEBUFFER        5       /* double buffering supported */
#define GLX_STEREO              6       /* stereo buffering supported */
#define GLX_AUX_BUFFERS         7       /* number of aux buffers */
#define GLX_RED_SIZE            8       /* number of red component bits */
#define GLX_GREEN_SIZE          9       /* number of green component bits */
#define GLX_BLUE_SIZE           10      /* number of blue component bits */
#define GLX_ALPHA_SIZE          11      /* number of alpha component bits */
#define GLX_DEPTH_SIZE          12      /* number of depth bits */
#define GLX_STENCIL_SIZE        13      /* number of stencil bits */
#define GLX_ACCUM_RED_SIZE      14      /* number of red accum bits */
#define GLX_ACCUM_GREEN_SIZE    15      /* number of green accum bits */
#define GLX_ACCUM_BLUE_SIZE     16      /* number of blue accum bits */
#define GLX_ACCUM_ALPHA_SIZE    17      /* number of alpha accum bits */

#define GLX_BAD_ATTRIB  2
#define GLX_BAD_VISUAL  4

#endif
#include <string.h>


struct _GdkGLContextPrivate {
#if !defined(_WIN32)
  Display    *xdisplay;
  GLXContext glxcontext;
#else
  HGLRC glxcontext;
  HDC   glhdc;
  HWND  glxwindow;
  gboolean  initialised;
  int  *attrList;
#endif
  guint ref_count;
};

typedef struct _GdkGLContextPrivate GdkGLContextPrivate;

GdkGLContext *gdk_gl_context_ref(GdkGLContext *context)
{
  GdkGLContextPrivate *private = (GdkGLContextPrivate*)context;

  g_return_val_if_fail(context != NULL, NULL);
  private->ref_count += 1;

  return context;
}

#if !defined(_WIN32)
GdkGLContext *gdk_gl_context_new(GdkVisual *visual)
{
  return gdk_gl_context_share_new(NULL,visual, NULL, FALSE);
}

static XVisualInfo *get_xvisualinfo(GdkVisual *visual)
{
  Display *dpy;
  XVisualInfo vinfo_template;
  XVisualInfo *vi;
  int nitems_return;

  dpy = GDK_DISPLAY();

  /* TODO: is this right way to get VisualInfo from Visual ?? */
  /* AFAIK VisualID and depth should be enough to uniquely identify visual */
  vinfo_template.visual   = GDK_VISUAL_XVISUAL(visual);
  vinfo_template.visualid = XVisualIDFromVisual(vinfo_template.visual);
  vinfo_template.depth    = visual->depth;
  vi = XGetVisualInfo(dpy, VisualIDMask|VisualDepthMask, &vinfo_template, &nitems_return);

  g_assert( vi!=0  && nitems_return==1 ); /* visualinfo needs to be unique */

  /* remember to XFree returned XVisualInfo !!! */
  return vi;
}



gint gdk_gl_query(void)
{
  return (glXQueryExtension(GDK_DISPLAY(),NULL,NULL) == True) ? TRUE : FALSE;
}

GdkVisual *gdk_gl_choose_visual(int *attrList)
{
  Display *dpy;
  XVisualInfo *vi;
  GdkVisual  *visual;

  g_return_val_if_fail(attrList != NULL, NULL);

  dpy = GDK_DISPLAY();
  /* TODO:  translate GDK_GL_ to GLX_ */
  if ((vi = glXChooseVisual(dpy,DefaultScreen(dpy), attrList)) == NULL) {
    return NULL;
  }
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

  /* TODO:  translate GDK_GL_ to GLX_ */
  if (glXGetConfig(dpy, vi, attrib, &value) == 0) {
    XFree(vi);
    return value;
  }
  XFree(vi);
  return -1;
}


GdkGLContext *gdk_gl_context_share_new(int * list,GdkVisual *visual, GdkGLContext *sharelist, gint direct)
{
  Display *dpy;
  XVisualInfo *vi;
  GLXContext glxcontext;
  GdkGLContextPrivate *private;

  g_return_val_if_fail(visual != NULL, NULL);

  dpy = GDK_DISPLAY();

  vi = get_xvisualinfo(visual);

  if (sharelist) {
    glxcontext = glXCreateContext(dpy, vi, ((GdkGLContextPrivate*)sharelist)->glxcontext, direct ? True : False);
  } else {
    glxcontext = glXCreateContext(dpy, vi, 0, direct ? True : False);
  }
  XFree(vi);
  if (glxcontext == NULL) {
    return NULL;
  }

  private = g_new(GdkGLContextPrivate, 1);
  private->xdisplay = dpy;
  private->glxcontext = glxcontext;
  private->ref_count = 1;

  return (GdkGLContext*)private;
}

void gdk_gl_context_unref(GdkGLContext *context)
{
  GdkGLContextPrivate *private = (GdkGLContextPrivate*)context;

  g_return_if_fail(context != NULL);

  if (private->ref_count > 1) {
    private->ref_count -= 1;
  } else {
    if (private->glxcontext == glXGetCurrentContext())
      glXMakeCurrent(private->xdisplay, None, NULL);
    glXDestroyContext(private->xdisplay, private->glxcontext);
    memset(context, 0, sizeof(GdkGLContextPrivate));
    g_free(context);
  }
}

gint gdk_gl_make_current(GdkDrawable *drawable, GdkGLContext *context)
{
  GdkGLContextPrivate *private = (GdkGLContextPrivate*)context;

  g_return_val_if_fail(drawable != NULL, FALSE);
  g_return_val_if_fail(context  != NULL, FALSE);

  return (glXMakeCurrent(private->xdisplay, GDK_WINDOW_XWINDOW(drawable), private->glxcontext) == True) ? TRUE : FALSE;
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


/* glpixmap stuff */

struct _GdkGLPixmapPrivate {
  Display   *xdisplay;
  GLXPixmap glxpixmap;
  GdkPixmap *front_left;
  guint     ref_count;
};

typedef struct _GdkGLPixmapPrivate GdkGLPixmapPrivate;


GdkGLPixmap *gdk_gl_pixmap_new(GdkVisual *visual, GdkPixmap *pixmap)
{
  Display *dpy;
  XVisualInfo *vi;
  Pixmap xpixmap;
  GdkGLPixmapPrivate *private;
  GLXPixmap glxpixmap;
  gint depth;

  g_return_val_if_fail(pixmap != NULL, NULL);
  g_return_val_if_fail(visual != NULL, NULL);
  g_return_val_if_fail(gdk_window_get_type(pixmap) == GDK_WINDOW_PIXMAP, NULL);

  gdk_window_get_geometry(pixmap, 0,0,0,0, &depth);
  g_return_val_if_fail(gdk_gl_get_config(visual, GDK_GL_BUFFER_SIZE) == depth, NULL);

  dpy = GDK_DISPLAY();

  vi = get_xvisualinfo(visual);
  xpixmap = ((GdkPixmapPrivate*)pixmap)->xwindow;
  glxpixmap = glXCreateGLXPixmap(dpy, vi, xpixmap);
  XFree(vi);

  g_return_val_if_fail(glxpixmap != None, NULL);

  private = g_new(GdkGLPixmapPrivate, 1);
  private->xdisplay  = dpy;
  private->glxpixmap = glxpixmap;
  private->front_left = gdk_pixmap_ref(pixmap);
  private->ref_count = 1;

  return (GdkGLPixmap*)private;
}


GdkGLPixmap *gdk_gl_pixmap_ref(GdkGLPixmap *glpixmap)
{
  GdkGLPixmapPrivate *private = (GdkGLPixmapPrivate*)glpixmap;

  g_return_val_if_fail(glpixmap != NULL, NULL);
  private->ref_count += 1;

  return glpixmap;
}

void gdk_gl_pixmap_unref(GdkGLPixmap *glpixmap)
{
  GdkGLPixmapPrivate *private = (GdkGLPixmapPrivate*)glpixmap;

  g_return_if_fail(glpixmap != NULL);

  if (private->ref_count > 1) {
    private->ref_count -= 1;
  } else {
    glXDestroyGLXPixmap(private->xdisplay, private->glxpixmap);
    glXWaitGL();
    gdk_pixmap_unref(private->front_left);
    glXWaitX();
    memset(glpixmap, 0, sizeof(GdkGLPixmapPrivate));
    g_free(glpixmap);
  }
}

gint gdk_gl_pixmap_make_current(GdkGLPixmap *glpixmap, GdkGLContext *context)
{
  Display  *dpy;
  GLXPixmap glxpixmap;
  GLXContext glxcontext;

  g_return_val_if_fail(glpixmap != NULL, FALSE);
  g_return_val_if_fail(context  != NULL, FALSE);

  dpy        = ((GdkGLContextPrivate*)context)->xdisplay;
  glxpixmap  = ((GdkGLPixmapPrivate*)glpixmap)->glxpixmap;
  glxcontext = ((GdkGLContextPrivate*)context)->glxcontext;

  return (glXMakeCurrent(dpy, glxpixmap, glxcontext) == True) ? TRUE : FALSE;
}

/* fonts */
void gdk_gl_use_gdk_font(GdkFont *font, int first, int count, int list_base)
{
  g_return_if_fail(font != NULL);
  glXUseXFont(gdk_font_id(font), first, count, list_base);
}
#else

HGLRC glWinChooseVisual(HWND hwnd, int *attribList)
{
	/* Ripped off from glut's win32_x11.c. Sorry.
	 */
  int *p = attribList;
  int pf;
  PIXELFORMATDESCRIPTOR pfd;
  PIXELFORMATDESCRIPTOR *match = NULL;
  int stereo = 0;
  HDC hdc;
  HGLRC hRC=NULL;
	hdc = GetDC(hwnd);
  /* Avoid seg-faults. */
  if (!p)
    return NULL;

  memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
  pfd.nSize = (sizeof(PIXELFORMATDESCRIPTOR));
  pfd.nVersion = 1;

  /* Defaults. */
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
  pfd.iPixelType = PFD_TYPE_COLORINDEX;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 0;

  while (*p) {
    switch (*p) {
    case GLX_USE_GL:
      pfd.dwFlags |= PFD_SUPPORT_OPENGL;
      break;
    case GLX_BUFFER_SIZE:
      pfd.cColorBits = *(++p);
      break;
    case GLX_LEVEL:
      /* the bReserved flag of the pfd contains the
         overlay/underlay info. */
      pfd.bReserved = *(++p);
      break;
    case GLX_RGBA:
      pfd.iPixelType = PFD_TYPE_RGBA;
      break;
    case GLX_DOUBLEBUFFER:
      pfd.dwFlags |= PFD_DOUBLEBUFFER;
      break;
    case GLX_STEREO:
      stereo = 1;
      pfd.dwFlags |= PFD_STEREO;
      break;
    case GLX_AUX_BUFFERS:
      pfd.cAuxBuffers = *(++p);
      break;
    case GLX_RED_SIZE:
      pfd.cRedBits = 8; /* Try to get the maximum. */
      ++p;
      break;
    case GLX_GREEN_SIZE:
      pfd.cGreenBits = 8;
      ++p;
      break;
    case GLX_BLUE_SIZE:
      pfd.cBlueBits = 8;
      ++p;
      break;
    case GLX_ALPHA_SIZE:
      pfd.cAlphaBits = 8;
      ++p;
      break;
    case GLX_DEPTH_SIZE:
      pfd.cDepthBits = 32;
      ++p;
      break;
    case GLX_STENCIL_SIZE:
      pfd.cStencilBits = *(++p);
      break;
    case GLX_ACCUM_RED_SIZE:
    case GLX_ACCUM_GREEN_SIZE:
    case GLX_ACCUM_BLUE_SIZE:
    case GLX_ACCUM_ALPHA_SIZE:
      /* I believe that WGL only used the cAccumRedBits,
	 cAccumBlueBits, cAccumGreenBits, and cAccumAlphaBits fields
	 when returning info about the accumulation buffer precision.
	 Only cAccumBits is used for requesting an accumulation
	 buffer. */
      pfd.cAccumBits = 1;
      ++p;
      break;
    }
    ++p;
  }

  /* Let Win32 choose one for us. */
  pf = ChoosePixelFormat(hdc, &pfd);
  if (pf > 0)
  {
    match = (PIXELFORMATDESCRIPTOR *) malloc(sizeof(PIXELFORMATDESCRIPTOR));
    DescribePixelFormat(hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), match);

    /* ChoosePixelFormat is dumb in that it will return a pixel
       format that doesn't have stereo even if it was requested
       so we need to make sure that if stereo was selected, we
       got it. */
    if (stereo)
    {
      if (!(match->dwFlags & PFD_STEREO))
      {
        free(match);
	return NULL;
      }
    }
    /* XXX Brad's Matrix Millenium II has problems creating
       color index windows in 24-bit mode (lead to GDI crash)
       and 32-bit mode (lead to black window).  The cColorBits
       filed of the PIXELFORMATDESCRIPTOR returned claims to
       have 24 and 32 bits respectively of color indices. 2^24
       and 2^32 are ridiculously huge writable colormaps.
       Assume that if we get back a color index
       PIXELFORMATDESCRIPTOR with 24 or more bits, the
       PIXELFORMATDESCRIPTOR doesn't really work and skip it.
       -mjk */
    if (match->iPixelType == PFD_TYPE_COLORINDEX
      && match->cColorBits >= 24)
	{
      		free(match);
      		return NULL;
    	}
	SetPixelFormat(hdc,pf,match);
	hRC=wglCreateContext(hdc);
  }
  return hRC;
}

GdkGLContext *gdk_gl_context_share_new(GdkVisual *visual, GdkGLContext *sharelist, gint direct, int *list)
{
  GdkGLContextPrivate *private;
  private = g_new(GdkGLContextPrivate, 1);
  private->glxwindow = NULL;
  private->glxcontext = NULL;
  private->attrList = list;
  private->initialised = FALSE;
  private->glhdc = NULL;
  private->ref_count = 1;

  return (GdkGLContext*)private;
}

gint gdk_gl_query(void)
{
	return TRUE;
}

GdkVisual *gdk_gl_choose_visual(int *attrList)
{
  g_return_val_if_fail(attrList != NULL, NULL);

	return gdk_visual_get_best();
}

void gdk_gl_context_unref(GdkGLContext *context)
{
GdkGLContextPrivate *private = (GdkGLContextPrivate*)context;

  g_return_if_fail(context != NULL);

  	if (private->ref_count > 1)
	{
    		private->ref_count -= 1;
  	}
	else
	{
	HDC hdc ;
		hdc = GetDC(private->glxwindow);
    		if (private->glxcontext == wglGetCurrentContext())
      			wglMakeCurrent(hdc, private->glxcontext);
    		wglDeleteContext(private->glxcontext);
    		memset(context, 0, sizeof(GdkGLContextPrivate));
    		g_free(context);

		ReleaseDC (private->glxwindow, hdc);
  	}
}

gint gdk_gl_make_current(GdkDrawable *drawable, GdkGLContext *context)
{
GdkGLContextPrivate *private = (GdkGLContextPrivate*)context;
GdkWindow *window = &(((GdkWindowPrivate*)drawable)->window);
HDC hdc ;
HWND hwnd;
HGLRC currentContext = wglGetCurrentContext();
HDC currentDc = wglGetCurrentDC();

/*	printf("Found context\n");/**/
	if(!private->initialised )
	{	/* Here is where lazy evaluation takes place... */
/*		printf("Found an unitiliased context\n");/**/
		hwnd = GDK_WINDOW_XWINDOW(window);
		private->glxcontext = glWinChooseVisual(hwnd, private->attrList);
		private->glhdc = GetDC(hwnd);
		private->glxwindow = hwnd;
	}
	g_return_val_if_fail(drawable != NULL, FALSE);
	g_return_val_if_fail(context  != NULL, FALSE);

	if (	!private->initialised ||
		currentContext != private->glxcontext ||
		currentDc != private->glhdc	)
			wglMakeCurrent(private->glhdc, private->glxcontext);

	private->initialised = TRUE;
	return TRUE;
}

void gdk_gl_swap_buffers(GdkDrawable *drawable)
{
GdkWindow *window = &(((GdkWindowPrivate*)drawable)->window);
HDC hdc ;
HWND hwnd;
	g_return_if_fail(drawable != NULL);

	hwnd = GDK_WINDOW_XWINDOW(window);
	if ((hdc = GetDC (hwnd)) == NULL)
	{
		g_warning ("gdk_gl_swap_buffers: GetDC failed");
		return;
	}

  	SwapBuffers(hdc);
}

void gdk_gl_wait_gdk(void)
{
}

void gdk_gl_wait_gl (void)
{
}

#endif
