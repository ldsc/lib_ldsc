#ifndef CETesteMediaDuasAmostras_h
#define CETesteMediaDuasAmostras_h

#include <Estatistica/CEstatistica.h>

//                 + Testing the means of two samples
/** 
 * @brief Classe de estatística para duas amostras.
 * */
class CETesteMediaDuasAmostras:public CEstatistica
{
public:

  /// Construtor
  CETesteMediaDuasAmostras ():CEstatistica ()
  {
  }	

  /// Destrutor
  virtual ~ CETesteMediaDuasAmostras ();	

  /// Realiza o teste de duas amostras
  int TesteMediaDuasAmostras (CEstatistica & B, double testedDifference,
			      double _probabilidade, double &calcT,
			      double &tableT, int &passTest);

};
#endif
