

#FEConectividade3D FEEsqueleto3D

inst_LIBRARIES = libFEMorfologiaMatematica3D.a

INCLUDES = $(all_includes)

libFEMorfologiaMatematica3D_a_SOURCES =  CFEMMDeslocamento3D.cpp  CFEMMIDF3d.cpp  CFEMMIDFdijk3d.cpp  \
					CFEMMIDFd3453D.cpp  CFEMMIDFeuclidiana3d.cpp  CFEMorfologiaMatematica3D.cpp

## Também peço para instalar os arquivos *.h no diretório /usr/local/include/lib_lmpt/Base
libFEMorfologiaMatematica3D_a_HEADERS= *.h

libFEMorfologiaMatematica3D_adir=$(pkgincludedir)/Filtro/FEspacial3D/FEMorfologiaMatematica3D

## instdir=/usr/local/lib

instdir=$(pkglibdir)

EXTRA_DIST = *.cpp *.h

####### kdevelop will overwrite this part!!! (end)############
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}

DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__
