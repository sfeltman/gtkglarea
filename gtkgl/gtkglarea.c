/*
 * Copyright (C) 1997-1998 Janne Löf <jlof@mail.student.oulu.fi>
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

#include "gdkgl.h"
#include "gtkglarea.h"
#include <GL/gl.h>
#include <stdarg.h>

static void gtk_gl_area_class_init    (GtkGLAreaClass *klass);
static void gtk_gl_area_init          (GtkGLArea      *glarea);
static void gtk_gl_area_destroy       (GtkObject      *object);
static void gtk_gl_area_realize       (GtkWidget      *widget);
static void gtk_gl_area_size_allocate (GtkWidget      *widget,
                                       GtkAllocation  *allocation);

static GtkWidgetClass *parent_class = NULL;


guint
gtk_gl_area_get_type ()
{
	static guint gl_area_type = 0;

	if (!gl_area_type) {
		GtkTypeInfo gl_area_info = {
			(gchar*)"GtkGLArea",
			sizeof (GtkGLArea),
			sizeof (GtkGLAreaClass),
			(GtkClassInitFunc) gtk_gl_area_class_init,
			(GtkObjectInitFunc) gtk_gl_area_init,
			/* reserved_1 */ NULL,
			/* reserved_2 */ NULL,
			(GtkClassInitFunc) NULL
		};
		gl_area_type = gtk_type_unique (gtk_widget_get_type (), &gl_area_info);
	}

	return gl_area_type;
}

static void
gtk_gl_area_class_init (GtkGLAreaClass *klass)
{
	GtkWidgetClass *widget_class;
	GtkObjectClass *object_class;

	widget_class = (GtkWidgetClass*) klass;
	object_class = (GtkObjectClass*) klass;

	parent_class = gtk_type_class(gtk_widget_get_type());

	object_class->destroy = gtk_gl_area_destroy;
	widget_class->realize = gtk_gl_area_realize;
	widget_class->size_allocate = gtk_gl_area_size_allocate;
}


static void
gtk_gl_area_init (GtkGLArea *gl_area)
{
	gl_area->glcontext = NULL;
}



GtkWidget*
gtk_gl_area_new_vargs(GtkGLArea *share, ...)
{
  GtkWidget *glarea;
  va_list ap;
  int i;
  gint *attrList;

  va_start(ap, share);
  i=1;
  while (va_arg(ap, int) != GDK_GL_NONE) /* get number of arguments */
    i++;
  va_end(ap);

  attrList = g_new(int,i);

  va_start(ap,share);
  i=0;
  while ( (attrList[i] = va_arg(ap, int)) != GDK_GL_NONE) /* copy args to list */
    i++;
  va_end(ap);
  
  glarea = gtk_gl_area_share_new(attrList, share);

  g_free(attrList);

  return glarea;
}

GtkWidget*
gtk_gl_area_new (int *attrList)
{
  return gtk_gl_area_share_new(attrList, NULL);
}

GtkWidget*
gtk_gl_area_share_new (int *attrList, GtkGLArea *share)
{
	GdkVisual *visual;
	GdkGLContext *glcontext;
	GtkGLArea *gl_area;

	g_return_val_if_fail(share == NULL || GTK_IS_GL_AREA(share), NULL);

	visual = gdk_gl_choose_visual(attrList);
	if (visual == NULL)
	  return NULL;

	glcontext = gdk_gl_context_share_new(visual, share ? share->glcontext : NULL , TRUE);
	if (glcontext == NULL)
	  return NULL;

	/* use colormap and visual suitable for OpenGL rendering */
	gtk_widget_push_colormap(gdk_colormap_new(visual,TRUE));
	gtk_widget_push_visual(visual);

	gl_area = gtk_type_new (gtk_gl_area_get_type());
	gl_area->glcontext = glcontext;

	/* pop back defaults */
	gtk_widget_pop_visual();
	gtk_widget_pop_colormap();

	return GTK_WIDGET(gl_area);
}

