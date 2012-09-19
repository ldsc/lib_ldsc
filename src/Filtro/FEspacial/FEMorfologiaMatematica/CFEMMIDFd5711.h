//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFEMMIDFd5711_h)
#define CFEMMIDFd5711_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e
Propriedades Termofisicas  dos Materiais.
Programadores: Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,  Liang Zirong,
Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFd5711.h
Nome da classe:      CFEMMIDFd5711
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------

#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdijk.h>

//  ----------------------------------------------------------------------------
//  Classe:       CFEMMIDFd5711
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro d5711.
 */
class CFEMMIDFd5711:public CFEMMIDFdijk
{
public:			//  mi mj mk rb
		/// Construtor
  CFEMMIDFd5711 (TCMatriz2D< int > * &matriz, int _indice=1, int _fundo=0)
     : CFEMMIDFdijk (matriz, 5, 7, 11, 2, _indice, _fundo)
  {
  }
  
///  Destrutor
  virtual ~ CFEMMIDFd5711 ()
  {
  }	

};
#endif //  CFEMMIDFd5711_h
