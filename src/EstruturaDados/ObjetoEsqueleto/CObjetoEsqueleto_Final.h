#ifndef CObjetoEsqueleto_Final_h
#define CObjetoEsqueleto_Final_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoEsqueleto_Final.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/
/*
-------------------------------------------------------------------------
Bibliotecas ANSI C++
-------------------------------------------------------------------------
*/
/*
-------------------------------------------------------------------------
Bibliotecas LIB_LDSC
-------------------------------------------------------------------------
*/
#ifndef CObjetoEsqueleto_h
#include <EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoEsqueleto_Final
// ===============================================================================
/**
 * @brief  é um objeto, herdeiro de CObjetoRede, acrescenta as coordenadas cx,cy,cz.
 * @author 	André Duarte Bueno
 * @see		grafos
 * @ingroup  HCObjetoGrafo
 * @todo: implementar template para tipo de cx cy cz
*/
class CObjetoEsqueleto_Final final : public CObjetoEsqueleto {
// --------------------------------------------------------------Atributos
public:
   /// Armazena a informação do tipo de objeto do grafo; desta forma não
   /// precisa de um monte de heranças e classes EST WEST.
   /// O problema é que aumenta o consumo de memória em um uint8_t
   ETipoObjetoGrafo tipo;
   
// -------------------------------------------------------------/**Construtor*/
   /// Construtor
   CObjetoEsqueleto_Final ( ETipoObjetoGrafo _tipo ) : tipo(_tipo) {   }

// --------------------------------------------------------------/**Destrutor*/
   /// Destrutor
   /*virtual*/ ~ CObjetoEsqueleto_Final ()  = default;

// ----------------------------------------------------------------Métodos
   /**
     * @brief Função usada para calcular novo valor de x (ex: pressão).
     */
   /*inline*//*virtual*/ long double Go ( long double d = 0 ) override ;

// --------------------------------------------------------------------Get
   /// @brief Retorna o tipo de objeto.
   ETipoObjetoGrafo  Tipo () const override  {
      return tipo;
   }

// --------------------------------------------------------------------Set
   /// @brief Seta vetor de condutâncias.
   void Tipo ( ETipoObjetoGrafo _tipo ) {
      tipo = _tipo;
   }
// -----------------------------------------------------------------Friend
   /// Sobrecarga do operador <<.
   friend std::ostream& operator<< ( std::ostream& os, CObjetoEsqueleto_Final& obj );
   // friend istream& operator>> (istream& is, CObjetoEsqueleto_Final& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream& operator<< ( std::ostream& os, CObjetoEsqueleto_Final& obj );
// istream& operator>> (istream& is, CObjetoEsqueleto_Final& obj);

#endif
