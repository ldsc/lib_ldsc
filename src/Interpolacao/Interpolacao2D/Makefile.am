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
noinst_LIBRARIES = libInterpolacao2D.a
libInterpolacao2D_a_SOURCES = CInterpolacao2D.cpp  CInt2DBilinear.cpp
libInterpolacao2D_a_HEADERS= *.h
libInterpolacao2D_adir=$(pkgincludedir)/Interpolacao/Interpolacao2D

# --------------------------------------------------------------------------FIM

