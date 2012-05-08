# ----------------------------------------Informa��es relativas a subdiret�rios
SUBDIRS = ConEqu \
	Correlacao \
	Reconstrucao3D \
	Estatistica \
	Filtro  \
	Geometria \
	Grafico \
	Grafo \
	Integral \
	Interpolacao \
	Rotulador \
	Matriz \
	AberturaDilatacao

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I../src  -I/usr/local/lib

DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE

LIBS= -pthread   -L/usr/local/lib -ldl -WL,--rpath -WL,/usr/local/lib

# ----------------------------Instru��es para montagem de biblioteca espec�fica

# ----------------------------------------Instru��es para montagem de programas
#bin_PROGRAMS = teste
#teste_SOURCES = CTeste.cpp main.cpp 
#teste_LDADD = $(top_builddir)/src/libldsc.a
# --------------------------------------------------------------------------FIM

