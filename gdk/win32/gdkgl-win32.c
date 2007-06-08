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

#include "gdkgl-win32.h"
#include "gdkglcontext-win32.h"

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
  pfd->cAccumBits = 0;
  
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
      pfd->cAccumBits += *(++p);
		break;
    }
    ++p;
  }
}

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

gint gdk_gl_make_current(GdkDrawable *drawable, GdkGLContext *context)
{

  g_return_val_if_fail (GDK_IS_DRAWABLE(drawable), FALSE );
  g_return_val_if_fail (GDK_IS_GL_CONTEXT(context), FALSE );

  if ( !context->initialised )
  {
    int pf;
    HWND hwnd = (HWND) gdk_win32_drawable_get_handle ( drawable );

    context->hdc = GetDC ( hwnd );

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
	  g_object_unref ( context->share );
	}

    context->initialised = TRUE;
  }

  g_return_val_if_fail ( context->hdc    != NULL, FALSE );
  g_return_val_if_fail ( context->hglrc  != NULL, FALSE );

  wglMakeCurrent ( context->hdc, context->hglrc );
  
  return TRUE;
}


void gdk_gl_swap_buffers(GdkDrawable *drawable)
{
  HDC   hdc;
  HWND  hwnd;

  g_return_if_fail ( GDK_IS_DRAWABLE(drawable) );

  hwnd = (HWND) gdk_win32_drawable_get_handle ( drawable );
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

#define __GDK_GL_WIN32_C__
