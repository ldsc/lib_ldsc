####### kdevelop will overwrite this part!!! (begin)##########
inst_LIBRARIES = libFEspacial.a
INCLUDES = $(all_includes)
libFEspacial_a_SOURCES = CFEspacial.cpp CFEBartlet.cpp CFEDeteccaoContornoMasc.cpp \
			CFEGaussiano.cpp CFEPassaAlta.cpp  CFEBorda.cpp \
			CFEEliminaBorda.cpp CFELaplaciano.cpp CFEPassaBaixa.cpp \
			 CFEBSubstitueValor.cpp CFEEmboss.cpp \
			CFEPassaAlta2.cpp CFERotacaoVerticalEDiferenca.cpp
## CFESobel.cpp esta no formato da coi-lib, precisa retornar para formato antigo
## Antigo: CFiltroEspacial.cpp

## Também peço para instalar os arquivos *.h no diretório /usr/local/include/lib_lmpt/Base
libFEspacial_a_HEADERS= *.h
libFEspacial_adir=$(pkgincludedir)/Filtro/FEspacial
## instdir=/usr/local/lib
instdir=$(pkglibdir)

EXTRA_DIST = *.cpp *.h

####### kdevelop will overwrite this part!!! (end)############
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__
