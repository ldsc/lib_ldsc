#ifndef TFEEsqueleto3D_h
#define TFEEsqueleto3D_h

/*
----------------------------------------------------------------------------
Bibliotecas e Defines
----------------------------------------------------------------------------
*/
//inclue a biblioteca de filtros
#ifndef TFEspacial3D_h
#include <AnaliseImagem/Filtro/FEspacial3D/FEspacial3D.h>
#endif

//inclue a biblioteca de filtros
#ifndef TMatriz3D_h
#include <AnaliseImagem/Matriz/TCMatriz3D.h>
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
Nome deste arquivo:	TFEEsqueleto3D.h
Nome da classe:      TFEEsqueleto3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:    TFEEsqueleto3D
============================================================================
Assunto:             Classe para esqueletiza��o
Superclasse:         TFiltro->TFiltroEspacial
Descri��o:

Acesso:           	import
Cardinalidade:    	1:1    ,
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/
#include <fstream.h>
class TFEEsqueleto3D:public TFEspacial3D
{
//Atributos
protected:
  int valorEsqueleto;		//valor final dos pixel's que pertencem ao esqueleto
  int valorObjeto;		//valor final dos pixel's que n�o pertencem ao esqueleto
  //mas fazem parte do objeto inicial (antes descascamento)
//M�todos
public:			//construtor
  TFEEsqueleto3D (TMatriz3D * &matriz, unsigned int _tamanhoMascara = 0):TFEspacial3D (matriz, _tamanhoMascara), valorEsqueleto (1),
    valorObjeto
    (0)
  {
  };

  ~TFEEsqueleto3D ()
  {
  };				//destrutor

//      virtual TMatriz3D * Go( TMatriz3D *& matriz,unsigned int _tamanhoMascara=0);
  void SetvalorEsqueleto (int ve)
  {
    valorEsqueleto = ve;
  };
  int GetvalorEsqueleto () const
  {
    return valorEsqueleto;
  };
  void SetvalorObjeto (int vo)
  {
    valorObjeto = vo;
  };
  int GetvalorObjeto () const
  {
    return valorObjeto;
  };

protected:
public:
  virtual void EliminaPeGalinha (TMatriz3D * imagem);	//
};
#endif // TFEEZhangSuen_h
