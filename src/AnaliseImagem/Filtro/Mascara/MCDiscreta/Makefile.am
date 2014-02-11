# ----------------------------------------Informações relativas a subdiretórios

# -------------------------------------------Informações relativas a instalação
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informações relativas a distribuição
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informações sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} \
		-I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instruções para montagem de biblioteca específica
noinst_LIBRARIES = libMCDiscreta.a
libMCDiscreta_a_SOURCES =  CMCDiscreta.cpp  \
			CMCdi.cpp  CMCd4.cpp  CMCd8.cpp  \
			CMCdij.cpp  CMCd34.cpp  \
			CMCdijk.cpp  CMCd5711.cpp  CMCEuclidiana.cpp
libMCDiscreta_a_HEADERS= *.h
libMCDiscreta_adir=$(pkgincludedir)/Mascara/MCDiscreta

# --------------------------------------------------------------------------FIM

# -I${PATH_LIB_LMPT_INCLUDES} -I${PATH_LIB_LDSC}
