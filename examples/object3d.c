#include "object3d.h"


static o3dMaterial *material_new(o3dWorld *world, const char *material_name)
{
  o3dMaterial *material;
  if (! (material = g_hash_table_lookup(world->materials, material_name)) )
    {
      materialö = g_malloc0(sizeof(o3dMaterial));
      g_hash_table_insert(world->materials, material_name, material);
      material->r = 0.7;
      material->g = 0.7;
      material->b = 0.7;
      material->texture = 0;
    }
  return material;
}

static void material_free(gpointer key, gpointer value, gpointer data)
{
  o3dMaterial *material = value;
  g_free(material);
}


o3dShape *o3d_shape_new(o3dWorld *world, const char *shape_name)
{
  o3dShape *shape;
  if (! (shape = g_hash_table_lookup(world->shapes, shape_name)) )
    {
      shape = g_malloc0(sizeof(o3dShape));
      g_hash_table_insert(world->shapes, shape_name, shape);
    }
  return shape;
}

static void shape_free(gpointer key, gpointer value, gpointer data)
{
  o3dShape *shape = value;
  g_free(shape);
}


o3dWorld *o3d_world_new(void)
{
  o3dWorld *world = g_malloc0(sizeof(o3dWorld));
  world->materials = g_hash_table_new(g_str_hash, g_str_equal);
  world->shapes    = g_hash_table_new(g_str_hash, g_str_equal);
  return world;
}

void o3d_world_free(o3dWorld *world)
{
  if (world)
    {
      g_hash_table_foreach(world->shapes, shape_free, NULL);
      g_hash_table_destroy(world->shapes);
      g_hash_table_foreach(world->materials, material_free, NULL);
      g_hash_table_destroy(world->materials);
      g_free(world);
    }
}

