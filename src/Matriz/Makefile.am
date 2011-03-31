# ----------------------------------------Informações relativas a subdiretórios

# -------------------------------------------Informações relativas a instalação
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informações relativas a distribuição
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informações sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instruções para montagem de biblioteca específica
inst_LIBRARIES = libMatriz.a
libMatriz_a_SOURCES = CImg.cpp CImagem3D.cpp CImagem.cpp \
			CHistograma.cpp CVetor.cpp CMatriz3D.cpp \
			CMatriz2D.cpp CBaseMatriz.cpp 
libMatriz_a_HEADERS= *.h
libMatriz_adir=$(pkgincludedir)/Matriz

# --------------------------------------------------------------------------FIM
