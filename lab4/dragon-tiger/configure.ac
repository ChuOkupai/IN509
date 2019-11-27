AC_INIT([dragon-tiger], [submission], [])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall subdir-objects foreign])
AM_SILENT_RULES([yes])
AC_CONFIG_HEADERS([config.h])
AC_PROG_BISON([2.7])
AC_PROG_YACC
AC_PROG_LEX
AC_PROG_CXX
AM_PROG_AS
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
AM_PROG_AR
AC_PROG_RANLIB

AC_REQUIRE_AUX_FILE([tap-driver.sh])

AX_BOOST_BASE([1.48],, [AC_MSG_ERROR([dragon-tiger needs Boost, but it was not found in your system])])
AX_BOOST_PROGRAM_OPTIONS

AX_LLVM([3.8],[8.9],[all])


AC_SUBST(LLVM_CPPFLAGS, $LLVM_CPPFLAGS)
AC_SUBST(LLVM_LDFLAGS, $LLVM_LDFLAGS)
AC_SUBST(LLVM_LIBS, $LLVM_LIBS)
AC_PATH_PROG([LLVM_AS], [llvm-as], [llvm-as], [$LLVM_BINDIR/$PATH_SEPARATOR$PATH])
AC_PATH_PROG([LLVM_LLC], [llc], [llc], [$LLVM_BINDIR/$PATH_SEPARATOR$PATH])
AC_PATH_PROG([LLVM_OPT], [opt], [opt], [$LLVM_BINDIR/$PATH_SEPARATOR$PATH])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/driver/Makefile
                 src/irgen/Makefile
                 src/utils/Makefile
                ])

AC_OUTPUT
