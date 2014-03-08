#ifndef CObjetoEsqueleto_h
#define CObjetoEsqueleto_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoEsqueleto.h
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
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CObjetoRede_h
#include <EstruturaDados/ObjetoRede/CObjetoRede.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoEsqueleto
// ===============================================================================
/**
 * @brief  é um objeto, herdeiro de CObjetoRede, acrescenta as coordenadas cx,cy,cz.
 * @author 	André Duarte Bueno
 * @see		grafos
 * @ingroup  HCObjetoGrafo
 * @todo: implementar template para tipo de cx cy cz
*/
class CObjetoEsqueleto : public CObjetoRede {
// --------------------------------------------------------------Atributos
public:
   float cx {0.0}; ///< Posição x do objeto na imagem tridimensional
   float cz {0.0}; ///< Posição y do objeto na imagem tridimensional
   float cy {0.0}; ///< Posição z do objeto na imagem tridimensional

// -------------------------------------------------------------/**Construtor*/
   /// Construtor
   CObjetoEsqueleto () = default;

// --------------------------------------------------------------/**Destrutor*/
   /// Destrutor
   virtual ~ CObjetoEsqueleto ()  = default;

   /// Retorna o tipo de objeto do grafo.
   virtual ETipoObjetoGrafo Tipo () const  override {
      return ETipoObjetoGrafo::ObjetoEsqueleto;
   }

   /// @brief Salva atributos do objeto em disco.
   virtual std::ostream& Write ( std::ostream& os ) const override ;

// ----------------------------------------------------------------Métodos
// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
   /// Sobrecarga do operador <<.
   friend std::ostream& operator<< ( std::ostream& os, CObjetoEsqueleto& obj );
   // friend istream& operator>> (istream& is, CObjetoEsqueleto& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream& operator<< ( std::ostream& os, CObjetoEsqueleto& obj );
// istream& operator>> (istream& is, CObjetoEsqueleto& obj);

// Cria o tipo CObjetoEsqueletoCentro, que é igual a CObjetoEsqueleto
using CObjetoEsqueleto_Centro = CObjetoEsqueleto ;

#endif
