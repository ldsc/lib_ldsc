#ifndef CEChiQuadrado_h
#define CEChiQuadrado_h

#ifndef CENormal_h
#include <Estatistica/CENormal.h>
#endif

/**
 * @brief Classe utilizada para determinar valores da curva ChiQuadrado.
 * 
 * Herdeira de CENormal.
 * 
 * O valor de z esta no intervalo: 0 < z < +00
 * ou seja z assume somente valores positivos.
  * A área assume valores entre 0 <= area <= 1.
  * Quanto maior o grau de liberdade (df1) o pico da curva CEChiQuadrado 
  * se desloca da esquerda para a direita,
  * e seu formato se aproxima do formato da normal.
*/
class CEChiQuadrado : public CENormal
{
public:

  /// Construtor
  CEChiQuadrado ();		

  /// Operador de atribuição
  CEChiQuadrado & operator= (const CEChiQuadrado & aCEChiQuadrado);

  /// Destrutor
  virtual ~ CEChiQuadrado ()
  {
  }
  
  /// Para determinado valor de z determina valor da Area.
  virtual double Go (double _z);	// retorna area

  /// Para determinado valor de area determina valor da z.
  virtual double GoInv (double _area);	// retorna z

};

#endif
