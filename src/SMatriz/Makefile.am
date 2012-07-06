# ----------------------------------------Informa��es relativas a subdiret�rios
SUBDIRS = SMDiagonal 

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src \
		    -I${PATH_LIB_IMAGO} \
		    -I${PATH_LIB_COILIB}
		    
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instru��es para montagem de biblioteca espec�fica
noinst_LIBRARIES = libSMatriz.a
libSMatriz_a_SOURCES = CSMatriz.cpp CSMParametroSolver.cpp 
libSMatriz_a_HEADERS = *.h
libSMatriz_adir=$(pkgincludedir)/SMatriz

# --------------------------------------------------------------------------FIM


