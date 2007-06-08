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

#include "gdkglpixmap-directfb.h"

GType gdk_gl_pixmap_get_type(void)
{
  g_warning ( "not implemented" );
  return NULL;
}

GdkGLPixmap *gdk_gl_pixmap_new(GdkVisual *visual, GdkPixmap *pixmap)
{
  g_warning ( "not implemented" );
  return NULL;
}

gint gdk_gl_pixmap_make_current(GdkGLPixmap *glpixmap, GdkGLContext *context)
{
  g_warning ( "not implemented" );
  return 0;
}

#define __GDK_GL_PIXMAP_DIRECTFB_C__
