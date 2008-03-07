## examples directory

EXTRA_DIST = README penguin.lwo alien.lwo makefile.mingw makefile.mingw.in

if HAVE_SHADERS
shader_programs = shaders
endif

noinst_PROGRAMS = simple zktor viewlw glpixmap gdkfont $(shader_programs) gtkglarea_demo

INCLUDES = -I$(top_srcdir)
LDADD = $(top_builddir)/gtkgl/libgtkgl-2.0.la $(GTK_LIBS) $(GL_LIBS)

simple_SOURCES   = simple.c
zktor_SOURCES    = zktor.c
glpixmap_SOURCES = glpixmap.c
viewlw_SOURCES   = viewlw.c lw.h lw.c trackball.h trackball.c
gdkfont_SOURCES  = gdkfont.c
shaders_SOURCES  = shaders.c
gtkglarea_demo_SOURCES = gtkglarea_demo.c
