/* 
 * Copyright (C) 1998 Janne Löf <jlof@mail.student.oulu.fi>
 * Copyright (C) 2000 Marc Flerackers <mflerackers@androme.be>
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


#include <windows.h>

#ifndef WIN32
 #define WIN32
#endif

#include "gdkgl.h"
#include <GL/gl.h>
#include <gdk/gdkprivate.h>
#include <gdk/win32/gdkwin32.h>

static void fill_pfd(PIXELFORMATDESCRIPTOR *pfd, int *attriblist)
{
  /*
   * Ripped from glut's win32_x11.c
   */
  
  int *p = attriblist;

  memset(pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
  pfd->nSize = (sizeof(PIXELFORMATDESCRIPTOR));
  pfd->nVersion = 1;
  
  /* Defaults. */
  pfd->dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
  pfd->iPixelType = PFD_TYPE_COLORINDEX;
  pfd->cColorBits = 32;
  pfd->cDepthBits = 0;
  
  while (*p) {
    switch (*p) {
    case GDK_GL_USE_GL:
      pfd->dwFlags |= PFD_SUPPORT_OPENGL;
      break;
    case GDK_GL_BUFFER_SIZE:
      pfd->cColorBits = *(++p);
      break;
    case GDK_GL_LEVEL:
      /* the bReserved flag of the pfd contains the
         overlay/underlay info. */
      pfd->bReserved = *(++p);
      break;
    case GDK_GL_RGBA:
      pfd->iPixelType = PFD_TYPE_RGBA;
      break;
    case GDK_GL_DOUBLEBUFFER:
      pfd->dwFlags |= PFD_DOUBLEBUFFER;
      break;
    case GDK_GL_STEREO:
      pfd->dwFlags |= PFD_STEREO;
      break;
    case GDK_GL_AUX_BUFFERS:
      pfd->cAuxBuffers = *(++p);
      break;
    case GDK_GL_RED_SIZE:
      pfd->cRedBits = 8; /* Try to get the maximum. */
      ++p;
      break;
    case GDK_GL_GREEN_SIZE:
      pfd->cGreenBits = 8;
      ++p;
      break;
    case GDK_GL_BLUE_SIZE:
      pfd->cBlueBits = 8;
      ++p;
      break;
    case GDK_GL_ALPHA_SIZE:
      pfd->cAlphaBits = 8;
      ++p;
      break;
    case GDK_GL_DEPTH_SIZE:
      pfd->cDepthBits = 32;
      ++p;
      break;
    case GDK_GL_STENCIL_SIZE:
      pfd->cStencilBits = *(++p);
      break;
    case GDK_GL_ACCUM_RED_SIZE:
    case GDK_GL_ACCUM_GREEN_SIZE:
    case GDK_GL_ACCUM_BLUE_SIZE:
    case GDK_GL_ACCUM_ALPHA_SIZE:
      /* I believe that WGL only used the cAccumRedBits,
	 cAccumBlueBits, cAccumGreenBits, and cAccumAlphaBits fields
	 when returning info about the accumulation buffer precision.
	 Only cAccumBits is used for requesting an accumulation
	 buffer. */
      pfd->cAccumBits = 1;
      ++p;
      break;
    }
    ++p;
  }
}

struct _GdkGLContextPrivate {
  gboolean  initialised;
  HGLRC     hglrc;
  HDC       hdc;
  HWND      hwnd;
  struct _GdkGLContextPrivate *share;
  PIXELFORMATDESCRIPTOR pfd;
  guint ref_count;
};

typedef struct _GdkGLContextPrivate GdkGLContextPrivate;

gint gdk_gl_query(void)
{
  return TRUE;
}

gchar *gdk_gl_get_info()
{
  return g_strdup_printf("VENDOR     : %s\n"
			 "VERSION    : %s\n"
			 "EXTENSIONS : %s\n",
			 glGetString ( GL_VENDOR ),
			 glGetString ( GL_VERSION ),
			 glGetString ( GL_EXTENSIONS ));
}

GdkVisual *gdk_gl_choose_visual(int *attrlist)
{
  return gdk_visual_get_system ();
}

int gdk_gl_get_config(GdkVisual *visual, int attrib)
{
  g_warning ( "not implemented" );
  return 0;
}

GdkGLContext *gdk_gl_context_new(GdkVisual *visual)
{
  return gdk_gl_context_share_new ( visual, NULL, FALSE );
}

