// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFABDoisPicos_h
#define CFABDoisPicos_h

#ifndef CFABinario_h
#include <Filtro/FAmplitude/CFABinario.h>
#endif

#include <Matriz/CHistograma.h>

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
Nome deste arquivo:	CFABDoisPicos.h      (twopeaks)
Nome da classe:      CFABDoisPicos
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/** @brief Filtro de amplitude, método de calculo baseado na determinação
 * dos dois picos de níveis de cinza na imagem.
 * 
 * é um filtro que atua sobre o a amplitude da imagem, ou seja
 * sobre o espaço de cor da imagem.
 * 
 * Método de calculo da posição de corte (threshold)
 * baseado na determinação dos dois picos de níveis de cinza na imagem
 * ideal para imagem com duas regiões de níveis de cinza
 * 
*/

class CFABDoisPicos : public CFABinario
{
public:

	/// Construtor			
  CFABDoisPicos (CMatriz2D * &_pm):CFABinario (_pm)
  {
  }			

				/// Destrutor
  ~CFABDoisPicos ()
  {
  }

  /// Realiza o processamento da filtragem
  virtual CMatriz2D *Go (CMatriz2D * &matriz, unsigned int _tamanhoMascara = 0);

  /// Função que determina o nivel de corte a partir do histograma da imagem
  unsigned int determinaNivelCorte (CHistograma * &hist); 

};
#endif //  CFABDoisPicos_h
