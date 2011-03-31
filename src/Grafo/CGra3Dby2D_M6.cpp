/*
   ===============================================================================
   PROJETO:          Biblioteca LIB_LDSC
   Ramo: TPadrao_ramo
   ===============================================================================
   @Desenvolvido_por:   Laboratorio de Desenvolvimento de Software Cientifico
     [LDSC].
   @author:          André Duarte Bueno
   @File:             CGra3Dby2D_M6.cpp
   @begin:            Sat Sep 16 2000
   @copyright:        (C) 2000 by André Duarte Bueno
   @email:            andre@lmpt.ufsc.br
 */
// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>
#include <cmath>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Grafo/CGra3Dby2D_M6.h>

#ifndef COGSitioLRCM_h
#include <Grafo/COGSitioLRCM.h>
#endif

#ifndef COGSitioEsqLRCM_h
#include <Grafo/COGSitioEsqLRCM.h>
#endif

#ifndef COGSitioDirLRCM_h
#include <Grafo/COGSitioDirLRCM.h>
#endif

#ifndef CMath_h
#include <Base/CMath.h>
#endif
using namespace std;

// -------------------------------------------------------------------------
// Função:       GetObjetoGrafo
// -------------------------------------------------------------------------
/**
@short  : Cria objeto herdeiro de CObjetoGrafo, de acordo com o tipo solicitado (COGSitioLR...).
@author :	André Duarte Bueno
@see    : grafos
@param  : CContorno::ETipoContorno tipoContorno
@return : Retorna um ponteiro para um sítio novo
*/
CObjetoGrafo *
CGra3Dby2D_M6::GetObjetoGrafo (CContorno::ETipoContorno tipoContorno)
{
  CObjetoGrafo *data;
  switch (tipoContorno)
    {
    case CContorno::CENTER:
      data = new COGSitioLRCM ();
      break;
    case CContorno::WEST:
      data = new COGSitioEsqLRCM ();
      break;
    case CContorno::EST:
      data = new COGSitioDirLRCM ();
      break;
    default:
      data = new COGSitioLRCM ();
      break;
    }
  assert (data);
  return data;
}

  // -------------------------------------------------------------------------
  // Função     AdicionarObjetos
  // -------------------------------------------------------------------------
/**
@short  :	Função que adiciona a lista de objetos do grafo, os objetos identificados em rotulador.
		Redefinida para inclusão das informações de cx,cy,cz
@author :	André Duarte Bueno
@see    :
@param  : Recebe a imagem rotulada com os objetos a serem incluídos (ra ou rp),	
 	o número do ultimo rótulo utilizado e o
	tipo de contorno (identifica o objeto a ser criado:
	COGSitioEsquerda = 0, CSitioCentro = 1,  COGSitioDireita = 2)
@return : void
*/
void
CGra3Dby2D_M6::AdicionarObjetos
  (CRotulador2DCm * rotulador, unsigned long int ultimoRotuloUtilizado,
   CContorno::ETipoContorno tipoContornoObjeto)
{

  // Calcula o centro de massa dos objetos da imagem rotulada (NOVO)
  rotulador->CentroMassaObjetos ();

  // Ponteiro para objeto a ser criado
  CObjetoGrafo *
    data;

  // Não deve considerar o objeto 0 que é o fundo.
  // inclue o rotulo final, o objeto final, ok
  // troquei cont por rotulo
  for (int rotulo = 1; rotulo <= rotulador->RotuloFinal (); rotulo++)

    {
      // Obtem um sítio novo passando o tipo
      data = GetObjetoGrafo (tipoContornoObjeto);
      assert (data);

      // No rotulador o objeto 0 é o fundo
      // como rotulo esta iniciando em 1, faço -1
      // para que o primeiro sitio tenha rotulo 0.
      data->rotulo = rotulo - 1 + ultimoRotuloUtilizado;

      // Propriedade raio hidraulico
      data->propriedade = rotulador->RaioHidraulicoObjetos (rotulo);

      // Passa para x o plano atual (sera usado na estimacao de x)
      data->x = this->plano;

      // AQUI, seta cx,cy,cz de cada sítio
      // Adiciona a posição do centro de massa
      COGSitioLRCM *
	sitio = dynamic_cast < COGSitioLRCM * >(data);
      assert (sitio);
      sitio->cx = rotulador->CMXObjetos (rotulo);
      sitio->cy = rotulador->CMYObjetos (rotulo);
      sitio->cz = plano;

      // Insere o objeto criado a lista de objetos do grafo
      objeto.push_back (data);
    }
}


  // -------------------------------------------------------------------------
  // Função:   CalculoCondutancias
  // -------------------------------------------------------------------------
