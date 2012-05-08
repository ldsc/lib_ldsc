
# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am *.txt *.pbm
MAINTAINERCLEANFILES = Makefile.in Makefile 

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB} -I../../../src

DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE

LIBS= -pthread    -L/usr/local/lib -ldl -WL,--rpath -WL,/usr/local/lib  

# ----------------------------Instru��es para montagem de biblioteca espec�fica

# ----------------------------------------Instru��es para montagem de programas
bin_PROGRAMS = cgnuplot.teste cgnuplot.teste.min

cgnuplot_teste_SOURCES = CGnuplot.teste.cpp
cgnuplot_teste_LDADD = $(top_builddir)/src/libldsc.a

cgnuplot_teste_min_SOURCES =  CGnuplot.teste.min.cpp
cgnuplot_teste_min_LDADD = $(top_builddir)/src/libldsc.a

# --------------------------------------------------------------------------FIM

