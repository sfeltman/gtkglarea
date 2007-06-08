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

#include "gdkgl-quartz.h"

gint gdk_gl_query(void)
{
  g_warning ( "not implemented" );
  return 0;
}

gchar *gdk_gl_get_info(void)
{
  g_warning ( "not implemented" );
  return NULL;
}

GdkVisual *gdk_gl_choose_visual(int *attrlist)
{
  g_warning ( "not implemented" );
  return NULL;
}

int gdk_gl_get_config(GdkVisual *visual, int attrib)
{
  g_warning ( "not implemented" );
  return 0;
}

gint gdk_gl_make_current(GdkDrawable *drawable, GdkGLContext *context)
{
  g_warning ( "not implemented" );
  return 0;
}

void gdk_gl_swap_buffers(GdkDrawable *drawable)
{
  g_warning ( "not implemented" );
}

void gdk_gl_wait_gdk(void)
{
  g_warning ( "not implemented" );
}

void gdk_gl_wait_gl(void)
{
  g_warning ( "not implemented" );
}

#define __GDK_GL_QUARTZ_C__
