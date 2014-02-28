#ifndef COGSitio_LR_EST_h
#define COGSitio_LR_EST_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       COGSitio_LR_EST.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_LR.h>

// ===============================================================================
// Documentacao CLASSE: COGSitio_LR_EST
// ===============================================================================
/** 
 * @brief Representa um sítio que armazena a informação associada a conexão.
 * Assume valor de contorno = 2.
 * O valor de contorno é usado no calculo dos fluxo da malha como
 * um todo.Ou seja, um algoritimo externo percorre toda
 * a malha, e se o contorno for igual ao solicitado
 * calcula alguma propriedade neste contorno.
 * @author 	André Duarte Bueno	
 * @see			grafos
 * @Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitio_LR->COGSitio_LR_EST
 * @todo renomear COGSitio_LR_EST -> COGSitio_LRDir
 */
class COGSitio_LR_EST : public COGSitio_LR
{
// --------------------------------------------------------------Atributos
public:

// --------------------------------------------------------------Construtor
/// Construtor
 COGSitio_LR_EST () = default;
// ---------------------------------------------------------------Destrutor 
/// Destrutor
   virtual ~ COGSitio_LR_EST ()= default;

// -----------------------------------------------------------------Métodos
// Redefinição de funções herdadas

   /// Retorna o tipo de contorno
  virtual CContorno::ETipoContorno Contorno () const override  {
     return CContorno::ETipoContorno::EST;
  }

   /**
    * @brief Função herdada da classe CParametroSolver usada para 
    * calcular o valor de x retorna a pressão na fronteira direita
    * Observe que retorna direto o valor de x, ou seja,
    * um objeto da fronteira nao recalcula seu fluxo
   */
  virtual long double Go (long double d = 0) override   {
    return x;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend ostream& operator<< (ostream& os, COGSitio_LR_EST& obj);
  // friend istream& operator>> (istream& is, COGSitio_LR_EST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGSitio_LR_EST& obj);
// istream& operator>> (istream& is, COGSitio_LR_EST& obj);
#endif
