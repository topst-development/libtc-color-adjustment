#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([TCCOLORADJUSTMENT], [1.0.1], [ksjeon@telechips.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_PREFIX_DEFAULT(/usr)

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AM_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# libtcutils version
TCCOLORADJ_MAJOR_VERSION=1
TCCOLORADJ_MINOR_VERSION=0
TCCOLORADJ_MICRO_VERSION=0
TCCOLORADJ_VERSION_INFO=$TCCOLORADJ_MAJOR_VERSION:$TCCOLORADJ_MINOR_VERSION:$TCCOLORADJ_MICRO_VERSION

# Confiugre Option

# Checks PKG-CONFIG
#PKG_CHECK_MODULES([TCCOLORADJUSTMENT], [m])

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-ltccoloradjustment':
AC_CHECK_LIB([tccoloradjustment], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([pow])

AC_SUBST(TCCOLORADJ_VERSION_INFO)

AC_CONFIG_FILES([Makefile
                 tc-color-adjustment.pc
                 include/Makefile
                 src/Makefile
                 test/Makefile])
AC_OUTPUT
