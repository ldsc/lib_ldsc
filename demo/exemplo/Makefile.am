# ----------------------------------------Informações relativas a subdiretórios

# -------------------------------------------Informações relativas a instalação
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informações relativas a distribuição
EXTRA_DIST = *.cpp *.h Makefile.am *.dat *.log
MAINTAINERCLEANFILES = Makefile.in Makefile 

# -------------------------------------------Informações sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB} -I/usr/local/include/cc++2
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE

LIBS= -pthread    -L/usr/local/lib  -ldl -WL,--rpath -WL,/usr/local/lib  
##LIBS= -pthread    -L/usr/local/lib -lccgnu2 -ldl -WL,--rpath -WL,/usr/local/lib  

# ----------------------------Instruções para montagem de biblioteca específica

# ----------------------------------------Instruções para montagem de programas
# Nome do programa a ser gerado
bin_PROGRAMS = exemplo

# Arquivos com código fonte
exemplo_SOURCES = exemplo.cpp

## Inclui acesso a uma biblioteca interna
exemplo_LDADD = $(top_builddir)/src/Matriz/libMatriz.a

## Não linkar a biblioteca dinâmica
exemplo_LDFLAGS = -all-static
# --------------------------------------------------------------------------FIM

