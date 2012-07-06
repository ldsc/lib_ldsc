# ----------------------------------------Informações relativas a subdiretórios

SUBDIRS=Interpolacao2D IntMetNum

# -------------------------------------------Informações relativas a instalação
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informações relativas a distribuição
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informações sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instruções para montagem de biblioteca específica
noinst_LIBRARIES = libInterpolacao.a
libInterpolacao_a_SOURCES = CIntLagrange.cpp CINewtonDiff.cpp  \
		CINewtonDivDiff.cpp  CIntBarycentric.cpp  \
		CIntCubSpline.cpp  CIntEDBarycentric.cpp  CInterpolacao.cpp
libInterpolacao_a_HEADERS= *.h
libInterpolacao_adir=$(pkgincludedir)/Interpolacao

# --------------------------------------------------------------------------FIM

