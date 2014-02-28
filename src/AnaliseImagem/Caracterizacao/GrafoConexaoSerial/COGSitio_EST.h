#ifndef COGSitio_EST_h
#define COGSitio_EST_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       COGSitio_EST.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio.h>

// ===============================================================================
// Documentacao CLASSE: COGSitio_EST
// ===============================================================================
/** 
 * @brief Representa um sítio(COGSitio) conectado a face direita do grafo.
 * 
 * Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitio_EST
 * Assume valor de contorno = CContorno::EST.
 * O valor de contorno é usado no calculo dos fluxo da malha como um todo. 
 * Ou seja, um algoritimo externo percorre toda a malha, 
 * e se o contorno for igual ao solicitado calcula alguma propriedade neste contorno.
 * @author André Duarte Bueno
 * @see    grafos
 * @todo   renomear COGSitio_EST -> COGSitioContornoEST
*/
class COGSitio_EST : public COGSitio
{
// --------------------------------------------------------------Atributos
public:

// -------------------------------------------------------------Construtor
/// Construtor
 COGSitio_EST () = default;
 
// --------------------------------------------------------------Destrutor  
/// Destrutor
  virtual ~ COGSitio_EST ()  = default;
  
// ----------------------------------------------------------------Métodos

  // Redefinição de funções herdadas
   /// Retorna o tipo de contorno
  virtual CContorno::ETipoContorno Contorno ()const  override {
    return CContorno::ETipoContorno::EST;
  }

   /**
    * @brief Função herdade da classe CParametroSolver usada para 
    * calcular o valor de x retorna a pressão na fronteira direita
   */
  virtual long double Go (long double d = 0)  override {
    return x;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend ostream& operator<< (ostream& os, COGSitio_EST& obj);
  // friend istream& operator>> (istream& is, COGSitio_EST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGSitio_EST& obj);
// istream& operator>> (istream& is, COGSitio_EST& obj);

#endif
