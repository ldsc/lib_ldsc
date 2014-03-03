#ifndef CObjetoRede_Sitio_CC_EST_h
#define CObjetoRede_Sitio_CC_EST_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_Sitio_CC_EST.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio_CC.h>

// ===============================================================================
// Documentacao Classe: CObjetoRede_Sitio_CC_EST
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
 * @Superclasse:    CParametroSolver->CObjetoGrafo->CObjetoRede_Sitio->CObjetoRede_Sitio_CC->CObjetoRede_Sitio_CC_EST
 * @todo renomear CObjetoRede_Sitio_CC_EST -> CObjetoRede_Sitio_CCDir
 * @ingroup  HCObjetoGrafo
 */
class CObjetoRede_Sitio_CC_EST : public CObjetoRede_Sitio_CC {
// --------------------------------------------------------------Atributos
public:

// --------------------------------------------------------------Construtor
/// Construtor
     CObjetoRede_Sitio_CC_EST () = default;
// ---------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoRede_Sitio_CC_EST () = default;

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
     virtual long double Go ( long double d = 0 ) override   {
          return x;
     }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
     // friend ostream& operator<< (ostream& os, CObjetoRede_Sitio_CC_EST& obj);
     // friend istream& operator>> (istream& is, CObjetoRede_Sitio_CC_EST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CObjetoRede_Sitio_CC_EST& obj);
// istream& operator>> (istream& is, CObjetoRede_Sitio_CC_EST& obj);
#endif
