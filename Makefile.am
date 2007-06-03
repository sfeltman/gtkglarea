## top diretory

# use the ACLOCAL_FLAGS env var when rerunning aclocal
ACLOCAL_FLAGS =
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

SUBDIRS= gtkglarea examples docs

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = gtkglarea-2.0.pc

DISTCLEANFILES = \
	gtkglarea-2.0.pc \
	depcomp

EXTRA_DIST = \
  gtkglarea-2.0.pc.in \
  gtkglarea.spec.in \
  gtkglarea.spec

