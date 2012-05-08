# ----------------------------------------Informa��es relativas a subdiret�rios

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am *.dat *.log
MAINTAINERCLEANFILES = Makefile.in Makefile 

# -------------------------------------------Informa��es sobre flags adicionais
## AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB} -I/usr/local/include/cc++2
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}  -I../../../src

DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE

## LIBS= -pthread    -L/usr/local/lib -lccgnu2 -ldl -WL,--rpath -WL,/usr/local/lib  
LIBS= -pthread    -L/usr/local/lib  -ldl -WL,--rpath -WL,/usr/local/lib  

# ----------------------------Instru��es para montagem de biblioteca espec�fica

# ----------------------------------------Instru��es para montagem de programas
bin_PROGRAMS =  correlacao
correlacao_SOURCES = correlacao.cpp
correlacao_LDADD   = ../../src/libldsc.a
# --------------------------------------------------------------------------FIM

