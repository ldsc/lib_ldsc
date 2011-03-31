# ----------------------------------------Informações relativas a subdiretórios

# -------------------------------------------Informações relativas a instalação
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informações relativas a distribuição
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informações sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instruções para montagem de biblioteca específica
noinst_LIBRARIES = libFEEsqueleto.a
libFEEsqueleto_a_SOURCES = CFEEsqueleto.cpp  \
				CFEEsqueletoV1.cpp  CFEEsqueletoV2.cpp  \
				CFEEsqueletoV3.cpp  CFEEsqueletoV4.cpp \
				 CFEEzhangsuen.cpp
libFEEsqueleto_a_HEADERS= *.h
libFEEsqueleto_adir=$(pkgincludedir)/Filtro/FEspacial/FEEsquelet

# --------------------------------------------------------------------------FIM

