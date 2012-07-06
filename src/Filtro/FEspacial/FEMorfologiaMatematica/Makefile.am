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
noinst_LIBRARIES = libFEMorfologiaMatematica.a
libFEMorfologiaMatematica_a_SOURCES =  \
CFEMMIDF.cpp CFEMorfologiaMatematica.cpp CFEMMIDFdj.cpp CFEMMIDFdijk.cpp \
CFEMMIDFdij.cpp CFEMMIDFdi.cpp CFEMMIDFdeuclidiana.cpp CFEMMIDFd8.cpp \
CFEMMIDFd5711.cpp CFEMMIDFd4.cpp CFEMMIDFd34.cpp CFEMMIDF.cpp 

libFEMorfologiaMatematica_a_HEADERS= *.h
libFEMorfologiaMatematica_adir=$(pkgincludedir)/Filtro/FEspacial/FEMorfologiaMatematica

# --------------------------------------------------------------------------FIM
