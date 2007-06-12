/* 
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

#ifndef __GDK_GL_CONTEXT_H__
#define __GDK_GL_CONTEXT_H__

struct _GdkGLContextClass {
  GObjectClass parent_class;
};
typedef struct _GdkGLContextClass GdkGLContextClass;

static GObjectClass *glcontext_parent_class;

static void   gdk_gl_context_class_init         (GdkGLContextClass *class);

GType         gdk_gl_context_get_type           (void);
GdkGLContext *gdk_gl_context_new                (GdkVisual *visual);
GdkGLContext *gdk_gl_context_share_new          (GdkVisual *visual,
                                                 GdkGLContext *sharelist,
                                                 gint direct);
GdkGLContext *gdk_gl_context_attrlist_share_new (int *attrlist,
                                                 GdkGLContext *sharelist,
                                                 gint direct)

#endif /* __GDK_GL_CONTEXT_H__ */
