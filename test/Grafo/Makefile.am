# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST =   input_data *.i3d *.pm3 *.pm3.dat vetor_cmx.txt vetor_cmy.txt *.h

MAINTAINERCLEANFILES = Makefile.in Makefile 

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB} \
		-I/usr/lib64 -I../../src

DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE

LIBS= -pthread -ldl -WL,--rpath -L/usr/lib64  
##-lccgnu2

# ----------------------------Instru��es para montagem de biblioteca espec�fica

# ----------------------------------------Instru��es para montagem de programas
bin_PROGRAMS = grafo
grafo_SOURCES = grafo.cpp CGrafoTeste.cpp
grafo_LDADD = $(top_builddir)/src/libldsc.a

# --------------------------------------------------------------------------FIM
