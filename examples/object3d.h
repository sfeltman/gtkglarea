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

#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <glib.h>

typedef struct {
  float r,g,b;        /* color, value of r,g and b between [0,1] */
  gpointer texture;    /* texture data */
} o3dMaterial;

typedef struct {
} o3dFace;

typedef struct {
  int faces;
} o3dShape;

typedef struct {
  GHashTable *materials;
  GHashTable *shapes;
} o3dWorld;


o3dWorld *o3d_world_new(void);
void      o3d_world_free(o3dWorld *o3d_world);

o3dShape *o3d_shape_new(o3dWorld *o3d_world, const char *shape_name);


#endif /* OBJECT3D_H */



