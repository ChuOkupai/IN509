bin_PROGRAMS = dtiger

dtiger_SOURCES = driver.cc
dtiger_CXXFLAGS = -pedantic -Wall $(LLVM_CPPFLAGS) -fexceptions
dtiger_LDADD = ../ast/libast.a ../parser/libparser.a ../irgen/libirgen.a ../utils/libutils.a $(BOOST_PROGRAM_OPTIONS_LIB) $(LLVM_LIBS)
AM_LDFLAGS = $(BOOST_LDFLAGS) $(LLVM_LDFLAGS)
CLEANFILES=
