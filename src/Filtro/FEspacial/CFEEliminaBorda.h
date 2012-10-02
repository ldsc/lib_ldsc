// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEEliminaBorda_h
#define CFEEliminaBorda_h

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
Nome deste arquivo:	CFEEliminaBorda.h
Nome da classe:      CFEEliminaBorda
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro CFEEliminaBorda.
 * 
 * Herdeiro de CFEspacial.
 * Elimina a borda de uma imagem, substituindo pontos da borda, por pontos
 * definidos pela funcao ValorBorda.
 * O valor default para a borda é zero (0).
 * 
 * Ex:
 * CFEEliminaBorda(valorBorda).Go(imagem,espessuraBorda);
 */

class CFEEliminaBorda : public TCFEspacial<int>
{
  int valorBorda;

public:
		/// Construtor
  CFEEliminaBorda (int vB = 0, TCMatriz2D< int > * matriz = 0, unsigned int _tamanhoMascara = 0)
		: TCFEspacial<int> (matriz, _tamanhoMascara),    valorBorda    (vB)
  {
  }

  /// Destrutor
  ~CFEEliminaBorda ()
  {
  }

  /// Cria a mascara adequada
  virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * imagem, int espessura);

  /// Seta valor da borda
  void ValorBorda (int v)
  {
    valorBorda = v;
  }

  /// Obtêm valor da borda
  int ValorBorda () const
  {
    return valorBorda;
  }
};
#endif // CFEEliminaBorda_h
