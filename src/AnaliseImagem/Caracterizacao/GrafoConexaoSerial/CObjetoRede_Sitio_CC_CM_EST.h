#ifndef CObjetoRede_Sitio_CC_CM_EST_h
#define CObjetoRede_Sitio_CC_CM_EST_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_Sitio_CC_CM_EST.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio_CC_CM.h>

// ===============================================================================
// Documentacao Classe: CObjetoRede_Sitio_CC_CM_EST
// ===============================================================================
/**
 * @brief  Representa um sítio que armazena a informação associada a conexão.
 * @short      	redefine funcao Contorno
 * @author 	André Duarte Bueno
 * @see		CGrafo
 * @ingroup  HCObjetoGrafo
 * @Superclasse:
 * CParametroSolver->CObjetoGrafo->CObjetoRede_Sitio->CObjetoRede_Sitio_CC->CObjetoRede_Sitio_CC_CM->CObjetoRede_Sitio_CC_CM_EST
 */
class CObjetoRede_Sitio_CC_CM_EST : public CObjetoRede_Sitio_CC_CM {
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoRede_Sitio_CC_CM_EST () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoRede_Sitio_CC_CM_EST () = default;

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
     virtual long double Go ( long double d = 0 )  {
          return x;
     }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
     // friend ostream& operator<< (ostream& os, CObjetoRede_Sitio_CC_CM_EST& obj);
     // friend istream& operator>> (istream& is, CObjetoRede_Sitio_CC_CM_EST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CObjetoRede_Sitio_CC_CM_EST& obj);
// istream& operator>> (istream& is, CObjetoRede_Sitio_CC_CM_EST& obj);

#endif
