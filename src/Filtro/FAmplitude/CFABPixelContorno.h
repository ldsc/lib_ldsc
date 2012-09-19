// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFABPixelContorno_h
#define CFABPixelContorno_h

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
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFABPixelContorno.h       (thrlap)
Nome da classe:      CFABPixelContorno
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Filtro de amplitude, baseado no contorno (usa laplaciano).
 * 
 * é um filtro que atua sobre o a amplitude da imagem, ou seja
 * sobre o espaço de cor da imagem.
 * 
 * Determina o nível de corte, a partir do histograma de níveis de cinza
 * calculado sobre os píxel's de contorno.
 * 0-Cria matrizLaplaciano, copia da matriz passada
 * 1-Executa o filtro laplaciano sobre a matrizLaplaciano
 * 2-Calcula o histograma da matrizLaplaciano
 * 3-Verifica o nívelCorteLaplaciano para o qual a matriz laplaciano tem 85% dos pontos ativos
 * 4-Na matriz Laplaciano elimina os pontos menores que nívelCorteLaplaciano
 * 5-Cria filtro de binarização CFABDoisPicos e executa passando a matrizLaplaciano .
*/

class CFABPixelContorno : public CFABinario
{
public:			 
  int PCHistLap;	///< Percentagem de corte no histograma dos pontos do laplaciano

public:			
	/// Construtor
  CFABPixelContorno (TMatriz2D< int > * &_pm, int _PCHistLap = 85)
    : CFABinario(_pm)
  {
    PCHistLap = _PCHistLap;
  }

		/// Destrutor
  ~CFABPixelContorno ()
  {
  }

  /// Realiza o processamento da filtragem
  virtual TMatriz2D< int > *Go (TMatriz2D< int > * &matriz, unsigned int _tamanhoMascara = 0);

};
#endif //  CFABPixelContorno_h
