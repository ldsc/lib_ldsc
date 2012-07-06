# ----------------------------------------Informa��es relativas a subdiret�rios
SUBDIRS = FEMorfologiaMatematica3D FEConectividade3D FEInversao3D

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instru��es para montagem de biblioteca espec�fica
noinst_LIBRARIES = libFEspacial3D.a
libFEspacial3D_a_SOURCES =  CFEspacial3D.cpp CFEBorda3D.cpp CFEBSubstitueValor3D.cpp 
libFEspacial3D_a_HEADERS = *.h
libFEspacial3D_adir = $(pkgincludedir)/Filtro/FEspacial3D

# --------------------------------------------------------------------------FIM
