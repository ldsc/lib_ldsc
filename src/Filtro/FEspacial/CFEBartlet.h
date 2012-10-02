// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEBartlet_h
#define CFEBartlet_h

// inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <Filtro/FEspacial/CFEspacial.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong,
		Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEBartlet.h
Nome da classe:      CFEBartlet
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro de bartlet, CFEBartlet.
 * 
 * Herdeiro de CFEspacial.
 * 
 * Cria e utiliza uma máscara pseudo-gaussiana para atuar sobre a imagem.
 * O filtro de bartlet se equivale a aplicação do filtro passa alta duas vezes.
 * (confirmar?)
 * O resultado geral é a atenuação dos contornos.
 * 
*/

class CFEBartlet : public CFEspacial<int>
{
public:
		/// Construtor
  CFEBartlet (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)
    :CFEspacial<int> (matriz,_tamanhoMascara)
  {
  }
  
  /// Cria a mascara adequada
  virtual void CriaMascara (unsigned int _tamanhoMascara);	
};
#endif // CFEBartlet_h
