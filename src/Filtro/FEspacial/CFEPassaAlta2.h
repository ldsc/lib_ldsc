// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEPassaAlta2_h
#define CFEPassaAlta2_h

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
Nome deste arquivo:	TFPassaAlta2.h
Nome da classe:      TFPassaAlta2
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro CFEPassaAlta2. 
 * Herdeiro de CFEspacial.
 * 
 * Cria e utiliza uma máscara passa alta para atuar sobre a imagem.
 * O resultado geral é o aumento dos contornos. A identificação dos contornos.
 * 
 */
class CFEPassaAlta2 : public CFEspacial
{
public:
		/// Construtor
  CFEPassaAlta2 (CMatriz2D * &matriz, unsigned int _tamanhoMascara)	
  : CFEspacial (matriz, _tamanhoMascara)
  {
  }

    /// Cria a mascara adequada
  virtual void CriaMascara (unsigned int _tamanhoMascara);
};
#endif //  TFPassaAlta2_h
