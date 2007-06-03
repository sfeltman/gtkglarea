## gtkgl directory

if OS_WIN32
export_symbols = -export-symbols gtkgl.def

install-libtool-import-lib:
	$(INSTALL) .libs/libgtkgl-2.0.dll.a $(DESTDIR)$(libdir)
uninstall-libtool-import-lib:
	-rm $(DESTDIR)$(libdir)/libgtkgl-2.0.dll.a
else
install-libtool-import-lib:
uninstall-libtool-import-lib:
endif

if MS_LIB_AVAILABLE
noinst_DATA = gtkgl-2.0.lib

install-ms-lib:
	$(INSTALL) gtkgl-2.0.lib $(DESTDIR)$(libdir)

uninstall-ms-lib:
	-rm $(DESTDIR)$(libdir)/gtkgl-2.0.lib
else
install-ms-lib:
uninstall-ms-lib:
endif

EXTRA_DIST = makefile.mingw makefile.mingw.in gdkgl-win32.c gtkgl.def

AM_CFLAGS = $(BASE_DEPENDENCIES_CFLAGS) $(GL_CFLAGS)
INCLUDES= -I$(top_srcdir)

gtkglincludedir = $(includedir)/gtkgl-2.0/gtkgl

lib_LTLIBRARIES= libgtkgl-2.0.la

gtkgl_x11_c_sources      = \
	gdkgl-x11.c
gtkgl_win32_c_sources    = \
	gdkgl-win32.c
gtkgl_linux_fb_c_sources = \
	gdkgl-unsupported.c
gtkgl_quartz_c_sources   = \
	gdkgl-unsupported.c
gtkgl_directfb_c_sources = \
	gdkgl-unsupported.c

gtkglinclude_HEADERS = gdkgl.h gtkglarea.h
gtkgl_c_sources = gtkglarea.c

if USE_WIN32
  gtkgl_c_sources += $(gtkgl_win32_c_sources)
endif

if USE_X11
  gtkgl_c_sources += $(gtkgl_x11_c_sources)
endif

if USE_DIRECTFB
  gtkgl_c_sources += $(gtkgl_directfb_c_sources)
endif

if USE_LINUX_FB
  gtkgl_c_sources += $(gtkgl_linux_fb_c_sources)
endif

if USE_QUARTZ
  gtkgl_c_sources += $(gtkgl_quartz_c_sources)
endif

libgtkgl_2_0_la_SOURCES = $(gtkgl_c_sources)

libgtkgl_2_0_la_LDFLAGS = -version-info $(GTKGL_LIBRARY_VERSION) $(export_symbols) -no-undefined
libgtkgl_2_0_la_LIBADD = $(BASE_DEPENDENCIES_LIBS) $(GL_LIBS)

gtkgl-2.0.lib: libgtkgl-2.0.la gtkgl.def
	lib -name:libgtkgl-2.0-@LT_CURRENT_MINUS_AGE@.dll -def:gtkgl.def -out:$@

install-data-local: install-ms-lib install-libtool-import-lib

uninstall-local: uninstall-ms-lib uninstall-libtool-import-lib
