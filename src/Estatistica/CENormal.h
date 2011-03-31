#ifndef CENormal_h
#define CENormal_h

#include <cmath>

/**
 * @brief Determina curva normal (gaussiana).
 * 
 * fz=normal(z)
 *          *
 *         ***
 *        ******      area=área
 *      **********
 *    **************
 *  *******************
 * ------------------------->
 *                           zn
 * O valor de z esta no intervalo -00 \< z < +00
 * ou seja z assume valores negativos e positivos.
 * A área assume valores entre 0 <= area <= 1.
 * 
 * Obs: A classe CENormal utiliza para seu cálculo polinômios que aproximam a
 * integral da equação normal. 
 * Se o erro admitido for muito pequeno, então o cálculo efetivo da integral, é realizada utilizando-se
 * um método de integração numérica enbutido. Posteriormente usar um objeto de integração numérica.
 * 
 * Tarefa:
 * Pode ser herdeira de CFuncao!
*/

class CENormal	
{
public:
  double z;		///< Coordenada z, posição onde se deseja determinar a normal

  double area;		///< Valor da integral da equação normal

  // double fz;         ///< Coordenada z da curva normal (implementar)

  double df1;		///< Utilizado nas classes herdeiras (CETStudent)

  double df2;		///< Utilizado nas classes herdeiras (TXQuadrado)

  double limiteErro;	///< O limite de erro define qual função de cálculo vai ser utilizada

protected:

 /// Calcula area usando polinomio
 virtual double Go1 (double _z);	

 /// Calcula area usando integração
 virtual double Go2 (double _z);	

 /// Calcula z usando polinomio
 virtual double GoInv1 (double _area);	
 
 /// Calcula z usando integração
 virtual double GoInv2 (double _area);	
 
public:
 /// Construtor
 CENormal ();			
    
 /// Destrutor
 virtual ~ CENormal ()
  {
  }

 /// Sobrecarga do operador = 
 CENormal & operator= (const CENormal & aCENormal);	
 
 /// Calcula a integral da equação normal
 virtual double Go (double _z);	
 
 /// Dado o valor da integral calcula z
 virtual double GoInv (double _area);	

 /// Funções sobrecarregadas para uso específico
 double Go (double _z, double _df1, double _df2 = 1.0)
  {
    df1 = _df1;

    df2 = _df2;

    return Go (z);
  }

 /// Funções sobrecarregadas para uso específico
 double GoInv (double _area, double _df1, double _df2 = 1.0)
  {

    df1 = _df1;

    df2 = _df2;

    return GoInv (_area);
  }
 // double SetLimiteErro(double _limiteErro){limiteErro=_limiteErro;};

};

#endif
