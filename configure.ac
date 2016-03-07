#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


# Autoconf requirements
AC_PREREQ([2.68])


# Initialize package
AC_INIT([exceptions4c], [3.0.6], [exceptions4c@guillermo.in], [exceptions4c], [https://github.com/guillermocalvo/exceptions4c/])


# Information on the package
AC_COPYRIGHT([Copyright 2016 Guillermo Calvo])
AC_REVISION([$PACKAGE_VERSION])
AC_MSG_NOTICE([
                         _   _                 _  _
  _____  _____ ___ _ __ | |_(_) ___  _ __  ___| || |   ___
 / _ \ \/ / __/ _ \ '_ \| __| |/ _ \| '_ \/ __| || |_ / __|
|  __/>  < (_|  __/ |_) | |_| | (_) | | | \__ \__   _| (__
 \___/_/\_\___\___| .__/ \__|_|\___/|_| |_|___/  |_|  \___|
                  |_| An exception handling framework for C
])


# Check if the source folder is correct
AC_CONFIG_SRCDIR([src/e4c.c])


# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_RANLIB
AM_PROG_AR


# Checks for header files.
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_CHECK_HEADERS([errno.h])
AC_CHECK_HEADERS([setjmp.h])
AC_CHECK_HEADERS([signal.h])
AC_CHECK_HEADERS([stdarg.h])
AC_CHECK_HEADERS([stdbool.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])


# Checks for types
AC_TYPE_SIZE_T
AC_CHECK_TYPES(jmp_buf)
AC_CHECK_TYPES(sigjmp_buf)


# Checks for compiler characteristics
AC_LANG([C])

dnl BEGIN variadic macro support
AC_MSG_CHECKING([for variadic macro support])
AC_COMPILE_IFELSE(AC_LANG_PROGRAM(
    [[
#include <stdio.h>
#include <string.h>
#define asprint(buffer, format, args...) asprintf(buffer, format, ## args)
    ]], [[AC_LANG_SOURCE([
char * buf;
asprint(buf, "testing %s %s\n", "variadic", "macros");
return( strcmp("testing variadic macros", buf) );
    ])]]),
    [ support_macro_varargs=yes ],
    [ support_macro_varargs=no ])
if test "x$support_macro_varargs" = xyes; then
AC_DEFINE(HAVE_VARIADIC_MACROS)
AC_MSG_RESULT(yes)
else
AC_MSG_RESULT(no)
fi
dnl END variadic macro support

dnl BEGIN function name support
AC_MSG_CHECKING([for __func__ support])
AC_COMPILE_IFELSE(AC_LANG_PROGRAM(
    [[
#include <stdio.h>
    ]], [[AC_LANG_SOURCE([
char * name = __func__;
printf("testing function name: %s\n", name);
return( strcmp("main", name) );
    ])]]),
    [ support_function_name=yes ],
    [ support_function_name=no ])
if test "x$support_function_name" = xyes; then
AC_DEFINE(HAVE_FUNC)
AC_MSG_RESULT(yes)
else
AC_MSG_RESULT(no)
fi
dnl END function name support


# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit])
AC_CHECK_FUNCS([calloc])
AC_CHECK_FUNCS([exit])
AC_CHECK_FUNCS([fflush])
AC_CHECK_FUNCS([fprintf])
AC_CHECK_FUNCS([free])
AC_CHECK_FUNCS([longjmp])
AC_CHECK_FUNCS([malloc])
AC_CHECK_FUNCS([setjmp])
AC_CHECK_FUNCS([siglongjmp])
AC_CHECK_FUNCS([signal])
AC_CHECK_FUNCS([sigsetjmp])
AC_CHECK_FUNCS([snprintf])
AC_CHECK_FUNCS([sprintf])
AC_CHECK_FUNCS([vsnprintf])


# The config file is generated but not used by the source code
#AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    e4c.pc
    Makefile
])


# Automake initialisation
AM_INIT_AUTOMAKE([
    -Wall
    -Werror
    foreign
	subdir-objects
    no-define
])


AC_OUTPUT