static void
gtk_gl_area_destroy(GtkObject *object)
{
	GtkGLArea *gl_area;

	g_return_if_fail (object != NULL);
	g_return_if_fail (GTK_IS_GL_AREA(object));

	gl_area = GTK_GL_AREA(object);
	gdk_gl_context_unref(gl_area->glcontext);

	if (GTK_OBJECT_CLASS (parent_class)->destroy)
		(* GTK_OBJECT_CLASS (parent_class)->destroy) (object);
}





static void
gtk_gl_area_realize(GtkWidget *widget)
{
	GtkGLArea *gl_area;
	GdkWindowAttr attributes;
	gint attributes_mask;
	GdkEventConfigure event;
	
	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_GL_AREA(widget));

	gl_area = GTK_GL_AREA(widget);
	GTK_WIDGET_SET_FLAGS(widget,GTK_REALIZED);
	
	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = widget->allocation.x;
	attributes.y = widget->allocation.y;
	attributes.width = widget->allocation.width;
	attributes.height = widget->allocation.height;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = gtk_widget_get_visual (widget);
	attributes.colormap = gtk_widget_get_colormap (widget);
	attributes.event_mask = gtk_widget_get_events (widget) | GDK_EXPOSURE_MASK;

	attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  	widget->window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask);
	gdk_window_set_user_data (widget->window, gl_area);

	widget->style = gtk_style_attach (widget->style, widget->window);
	gtk_style_set_background (widget->style, widget->window, GTK_STATE_NORMAL);

	/* send configure */
	event.type = GDK_CONFIGURE;
	event.window = widget->window;
	event.x = widget->allocation.x;
	event.y = widget->allocation.y;
	event.width  = widget->allocation.width;
	event.height = widget->allocation.height;
	gtk_widget_event (widget, (GdkEvent*) &event);
}

static void
gtk_gl_area_size_allocate (GtkWidget     *widget,
                          GtkAllocation *allocation)
{
	 GdkEventConfigure event;

	g_return_if_fail (widget != NULL);
	g_return_if_fail (GTK_IS_GL_AREA (widget));
	g_return_if_fail (allocation != NULL);

	widget->allocation = *allocation;
	if (GTK_WIDGET_REALIZED (widget)) {
		gdk_window_move_resize (widget->window,
		                        allocation->x, allocation->y,
		                        allocation->width, allocation->height);
		/* send configure */
		event.type = GDK_CONFIGURE;
		event.window = widget->window;
		event.x = allocation->x;
		event.y = allocation->y;
		event.width  = allocation->width;
		event.height = allocation->height;
		gtk_widget_event (widget, (GdkEvent*) &event);
	}
}


gint gtk_gl_area_begingl(GtkGLArea *gl_area)
{
	g_return_val_if_fail(gl_area != NULL, FALSE);
	g_return_val_if_fail(GTK_IS_GL_AREA (gl_area), FALSE);
	g_return_val_if_fail(GTK_WIDGET_REALIZED(gl_area), FALSE);

	return gdk_gl_make_current(GTK_WIDGET(gl_area)->window, gl_area->glcontext);
}
void gtk_gl_area_endgl(GtkGLArea *gl_area)
{
	g_return_if_fail(gl_area != NULL);
	g_return_if_fail(GTK_IS_GL_AREA(gl_area));
	glFlush();
}

void gtk_gl_area_swapbuffers(GtkGLArea *gl_area)
{
	g_return_if_fail(gl_area != NULL);
	g_return_if_fail(GTK_IS_GL_AREA(gl_area));
	g_return_if_fail(GTK_WIDGET_REALIZED(gl_area));

	gdk_gl_swap_buffers(GTK_WIDGET(gl_area)->window);
}

void gtk_gl_area_size (GtkGLArea *glarea, gint width, gint height)
{
  g_return_if_fail (glarea != NULL);
  g_return_if_fail (GTK_IS_GL_AREA (glarea));

  GTK_WIDGET (glarea)->requisition.width = width;
  GTK_WIDGET (glarea)->requisition.height = height;
}
