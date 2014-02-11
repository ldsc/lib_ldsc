#ifndef CMSobelVertical_h
#define CMSobelVertical_h

#include  <AnaliseImagem/Filtro/Mascara/CMascara.h>

class CMSobelVertical : public CMascara {
public:
  /// Construtor
   CMSobelVertical (unsigned int tamanhoMascara):CMascara (tamanhoMascara) {
      CMSobelVertical::PreencheMascara ();
   }

   /// Destrutor
   ~CMSobelVertical () { }

protected:
  /// Preenche a mascara com valores adequados.
   virtual void PreencheMascara ();

};
#endif // CMSobelVertical_h
