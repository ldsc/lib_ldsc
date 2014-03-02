#ifndef COGSitio_CC_CM_WEST_h
#define COGSitio_CC_CM_WEST_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       COGSitio_CC_CM_WEST.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_CC_CM.h>

// ===============================================================================
// Documentacao Classe: COGSitio_CC_CM_WEST
// ===============================================================================
/**
 * @brief Representa um sítio conectado a face esquerda do grafo e que
 * armazena a informação da conexão.
 *
 * Assume valor de contorno = CContorno::WEST.
 * O valor de contorno é usado no calculo dos fluxo da malha como
 * um todo. Ou seja, um algoritimo externo percorre todo
 * o grafo, e se o contorno for igual ao solicitado
 * calcula alguma propriedade neste contorno.
 * @see:        Grafo
 * Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitio_CC->COGSitio_CC_WEST
 * @author 	André Duarte Bueno
 * @ingroup  HCObjetoGrafo
*/
class COGSitio_CC_CM_WEST : public COGSitio_CC_CM {
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
/// Construtor
     COGSitio_CC_CM_WEST () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ COGSitio_CC_CM_WEST () = default;

// ----------------------------------------------------------------Métodos

public:
     /// Retorna o tipo
     virtual CContorno::ETipoContorno Contorno () const   {
          return CContorno::ETipoContorno::WEST;
     }

     /**
      * @brief Função herdade da classe CParametroSolver usada para
      * calcular o valor de x como é um objeto da fronteira,
      * não altera seu valor de x
     */
     virtual long double Go ( long double d = 0 )  {
          return x;
     }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend

//       friend ostream& operator<< (ostream& os, COGSitio_CC_CM_WEST& obj);
//       friend istream& operator>> (istream& is, COGSitio_CC_CM_WEST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGSitio_CC_CM_WEST& obj);
// istream& operator>> (istream& is, COGSitio_CC_CM_WEST& obj);

#endif
