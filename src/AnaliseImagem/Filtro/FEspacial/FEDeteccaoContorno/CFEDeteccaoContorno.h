#ifndef CFEDeteccaoContorno_h
#define CFEDeteccaoContorno_h

/*
----------------------------------------------------------------------------
Bibliotecas e Defines
----------------------------------------------------------------------------
*/
#include <AnaliseImagem/Filtro/FEspacial/TCFEspacial.h>

/**
 * @brief Classe base para detecção de contorno usando diferentes métodos.
*/
class CFEDeteccaoContorno : public TCFEspacial<int>
{
	

public:			/// Construtor
	CFEDeteccaoContorno (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)
		: TCFEspacial<int> (matriz, _tamanhoMascara)
  {
  }

  	/// Destrutor
  ~CFEDeteccaoContorno ()
  {
  }			

};
#endif //  CFEDeteccaoContorno
