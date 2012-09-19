// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFABEntropiaJohansen_h
#define CFABEntropiaJohansen_h

#ifndef CFABinario_h
#include <Filtro/FAmplitude/CFABinario.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, 
Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFABEntropiaJohansen.h  (thr_joh)
Nome da classe:      CFABEntropiaJohansen
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Filtro de amplitude, usa Método de Johannsen.
 * 
 * É um filtro que atua sobre o a amplitude da imagem, ou seja
 * sobre o espaço de cor da imagem.
 * Método de Johannsen , usa entropia
 * É uma melhoria no método da entropia.
*/
class CFABEntropiaJohansen : public CFABinario
{
public:			/// Construtor

  CFABEntropiaJohansen (TMatriz2D< int > * &_pm):CFABinario (_pm)
  {
  }

				/// Destrutor
  ~CFABEntropiaJohansen ()
  {
  }

  /// Realiza o processamento da filtragem
  virtual TMatriz2D< int > *Go (TMatriz2D< int > * &matriz, unsigned int _tamanhoMascara =0);

protected:

	/// Calcula a entropia
  float entropy (float h);	

  /// Calcula o logarítimo de número float

  float flog (float x);		
};
#endif //  CFABEntropiaJohansen_h
