#ifndef CObjetoRedePercolacao_h
#define CObjetoRedePercolacao_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     Leandro Puerari
@file       CObjetoRedePercolacao.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by Leandro Puerari
@email      puerari@gmail.com
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
// Documentacao Classe: CObjetoRedePercolacao
// ===============================================================================
/**
 * @brief  é um objeto, herdeiro de CObjetoRede, acrescenta as coordenadas cx,cy,cz.
 * @author 	Leandro Puerari
 * @see		grafos
 * @ingroup  HCObjetoGrafo
 * @todo: implementar template para tipo de cx cy cz
*/
class CObjetoRedePercolacao : public CObjetoRede {
// --------------------------------------------------------------Atributos
public:
   float cx {0.0}; ///< Posição x do objeto na imagem tridimensional
	 float cy {0.0}; ///< Posição y do objeto na imagem tridimensional
	 float cz {0.0}; ///< Posição z do objeto na imagem tridimensional

// -------------------------------------------------------------/**Construtor*/
   /// Construtor
   CObjetoRedePercolacao () = default;

// --------------------------------------------------------------/**Destrutor*/
   /// Destrutor
   virtual ~ CObjetoRedePercolacao ()  = default;

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
   friend std::ostream& operator<< ( std::ostream& os, CObjetoRedePercolacao& obj );
   // friend istream& operator>> (istream& is, CObjetoRedePercolacao& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream& operator<< ( std::ostream& os, CObjetoRedePercolacao& obj );
// istream& operator>> (istream& is, CObjetoRedePercolacao& obj);

// Cria o tipo CObjetoRedePercolacaoCentro, que é igual a CObjetoRedePercolacao
using CObjetoRedePercolacao_Centro = CObjetoRedePercolacao ;

#endif
