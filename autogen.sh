builddir=`pwd`
srcdir=`dirname $0`
cd $srcdir

echo "Running glib-gettextize ..."
glib-gettextize --force --copy ||
	{ echo "**Error**: glib-gettextize failed."; exit 1; }

# echo "Running intltoolize ..."
# intltoolize --force --copy --automake ||
# 	{ echo "**Error**: intltoolize failed."; exit 1; }

echo "Running aclocal $ACLOCAL_FLAGS ..."
aclocal $ACLOCAL_FLAGS || {
  echo
  echo "**Error**: aclocal failed. This may mean that you have not"
  echo "installed all of the packages you need, or you may need to"
  echo "set ACLOCAL_FLAGS to include \"-I \$prefix/share/aclocal\""
  echo "for the prefix where you installed the packages whose"
  echo "macros were not found"
  exit 1
}

# checking for automake 
(automake --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`automake' installed to compile gtkglarea."
  echo "Get ftp://ftp.gnu.org/pub/gnu/automake-1.3.tar.gz"
  echo "(or a newer version if it is available)"
  DIE=1
  NO_AUTOMAKE=yes
}

echo "Running automake --gnu $am_opt ..."
automake --add-missing --gnu $am_opt ||
	{ echo "**Error**: automake failed."; exit 1; }

echo "running autoconf ..."
WANT_AUTOCONF=2.5 autoconf || {
  echo "**Error**: autoconf failed."; exit 1; }

conf_flags="--enable-maintainer-mode --enable-compile-warnings"

if test x$NOCONFIGURE = x; then
  echo Running $srcdir/configure $conf_flags "$@" ...
  cd $builddir
  $srcdir/configure $conf_flags "$@" \
  && echo Now type \`make\' to compile $PKG_NAME || exit 1
else
  echo Skipping configure process.
fi
