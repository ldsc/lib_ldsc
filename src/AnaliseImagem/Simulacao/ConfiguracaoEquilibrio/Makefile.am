# ----------------------------------------Informa��es relativas a subdiret�rios

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} \
		-I${PATH_LIB_COILIB}  -I/usr/local/include/cc++2
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

AM_LDFLAGS = -pthread  -L/usr/local/lib  -l/usr/local/lib/ccgnu2

# ----------------------------Instru��es para montagem de biblioteca espec�fica
noinst_LIBRARIES = libConEqu.a
libConEqu_a_SOURCES = CConfiguracoesEquilibrio2D.cpp \
			CConfiguracoesEquilibrio3D.cpp \
		 	CCamara.cpp CCamara2D.cpp CCamara3D.cpp
libConEqu_a_HEADERS= *.h
libConEqu_adir=$(pkgincludedir)/ConEqu

# --------------------------------------------------------------------------FIM

