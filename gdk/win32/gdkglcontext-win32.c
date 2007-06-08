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

#include "gdkglcontext-win32.h"

GType
gdk_gl_context_get_type (void)
{
  static GType object_type = 0;

  if (!object_type)
    {
      static const GTypeInfo object_info =
      {
        sizeof (GdkGLContextClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gdk_gl_context_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GdkGLContext),
        0,              /* n_preallocs */
        (GInstanceInitFunc) NULL,
      };
      
      object_type = g_type_register_static (G_TYPE_OBJECT,
                                            "GdkGLContext",
                                            &object_info, 0);
    }
  return object_type;
}

static void
gdk_gl_context_finalize(GObject *object)
{
  GdkGLContext *context;

  context = GDK_GL_CONTEXT(object);

  if (context->hglrc == wglGetCurrentContext () )
    wglMakeCurrent ( NULL, NULL );

  wglDeleteContext ( context->hglrc );
	
  if ( context->hwnd )
    ReleaseDC ( context->hwnd, context->hdc );
  else
    DeleteDC ( context->hdc );

  (* glcontext_parent_class->finalize)(object);
}

static void
gdk_gl_context_class_init(GdkGLContextClass *class)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS(class);
  glcontext_parent_class = g_type_class_peek_parent(class);

  gobject_class->finalize = gdk_gl_context_finalize;
}

GdkGLContext *
gdk_gl_context_new(GdkVisual *visual)
{
  return gdk_gl_context_share_new ( visual, NULL, FALSE );
}

GdkGLContext *
gdk_gl_context_share_new(GdkVisual *visual, GdkGLContext *sharelist, gint direct)
{
  GdkGLContext *context;
  
  g_return_val_if_fail ( visual != NULL, NULL );

  context = g_object_new(GDK_TYPE_GL_CONTEXT, NULL);
  if (!context) return NULL;

  context->initialised = FALSE;
  context->hglrc   = NULL;
  context->hdc     = NULL;
  context->hwnd    = NULL;
  context->share   = sharelist ? g_object_ref(sharelist) : NULL;

  memset ( &(context->pfd), 0, sizeof(PIXELFORMATDESCRIPTOR) );

  /* if direct is TRUE, we create a context which renders to the screen, otherwise
     we create one to render to an offscreen bitmap */
  if ( direct )
  {
    context->pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    context->pfd.nVersion = 1;
    context->pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    context->pfd.iPixelType = PFD_TYPE_RGBA;
    context->pfd.cColorBits = 24;
    context->pfd.cDepthBits = 32;
    context->pfd.iLayerType = PFD_MAIN_PLANE;
  } 
  else
  {
    context->pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    context->pfd.nVersion = 1;
    context->pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_BITMAP | PFD_SUPPORT_GDI;
    context->pfd.iPixelType = PFD_TYPE_RGBA;
    context->pfd.cColorBits = 24;
    context->pfd.cDepthBits = 32;
    context->pfd.iLayerType = PFD_MAIN_PLANE;
  }
  
  return context;
}

GdkGLContext *
gdk_gl_context_attrlist_share_new(int *attrlist, GdkGLContext *sharelist, gint direct)
{
  GdkGLContext *context;

  g_return_val_if_fail(attrlist != NULL, NULL);

  context = g_object_new(GDK_TYPE_GL_CONTEXT, NULL);
  if (!context) return NULL;

  context->initialised = FALSE;
  context->hglrc    = NULL;
  context->hdc      = NULL;
  context->hwnd     = NULL;
  context->share    = sharelist ? g_object_ref(sharelist) : NULL;
  fill_pfd(&context->pfd, attrlist);

  return context;
}

#define __GDK_GL_CONTEXT_WIN32_C__
