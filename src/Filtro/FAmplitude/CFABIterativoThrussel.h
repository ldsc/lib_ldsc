// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFABIterativoThrussel_h
#define CFABIterativoThrussel_h

#ifndef CFABinario_h
#include <Filtro/FAmplitude/CFABinario.h>
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
Nome deste arquivo:	CFABIterativoThrussel.h      (thris)
Nome da classe:      CFABIterativoThrussel
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Filtro de amplitude, usa método iterativo, (Thrussel, 1979).
 * 
 * é um filtro que atua sobre o a amplitude da imagem, ou seja
 * sobre o espaço de cor da imagem.
 * Método iterativo, (Thrussel, 1979)
 * a partir de um nível de corte inicial, divide a imagem em duas regiões
 * depois para cada uma das regiões, calcula o nível de corte médio.
 * Isto é feito iterativamente, para diferentes níveis de corte
 * o processo termina quando não ocorrer mais variação do nível de corte
*/

class CFABIterativoThrussel : public CFABinario
{
public:
				/// Construtor
  CFABIterativoThrussel (CMatriz2D * &_pm):CFABinario (_pm)
  {
  }

				/// Destrutor
  ~CFABIterativoThrussel ()
  {
  }

  /// Realiza o processamento da filtragem
  virtual CMatriz2D *Go (CMatriz2D * &matriz, unsigned int _tamanhoMascara = 0);

};
#endif //  CFABIterativoThrussel_h
