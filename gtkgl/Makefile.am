## gtkgl directory

EXTRA_DIST = makefile.cygwin gdkgl-win32.c

INCLUDES= -I$(top_srcdir)

gtkglincludedir = $(includedir)/gtkgl

lib_LTLIBRARIES= libgtkgl.la

gtkglinclude_HEADERS= gdkgl.h gtkglarea.h
libgtkgl_la_SOURCES = gdkgl.c gtkglarea.c
libgtkgl_la_LDFLAGS = -version-info $(GTKGL_LIBRARY_VERSION)
