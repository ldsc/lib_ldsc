# ----------------------------------------Informações relativas a subdiretórios
SUBDIRS =   MCDiscreta

# -------------------------------------------Informações relativas a instalação
instdir = $(pkglibdir)
INCLUDES = $(all_includes)

# -----------------------------------------Informações relativas a distribuição
EXTRA_DIST = *.cpp *.h Makefile.am

# -------------------------------------------Informações sobre flags adicionais
AM_CXXFLAGS = -I${PATH_LIB_LDSC}/src -I${PATH_LIB_IMAGO} \
		-I${PATH_LIB_COILIB}
DEFS = -D__LINUX__ -D__INTEL__ -D__X11__ -D__MESA__

# ----------------------------Instruções para montagem de biblioteca específica
noinst_LIBRARIES = libMascara.a
libMascara_a_SOURCES = CMascara.cpp CMPassaBaixa.cpp CMPAssaAlta.cpp \
			CMBartlet.cpp CMEmboss.cpp CMLaplaciano.cpp  \
			CMDeteccaoContorno.cpp CMGaussiano.cpp \
			CMPassaAlta2.cpp CMRotacaoVerticaleDiferenca.cpp \
			CMascara3D.cpp  CMPassaBaixa3d.cpp  
libMascara_a_HEADERS= *.h
libMascara_adir=$(pkgincludedir)/Mascara

# --------------------------------------------------------------------------FIM
