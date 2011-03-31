# ----------------------------------------Informa��es relativas a subdiret�rios

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instru��es para montagem de biblioteca espec�fica
noinst_LIBRARIES = libEstatistica.a
##libEstatistica_a_SOURCES = CEstatisticaBasica.cpp CERandomico.cpp \
libEstatistica_a_SOURCES = 	CERandomico.cpp   \
	CEstatistica.cpp  \
	CEFatorial.cpp    \
	CECombinacoes.cpp \
	CEMomentos.cpp    \
	CENormal.cpp      \
	CETStudent.cpp    \
	CEChiQuadrado.cpp \
	CEFSnedecor.cpp   \
	CEIntervalosConfianca.cpp    \
	CETesteMediaDuasAmostras.cpp 
libEstatistica_a_HEADERS= *.h
libEstatistica_adir=$(pkgincludedir)/Estatistica

# --------------------------------------------------------------------------FIM

