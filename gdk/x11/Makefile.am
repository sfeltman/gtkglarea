## Process this file with automake to produce Makefile.in

INCLUDES = 				\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/gdk		\
	-I$(top_builddir)/gdk		\
	$(BASE_DEPENDENCIES_CFLAGS)

LDADDS = $(BASE_DEPENDENCIES_LIBS)

noinst_LTLIBRARIES = libgdkgl-x11.la

libgdkgl_x11_la_SOURCES =    	\
	gdkgl-x11.c
