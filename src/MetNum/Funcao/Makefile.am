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
noinst_LIBRARIES = libFuncao.a
libFuncao_a_SOURCES =  CFChebyshev.cpp  CFGamma.cpp CFLegendre.cpp CFPolinomio.cpp \
		CFBesselJ.cpp CFCi.cpp CFHermite.cpp CFNaoLinearRzRy.cpp  \
		CFSi.cpp CFBesselY.cpp CFEi.cpp   CFIterativa.cpp  CFPHermite.cpp \
		CFSomatorioCm2RyM.cpp CFBeta.cpp  CFErf.cpp   CFLaguerre.cpp   \
		CFPHermiteRec.cpp CFuncoesEspeciais.cpp
libFuncao_a_HEADERS= *.h
libFuncao_adir=$(pkgincludedir)/Funcao

# --------------------------------------------------------------------------FIM



