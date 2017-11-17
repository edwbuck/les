AC_CONFIG_AUX_DIR([ac-tools])
AC_INIT([les], [1.0], [gelbman@gmail.com])
AC_CONFIG_MACRO_DIR([m4])
AC_HEADER_TIME
AC_CHECK_HEADERS([fcntl.h signal.h stdarg.h stdio.h stddef.h sys/time.h term.h termios.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AX_WITH_CURSES
AC_PROG_CPP
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 man/Makefile
 include/Makefile
 src/Makefile
])
AC_OUTPUT
