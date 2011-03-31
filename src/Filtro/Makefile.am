# ----------------------------------------Informa��es relativas a subdiret�rios
SUBDIRS = FAmplitude FEspacial FEspacial3D FReconstrucao

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instru��es para montagem de biblioteca espec�fica
noinst_LIBRARIES = libFiltro.a
libFiltro_a_SOURCES = CFiltro.cpp CFiltro3D.cpp 
libFiltro_a_HEADERS= *.h
libFiltro_adir=$(pkgincludedir)/Filtro

# --------------------------------------------------------------------------FIM
