# ----------------------------------------Informa��es relativas a subdiret�rios

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = 	-I${PATH_LIB_LDSC}/src \
			-I${PATH_LIB_IMAGO} \
			-I${PATH_LIB_COILIB} \
			-I/usr/lib64
			
			
# -I/usr/include/cc++

DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

LIBS= -pthread  -L/usr/lib64  -lccgnu2

# ----------------------------Instru��es para montagem de biblioteca espec�fica
noinst_LIBRARIES = libSMDiagonal.a
libSMDiagonal_a_SOURCES = CSMDGaussSeidel.cpp  \
  			CSMDSOR.cpp CSMDiagonalDominante.cpp  CSMDJacobi.cpp  \
  			CSMDiagonalDominanteThreads.cpp

libSMDiagonal_a_HEADERS= *.h
libSMDiagonal_adir=$(pkgincludedir)/SMatriz/SMDiagonal
# --------------------------------------------------------------------------FIM


