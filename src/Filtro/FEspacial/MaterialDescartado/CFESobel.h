
#ifndef CFiltroSobel_h
#define CFiltroSobel_h

#ifndef CFiltroEspacial_h
#include <Filtro/FEspacial/CFiltroEspacial.h>
#endif

//  class definition requiered ==================================================
class CFiltroSobel : public CFiltroEspacial
{
public:
  CFiltroSobel (CMatriz2D * matriz) : CFiltroEspacial (matriz, 3)
  {
  }

  // bool CanGo( const CMatriz2D *rawImg ) const { return (rawImg->GetBPP() == 8); }
  virtual CMatriz2D *Go ();
};
#endif //  FiltroSobel_h
