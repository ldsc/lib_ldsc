// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFMMIDFEuclidiana_h)
#define CFMMIDFEuclidiana_h
/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFEuclidiana.h
Nome da classe:      CFMMIDFEuclidiana
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao CLASSE:		CFMMIDFEuclidiana
============================================================================
Nome Classe:         CFMMIDFEuclidiana
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         CFMMIDFEuclidiana
Descri��o:
Acesso:              import
Cardinalidade:
Concorr�ncia:
Transformacoes:
Especificacoes da linguagem:
Persistencia:
Tamanho:
Abstrata/Concreta:   Concreta
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <Base/TMath.h>
#include <TFiltro/TFEspacial/TFEMorfologiaMatematica/CFMMIDFdij.h>

// ----------------------------------------------------------------------------
// Classe:       CFMMIDFEuclidiana
// ----------------------------------------------------------------------------
class CFMMIDFEuclidiana:public CFMMIDFdij
{
public:			// mi,  mj,               rb
  CFMMIDFEuclidiana (CMatriz2D * &matriz):CFMMIDFdij (matriz, 100,
						      TMath::MSQRT2 * 100,
						      100)
  {
  }

  virtual ~ CFMMIDFEuclidiana ()
  {
  }				// Destrutor

public:
  // virtual CMatriz2D * Go( CMatriz2D *& matriz,unsigned int _tamanhoMascara=0);
  // virtual CMatriz2D*  Abertura(CMatriz2D*& matriz,unsigned int _RaioBola);    // 
  // virtual CMatriz2D*  Erosao(CMatriz2D*& matriz,unsigned int _RaioBola);    // 
  // virtual CMatriz2D*  Dilatacao(CMatriz2D*& matriz,unsigned int _RaioBola); // 
  // virtual CMatriz2D*  Fechamento(CMatriz2D*& matriz,unsigned int _RaioBola);// 

protected:
  virtual void CriaMascara (unsigned int _tamanhoMascara);	// Cria bola  euclidiana

};
#endif // CFMMIDFEuclidiana_h
