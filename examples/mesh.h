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

#ifndef MESH_H
#define MESH_H

#include <glib.h>

#define MK_ID(a,b,c,d) ((((guint32)(a))<<24)| \
			(((guint32)(b))<<16)| \
			(((guint32)(c))<< 8)| \
			(((guint32)(d))    ))
typedef struct {
  guint32 id;
} meshObject;


meshObject *mesh_object_read(const char *file);
void        mesh_object_free(meshObject *mesh_object);
void        mesh_object_show(meshObject *mesh_object);

float       mesh_object_radius(meshObject *mesh_object);
float       mesh_object_scale (meshObject *mesh_object, float scalex, float scaley, float scalez);


#endif /* MESH_H */



