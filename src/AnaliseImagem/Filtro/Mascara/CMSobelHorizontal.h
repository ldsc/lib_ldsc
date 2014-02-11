#ifndef CMSobelHorizontal_h
#define CMSobelHorizontal_h

#include  <AnaliseImagem/Filtro/Mascara/CMascara.h>

class CMSobelHorizontal : public CMascara {
public:
  /// Construtor
   CMSobelHorizontal (unsigned int tamanhoMascara):CMascara (tamanhoMascara) {
      CMSobelHorizontal::PreencheMascara ();
   }

   /// Destrutor
   ~CMSobelHorizontal () { }

protected:
  /// Preenche a mascara com valores adequados.
   virtual void PreencheMascara ();

};
#endif // CMSobelHorizontal_h
