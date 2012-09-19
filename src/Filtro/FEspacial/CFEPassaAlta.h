// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEPassaAlta_h
#define CFEPassaAlta_h

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
Nome deste arquivo:	CFEPassaAlta.h
Nome da classe:      CFEPassaAlta
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro CFEPassaAlta.
 * 
 * Herdeiro de CFiltroEspacial.
 * 
 * Cria e utiliza uma máscara passa alta para atuar sobre a imagem.
 * O resultado geral é o aumento dos contornos. A identificação dos contornos.
 */

class CFEPassaAlta : public CFEspacial
{
public:
		/// Construtor
  CFEPassaAlta (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)	
  : CFEspacial (matriz, _tamanhoMascara)
  {
  }

    /// Cria a mascara adequada
  virtual void CriaMascara (unsigned int _tamanhoMascara);	

};
#endif //  CFEPassaAlta_h
