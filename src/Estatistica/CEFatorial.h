#ifndef CEFatorial_h
#define CEFatorial_h


#include <Base/CFuncao.h>

/**
 * @brief Classe para cálculo do fatorial.
 * 
 * Cria um vetor e armazena os valores de fatorial ate nMaximo
 * passado para o construtor.
 * O objetivo e acelerar o processo, evitando o recálculo do fatorial.
 * Se n passado for maior que nMaximo passado para o construtor,
 * calcula o fatorial a partir de nMaximo
 * mas neste caso o vetor de fatoriais nao é alterado.
 * */
class CEFatorial : public CFuncao
{
	
  double *fatorial; ///< Vetor com os fatoriais

  const int nMaximo; ///< Valor maximo determinado, tamanho do vetor de fatoriais

public:		
/// Construtor, nMax é o tamanho do vetor, se for negativo, não aloca o vetor
    CEFatorial (int nMax = -1);	

    /// Destrutor
    virtual ~ CEFatorial (); 

  /// Retorna o fatorial de x
  virtual double Go (double x = 0, double y = 0);
  
  /// Sobrecarga do operador (), chama Go().
  double operator()(double x = 0)
  {
    return Go(x);
  }

  /// Sobrecarga do operador[], retorna o fatorial de i
  double operator[](int i) // VERIFICAR
  {
    if(i <= nMaximo)
      return fatorial [i];
    else
      return Go(i);
  }
};
#endif
