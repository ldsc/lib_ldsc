// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEPassaBaixa_h
#define CFEPassaBaixa_h

// inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <Filtro/FEspacial/TCFEspacial.h>
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
Nome deste arquivo:	CFEPassaBaixa.h
Nome da classe:      CFEPassaBaixa
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro passa baixa, CFEPassaBaixa.
 * Herdeiro de CFiltroEspacial.
 * 
 * Cria e utiliza uma máscara passa baixa para atuar sobre a imagem.
 * O resultado geral é a atenuação dos contornos, pois a mascara é formada
 * por números 1, e o resultado é uma média dos píxel's vizinhos.
 */
class CFEPassaBaixa : public TCFEspacial<int>
{
public:
		/// Construtor
	CFEPassaBaixa (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)
		: TCFEspacial<int> (matriz,  _tamanhoMascara)
  {
  }

    /// Cria a mascara adequada
  virtual void CriaMascara (unsigned int _tamanhoMascara);	
};
#endif // CFEPassaBaixa_h
