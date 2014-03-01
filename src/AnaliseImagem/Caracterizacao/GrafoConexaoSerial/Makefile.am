# ----------------------------------------Informa��es relativas a subdiret�rios

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB} 
# -I/usr/local/include/cc++2
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

AM_LDFLAGS = -pthread  -L/usr/local/lib 
# -l/usr/local/lib/ccgnu2

# ----------------------------Instru��es para montagem de biblioteca espec�fica
noinst_LIBRARIES = libGrafo.a
libGrafo_a_SOURCES = CObjetoGrafo.cpp    COGComponenteGrafo.cpp  COGLigacao.cpp \
	COGSitio.cpp       COGSitio_EST.cpp       COGSitio_WEST.cpp \
	COGSitio_LR.cpp    COGSitio_LR_EST.cpp    COGSitio_LR_WEST.cpp \
	COGSitio_LR_CM.cpp COGSitio_LR_CM_EST.cpp COGSitio_LR_CM_WEST.cpp \
	CGrafo.cpp         CGrafoContorno.cpp     CGra3Dby2D.cpp \
	CGra3Dby2D_M1.cpp  CGra3Dby2D_M2.cpp      CGra3Dby2D_M3.cpp \
	CGra3Dby2D_M4.cpp  CGra3Dby2D_M5.cpp      CGra3Dby2D_M6.cpp  

##CGrafoTest.cpp movido para test
libGrafo_a_HEADERS= *.h
libGrafo_adir=$(pkgincludedir)/Grafo

# --------------------------------------------------------------------------FIM
