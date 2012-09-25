#ifndef COGSitioLRCM_h
#define COGSitioLRCM_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CGrafo
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       COGSitioLRCM.h
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

#ifndef COGSitioLR_h
#include <Grafo/COGSitioLR.h>
#endif


// ===============================================================================
// Documentacao CLASSE: COGSitioLRCM
// ===============================================================================
/** 
 * @brief  é um sítio, herdeiro de COGSitioLR, acrescenta as 
 * posições cx,cy,cz.
 * @Assunto:        grafo
 * @Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitioLR->COGSitioLRCM
 * 
 * @author 	André Duarte Bueno
 * @see		grafos
*/
class COGSitioLRCM : public COGSitioLR
{
// --------------------------------------------------------------Atributos
public:
	double cx;  ///< Posição x do objeto na imagem tridimensional 
	double cz;  ///< Posição y do objeto na imagem tridimensional 
	double cy;  ///< Posição z do objeto na imagem tridimensional 

// -------------------------------------------------------------/**Construtor*/
/// Destrutor
  COGSitioLRCM ():cx (0), cy (0), cz (0)
  {
  }

// --------------------------------------------------------------/**Destrutor*/
/// Construtor
  virtual ~ COGSitioLRCM ()
  {
  }

// ----------------------------------------------------------------Métodos
  /**Salva atributos do objeto em disco redefinida*/
  // virtual std::ostream&  Write(std::ostream& os) const;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend std::ostream& operator<< (std::ostream& os, COGSitioLRCM& obj);

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// std::ostream& operator<< (std::ostream& os, COGSitioLRCM& obj);
// istream& operator>> (istream& is, COGSitioLRCM& obj);

// Cria o tipo COGSitioLRCMCentro, que é igual a COGSitioLRCM
typedef COGSitioLRCM COGSitioLRCMCentro;

#endif
