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
noinst_LIBRARIES = libBCDiscreta3D.a
libBCDiscreta3D_a_SOURCES = CBCd3453D.cpp  CBCdijk3D.cpp  CBCDiscreta3D.cpp \
			 CBCEuclidiana3D.cpp
libBCDiscreta3D_a_HEADERS= *.h
libBCDiscreta3D_adir=$(pkgincludedir)/Geometria/Bola/BCDiscreta3D

# --------------------------------------------------------------------------FIM

