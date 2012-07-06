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
noinst_LIBRARIES = libIntegral.a
libIntegral_a_SOURCES = CIGauss.cpp CIGHermiteQuadrature.cpp   \
	CIGLegendreQuadrature.cpp   CIGChebyshevQuadrature.cpp \
	CIGLaguerreQuadrature.cpp  CIntegral.cpp   CISimpson.cpp
libIntegral_a_HEADERS= *.h
libIntegral_adir=$(pkgincludedir)/Integral

# --------------------------------------------------------------------------FIM