/**
@short  : Redefinida, em relação a CGrafo.
		Adiciona o calculo das condutâncias das ligações
@author :	André Duarte Bueno
@see    : Condutância
@param  : nada
@return : void
*/
void CGra3Dby2D_M6::CalculoCondutancias (long double _viscosidade, long double _sizePixel, unsigned long int _fatorAmplificacao)
{
  ofstream saida ("fatorCorrecao.txt");

  // Chama função da classe base que calcula as condutancias
  CGra3Dby2D_M3::CalculoCondutancias (_viscosidade, _sizePixel, _fatorAmplificacao);

  // Inicio do calculo da correção das condutancias
  // Ponteiro para sitio derivado
  COGSitioLRCM *
    sitio = NULL;

  // Distancia dx entre os dois sítios
  double
    dx = 0.0;
  double
    dy = 0.0;

  // Fator de correção da condutancia (distancia total entre os dois sitios)
  double
    fatorCorrecaoDistancias = 0.0;
  // Percorre  todos os objetos do  grafo
  for (unsigned long int k = 0; k < objeto.size (); k++)
    {
      // Converte o ponteiro ObjetoGrafo para COGSitioLRCM, para ter acesso ao vetor condutancia[link] e cx,cy,cz
      sitio = dynamic_cast < COGSitioLRCM * >(objeto[k]);
      assert (sitio);

      // Obtêm a informação do cmx e cmy do sitio atual (k)
      double
	cmxSitio = sitio->cx;
      double
	cmySitio = sitio->cy;

      // Percorre todas as conecções do sitio atual
      COGSitioLRCM *
	sitioConexo = NULL;
      for (unsigned int link = 0; link < sitio->coneccao.size (); link++)
	{
	  sitioConexo = dynamic_cast < COGSitioLRCM * >(sitio->coneccao[link]);
	  assert (sitioConexo);

	  // Recupera a informação  do centro de massa na direção x, do sitio conexo
	  double
	    cmxSitioConexo = sitioConexo->cx;
	  double
	    cmySitioConexo = sitioConexo->cy;

	  // Determina a distância dx e dy entre o sítio e o sitio conexo
	  dx =
	    cmxSitio >
	    cmxSitioConexo ? cmxSitio - cmxSitioConexo : cmxSitioConexo -
	    cmxSitio;
	  dy =
	    cmySitio >
	    cmySitioConexo ? cmySitio - cmySitioConexo : cmySitioConexo -
	    cmySitio;

	  // Calcula o fator de correção da condutancia em função  da distância entre os sitios
	  fatorCorrecaoDistancias = sqrt (1.0 + dx * dx + dy * dy);
	  saida << fatorCorrecaoDistancias << endl;
	  // Corrige a condutancia, considerando o fator de correção

// ***********NOVO EVITA EXCESSO DE CORRECAO*******
	  if (fatorCorrecaoDistancias < 2.0)
	    sitio->condutancia[link] /= fatorCorrecaoDistancias;
	  else
	    sitio->condutancia[link] /= 2.0;

// FIM NOVO*********             
	  // sitio->condutancia[link] /= sqrt (1.0 + dx * dx + dy * dy);
	}			// for link
    }				// for k

  saida.close ();
  return;
}
