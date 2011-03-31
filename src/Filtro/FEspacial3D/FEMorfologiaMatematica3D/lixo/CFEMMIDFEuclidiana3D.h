// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFEMMIDFEuclidiana3D_h)
#define CFEMMIDFEuclidiana3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFEuclidiana3D.h
Nome da classe:      CFEMMIDFEuclidiana3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao CLASSE:		CFEMMIDFEuclidiana3D
============================================================================
Nome Classe:         CFEMMIDFEuclidiana3D
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         CFEMMIDFEuclidiana3D
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

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMIDFdijk3D.h>
#include <Base/CMath.h>

// ----------------------------------------------------------------------------
// Classe:       CFEMMIDFEuclidiana3D
// ----------------------------------------------------------------------------
class CFEMMIDFEuclidiana3D:public CFEMMIDFdijk3D
{
public:
  // imagem,mi, mj               ,mk               ,rb
  CFEMMIDFEuclidiana3D (TMatriz3D * &matriz)
    : CFEMMIDFdijk3D (matriz, 100,   TMath::MSQRT2 *   100,
	       	   TMath::MSQRT3 *   100, 100)
  {
  }

  virtual ~ CFEMMIDFEuclidiana3D ()
  {
  }

};
#endif // CFEMMIDFEuclidiana3D_h
