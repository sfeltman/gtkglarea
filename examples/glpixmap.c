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


#include <math.h>
#include <gtk/gtk.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <gdk/gdk.h>
#include <gtkgl/gdkgl.h>


int visual_attributes[] = { GDK_GL_RGBA,
			    GDK_GL_RED_SIZE,1,
			    GDK_GL_GREEN_SIZE,1,
			    GDK_GL_BLUE_SIZE,1,
			    GDK_GL_NONE };


int main(int argc, char **argv)
{
  GtkWidget *window,*pixmapwidget;
  GdkVisual *visual;
  GdkPixmap *pixmap;
  GdkGLPixmap *glpixmap;
  GdkGLContext *context;

  gtk_init(&argc, &argv);

  /* check opengl */
  if (gdk_gl_query() == FALSE) {
    g_print("OpenGL not supported\n");
    return 0;
  }

  /* select and use visual as default so all widgets are OpenGL renderable */
  visual = gdk_gl_choose_visual(visual_attributes);
  if (visual == NULL) {
    g_print("Can't get visual\n");
    return 0;
  }
  gtk_widget_set_default_colormap(gdk_colormap_new(visual, TRUE));
  gtk_widget_set_default_visual(visual);

  /* top level window. */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "glpixmap");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_signal_connect(GTK_OBJECT(window), "delete_event",
		     GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
  gtk_widget_set_usize(window, 100,100);
  gtk_widget_show(window);

  /* pixmap */
  context  = gdk_gl_context_new(visual);
  pixmap = gdk_pixmap_new(NULL, 80,80, visual->depth);
  glpixmap = gdk_gl_pixmap_new(visual, pixmap);
  if (gdk_gl_pixmap_make_current(glpixmap, context)) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,100,100,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    glBegin(GL_TRIANGLES);
    glVertex2f(10,10);
    glVertex2f(10,90);
    glVertex2f(90,90);
    glEnd();
  }
  gdk_gl_context_unref(context);
  gdk_gl_pixmap_unref(glpixmap);


  /* pixmapwidget */
  pixmapwidget = gtk_pixmap_new( pixmap, NULL );
  gdk_pixmap_unref(pixmap);

  gtk_container_add(GTK_CONTAINER(window), pixmapwidget);
  gtk_widget_show(pixmapwidget);

  gtk_main();

  return 0;
}
