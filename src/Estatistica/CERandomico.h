#ifndef CERandomico_h
#define CERandomico_h

/**
 * @brief Gera números aleatórios.
 * 
 * O construtor recebe uma semente, que se não for passada, assume valor 0.
 * A funcao SetMin e SetMax é utilizada para passar os atributos
 * minimos e maximos, que sao utilizados para definir o intervalo
 * de geracao do numero aleatorio, o default é min=0 max=1.
*/

class CERandomico		/*:public CEstatistica  */
{
  // atritubos
 public:

  float minimo;		///< valor mínimo do numero aleatorio (inclusive)

  float maximo;		///< valor máximo do numero aleatorio (inclusive)

  float intervalo;	///< intervalo

 protected:
  long int semente;	///< semente do gerador

 private:
  double ran2 (long *idum);	///< função de cálculo do número randomico

  // metodos
 public:
	 
  /// Construtor
  CERandomico (int _semente = 0)
    {
      minimo = 0;

      maximo = 1;

      intervalo = 1;

      semente = _semente;
      // randomize();// inicializa gerador de numero randomico do C++
    }				// construtor

    /// destrutor
  virtual ~ CERandomico ()
    {
    }				

  /// Seta valor mínimo
  inline void Minimo (float min)
    {
      minimo = min;
      intervalo = maximo - minimo;
    }

  /// Seta valor máximo
  inline void Maximo (float max)
    {
      maximo = max;
      intervalo = maximo - minimo;
    }

/// Calcula a funcao em si y=f(x)
  virtual double Go (double x = 0, double y = 0);	

};
#endif
