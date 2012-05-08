
# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am *.txt *.pm3
MAINTAINERCLEANFILES = Makefile.in Makefile 

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} \
	-I${PATH_LIB_COILIB}  -I../../../src -I/usr/local/include/cc++2
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE

## LIBS= -pthread    -L/usr/local/lib -lccgnu2 -ldl -WL,--rpath -WL,/usr/local/lib  
LIBS= -pthread    -L/usr/local/lib -ldl -WL,--rpath -WL,/usr/local/lib  

# ----------------------------Instru��es para montagem de biblioteca espec�fica

# ----------------------------------------Instru��es para montagem de programas

bin_PROGRAMS = confeq3D
confeq3D_SOURCES = confeq3D.cpp
confeq3D_LDADD = $(top_builddir)/src/libldsc.a

# --------------------------------------------------------------------------FIM


