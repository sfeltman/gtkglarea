## gtkgl directory

EXTRA_DIST = makefile.mingw makefile.mingw.in gdkgl-win32.c

INCLUDES= -I$(top_srcdir)

gtkglincludedir = $(includedir)/gtkgl

lib_LTLIBRARIES= libgtkgl.la

gtkglinclude_HEADERS= gdkgl.h gtkglarea.h
libgtkgl_la_SOURCES = gdkgl.c gtkglarea.c
libgtkgl_la_LDFLAGS = -version-info $(GTKGL_LIBRARY_VERSION)

makefile.mingw: $(top_builddir)/config.status $(top_srcdir)/gtkgl/makefile.mingw.in
	cd $(top_builddir) && CONFIG_FILES=gtkgl/$@ CONFIG_HEADERS= $(SHELL) ./config.status

