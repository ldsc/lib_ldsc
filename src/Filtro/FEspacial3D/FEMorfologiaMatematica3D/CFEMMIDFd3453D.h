// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFd3453D_h
#define CFEMMIDFd3453D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFd3453D.h
Nome da classe:      CFEMMIDFd3453D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:		CFEMMIDFd3453D
==================================================================================
Nome Classe:         CFEMMIDFd3453D
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         CFEMMIDFd3453D
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

// ----------------------------------------------------------------------------
// Classe:       CFEMMIDFd3453D
// ----------------------------------------------------------------------------
/**
 * @brief Matriz IDF de uma imagem usando mascara de chanfro d345.
 *
 */
class CFEMMIDFd3453D:public CFEMMIDFdijk3D
{
public:
  /// Construtor:
  // ponteiro para matriz imagem, tamanho da mascara a ser criada,raio máximo a ser usado
  CFEMMIDFd3453D (TCMatriz3D<int> * &matriz, int _indice=1, int _fundo=0) : CFEMMIDFdijk3D (matriz, 3, 4, 5, 1, _indice, _fundo)
  {
  }

  virtual ~ CFEMMIDFd3453D ()
  {
  }				// Destrutor

  // virtual TCMatriz3D<int> * Go( TCMatriz3D<int> *& matriz,unsigned int _tamanhoMascara=0);// redefinida

};

#endif // CFEMMIDFd345_h