GdkGLContext *gdk_gl_context_share_new(GdkVisual *visual, GdkGLContext *sharelist, gint direct)
{
  GdkGLContextPrivate *private;
  
  g_return_val_if_fail ( visual != NULL, NULL );
  
  private = g_new ( GdkGLContextPrivate, 1 );
  private->initialised = FALSE;
  private->hglrc   = NULL;
  private->hdc     = NULL;
  private->hwnd    = NULL;
  private->share   = sharelist ? (GdkGLContextPrivate*)gdk_gl_context_ref(sharelist) : NULL;


  memset ( &(private->pfd), 0, sizeof(PIXELFORMATDESCRIPTOR) );

  /* if direct is TRUE, we create a context which renders to the screen, otherwise
     we create one to render to an offscreen bitmap */
  if ( direct )
  {
    private->pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    private->pfd.nVersion = 1;
    private->pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    private->pfd.iPixelType = PFD_TYPE_RGBA;
	private->pfd.cColorBits = 24;
	private->pfd.cDepthBits = 32;
	private->pfd.iLayerType = PFD_MAIN_PLANE;
  } 
  else
  {
    private->pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    private->pfd.nVersion = 1;
    private->pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_BITMAP | PFD_SUPPORT_GDI;
    private->pfd.iPixelType = PFD_TYPE_RGBA;
	private->pfd.cColorBits = 24;
	private->pfd.cDepthBits = 32;
	private->pfd.iLayerType = PFD_MAIN_PLANE;
  }
  
  private->ref_count = 1;
  
  return (GdkGLContext*)private;
}

GdkGLContext *gdk_gl_context_attrlist_share_new(int *attrlist, GdkGLContext *sharelist, gint direct)
{
  GdkGLContextPrivate *private;

  g_return_val_if_fail(attrlist != NULL, NULL);

  private = g_new(GdkGLContextPrivate, 1);
  private->initialised = FALSE;
  private->hglrc    = NULL;
  private->hdc      = NULL;
  private->hwnd     = NULL;
  private->share    = sharelist ? (GdkGLContextPrivate*)gdk_gl_context_ref(sharelist) : NULL;
  fill_pfd(&private->pfd, attrlist);
  private->ref_count = 1;

  return (GdkGLContext*)private;
}

GdkGLContext *gdk_gl_context_ref(GdkGLContext *context)
{
  GdkGLContextPrivate *private = (GdkGLContextPrivate*)context;

  g_return_val_if_fail ( context != NULL, NULL );
  private->ref_count += 1;

  return context;
}

void gdk_gl_context_unref(GdkGLContext *context)
{
  GdkGLContextPrivate *private = (GdkGLContextPrivate*)context;
  
  g_return_if_fail ( context != NULL );
  
  if ( private->ref_count > 1 )
  {
     private->ref_count -= 1;
  }
  else
  {
    if ( private->hglrc == wglGetCurrentContext () )
      wglMakeCurrent ( NULL, NULL );

    wglDeleteContext ( private->hglrc );
	
	if ( private->hwnd )
      ReleaseDC ( private->hwnd, private->hdc );
    else
      DeleteDC ( private->hdc );

    g_free ( private );
  }
}

gint gdk_gl_make_current(GdkDrawable *drawable, GdkGLContext *context)
{
  GdkGLContextPrivate *private = (GdkGLContextPrivate*)context;

  g_return_val_if_fail ( drawable != NULL, FALSE );
  g_return_val_if_fail ( context  != NULL, FALSE );

  if ( !private->initialised )
  {
    int pf;
    HWND hwnd = (HWND)GDK_DRAWABLE_XID(drawable);

    private->hdc = GetDC ( hwnd );

    pf = ChoosePixelFormat ( private->hdc, &private->pfd );

    if ( pf != 0 )
	{
	  SetPixelFormat ( private->hdc, pf, &private->pfd );
	  private->hglrc = wglCreateContext ( private->hdc );
	}

    if (private->share)
	{
	  if ( private->share->hglrc )
	    wglShareLists ( private->share->hglrc, private->hglrc );
	  gdk_gl_context_unref ( (GdkGLContext*)private->share );
	}

    private->initialised = TRUE;
  }

  g_return_val_if_fail ( private->hdc    != NULL, FALSE );
  g_return_val_if_fail ( private->hglrc  != NULL, FALSE );

  wglMakeCurrent ( private->hdc, private->hglrc );
  
  return TRUE;
}


