# ----------------------------------------Informa��es relativas a subdiret�rios
## Diretorios com arquivos do Imago estao em: coilib-imago

SUBDIRS = \
	Base ConEqu Contorno Correlacao Estatistica FFT Filtro Funcao \
	Geometria Geometria Grafico Grafo Integral Interpolacao Mascara \
	Material Matriz Permeabilidade Reconstrucao Rotulador SFuncao \
	SMatriz Tempo

## Lista de diretorios que nao vao ser processados, mas vao ser instalados 
DIST_SUBDIRS =  \
	Base ConEqu Contorno Correlacao Estatistica FFT Filtro Funcao \
	Geometria Geometria Grafico Grafo Integral Interpolacao Mascara \
	Material Matriz Permeabilidade Reconstrucao Rotulador SFuncao \
	SMatriz Tempo
	

# -------------------------------------------Informa��es relativas a instala��o
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informa��es relativas a distribui��o
EXTRA_DIST = gera_lib  *.xmi gera_lib instala_lib listaDiretorios* *.am 

# -------------------------------------------Informa��es sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} -I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instru��es para montagem de biblioteca espec�fica
inst_LIBRARIES = libldsc.a
libldsc_a_SOURCES = 
libldsc_a_HEADERS= 
#libldsc_adir=$(pkgincludedir)
libldsc_a_LIBADD = \
	Base/*.o \
	Tempo/*.o \
	Material/*.o \
	Matriz/*.o \
	Rotulador/*.o \
     Mascara/*.o \
     Mascara/MCDiscreta/*.o \
	Filtro/*.o \
	Filtro/FAmplitude/*.o \
     Filtro/FEspacial/*.o \
	Filtro/FEspacial/FEDeteccaoContorno/*.o \
	Filtro/FEspacial/FEMorfologiaMatematica/*.o \
	Filtro/FEspacial/FEEsqueleto/*.o \
	Filtro/FEspacial/FEInversao/*.o \
     Filtro/FEspacial3D/*.o \
     Filtro/FEspacial3D/FEConectividade3D/*.o \
     Filtro/FEspacial3D/FEInversao3D/*.o \
     Filtro/FEspacial3D/FEMorfologiaMatematica3D/*.o \
     Filtro/FReconstrucao/*.o \
	Contorno/*.o \
	Funcao/*.o \
	Estatistica/*.o \
	Integral/*.o \
	SFuncao/*.o \
	Geometria/Bola/BCDiscreta/*.o \
	Geometria/Bola/BCDiscreta3D/*.o \
	Grafico/Gnuplot/*.o \
	Interpolacao/*.o \
	Interpolacao/Interpolacao2D/*.o \
	Interpolacao/IntMetNum/*.o \
	FFT/*.o \
	Correlacao/*.o \
	SMatriz/*.o \
	SMatriz/SMDiagonal/*.o \
	Permeabilidade/*.o \
	Grafo/*.o \
	Reconstrucao/*.o \
	ConEqu/*.o 

#libldsc_a_LIBADD = $(top_builddir)/src/Tempo/libTempo.a \
#	$(top_builddir)/src/SMatriz/SMDiagonal/libSMDiagonal.a \
#	$(top_builddir)/src/SMatriz/libSMatriz.a \
#	$(top_builddir)/src/SFuncao/libSFuncao.a \
#	$(top_builddir)/src/Rotulador/libRotulador.a \
#	$(top_builddir)/src/Reconstrucao/libReconstrucao.a \
#	$(top_builddir)/src/Permeabilidade/libPermeabilidade.a \
#	$(top_builddir)/src/Matriz/libMatriz.a \
#	$(top_builddir)/src/Material/libMaterial.a \
#	$(top_builddir)/src/Mascara/MCDiscreta/libMCDiscreta.a \
#	$(top_builddir)/src/Interpolacao/Interpolacao2D/libInterpolacao2D.a \
#	$(top_builddir)/src/Interpolacao/IntMetNum/libIntMetNum.a \
#	$(top_builddir)/src/Interpolacao/libInterpolacao.a \
#	$(top_builddir)/src/Integral/libIntegral.a \
#	$(top_builddir)/src/Grafo/libGrafo.a \
#	$(top_builddir)/src/Geometria/Bola/BCDiscreta3D/libBCDiscreta3D.a \
#	$(top_builddir)/src/Geometria/Bola/BCDiscreta/libBCDiscreta.a \
#	$(top_builddir)/src/Funcao/libFuncao.a \
#	$(top_builddir)/src/Filtro/libFiltro.a \
#	$(top_builddir)/src/Filtro/FAmplitude/libFAmplitude.a \
#	$(top_builddir)/src/Filtro/FEspacial/FEMorfologiaMatematica/libFEMorfologiaMatematica.a \
#	$(top_builddir)/src/Filtro/FEspacial/FEEsqueleto/libFEEsqueleto.a \
#	$(top_builddir)/src/Filtro/FEspacial/FEDeteccaoContorno/libFEDeteccaoContorno.a \
#	$(top_builddir)/src/Filtro/FEspacial/libFEspacial.a \
#	$(top_builddir)/src/Filtro/FEspacial3D/libFEspacial3D.a \
#	$(top_builddir)/src/Filtro/FEspacial3D/FEConectividade3D/libFEConectividade3D.a \
#	$(top_builddir)/src/Filtro/FEspacial3D/FEMorfologiaMatematica3D/libFEMorfologiaMatematica3D.a \
#	$(top_builddir)/src/FFT/libFFT.a \
#	$(top_builddir)/src/Estatistica/libEstatistica.a \
#	$(top_builddir)/src/Correlacao/libCorrelacao.a \
#	$(top_builddir)/src/Contorno/libContorno.a \
#	$(top_builddir)/src/ConEqu/libConEqu.a \
#	$(top_builddir)/src/Base/libBase.a

libldsc_adir=$(pkgincludedir)

# -----------------------------------------Instru��es para montagem de programa
bin_PROGRAMS = libldsc
libldsc_SOURCES = libldsc.cpp

# --------------------------------------------------------------------------FIM
## Usando libtool
##lib_LTLIBRARIES = liblib_lmpt.la
##liblib_lmpt_la_LIBADD=Base/libBase.a Grafo/libTGrafo.a Matriz/libTMatriz.a Rotulador/libTRotulador.a \
##SMatriz/libTSMatriz.a Permeabilidade/libTPermeabilidade.a Contorno/libTContorno.a \
##Material/libTMaterial.a Tempo/libTTempo.a
##liblib_lmpt_la_SOURCES=
##liblib_lmpt_la_LDFLAGS=


