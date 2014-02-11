// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEBSubstitueValor_h
#define CFEBSubstitueValor_h

// inclue a biblioteca de filtros
#ifndef CFEBorda_h
#include <AnaliseImagem/Filtro/FEspacial/CFEBorda.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas  dos Materiais.
Programadores:  	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEBSubstitueValor.h
Nome da classe:      CFEBSubstitueValor
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro CFEBSubstitueValor.
 * 
 * Herdeiro de CFEBorda.
 * 
 * Substitue os pontos da borda com valor i por j.
 * 
 * Ex:
 * Exemplo de uso: CFEBSubstitueValor(valorAntigo,novoValor).Go(imagem);
 */

class CFEBSubstitueValor : public CFEBorda
{
  int valorAntigo;

public:
		/// Construtor
  CFEBSubstitueValor (int _valorAntigo, int vB = 0, TCMatriz2D< int > * matriz = 0, 
		      unsigned int _tamanhoMascara = 0)
	:CFEBorda (vB, matriz, _tamanhoMascara),   
		    valorAntigo    (_valorAntigo)
  {
  }

  /// Destrutor
  virtual ~ CFEBSubstitueValor ()
  {
  }

    /// Cria a mascara adequada
  virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * &imagem, unsigned int espessura = 1);

};
#endif // CFEBSubstitueValor
