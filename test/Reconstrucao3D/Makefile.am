
# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h *.txt 320220.cor correlacao.cor saidaRzRy.rzy \
		saida.img \
		gt1 gt2

MAINTAINERCLEANFILES = Makefile.in Makefile 

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} \
	-I${PATH_LIB_COILIB} -I/usr/local/include/cc++2 -I../../src
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE

## LIBS= -pthread    -L/usr/local/lib -lccgnu2 -ldl -WL,--rpath -WL,/usr/local/lib  
LIBS= -pthread    -L/usr/local/lib -ldl -WL,--rpath -WL,/usr/local/lib  

# ----------------------------Instru��es para montagem de biblioteca espec�fica

# ----------------------------------------Instru��es para montagem de programas
bin_PROGRAMS = reconstrucao3d 
reconstrucao3d_SOURCES =  reconstrucao3d.cpp
reconstrucao3d_LDADD = $(top_builddir)/src/libldsc.a

bin_PROGRAMS += testeRzRy
testeRzRy_SOURCES =  TesteRzRy.cpp mainTesteRzRy.cpp
testeRzRy_LDADD   = ../../src/libldsc.a
# --------------------------------------------------------------------------FIM


