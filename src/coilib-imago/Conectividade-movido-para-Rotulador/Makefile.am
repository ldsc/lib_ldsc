####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libConectividade.a
instdir=$(pkglibdir)

INCLUDES = $(all_includes)

libConectividade_a_SOURCES = CFEConectividade2D.cpp  CFEConectividade3D.cpp
#CFEConectividade3D.cpp 

EXTRA_DIST = *.cpp *.h

libConectividade_a_HEADERS= *.h
libConectividade_adir=$(pkgincludedir)/Conectividade

####### kdevelop will overwrite this part!!! (end)############
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__
