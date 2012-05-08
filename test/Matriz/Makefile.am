# ----------------------------------------Informa��es relativas a subdiret�rios

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)

# -----------------------------------------Informa��es relativas a distribui��o
MAINTAINERCLEANFILES = Makefile.in Makefile *.pm3 

EXTRA_DIST = README CMatriz3D.*.pm3

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB} \
 	-I../../src
 	
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

INCLUDES = -I$(top_srcdir)/include

# ----------------------------Instru��es para montagem de biblioteca espec�fica

# -----------------------------------------Instru��es para montagem de programa

noinst_PROGRAMS = CMatriz3D.test

# Matriz3D usa diretamente a biblioteca libMatriz.a
CMatriz3D_test_SOURCES = CMatriz3D.test.cpp
CMatriz3D_test_LDADD =  ../../src/Matriz/libMatriz.a

# --------------------------------------------------------------------------FIM
