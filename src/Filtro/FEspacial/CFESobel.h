
#ifndef CFESobel_h
#define CFESobel_h

#ifndef CFEspacial_h
#include <Filtro/FEspacial/CFEspacial.h>
#endif

/**
 * @brief Implementa o filtro de Sobel.
 * Herdeiro de CFEspacial.
*/
class CFESobel : public CFEspacial
{
public:
	/// Construtor
  CFESobel (CMatriz2D * matriz) : CFEspacial (matriz, 3)
  {
  }

  /// Processa o filtro.
  virtual CMatriz2D *Go ();
};
#endif //  FiltroSobel_h
