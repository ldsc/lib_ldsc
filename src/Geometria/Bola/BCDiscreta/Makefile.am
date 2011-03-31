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
noinst_LIBRARIES = libBCDiscreta.a
libBCDiscreta_a_SOURCES = CBCdi.cpp CBCdj.cpp CBCdij.cpp CBCdijk.cpp \
			  CBCd4.cpp CBCd8.cpp CBCd34.cpp CBCd5711.cpp  \
				CBCDiscreta.cpp  CBCEuclidiana.cpp
libBCDiscreta_a_HEADERS= *.h
libBCDiscreta_adir=$(pkgincludedir)/Geometria/Bola/BCDiscreta

# --------------------------------------------------------------------------FIM


