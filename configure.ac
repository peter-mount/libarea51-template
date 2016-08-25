AC_INIT([libarea51-template], [0.1], [peter@retep.org])

# Check for gcc
AC_PROG_CC
AC_LANG(C)

# Math library
#AC_CHECK_LIB(m, cos)

# ======================================================================
APPS=""
DIRS=""

# Project base for include paths
LDFLAGS="$LDFLAGS -L../build/package/usr/lib"

# header files
CPPFLAGS="$CPPFLAGS -I../include"

# This is a shared library so compile position independent code
CCFLAGS="$CCFLAGS -fPIC"

# ======================================================================
head APKBUILD >/tmp/apkbuild
. /tmp/apkbuild
AC_SUBST([PKGBANNER],["$pkgdesc"])
date=$(date)
AC_SUBST([PKGBUILD],["$pkgname ${pkgver}r${pkgrel} $arch $date"])

# ======================================================================
# Finish off generating all files

LDFLAGS="-larea51-template $LDFLAGS -larea51-rest -lmicrohttpd -larea51 -ljson-c -lcurl"

# add libraries based on the directory names, in reverse order to aid linking
for d in $DIRS
do
    LDFLAGS="-l$d $LDFLAGS"
done

LDFLAGS="$LDFLAGS -lpthread -lm"

DIRS="lib $DIRS $APPS"

AC_SUBST([DIRECTORIES],[$DIRS])
AC_SUBST([CPPFLAGS],[$CPPFLAGS])
AC_SUBST([CCFLAGS],[$CCFLAGS])
AC_SUBST([LDFLAGS],[$LDFLAGS])

# Build make files
MAKEFILES="SubMakefile $(find . -type f -name Makefile.in|sed s/\\.in//g)"
# custom .h files
MAKEFILES="$MAKEFILES $(find . -type f -name '*.h.in'|sed s/\\.in//g)"
# Package files
MAKEFILES="$MAKEFILES $(find package -type f -name '*.in'|sed s/\\.in//g)"
AC_CONFIG_FILES([$MAKEFILES])

AC_OUTPUT()

AC_MSG_NOTICE([Now run make clean;make])
