####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libTConEquCOILIB.a
instdir=$(pkglibdir)
INCLUDES = $(all_includes)

libTConEquCOILIB_a_SOURCES = TIConfEq3DSim.cpp TIConfEq3DSimPermRel.cpp TIConfEq3DSimPermRelEsq3Dby2D.cpp TISChamber.cpp TISCMercuryInjectionInternal.cpp TISCMercuryInjection6F.cpp TISCMercuryInjection4F.cpp TISCMercuryInjection2F.cpp TISCMercuryInjection1F.cpp TISCImbibition.cpp TISCDraynage.cpp TIConfiguracoesEquilibrio3D.cpp TIChamber.cpp TICChamber.cpp 

EXTRA_DIST = *.cpp *.h

libTConEquCOILIB_a_HEADERS= *.h
libTConEquCOILIB_adir=$(pkgincludedir)/TConEquCOILIB

####### kdevelop will overwrite this part!!! (end)############
#Novo define as paths
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB} -I/usr/local/include/cc++2
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__  -D_GNU_SOURCE 
AM_LDFLAGS = -pthread  -L/usr/local/lib  -l/usr/local/lib/ccgnu2
