
#ifndef CFESobel_h
#define CFESobel_h

#ifndef TCFEspacial_h
#include <Filtro/FEspacial/TCFEspacial.h>
#endif

/**
 * @brief Implementa o filtro de Sobel.
 * Herdeiro de CFEspacial.
*/
class CFESobel : public TCFEspacial<int>
{
public:
  /// Construtor
  CFESobel (TCMatriz2D<int> * matriz) : TCFEspacial<int> (matriz, 3)
  {
  }

  /// Processa o filtro.
  virtual TCMatriz2D<int> *Go ();
};

#endif //  FiltroSobel_h
