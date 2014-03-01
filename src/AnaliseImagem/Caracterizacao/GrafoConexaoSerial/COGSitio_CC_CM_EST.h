#ifndef COGSitio_CC_CM_EST_h
#define COGSitio_CC_CM_EST_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico 
            [LDSC].
@author     André Duarte Bueno
@file       COGSitio_CC_CM_EST.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_CC_CM.h>

// ===============================================================================
// Documentacao CLASSE: COGSitio_CC_CM_EST
// ===============================================================================
/** 
 * @brief  Representa um sítio que armazena a informação associada a conexão.
 * @short      	redefine funcao Contorno
 * @author 	André Duarte Bueno
 * @see		CGrafo
 * @Superclasse:
 * CParametroSolver->CObjetoGrafo->COGSitio->COGSitio_CC->COGSitio_CC_CM->COGSitio_CC_CM_EST
 */
class COGSitio_CC_CM_EST : public COGSitio_CC_CM
{
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
/// Construtor
    COGSitio_CC_CM_EST () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor  
  virtual ~ COGSitio_CC_CM_EST () = default;

// ----------------------------------------------------------------Métodos
public:
  // Redefinição de funções herdadas
   /// Retorna o tipo de contorno
  virtual CContorno::ETipoContorno Contorno () const {
        return CContorno::ETipoContorno::EST;
  }

   /** 
    * @brief Função herdada da classe CParametroSolver,
    * usada para calcular o valor de x.
    * Observe que retorna direto o valor de x, ou seja,
    * um objeto da fronteira não recalcula seu fluxo*/
  virtual long double Go (long double d = 0)  {
    return x;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend ostream& operator<< (ostream& os, COGSitio_CC_CM_EST& obj);
  // friend istream& operator>> (istream& is, COGSitio_CC_CM_EST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGSitio_CC_CM_EST& obj);
// istream& operator>> (istream& is, COGSitio_CC_CM_EST& obj);

#endif
