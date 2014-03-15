#ifndef CETStudent_h
#define CETStudent_h

#ifndef CENormal_h
#include <Estatistica/CENormal.h>
#endif

/**
 * @brief Classe para cálculo da curva de TStudent.
 * 
 * Herdeira de CENormal.
 * O valor de z esta no intervalo -00 \<z< +00
 * ou seja z assume valores negativos e positivos.
 * A área assume valores entre 0 <= area <= 1.
 * Quanto maior o grau de liberdade (df1) mais a TStudent se aproxima da CENormal.
*/
class CETStudent : public CENormal
{
public:
  // double df1; // graus de libedade

public:
  /// Construtor
  CETStudent ();		

  /// Operador de atribuição
  CETStudent & operator= (const CETStudent & aCETStudent);	
  
  /// Destrutor
  virtual ~ CETStudent ()
  {
  }			

  /// Recebe z e determina a área.
  virtual double Go (double _z);

  /// Recebe a area e determina z.
  virtual double GoInv (double _area);	// retorna z

  // virtual double Go(double _z, double _df1);          // retorna area

  // virtual double GoInv(double _area, double _df1);    // retorna z

};

#endif
