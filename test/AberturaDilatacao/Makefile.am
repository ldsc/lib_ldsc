# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST =   *.h

MAINTAINERCLEANFILES = Makefile.in Makefile 

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB} \
		-I/usr/lib64 -I../../src

DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE

LIBS= -pthread -ldl -WL,--rpath -L/usr/lib64  
##-lccgnu2

# ----------------------------Instru��es para montagem de biblioteca espec�fica

# ----------------------------------------Instru��es para montagem de programas
bin_PROGRAMS = abertura_dilatacao
abertura_dilatacao_SOURCES = CAberturaDilatacao.teste.cpp CUtil.cpp
abertura_dilatacao_LDADD = $(top_builddir)/src/libldsc.a

# --------------------------------------------------------------------------FIM
