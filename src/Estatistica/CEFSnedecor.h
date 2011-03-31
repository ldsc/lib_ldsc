#ifndef CEFSnedecor_h
#define CEFSnedecor_h

#ifndef CENormal_h
#include <Estatistica/CENormal.h>
#endif

class CEFSnedecor : public CENormal
{
public:
  /// Construtor default.
  CEFSnedecor ();

  /// Construtor de cópia
  CEFSnedecor & operator= (const CEFSnedecor & aCEFSnedecor);	// Operator= Assignment Operator

  /// Destrutor  
  virtual ~ CEFSnedecor ()
  {
  }

  /// Retorna valor da área a partir de z.
  virtual double Go (double _z);

  /// Retorna valor de z a partir da área
  virtual double GoInv (double _area);	

};

#endif

