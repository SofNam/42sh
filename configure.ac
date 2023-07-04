AC_INIT([42sh], [1.0.0])
AC_PREREQ([2.69])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_REQUIRE_AUX_FILE([tap-driver.sh])
AM_INIT_AUTOMAKE([1.11 -Wall -Werror subdir-objects])
AC_CONFIG_FILES([Makefile])
AC_PROG_CC_C99

# Checks for check
PKG_CHECK_MODULES(
        [CHECK],
        [check],
        [HAVE_CHECK="yes"],
        [HAVE_CHECK="no"]
)

AM_CONDITIONAL([WITH_CHECK], [test "x$HAVE_CHECK" = xyes])

AC_OUTPUT
