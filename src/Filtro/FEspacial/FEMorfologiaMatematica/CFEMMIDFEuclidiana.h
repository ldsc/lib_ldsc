//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFEuclidiana_h
#define CFEMMIDFEuclidiana_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFEuclidiana.h
Nome da classe:      CFEMMIDFEuclidiana
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:		CFEMMIDFEuclidiana
==================================================================================
Nome Classe:         CFEMMIDFEuclidiana
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         CFEMMIDFEuclidiana
Descrição:
Acesso:              import
Cardinalidade:
Concorrência:
Transformacoes:
Especificacoes da linguagem:
Persistencia:
Tamanho:
Abstrata/Concreta:   Concreta
Arquivo de documentacao auxiliar:
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Base/CMath.h>
#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdij.h>

//  ----------------------------------------------------------------------------
//  Classe:       CFEMMIDFEuclidiana
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro pseudo euclidiana.
 */
class CFEMMIDFEuclidiana:public CFEMMIDFdij
{
public:			//  mi,  mj,               rb
		/// Construtor
  CFEMMIDFEuclidiana (CMatriz2D * &matriz, int _indice=1, int _fundo=0)
    : CFEMMIDFdij (matriz, 100,	(int)CMath::MSQRT2 * 100, 100, _indice, _fundo)
  {
  }

  ///  Destrutor
  virtual ~ CFEMMIDFEuclidiana ()
  {
  }				

public:
  //  virtual CMatriz2D * Go( CMatriz2D *& matriz,unsigned int _tamanhoMascara=0);
  //  virtual CMatriz2D*  Abertura(CMatriz2D*& matriz,unsigned int _RaioBola);    //  
  //  virtual CMatriz2D*  Erosao(CMatriz2D*& matriz,unsigned int _RaioBola);    //  
  //  virtual CMatriz2D*  Dilatacao(CMatriz2D*& matriz,unsigned int _RaioBola); //  
  //  virtual CMatriz2D*  Fechamento(CMatriz2D*& matriz,unsigned int _RaioBola);//  

protected:
	/// Cria mascara. //  Cria bola  euclidiana?.
  virtual void CriaMascara (unsigned int _tamanhoMascara);	

};
#endif //  CFEMMIDFEuclidiana_h
