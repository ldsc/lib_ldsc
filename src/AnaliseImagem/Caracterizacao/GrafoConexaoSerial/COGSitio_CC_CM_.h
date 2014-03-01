#ifndef COGSitio_CC_CM_h
#define COGSitio_CC_CM_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       COGSitio_CC_CM.h
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

#ifndef COGSitio_CC_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_CC.h>
#endif


// ===============================================================================
// Documentacao Classe: COGSitio_CC_CM
// ===============================================================================
/** 
 * @brief  é um sítio, herdeiro de COGSitio_CC, acrescenta as
 * posições cx,cy,cz.
 * @Assunto:        grafo
 * @Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitio_CC->COGSitio_CC_CM
 * 
 * @author 	André Duarte Bueno
 * @see		grafos
*/
class COGSitio_CC_CM : public COGSitio_CC
{
// --------------------------------------------------------------Atributos
public:
	double cx;  ///< Posição x do objeto na imagem tridimensional 
	double cz;  ///< Posição y do objeto na imagem tridimensional 
	double cy;  ///< Posição z do objeto na imagem tridimensional 

// -------------------------------------------------------------/**Construtor*/
/// Destrutor
  COGSitio_CC_CM ():cx (0), cy (0), cz (0)
  {
  }

// --------------------------------------------------------------/**Destrutor*/
/// Construtor
  virtual ~ COGSitio_CC_CM ()
  {
  }

// ----------------------------------------------------------------Métodos
  /**Salva atributos do objeto em disco redefinida*/
  // virtual std::ostream&  Write(std::ostream& os) const;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend std::ostream& operator<< (std::ostream& os, COGSitio_CC_CM& obj);

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// std::ostream& operator<< (std::ostream& os, COGSitio_CC_CM& obj);
// istream& operator>> (istream& is, COGSitio_CC_CM& obj);

// Cria o tipo COGSitio_CC_CMCentro, que é igual a COGSitio_CC_CM
using COGSitio_CC_Centro = COGSitio_CC_CM ;

#endif
