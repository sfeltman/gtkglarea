## examples directory

EXTRA_DIST = README penguin.lwo alien.lwo makefile.cygwin

noinst_PROGRAMS = simple zktor viewlw glpixmap gdkfont gtkglarea_demo

INCLUDES = -I$(top_srcdir)
LDADD = $(top_builddir)/gtkgl/libgtkgl.la @GTK_LIBS@ @GL_LIBS@

simple_SOURCES   = simple.c
zktor_SOURCES    = zktor.c
glpixmap_SOURCES = glpixmap.c
viewlw_SOURCES   = viewlw.c lw.h lw.c trackball.h trackball.c
gdkfont_SOURCES  = gdkfont.c
gtkglarea_demo_SOURCES = gtkglarea_demo.c
