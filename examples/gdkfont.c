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
#include <gtkgl/gtkglarea.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <string.h>

GLuint fontbase;       /* display list base for fonts */
int fontheight;
char *fontname;


gint init(GtkWidget *widget)
{
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) {
    GdkFont *font;

    /* set viewport */
    glViewport(0,0, widget->allocation.width, widget->allocation.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,widget->allocation.width, 0,widget->allocation.height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* generate font display lists */
    fontbase = glGenLists( 128 );
    font = gdk_font_load(fontname);
    if (!font) {
      g_print("Can't load font '%s'\n", fontname);
      gtk_exit(1);
    }

    gdk_gl_use_gdk_font(font, 0, 128, fontbase);
    fontheight = font->ascent+font->descent;
    gdk_font_unref(font);

  }
  return TRUE;
}


/* When widget is exposed it's contents are redrawn. */
gint draw(GtkWidget *widget, GdkEventExpose *event)
{
  /* Draw only last expose. */
  if (event->count > 0) {
    return TRUE;
  }

  if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) {
    int i,j;

    /* clear screen */
    glClearColor(0,0,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw some text */
    glColor3f(1,1,1);
    for (i=1; i<5; i++) {
      glRasterPos2f(10, widget->allocation.height-i*fontheight);
      for (j=' '; j<='Z'; j++) {
	glCallList(fontbase+j);
      }
    }

    /* show font name */
    glColor3f(1,1,0);
    glRasterPos2f(10,5);
    glListBase(fontbase);
    glCallLists(strlen(fontname), GL_UNSIGNED_BYTE, fontname);

  }
  return TRUE;
}

/* When glarea widget size changes, viewport size is set to match the new size */
gint reshape(GtkWidget *widget, GdkEventConfigure *event)
{
   if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) {

    glViewport(0,0, widget->allocation.width, widget->allocation.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,widget->allocation.width, 0,widget->allocation.height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  }
  return TRUE;
}


int main(int argc, char **argv)
{
  GtkWidget *window,*glarea;

  gtk_init(&argc, &argv);

  if (argc == 1) {
    fontname = "-adobe-helvetica-medium-r-normal--*-120-*-*-*-*-*-*";
  } else if (argc == 2) {
    fontname = argv[1];
  } else if (argc > 2) {
    g_print("Usage: gdkfont [font]\n");
    return 0;
  }


  /* Check if OpenGL (GLX extension) is supported. */
  if (gdk_gl_query() == FALSE) {
    g_print("OpenGL not supported\n");
    return 0;
  }

  /* Create new top level window. */
  window = gtk_window_new( GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "gdkfont");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  /* Quit form main if got delete event */
  gtk_signal_connect(GTK_OBJECT(window), "delete_event",
		     GTK_SIGNAL_FUNC(gtk_main_quit), NULL);

  /* Delete  widget before exit */
  gtk_quit_add_destroy(1, GTK_OBJECT(window));

  /* Create new OpenGL widget. */
  glarea = GTK_WIDGET(gtk_gl_area_new_vargs(NULL,
					    GDK_GL_RGBA,
					    GDK_GL_RED_SIZE,1,
					    GDK_GL_GREEN_SIZE,1,
					    GDK_GL_BLUE_SIZE,1,
					    GDK_GL_DEPTH_SIZE,1,
					    GDK_GL_NONE));

  gtk_widget_set_events(GTK_WIDGET(glarea),
			GDK_EXPOSURE_MASK|
			GDK_BUTTON_PRESS_MASK);

  /* Connect signal handlers */
  gtk_signal_connect(GTK_OBJECT(glarea), "expose_event",
		     GTK_SIGNAL_FUNC(draw), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "configure_event",
		     GTK_SIGNAL_FUNC(reshape), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "realize",
		     GTK_SIGNAL_FUNC(init), NULL);

  gtk_widget_set_usize(GTK_WIDGET(glarea), 450,100);
		    
  /* put glarea into window and show it all */
  gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(glarea));
  gtk_widget_show(GTK_WIDGET(glarea));
  gtk_widget_show(GTK_WIDGET(window));

  gtk_main();

  return 0;
}
