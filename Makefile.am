## top diretory

SUBDIRS= gtkgl examples docs

EXTRA_DIST=	\
	gtkgl.m4 \
	gtkglarea.spec.in \
	makefile.cygwin

m4datadir= $(datadir)/aclocal
m4data_DATA= gtkgl.m4

dist-hook:
	cp gtkglarea.spec $(distdir)
