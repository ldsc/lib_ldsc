/*
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: TPadrao_Desenvolvido
  ===============================================================================

  ramo por:	Laboratorio de Desenvolvimento de Software Cientifico
  	[LDSC].
  @author:          André Duarte Bueno
  File:             CRotulador2DCm.cpp
  begin:            Sat Sep 16 2000
  copyright:        (C) 2000 by André Duarte Bueno
  email:            andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>
// #include <assert.h>
#include <cassert>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Rotulador/CRotulador2DCm.h>
// using namespace std;

/*
  -------------------------------------------------------------------------
  Função:
  -------------------------------------------------------------------------
  @short  :	Calcula o centro de massa cmx e cmy
  @author :	André Duarte Bueno
  @see    :
  o intervalo de um int -2.147.483.648 -> 2.147.483.647 (2bi)
  @param  : void
  @return : void
*/

void
CRotulador2DCm::CentroMassaObjetos ()
{
  // Precisa determinar a área dos objetos                                
  // se já calculado calcula novamente                      
  CalculaAreaObjetos ();

  // Só procede o calculo se a matriz rótulo foi determinada
  if (rotulado)
    {
      // CALCULO DO CENTRO DE MASSA NAS DIRECOES X e Y
      cmx.reserve (numeroObjetos);
      cmy.reserve (numeroObjetos);

      // Zera o centro de  massa
      for (int i = 0; i < numeroObjetos; i++)
	{
	  cmx[i] = 0;
	  cmy[i] = 0;
	}

      // Acumula os valores
      for (int i = 0; i < NX (); i++)
	for (int j = 0; j < NY (); j++)
	  {
	    cmx[data2D[i][j]] += i;
	    cmy[data2D[i][j]] += j;
	  }

      // Calcula o centro de massa     
      for (long int no = 0; no < numeroObjetos; no++)
	{
	  cmx[no] = cmx[no] / (double) areaObjetos->data1D[no];
	  cmy[no] = cmy[no] / (double) areaObjetos->data1D[no];
	}
    }				// if(rotulado)
}

/*
  void CRotulador2DCm::CentroMassaObjetos()
  {
  // Precisa determinar a área dos objetos
  AreaObjetos();// se já calculado calcula novamente

  if( rotulado )	// Só procede o calculo se a matriz rótulo foi determinada
  {
  // CALCULO DO CENTRO DE MASSA NAS DIRECOES X e Y
  if ( cmx )		// Cria vetor com os centros de massa em x
  delete cmx;
  cmx = new CVetor (numeroObjetos);  		
  assert( cmx );

  if( cmy )		// Cria vetor com os centros de massa em y
  delete cmy;
  cmy = new CVetor ( numeroObjetos );
  assert( cmy );

  if( cmx && cmy)				// Se vetores alocados corretamente
  {
  cmx->Constante(0);		// Zera o vetor centros de massa na direção x
  cmy->Constante(0);		// Zera o vetor centros de massa na direção y

  // Calculo em só do centro de massa
  for(  int i = 0; i < NX(); i++)   // Percorre direcao x
  for(  int j = 0; j < NY(); j++) // Percorre direcao y
  {
  cmx->data1D[ data2D[i][j] ] += i ;			// Acumula os valores das posicoes i
  cmy->data1D[ data2D[i][j] ] += j ;			// Acumula os valores das posicoes i
  }	

  for(  long int no = 0 ; no < numeroObjetos ; no++)
  {
  cmx->data1D[ no ] = 1000000.0* (double) cmx->data1D[ no ] / (double) areaObjetos->data1D[ no ];	
  cmy->data1D[ no ] = 1000000.0* (double) cmy->data1D[ no ] / (double) areaObjetos->data1D[ no ];	
  }
  }// if( cmxObjetos && cmyObjetos)
  }// if(rotulado)
  }

*/

/*
  -------------------------------------------------------------------------
  Função: CMXObjetos(  int k)
  -------------------------------------------------------------------------
  @short  : Retorna o centro de massa  na direção x
  @author : André Duarte Bueno
  @see    :
  @param  :
  @return :
*/
double
CRotulador2DCm::CMXObjetos (int k) const 
{
  if (k < (numeroObjetos))
    return cmx[k];
  return 0.0;
};

/*
  double CRotulador2DCm::CMXObjetos(  int k)			const
  {
  if(k < (numeroObjetos) 	  && cmx != NULL     )
  // return areaObjetos[k - rotuloInicial];
  return (long double)cmx->data1D[ k ]/1000000.0;
  return 0.0;
  };
*/

/*
  -------------------------------------------------------------------------
  Função: CMYObjetos(  int k)
  -------------------------------------------------------------------------
  @short  : Retorna o centro de massa  na direção y
  @author : André Duarte Bueno
  @see    :
  @param  :
  @return :
*/
double
CRotulador2DCm::CMYObjetos (int k) const 
{
  if (k < (numeroObjetos))
    return cmy[k];
  return 0.0;
};

/*
  double CRotulador2DCm::CMYObjetos(  int k)			const
  {
  if(k < (numeroObjetos) 	  && cmy != NULL     )
  return (long double)cmy->data1D[ k ]/1000000.0;
  return 0.0;
  };

*/
