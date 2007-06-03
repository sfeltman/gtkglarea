## gtkglarea directory

if OS_WIN32
export_symbols = -export-symbols gtkglarea.def

install-libtool-import-lib:
	$(INSTALL) .libs/libgtkglarea-2.0.dll.a $(DESTDIR)$(libdir)
uninstall-libtool-import-lib:
	-rm $(DESTDIR)$(libdir)/libgtkglarea-2.0.dll.a
else
install-libtool-import-lib:
uninstall-libtool-import-lib:
endif

if MS_LIB_AVAILABLE
noinst_DATA = gtkglarea-2.0.lib

install-ms-lib:
	$(INSTALL) gtkglarea-2.0.lib $(DESTDIR)$(libdir)

uninstall-ms-lib:
	-rm $(DESTDIR)$(libdir)/gtkglarea-2.0.lib
else
install-ms-lib:
uninstall-ms-lib:
endif

EXTRA_DIST = makefile.mingw makefile.mingw.in gdkglarea-win32.c gtkglarea.def

AM_CFLAGS = $(BASE_DEPENDENCIES_CFLAGS) $(GL_CFLAGS)
INCLUDES= -I$(top_srcdir)

gtkglareaincludedir = $(includedir)/gtkglarea-2.0/gtkglarea

lib_LTLIBRARIES= libgtkglarea-2.0.la

gtkglarea_x11_c_sources      = \
	gdkglarea-x11.c
gtkglarea_win32_c_sources    = \
	gdkglarea-win32.c
gtkglarea_linux_fb_c_sources = \
	gdkglarea-unsupported.c
gtkglarea_quartz_c_sources   = \
	gdkglarea-unsupported.c
gtkglarea_directfb_c_sources = \
	gdkglarea-unsupported.c

gtkglareainclude_HEADERS = gdkglarea.h gtkglarea.h
gtkglarea_c_sources = gtkglarea.c

if USE_WIN32
  gtkglarea_c_sources += $(gtkglarea_win32_c_sources)
endif

if USE_X11
  gtkglarea_c_sources += $(gtkglarea_x11_c_sources)
endif

if USE_DIRECTFB
  gtkglarea_c_sources += $(gtkglarea_directfb_c_sources)
endif

if USE_LINUX_FB
  gtkglarea_c_sources += $(gtkglarea_linux_fb_c_sources)
endif

if USE_QUARTZ
  gtkglarea_c_sources += $(gtkglarea_quartz_c_sources)
endif

libgtkglarea_2_0_la_SOURCES = $(gtkglarea_c_sources)

libgtkglarea_2_0_la_LDFLAGS = \
	-version-info $(GTKGLAREA_LIBRARY_VERSION) $(export_symbols) \
	-no-undefined
libgtkglarea_2_0_la_LIBADD = $(BASE_DEPENDENCIES_LIBS) $(GL_LIBS)

gtkglarea-2.0.lib: libgtkglarea-2.0.la gtkglarea.def
	lib -name:libgtkglarea-2.0-@LT_CURRENT_MINUS_AGE@.dll \
		-def:gtkglarea.def \
		-out:$@

install-data-local: install-ms-lib install-libtool-import-lib

uninstall-local: uninstall-ms-lib uninstall-libtool-import-lib
