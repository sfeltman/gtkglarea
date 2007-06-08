## Makefile.am for gtkglarea/gdk

SUBDIRS = $(gdktarget)
DIST_SUBDIRS = linux-fb win32 x11 quartz directfb
#
# setup source file variables
#
#
# GDK GL header files for public installation (non-generated)
#
gdkgl_public_h_sources =				\
	gdkgl.h

common_sources = gdkgl.c

libgdk_x11_2_0_la_SOURCES = $(common_sources)
libgdk_x11_2_0_la_LIBADD = x11/libgdk-x11.la $(GDK_DEP_LIBS) \
  $(top_builddir)/gdk-pixbuf/libgdk_pixbuf-$(GTK_API_VERSION).la
libgdk_x11_2_0_la_LDFLAGS = $(LDADD)
