#ifndef TFEEsqueletoV13D_h
#define TFEEsqueletoV13D_h

/*
----------------------------------------------------------------------------
Bibliotecas e Defines
----------------------------------------------------------------------------
*/
#ifndef TFEEsqueleto3D_h
#include <TFiltro/TFEspacial3D/TFEEsqueleto3D/TFEEsqueleto3D.h>
#endif

#ifndef TMatriz3D_h
#include <TMatriz/TMatriz3D.h>
#endif

#ifndef TFMMIDF3D_h
#include <TFiltro/TFEspacial3D/TFEMorfologiaMatematica3D/TFMMIDF3D.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LMPT
			Biblioteca para manipulacao de imagens
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas
			dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
			Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TFEEsqueletoV13D.h
Nome da classe:      TFEEsqueletoV13D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:    TFEEsqueletoV13D
============================================================================
Assunto:             Classe para esqueletiza��o
Superclasse:         TFiltro->TFiltroEspacial
Descri��o:           Classe desenvolvida para teste de alguns tipos de algor�timos
		     de esqueletiza��o.

Acesso:           	import
Cardinalidade:    	1:1    ,
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/
class TFEEsqueletoV13D:public TFEEsqueleto3D
{
//Atributos
protected:
  TFMMIDF3D * idf;		//

//M�todos
public:			//Construtor

TFEEsqueletoV13D (TMatriz3D * &matriz, unsigned int _tamanhoMascara = 0):TFEEsqueleto3D (matriz, _tamanhoMascara),
    idf
    (0)
  {
  };

/*	TFEEsqueletoV13D( TFMMIDF*& idf, unsigned int _tamanhoMascara )
			:  TFEEsqueleto3D(matriz, _tamanhoMascara ),idf(_idf)	{ };    */

  ~TFEEsqueletoV13D ()
  {
    if (idf)
      delete idf;
  };				//Destrutor

  virtual TMatriz3D *Go (TMatriz3D * &matriz, unsigned int _tamanhoMascara =
			 0);

protected:
};
#endif // TFEEsqueletoV13D_h
