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

#ifndef __GDK_GL_PIXMAP_X11_H__
#define __GDK_GL_PIXMAP_X11_H__

#include "gdk/gdkglpixmap.h"

#include <gdk/gdkx.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <string.h>

struct _GdkGLPixmap {
  GObject   parent;
  Display   *xdisplay;
  GLXPixmap glxpixmap;
  GdkPixmap *front_left;
};

struct _GdkGLPixmapClass {
  GObjectClass parent_class;
};
typedef struct _GdkGLPixmapClass GdkGLPixmapClass;

static GObjectClass *glpixmap_parent_class;

static void gdk_gl_pixmap_class_init (GdkGLPixmapClass *class);

#endif /* __GDK_GL_PIXMAP_X11_H__ */
