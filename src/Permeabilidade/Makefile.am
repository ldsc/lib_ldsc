# ----------------------------------------Informa��es relativas a subdiret�rios

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instru��es para montagem de biblioteca espec�fica
noinst_LIBRARIES = libPermeabilidade.a
libPermeabilidade_a_SOURCES = CPermeabilidade.cpp CPermeabilidadeGrafo.cpp \
	CPermeabilidadeIntrinseca.cpp CPermeabilidadeRelativa.cpp
libPermeabilidade_a_HEADERS= *.h
libPermeabilidade_adir=$(pkgincludedir)/Permeabilidade

# --------------------------------------------------------------------------FIM
noinst_HEADERS = CPermeabilidadeIntrinseca.h
