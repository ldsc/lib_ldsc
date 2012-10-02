// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFABinario_h
#define CFABinario_h

#ifndef CFAmplitude_h
#include <Filtro/FAmplitude/CFAmplitude.h>
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
Nome deste arquivo:	CFABinario.h
Nome da classe:      CFABinario
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Filtro de amplitude, realiza a binarizacao usando um valor de corte (nivel).
 * 
 * é um filtro que atua sobre a amplitude da imagem, ou seja
 * sobre o espaço de cor da imagem.
 * 
 * O CFABinario realiza a binarizacao usando um valor de corte (nivel).
 * 
 * Se o valor de cor do pixel for maior que o nivel de corte, assume o valor 1
 * caso contrário assume valor  0.
 * A aplicacao deste filtro é baseada no histograma de níveis de cinza.
 * 
 * O usuário analisa o histograma de nível de cinza e toma a decisao sobre o nivel
 * de corte ideal. Então aplica o filtro binario.
*/
template<typename T>
class CFABinario : public CFAmplitude<T>
{
protected:
  unsigned int nivel;		///< Nível de corte

public:
				/// Construtor
	CFABinario (TCMatriz2D< T > * &_pm, unsigned int _nivel = 128)
		: CFAmplitude<T> (_pm),
		nivel (_nivel) {
  }

				/// Destrutor
  ~CFABinario ()
  {
  }

  /// Realiza o processamento da filtragem
	virtual TCMatriz2D< T > *Go (TCMatriz2D< T > * &matriz, unsigned int _tamanhoMascara = 0);

  /// Define nivel de corte
  unsigned int Level () const
  {
    return nivel;
  }				/// Obtem nível de corte

  /// Define nivel de corte
  void Level (unsigned int _nivel)
  {
    nivel = _nivel;
  }				
};
#endif //  CFABinario_h
