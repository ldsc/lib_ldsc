//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CMCEuclidiana_h
#define CMCEuclidiana_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
		       	Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:      	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:         	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMCEuclidiana.h
Nome da classe:      CMCEuclidiana
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
//  #include "TMascara\TMascara.h"
//  #include "TMascara\CMCDiscreta\CMCDiscreta.h"
#include <Base/CMath.h>
#include <Mascara/MCDiscreta/CMCdij.h>

//  ----------------------------------------------------------------------------
//  Classe:       CMCEuclidiana
//  ----------------------------------------------------------------------------
/**
 * @brief Mascara  de chanfro pseudo Euclidiana.
 * 
 *  Implementa as funções da classe CMCEuclidiana.
 * A CMCEuclidiana é uma matriz bidimensional cujos valores,
 * são 0 se a distância do ponto x,y for maior que o raio, e
 * 1 se menor.
 * A função PreencheMascara calcula a distância de cada ponto x,y
 * e determina se data2D[x][y]=0 ou 1.

 * Representa uma mascara de chanfro.* 
 */
class CMCEuclidiana : public CMCdij	
{
//  Metodos
public:		//  (1,sqrt(2)) multiplicados por 100
	///  Construtor, chama PreencheMascara
  CMCEuclidiana ():CMCdij (100, (int) CMath::MSQRT2 * 100, 1)
  {
  }

	///  Destrutor
  virtual ~ CMCEuclidiana ()
  {
  }

};

#endif
