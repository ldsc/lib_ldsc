#ifndef COGSitio_LR_CM_h
#define COGSitio_LR_CM_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       COGSitio_LR_CM.h
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

#ifndef COGSitio_LR_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_LR.h>
#endif


// ===============================================================================
// Documentacao CLASSE: COGSitio_LR_CM
// ===============================================================================
/** 
 * @brief  é um sítio, herdeiro de COGSitio_LR, acrescenta as
 * posições cx,cy,cz.
 * @Assunto:        grafo
 * @Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitio_LR->COGSitio_LR_CM
 * 
 * @author 	André Duarte Bueno
 * @see		grafos
*/
class COGSitio_LR_CM : public COGSitio_LR
{
// --------------------------------------------------------------Atributos
public:
	double cx;  ///< Posição x do objeto na imagem tridimensional 
	double cz;  ///< Posição y do objeto na imagem tridimensional 
	double cy;  ///< Posição z do objeto na imagem tridimensional 

// -------------------------------------------------------------/**Construtor*/
/// Destrutor
  COGSitio_LR_CM ():cx (0), cy (0), cz (0)
  {
  }

// --------------------------------------------------------------/**Destrutor*/
/// Construtor
  virtual ~ COGSitio_LR_CM ()
  {
  }

// ----------------------------------------------------------------Métodos
  /**Salva atributos do objeto em disco redefinida*/
  // virtual std::ostream&  Write(std::ostream& os) const;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend std::ostream& operator<< (std::ostream& os, COGSitio_LR_CM& obj);

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// std::ostream& operator<< (std::ostream& os, COGSitio_LR_CM& obj);
// istream& operator>> (istream& is, COGSitio_LR_CM& obj);

// Cria o tipo COGSitio_LR_CMCentro, que é igual a COGSitio_LR_CM
using COGSitio_LR_Centro = COGSitio_LR_CM ;

#endif
