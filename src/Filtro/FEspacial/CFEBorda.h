// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEBorda_h
#define CFEBorda_h

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
Nome deste arquivo:	CFEBorda.h
Nome da classe:      CFEBorda
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro de borda, CFEBorda.
 * 
 * Herdeiro de CFiltroEspacial.
 * 
 * Filtros espaciais (atuam sobre o espaço geométrico da imagem).
 * 
 * Elimina a borda de uma imagem, substituindo pontos da borda por valorBorda
 * O valor default para a borda é zero (0).
 * 
 * Ex:
 * CFEBorda(valorBorda).Go(imagem,espessuraBorda);
*/
class CFEBorda : public TCFEspacial<int>
{
protected:
  int valorBorda;		///< Valor dos pontos da borda
//  int espessura;                   ///< Espessura da borda

public:
	/// Construtor
  CFEBorda (int vB = 0, TCMatriz2D< int > * matriz = 0, unsigned int _tamanhoMascara = 0)
		:TCFEspacial<int> (matriz, _tamanhoMascara),    valorBorda (vB)
  {
  }

  /// Destrutor
  ~CFEBorda ()
  {
  }

    /// Processa o filtro da borda.
  virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * &imagem, unsigned int espessura = 1);

  /// Seta o valor da borda
  void ValorBorda (int v)
  {
    valorBorda = v;
  }

  /// Obtêm o valor da borda
  int ValorBorda () const
  {
    return valorBorda;
  }
};
#endif // CFEBorda_h
