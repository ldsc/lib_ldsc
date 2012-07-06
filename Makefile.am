# ----------------------------------------Informa��es relativas a subdiret�rios
## Diretorios a serem processados
SUBDIRS = src demo test #docs

## Diretorios a serem incluidos na distribuicao
DIST_SUBDIRS = src  demo test docs data #debug  #templates

## Para gerar distribuicoes em outros formatos adicione os itens
AUTOMAKE_OPTIONS = foreign dist-shar dist-zip dist-bzip2

## extraido do exemplo hello da gnu
MAINT_CHARSET = latin1

# -------------------------------------------Informa��es relativas a instala��o
# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = AUTHORS COPYING ChangeLog INSTALL README TODO  \
		ORGANIZATION HOWTO  \
		Makefile.am *.kde*

# -------------------------------------------Informa��es sobre flags adicionais
# Sem Threads
#AM_CXXFLAGS = -I${PATH_LIB_LMPT_INCLUDES} -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
#LDFLAGS= -L/usr/local/lib

#Com threads
## AM_CXXFLAGS = -I${PATH_LIB_LMPT_INCLUDES} -I${PATH_LIB_IMAGO} \
##	-I${PATH_LIB_COILIB} -I/usr/local/include/cc++2

## AM_CXXFLAGS = -I${LIB_LDSC_INCLUDES} -I${PATH_LIB_IMAGO} \
##	-I${PATH_LIB_COILIB} -I/usr/local/include/cc++2

AM_CXXFLAGS =  -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}

DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE
AM_CXXFLAGS2 = @AM_CXXFLAGS@
#LDFLAGS= -pthread  -L/usr/local/lib  -lccgnu2 -llibccgnu2 -l/usr/local/lib/libccgnu2.a

# ----------------------------Instru��es para montagem de biblioteca espec�fica

# --------------------------------------------------------------------------FIM
#/* A saida obtida na compilacao e dada por:
# g++ -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__
# -I/home/andre/Andre/Desenvolvimento/LIB_LMPT/lib_lmpt/include
# -I/usr/include/imago/
# -I/usr/include/coi20/
# -I/usr/local/include/cc++2
# -D_GNU_SOURCE
# -pthread
# -L/usr/local/lib
# -c TSMDiagonalDominanteThreads.cpp
#*/
