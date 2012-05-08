
bin_PROGRAMS = reconstrucao3d 
reconstrucao3d_SOURCES =  reconstrucao3d.cpp
reconstrucao3d_LDADD   = ../../src/liblenep.a

bin_PROGRAMS += testeRzRy
testeRzRy_SOURCES =  TesteRzRy.cpp mainTesteRzRy.cpp
testeRzRy_LDADD   = ../../src/liblenep.a


EXTRA_DIST = 



AM_CXXFLAGS = -I${PATH_LIB_LMPT_INCLUDES} -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB} -I/usr/local/include/cc++2
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__ -D_GNU_SOURCE

