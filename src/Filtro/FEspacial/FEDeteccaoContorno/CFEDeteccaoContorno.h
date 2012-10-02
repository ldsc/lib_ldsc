#ifndef CFEDeteccaoContorno_h
#define CFEDeteccaoContorno_h

/*
----------------------------------------------------------------------------
Bibliotecas e Defines
----------------------------------------------------------------------------
*/
#include <Filtro/FEspacial/CFEspacial.h>

/**
 * @brief Classe base para detecção de contorno usando diferentes métodos.
*/
class CFEDeteccaoContorno : public CFEspacial<int>
{
	

public:			/// Construtor
  CFEDeteccaoContorno (TCMatriz2D< int > * &matriz,
		       unsigned int _tamanhoMascara)
    : CFEspacial<int> (matriz, _tamanhoMascara)
  {
  }

  	/// Destrutor
  ~CFEDeteccaoContorno ()
  {
  }			

};
#endif //  CFEDeteccaoContorno
