## gtkgl directory

EXTRA_DIST = makefile.mingw makefile.mingw.in gdkgl-win32.c

AM_CFLAGS = $(GTK_CFLAGS) $(GL_CFLAGS)
INCLUDES= -I$(top_srcdir)

gtkglincludedir = $(includedir)/gtkgl-2.0/gtkgl

lib_LTLIBRARIES= libgtkgl-2.0.la

gtkglinclude_HEADERS= gdkgl.h gtkglarea.h
libgtkgl_2_0_la_SOURCES = gdkgl.c gtkglarea.c
libgtkgl_2_0_la_LDFLAGS = -version-info $(GTKGL_LIBRARY_VERSION)
libgtkgl_2_0_la_LIBADD = $(GTK_LIBS) $(GL_LIBS)

