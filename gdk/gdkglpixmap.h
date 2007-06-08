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

#ifndef __GDK_GL_PIXMAP_H__
#define __GDK_GL_PIXMAP_H__

#include "gdk/gdkgl.h"

#include <glib.h>

#ifdef G_OS_WIN32
 /* The GL/gl.h on Windows requires you to include <windows.h>
  * anyway, so we might as well include it here.
  */
 #include <windows.h>
#endif

#include <gdk/gdk.h>

G_BEGIN_DECLS

#define GDK_TYPE_GL_PIXMAP            (gdk_gl_pixmap_get_type())
#define GDK_GL_PIXMAP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GDK_TYPE_GL_PIXMAP, GdkGLPixmap))
#define GDK_GL_PIXMAP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST (klass, GDK_TYPE_GL_PIXMAP, GdkGLPixmapClass))
#define GDK_IS_GL_PIXMAP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GDK_TYPE_GL_PIXMAP))
#define GDK_IS_GL_PIXMAP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_GL_PIXMAP))
#define GDK_GL_PIXMAP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_GL_PIXMAP, GdkGLPixmap))

typedef struct _GdkGLPixmap GdkGLPixmap;

GType        gdk_gl_pixmap_get_type     (void);
GdkGLPixmap *gdk_gl_pixmap_new          (GdkVisual *visual,
                                         GdkPixmap *pixmap);
gint         gdk_gl_pixmap_make_current (GdkGLPixmap *glpixmap,
                                         GdkGLContext *context);

G_END_DECLS

#endif /* __GDK_GL_PIXMAP_H__ */
