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


#ifndef __GGLA_WIDGET_H__
#define __GGLA_WIDGET_H__

#include <gdk/gdk.h>
#include <gtkgl/gdkgl.h>
#include <gtk/gtk.h>


G_BEGIN_DECLS

#define GGLA_TYPE_WIDGET            (ggla_widget_get_type())
#define GGLA_WIDGET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GGLA_TYPE_WIDGET, GglaWidget))
#define GGLA_WIDGET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST (klass, GGLA_TYPE_WIDGET, GglaWidgetClass))
#define GGLA_IS_WIDGET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGLA_TYPE_WIDGET))
#define GGLA_IS_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GGLA_TYPE_WIDGET))
#define GGLA_WIDGET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GGLA_TYPE_WIDGET, GglaWidget))


typedef struct _GglaWidget       GglaWidget;
typedef struct _GglaWidgetClass  GglaWidgetClass;


struct _GglaWidget
{
  GtkDrawingArea  darea;
  GglaContext *glcontext;
};

struct _GglaWidgetClass
{
  GtkDrawingAreaClass parent_class;
};

GType      ggla_widget_get_type   (void);
GtkWidget* ggla_widget_new        (int       *attrList);
GtkWidget* ggla_widget_share_new  (int       *attrList,
                                   GglaWidget *share);
GtkWidget* ggla_widget_new_vargs  (GglaWidget *share,
				   ...);


gint       ggla_widget_make_current(GglaWidget *glarea);

void       ggla_widget_swap_buffers(GglaWidget *glarea);


G_END_DECLS

#endif /* __GGLA_WIDGET_H__ */
