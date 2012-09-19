// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFELaplaciano_h
#define CFELaplaciano_h

// inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <Filtro/FEspacial/CFEspacial.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFELaplaciano.h
Nome da classe:      CFELaplaciano
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro do laplaciano, CFELaplaciano.
 * 
 * Herdeiro de CFEspacial.
 * 
 * Cria e utiliza uma máscara laplaciana para atuar sobre a imagem.
 * 
 * O resultado geral é a detecção dos contornos dos objetos.
 * 
 */

class CFELaplaciano : public CFEspacial
{
public:
	/// Construtor
  CFELaplaciano (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)	
  : CFEspacial (matriz, _tamanhoMascara)
  {
  }

    /// Cria a mascara adequada
  virtual void CriaMascara (unsigned int _tamanhoMascara);	

};
#endif //  CFELaplaciano_h
