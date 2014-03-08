#ifndef CObjetoRede_Final_h
#define CObjetoRede_Final_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_Final.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
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

#ifndef CObjetoRede_h
#include <EstruturaDados/ObjetoRede/CObjetoRede.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoRede_Final
// ===============================================================================
/**
 * @brief  Representa um objeto completo de uma rede, Não é herdeiro de CObjetoGrafo,
 * apenas de CSMParametroSolver.
 * Todos os atributos e métodos são criados aqui, de forma a não ter métodos virtuais.
 *
 * @author:  André Duarte Bueno
 * @see:     grafos
 * @ingroup  HCObjetoGrafo
*/
class CObjetoRede_Final final : public CObjetoRede {
// --------------------------------------------------------------Atributos
public:
   /// Acrescenta a informação do tipo de objeto do grafo; desta forma não
   /// precisa de um monte de heranças e classes EST WEST.
   /// O problema é que aumenta o consumo de memória em um uint8_t
   ETipoObjetoGrafo tipo;

// -------------------------------------------------------------Construtor
/// Construtor
   CObjetoRede_Final ( ETipoObjetoGrafo _tipo ) : tipo(_tipo) {   }

// --------------------------------------------------------------Destrutor
/// Destrutor
   /*virtual*/ ~ CObjetoRede_Final () = default;

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
   friend std::ostream& operator<< ( std::ostream& os, CObjetoRede_Final& obj );
//       friend istream& operator>> (istream& is, CObjetoRede_Final& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream& operator<< ( std::ostream& os, CObjetoRede_Final& obj );
// istream& operator>> (istream& is, CObjetoRede_Final& obj);

// Cria o tipo CObjetoRede_Final_CENTER, que é igual a CObjetoRede_Final
// typedef CObjetoRede_Final CObjetoRede_Final_CENTER;
// using CObjetoRede_Final_CENTER = CObjetoRede_Final;

#endif
