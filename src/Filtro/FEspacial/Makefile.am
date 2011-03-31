# ----------------------------------------Informações relativas a subdiretórios
SUBDIRS = FEDeteccaoContorno FEMorfologiaMatematica FEEsqueleto FEInversao

# -------------------------------------------Informações relativas a instalação
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informações relativas a distribuição
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informações sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instruções para montagem de biblioteca específica
inst_LIBRARIES = libFEspacial.a
libFEspacial_a_SOURCES = CFEspacial.cpp CFEBartlet.cpp CFEDeteccaoContornoMasc.cpp \
			CFEGaussiano.cpp CFEPassaAlta.cpp  CFEBorda.cpp \
			CFEEliminaBorda.cpp CFELaplaciano.cpp CFEPassaBaixa.cpp \
			 CFEBSubstitueValor.cpp CFEEmboss.cpp \
			CFEPassaAlta2.cpp CFERotacaoVerticalEDiferenca.cpp
libFEspacial_a_HEADERS= *.h
libFEspacial_adir=$(pkgincludedir)/Filtro/FEspacial

# --------------------------------------------------------------------------FIM
