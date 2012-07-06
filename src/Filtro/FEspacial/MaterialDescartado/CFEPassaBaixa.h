// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEPassaBaixa_h
#define CFEPassaBaixa_h

// inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <Filtro/FEspacial/CFEspacial.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LMPT
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEPassaBaixa.h
Nome da classe:      CFEPassaBaixa
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
============================================================================
Documentacao CLASSE:    CFEPassaBaixa
============================================================================
Assunto:             Filtros espaciais (atuam sobre o espaço geométrico da imagem).
Superclasse:         CFiltro-> CFEspacial
Descrição:           Cria e utiliza uma máscara passa baixa para atuar sobre a imagem.
                     O resultado geral é a atenuação dos contornos, pois a mascara é formada
                     por números 1, e o resultado é uma méddia dos píxel's vizinhos.
Acesso:           import
Cardinalidade:    1:1    ,de uma maneira geral usa-se um filtro para atuar sobre uma imagem
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/

class CFEPassaBaixa : public CFEspacial
{
public:
  CFEPassaBaixa (CMatriz2D * &matriz,
		 unsigned int _tamanhoMascara)
                : CFEspacial (matriz,  _tamanhoMascara)
  {
  }

  virtual void CriaMascara (unsigned int _tamanhoMascara);	// Cria a mascara adequada
};
#endif // CFEPassaBaixa_h
