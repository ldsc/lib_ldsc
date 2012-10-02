// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEGaussiano_h
#define CFEGaussiano_h

// inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <Filtro/FEspacial/TCFEspacial.h>
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
Nome deste arquivo:	CFEGaussiano.h
Nome da classe:      CFEGaussiano
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro gaussiano, CFEGaussiano.
 * 
 * Herdeiro de CFEspacial.
 * 
 * Cria e utiliza uma máscara gaussiana para atuar sobre a imagem.
 * O resultado geral é a atenuação dos contornos.
 */

class CFEGaussiano : public TCFEspacial<int>
{
public:
	/// Construtor
	CFEGaussiano (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)
		: TCFEspacial<int> (matriz,	  _tamanhoMascara) {
  }

/// Destrutor
  ~CFEGaussiano ()
  {
  }

    /// Cria a mascara adequada
  virtual void CriaMascara (unsigned int _tamanhoMascara);	
};
#endif //  CFEGaussiano_h
