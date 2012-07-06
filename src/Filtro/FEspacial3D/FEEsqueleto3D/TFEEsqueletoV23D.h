#ifndef TFEEsqueletoV23D_h
#define TFEEsqueletoV23D_h

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
Nome deste arquivo:	TFEEsqueletoV23D.h
Nome da classe:      TFEEsqueletoV23D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao CLASSE:    TFEEsqueletoV23D
============================================================================
Assunto:             Classe para esqueletização
Superclasse:         TFiltro->TFiltroEspacial
Descrição:           Classe desenvolvida para teste de alguns tipos de algorítimos
		de esqueletização.

Acesso:           	import
Cardinalidade:    	1:1    ,
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/
class TFEEsqueletoV23D:public TFEEsqueleto3D
{
//Atributos
protected:
  TFMMIDF3D * idf;		//

//Métodos
public:			//Construtor

TFEEsqueletoV23D (TMatriz3D * &matriz, unsigned int _tamanhoMascara = 0):TFEEsqueleto3D (matriz, _tamanhoMascara),
    idf
    (0)
  {
  };

/*	TFEEsqueletoV23D( TFMMIDF*& idf, unsigned int _tamanhoMascara )
			:  TFEEsqueleto3D(matriz, _tamanhoMascara ),idf(_idf)	{ };    */

  ~TFEEsqueletoV23D ()
  {
    if (idf)
      delete idf;
  };				//Destrutor

  virtual TMatriz3D *Go (TMatriz3D * &matriz, unsigned int _tamanhoMascara =
			 0);

protected:
};
#endif // TFEEsqueletoV23D_h
