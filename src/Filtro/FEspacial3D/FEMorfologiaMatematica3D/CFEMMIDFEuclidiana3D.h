// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFEuclidiana3D_h
#define CFEMMIDFEuclidiana3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFEuclidiana3D.h
Nome da classe:      CFEMMIDFEuclidiana3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
==================================================================================
Documentacao CLASSE:		CFEMMIDFEuclidiana3D
==================================================================================
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

#include <Base/CMath.h>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMIDFdijk3D.h>

// ----------------------------------------------------------------------------
// Classe:       CFEMMIDFEuclidiana3D
// ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem 3D usando mascara de chanfro pseudo euclidiana.
 */
class CFEMMIDFEuclidiana3D:public CFEMMIDFdijk3D
{
public:
  // imagem,mi, mj               ,mk               ,rb
  CFEMMIDFEuclidiana3D (CMatriz3D * &matriz, int _indice=1, int _fundo=0)
    : CFEMMIDFdijk3D (matriz, 100, (int)CMath::MSQRT2 * 100, (int)CMath::MSQRT3 * 100, 100, _indice, _fundo)
  {
  }

  virtual ~ CFEMMIDFEuclidiana3D ()
  {
  }
protected:
  /// Cria mascara CBCEuclidiana3D.
  virtual void CriaMascara (unsigned int _tamanhoMascara);
};
#endif // CFEMMIDFEuclidiana3D_h