void gdk_gl_swap_buffers(GdkDrawable *drawable)
{
  HDC   hdc;
  HWND  hwnd;

  g_return_if_fail ( drawable != NULL );

  hwnd = (HWND)GDK_DRAWABLE_XID(drawable);
  hdc  = GetDC ( hwnd );
  if ( hdc  == NULL )
  {
     g_warning ( "gdk_gl_swap_buffers: GetDC failed" );
     return;
  }
  SwapBuffers ( hdc );
  ReleaseDC ( hwnd, hdc );
}


void gdk_gl_wait_gdk(void)
{
	GdiFlush ();
}

void gdk_gl_wait_gl (void)
{
	glFinish ();
}

/* glpixmap stuff */

struct _GdkGLPixmapPrivate {
  gboolean  initialised;
  HDC       hdc;
  HBITMAP   hbitmap;
  GdkPixmap *pixmap;
  guint     ref_count;
};

typedef struct _GdkGLPixmapPrivate GdkGLPixmapPrivate;

GdkGLPixmap *gdk_gl_pixmap_new(GdkVisual *visual, GdkPixmap *pixmap)
{
  GdkGLPixmapPrivate *private;

  private = g_new ( GdkGLPixmapPrivate, 1 );
  private->initialised = FALSE;
  private->hdc = NULL;
  private->hbitmap = NULL;
  private->pixmap = gdk_pixmap_ref ( pixmap );
  private->ref_count = 1;

  return (GdkGLPixmap*)private;
}

GdkGLPixmap *gdk_gl_pixmap_ref(GdkGLPixmap *glpixmap)
{
  GdkGLPixmapPrivate *private = (GdkGLPixmapPrivate*)glpixmap;

  g_return_val_if_fail ( glpixmap != NULL, NULL );
  private->ref_count += 1;

  return glpixmap;
}

void gdk_gl_pixmap_unref(GdkGLPixmap *glpixmap)
{
  GdkGLPixmapPrivate *private = (GdkGLPixmapPrivate*)glpixmap;

  g_return_if_fail ( glpixmap != NULL );

  if ( private->ref_count > 1 )
  {
    private->ref_count -= 1;
  }
  else
  {
    glFinish ();
	SelectObject ( private->hdc, private->hbitmap );
    gdk_pixmap_unref ( private->pixmap );
    memset ( glpixmap, 0, sizeof(GdkGLPixmapPrivate) );
    g_free ( glpixmap );
  }
}

gint gdk_gl_pixmap_make_current(GdkGLPixmap *glpixmap, GdkGLContext *context)
{
  GdkGLContextPrivate *private = (GdkGLContextPrivate*)context;
  GdkGLPixmapPrivate *pixmap_private = (GdkGLPixmapPrivate*)glpixmap;

  g_return_val_if_fail ( pixmap_private != NULL, FALSE );
  g_return_val_if_fail ( context  != NULL, FALSE );

  if ( !private->initialised )
  {
    int pf;

    private->hdc = CreateCompatibleDC ( NULL );
	pixmap_private->hdc = private->hdc;
    pixmap_private->hbitmap = SelectObject ( private->hdc, (HBITMAP)GDK_DRAWABLE_XID (pixmap_private->pixmap) );

    pf = ChoosePixelFormat ( private->hdc, &private->pfd );

    if ( pf != 0 )
	{
	  SetPixelFormat ( private->hdc, pf, &private->pfd );
	  private->hglrc = wglCreateContext ( private->hdc );
	}

    if (private->share)
	{
	  if ( private->share->hglrc )
	    wglShareLists ( private->share->hglrc, private->hglrc );
	  gdk_gl_context_unref ( (GdkGLContext*)private->share );
	}

    private->initialised = TRUE;
  }

  g_return_val_if_fail ( private->hdc    != NULL, FALSE );
  g_return_val_if_fail ( private->hglrc  != NULL, FALSE );

  wglMakeCurrent ( private->hdc, private->hglrc );
  
  return TRUE;
}

/* fonts */
void gdk_gl_use_gdk_font(GdkFont *font, int first, int count, int list_base)
{
  HDC dc = CreateCompatibleDC ( NULL );
  HFONT old_font = SelectObject ( dc, GDK_FONT_XFONT ( font ) );

  wglUseFontBitmaps ( dc, first, count, list_base );

  SelectObject ( dc, old_font );
  DeleteDC ( dc );
}

