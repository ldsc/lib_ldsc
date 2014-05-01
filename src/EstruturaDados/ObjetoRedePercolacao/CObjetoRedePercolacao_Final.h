#ifndef CObjetoRedePercolacao_Final_h
#define CObjetoRedePercolacao_Final_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     Leandro Puerari
@file       CObjetoRedePercolacao_Final.h

@copyright  (C) 2000 by Leandro Puerari
@email      puerari@gmail.com
*/

// // -----------------------------------------------------------------------
// // Bibliotecas C/C++
// // -----------------------------------------------------------------------
// // Interface disco
// #include <fstream>
// 
// // Classe container para vetores
// #include <vector>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// #ifndef _LIB_LDSC_CLASS_h
// #include <Base/_LIB_LDSC_CLASS.h>
// #endif

#ifndef CObjetoRedePercolacao_h
#include <EstruturaDados/ObjetoRedePercolacao/CObjetoRedePercolacao.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoRedePercolacao_Final
// ===============================================================================
/**
 * @brief  Representa um objeto completo de uma rede, Não é herdeiro de CObjetoGrafo,
 * apenas de CSolverMatriz_ParametroSolver.
 * Todos os atributos e métodos são criados aqui, de forma a não ter métodos virtuais.
 *
 * @author:  Leandro Puerari
 * @see:     grafos
 * @ingroup  HCObjetoGrafo
*/
class CObjetoRedePercolacao_Final final : public CObjetoRedePercolacao {
// --------------------------------------------------------------Atributos
public:
   /// Acrescenta a informação do tipo de objeto do grafo; desta forma não
   /// precisa de um monte de heranças e classes EST WEST.
   /// O problema é que aumenta o consumo de memória em um uint8_t
   ETipoObjetoGrafo tipo;

// -------------------------------------------------------------Construtor
/// Construtor
   CObjetoRedePercolacao_Final ( ETipoObjetoGrafo _tipo ) : tipo(_tipo) {   }

// --------------------------------------------------------------Destrutor
/// Destrutor
   /*virtual*/ ~ CObjetoRedePercolacao_Final () = default;

// ----------------------------------------------------------------Métodos
   /**
     * @brief Função usada para calcular novo valor de x (ex: pressão).
     */
   /*inline*//*virtual*/ long double Go ( long double d = 0 ) final;

// --------------------------------------------------------------------Get
   /// @brief Retorna o tipo de objeto.
   ETipoObjetoGrafo  Tipo () const  final {
      return tipo;
   }

// --------------------------------------------------------------------Set
   /// @brief Seta vetor de condutâncias.
   void Tipo ( ETipoObjetoGrafo _tipo ) {
      tipo = _tipo;
   }

   // -----------------------------------------------------------------Friend
   friend std::ostream& operator<< ( std::ostream& os, CObjetoRedePercolacao_Final& obj );
//       friend istream& operator>> (istream& is, CObjetoRedePercolacao_Final& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream& operator<< ( std::ostream& os, CObjetoRedePercolacao_Final& obj );
// istream& operator>> (istream& is, CObjetoRedePercolacao_Final& obj);

// Cria o tipo CObjetoRedePercolacao_Final_CENTER, que é igual a CObjetoRedePercolacao_Final
// typedef CObjetoRedePercolacao_Final CObjetoRedePercolacao_Final_CENTER;
// using CObjetoRedePercolacao_Final_CENTER = CObjetoRedePercolacao_Final;

#endif
