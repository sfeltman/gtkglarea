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


#include <windows.h>

#ifndef WIN32
 #define WIN32
#endif

#include <GL/gl.h>

#include "gdkgl.h"
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

GdkVisual *gdk_gl_choose_visual(int *attrlist)
{
  g_warning("not implemented");
  return NULL;
}

int gdk_gl_get_config(GdkVisual *visual, int attrib)
{
  g_warning("not implemented");
  return 0;
}

GdkGLContext *gdk_gl_context_new(GdkVisual *visual)
{
  g_warning("not implemented");
  return NULL;
}

GdkGLContext *gdk_gl_context_share_new(GdkVisual *visual, GdkGLContext *sharelist, gint direct)
{
  g_warning("not implemented");
  return NULL;
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

  g_return_val_if_fail(context != NULL, NULL);
  private->ref_count += 1;

  return context;
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
      if (private->hglrc == wglGetCurrentContext())
	wglMakeCurrent(NULL, NULL);

      wglDeleteContext(private->hglrc);
      ReleaseDC(private->hwnd, private->hdc);

      g_free(private);
    }
}



gint gdk_gl_make_current(GdkDrawable *drawable, GdkGLContext *context)
{
  GdkGLContextPrivate *private = (GdkGLContextPrivate*)context;

  g_return_val_if_fail(drawable != NULL, FALSE);
  g_return_val_if_fail(context  != NULL, FALSE);

  if(!private->initialised )
    {
      /* Here is where lazy evaluation takes place... */
      int pf;
      private->hglrc = (HWND)GDK_DRAWABLE_XID(drawable);
      private->hdc   = GetDC(private->hglrc);
      pf = ChoosePixelFormat(private->hdc, &private->pfd);
      if (pf != 0)
	{
	  SetPixelFormat(private->hdc, pf, &private->pfd);
	  private->hglrc = wglCreateContext(private->hdc);
	}
      if (private->share)
	{
	  if (private->share->hglrc)
	    wglShareLists(private->share->hglrc, private->hglrc);
	  gdk_gl_context_unref((GdkGLContext*)private->share);
	}
      private->initialised = TRUE;
    }

  g_return_val_if_fail(private->hdc    != NULL, FALSE);
  g_return_val_if_fail(private->hglrc  != NULL, FALSE);

  wglMakeCurrent(private->hdc, private->hglrc);
  
  return TRUE;
}


void gdk_gl_swap_buffers(GdkDrawable *drawable)
{
  HDC   hdc;
  HWND  hwnd;

  g_return_if_fail(drawable != NULL);

  hwnd = (HWND)GDK_DRAWABLE_XID(drawable);
  hdc  = GetDC(hwnd);
  if (hdc  == NULL)
    {
      g_warning("gdk_gl_swap_buffers: GetDC failed");
      return;
    }
  SwapBuffers(hdc);
  ReleaseDC(hwnd,hdc);
}


void gdk_gl_wait_gdk(void)
{
}

void gdk_gl_wait_gl (void)
{
}

/* pixmap routines not implemented */


void gdk_gl_use_gdk_font(GdkFont *font, int first, int count, int list_base)
{
  g_warning("not implemented");
}

