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
noinst_LIBRARIES = libFEDeteccaoContorno.a
libFEDeteccaoContorno_a_SOURCES = CFEDCCanny.cpp  CFEDCMarrHildreth.cpp  \
			CFEDCShenCastan.cpp  CFEDeteccaoContorno.cpp
libFEDeteccaoContorno_a_HEADERS= *.h
libFEDeteccaoContorno_adir=$(pkgincludedir)/Filtro/FEspacial/FEDeteccaoContorno

# --------------------------------------------------------------------------FIM



