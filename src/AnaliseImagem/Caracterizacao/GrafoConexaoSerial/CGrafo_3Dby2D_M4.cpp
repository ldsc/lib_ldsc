/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:	
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          Andre Duarte Bueno
@file:             CGrafo_3Dby2D_M4.cpp
@begin:            Sat Sep 16 2000
copyright:        (C) 2000 by Andre Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cmath>
#include <iomanip>
#include <fstream>

using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef	CGrafo_3Dby2D_M4_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafo_3Dby2D_M4.h>
#endif

#ifndef CSitio_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio.h>
#endif

#ifndef CObjetoRede_CC_Sitio_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_CC_Sitio.h>
#endif

#ifndef CObjetoRede_CC_Sitio_WEST_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_CC_Sitio_WEST.h>
#endif

#ifndef CObjetoRede_CC_Sitio_EST_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_CC_Sitio_EST.h>
#endif

#ifndef CMath_h
#include <Base/CMath.h>
#endif

// -------------------------------------------------------------------------
// Funcao     AdicionarObjetos
// -------------------------------------------------------------------------
/** @short  : Funcao que adiciona ao grafo os objetos deste plano.
 *	Redefinida em CGrafo_3Dby2D_M4, para armazenar a informação
 *	cmx e cmy do objeto.
 *	Observe que cmx e cmy é um vector de float pertencente a this.
 *	Ou seja, cmx e cmy estao sendo armazenada no grafo e nao nos objetos.
 *	
 *	PS1: Posteriomente criar uma hierarquia de sitios com
 *	cmx e cmy para visualização 3D.
 *	
 *	PS2: a vantagem de armazenar cmx no grafo é deletar
 *	todo o vetor cmx logo após o calculo das condutâncias.
 *	
 * @author :	Andre Duarte Bueno
 * @see    :
 * @param  : Recebe a imagem rotulada com os objetos a serem incluídos,	o número do ultimo 
 * rótulo utilizado e o tipo de contorno (identifica o objeto a ser criado:
 *    CObjetoRede_Sitio_EST = 0, CSitioCentro = 1,  CObjetoRede_Sitio_EST = 2)
 * @return : void
*/
void
CGrafo_3Dby2D_M4::AdicionarObjetos
  (CRotulador2DCm * rotulador,
   unsigned long int ultimoRotuloUtilizado,
   CContorno::ETipoContorno tipoContornoObjeto)
{
  // Chama função da classe base
  CGrafo_3Dby2D::AdicionarObjetos (rotulador, ultimoRotuloUtilizado, tipoContornoObjeto);

  // Calcula o centro de massa dos objetos da imagem rotulada
  rotulador->CentroMassaObjetos ();

  // ---------------------------------------------------------------------------------------
  // Percorre todos os objetos novos deste plano (rotulador)
  for (unsigned long int cont = 1; cont <= rotulador->RotuloFinal ();
       cont++)
    {
      // Adiciona ao vetor cmx a informação do cmx do objeto (long double)
      // o primeiro cmx tem índice 0
      cmx.push_back (rotulador->CMXObjeto (cont));
      cmy.push_back (rotulador->CMYObjeto (cont));
      cmz.push_back (plano);
    }
}

// -------------------------------------------------------------------------
// Funcao:   CalculoDosCentroDeMassa
// -------------------------------------------------------------------------
/*@short  :A diferença é considerar um fator de correção das condutâncias
 *	que considera a distância efetiva entre os sítios conexos.
 *  A função AdicionarObjetos foi redefinida, de forma a armazenar no vetor
 * 	this->cmx a informação do centro de massa na direção x do sítio.
 *	PS1: Como o modelo considera a interação entre planos consecutivos,
 *	nao é necessário armazenar o cmy pois a distancia na direção y
 *	vai ser sempre de 1 pixel.
 * @author :	Andre Duarte Bueno
 * @see    : Condutância
 * @param  : nada
 * @return : void
*/
/*
void  CGrafo_3Dby2D_M4::CalcularCentroMassa()
{
  unsigned long int i,j,k;               		
  // rp		rotulador ja criado em Go, posso usar pois Go me chama
	// img2D imagem ja criada em Go, posso usar pois Go me chama
	// Para todos os planos da imagem
  for(  k = 0; k < img3D->NZ(); k++)
   {
   plano = k;
   for( i = 0; i< img3D->NX(); i++)
	  for( j = 0; j< img3D->NY(); j++)            	
			// Copia plano k para img2D	
	    img2D->data2D[i][j]= img3D->data3D[i][j][k];

	// Rotula
	rp->Go(img2D);										

	// Calcula os vetores area e perimetro
  rp->RaioHidraulicoObjetos();			

	// Calcula o centro de massa dos objetos da imagem rotulada

	rotulador->CentroMassaObjetos();
	

	// Percorre todos os objetos novos deste plano (rotulador)
	for (	unsigned long int  	cont = 1;	cont <= rotulador->RotuloFinal(); cont++ )
       {
			 // Adiciona ao vetor cmx a informação do cmx do objeto (long double)
// 
// qual a relação deste rotulo com o rotulo da lista de sítios ??
// quando deletou os sitios, perdeu a referencia dos rotulos????
// Esta função sequer foi terminada
// vai calcular o cmx e cmy como antes, ao deletar os objetos
// do grafo vai deletar os objetos do cmx e cmy simulataneamente

       cmx.push_back ( rotulador->GetCMXObjeto(cont)    );
       cmy.push_back ( rotulador->GetCMYObjeto(cont)    );
			 // Fim novo
       }
  }
}
*/

// -------------------------------------------------------------------------
// Funcao:   CalcularCondutancias
// -------------------------------------------------------------------------
/** @short  : Redefinida, em relação a CGrafo_3Dby2D_M3
 *	Faz o calculo da condutancia de forma semelhante aquela definida em CGrafo_3Dby2D_M3.
 *	A diferença é considerar um fator de correção das condutâncias
 *	que considera a distância efetiva entre os sítios conexos.
 *	A função AdicionarObjetos foi redefinida, de forma a armazenar no vetor
 *	this->cmx a informação do centro de massa na direção x do sítio.
 *	PS1: Como o modelo considera a interação entre planos consecutivos,
 *	nao é necessário armazenar o cmz pois a distancia na direção z
 *	vai ser sempre de 1 pixel.
 *					
 * @author :	Andre Duarte Bueno
 * @see    : Condutância
 * @param  : nada
 * @return : void
*/
void
CGrafo_3Dby2D_M4::CalcularCondutancias
  (long double _viscosidade, long double _dimensaoPixel,
   unsigned long int _fatorAmplificacao)
{
  // Chama função da classe base que calcula as condutâncias
  CGrafo_3Dby2D_M3::CalcularCondutancias (_viscosidade, _dimensaoPixel,
				       _fatorAmplificacao);

  // Inicio do calculo da correção das condutâncias

  // Ponteiro para sitio derivado
  CObjetoRede_CC_Sitio *
    ptrSitioLR = nullptr;

  // Centro de massa na direção x,y do sitio
  float
    cmxSitio = 0.0;
  float
    cmySitio = 0.0;

  // Centro de massa na direção x,y do sitio conexo
  float
    cmxSitioConexo = 0.0;
  float
    cmySitioConexo = 0.0;

  // Distancia dx entre os dois sítios
  float
    dx = 0.0;
  float
    dy = 0.0;

  // Fator de correção da condutancia (distancia total entre os dois sitios)
  long double
    fatorCorrecaoDistancias = 0.0;

  // Percorre  todos os objetos do  grafo
  for (unsigned long int k = 0; k < objeto.size (); k++)
    {
      // Converte o ponteiro ObjetoGrafo para CObjetoRede_CC_Sitio, para ter acesso ao vetor condutancia[link]
      ptrSitioLR = dynamic_cast < CObjetoRede_CC_Sitio * >(objeto[k]);

      // Obtem a informação do cmx e cmy do sitio atual (k)
      cmxSitio = cmx[k];
      cmySitio = cmy[k];

      // Percorre todas as conexões do sitio atual
      for (unsigned int link = 0; link < ptrSitioLR->conexao.size (); link++)
	{

	  // Recupera a informação  do centro de massa na direção x do sitio conexo
	  // Pega o rotulo do sitio conexo, e recupera o cmx do objeto
	  cmxSitioConexo = cmx[ptrSitioLR->conexao[link]->rotulo];
	  cmySitioConexo = cmy[ptrSitioLR->conexao[link]->rotulo];

	  // Determina a distância dx e dy entre o sítio e o sitio conexo 
	  // Correção Bueno/23/1/2014 - como faz dx*dx não precisa achar módulo
	  dx = cmxSitio - cmxSitioConexo ;
	  dy = cmySitio - cmySitioConexo ;

	  // Calcula o fator de correção da condutancia em função  da distância entre os sitios
	  fatorCorrecaoDistancias = sqrt (1.0 + dx * dx + dy * dy);

	  // Corrige a condutancia, considerando o fator de correção
	  ptrSitioLR->condutancia[link] /= fatorCorrecaoDistancias;
	}
    }

// /*
// DEBUG   SALVA vetores cmx e cmy em disco
  {
    ofstream
    fout ("vetor_cmx.txt");
    if (fout.fail ())
      return;
    fout << "size=" << cmx.size () << endl;
    for (unsigned int i = 0; i < cmx.size (); i++)
      fout << "cmx[" << i << "]=" << cmx[i] << endl;
//     cmx.resize (0);		// redimensiona o vetor cmx para tamanho zero (0).// o equivalente a deletar
//     fout << "size=" << cmx.size () << endl;
    fout.close ();
  }
  {
    ofstream
    fout ("vetor_cmy.txt");
    if (fout.fail ())
      return;
    fout << "size=" << cmy.size () << endl;
    for (unsigned int i = 0; i < cmy.size (); i++)
      fout << "cmy[" << i << "]=" << cmy[i] << endl;
//     cmy.resize (0);		// redimensiona o vetor cmx para tamanho zero (0).// o equivalente a deletar
//     fout << "size=" << cmy.size () << endl;
    fout.close ();
  }
// */
// cmx.resize(0); // cmx.clear()
// cmy.resize(0); // cmy.clear()

  return;
}

// -------------------------------------------------------------------------
// Funcao:   Write
// -------------------------------------------------------------------------
// Salva o grafo em disco
// Salva adicionalmente a informacao dos centros de massa
void
CGrafo_3Dby2D_M4::Write ()
{
  ofstream out ( NomeGrafo().c_str() );
  if (!out)
    return;

  out.setf (ios::right);

  // Numero de objetos
  out << setw (5) << objeto.size () << endl;

  // Percorre os objetos e salva em disco as informações de cada objeto
  for (unsigned long int i = 0; i < objeto.size (); i++)
    {
      objeto[i]->Write (out);
      out << endl;

      // fout << (*objeto[i]);
    }

  // Inicio novo
  for (unsigned long int k = 0; k < objeto.size (); k++)
    {
      // ptrSitioLR = dynamic_cast<CObjetoRede_CC_Sitio*> (objeto[k]);
      // Obtem a informação do cmx do sitio atual (k)
      out << "\nobj[" << setw (5) << k << "]" << " cx=" << setw (5) << cmx[k]
	<< " cy=" << cmy[k] << " cz=" << cmz[k];;
    }

  // Fim novo
  out.close ();
}

// -------------------------------------------------------------------------
// Funcao:   CalcularCondutancias
// -------------------------------------------------------------------------
/** @short  : Chamada ao final de EliminarRamosMortos
 *          Após a eliminação dos ramos mortos, um conjunto de objetos foi destruído
 *          Como os vetores cmx e cmy estao separados dos objetos, perciso reorganizar
 *          cmx e cmy, de forma a levarem em conta a nova situação do grafo.
 *          Observe que se as informações de cmx e cmy estivessem nos próprios objetos
 *          isto nao seria necessário.
 * @author :	Andre Duarte Bueno
 * @see    : Condutância
 * @param  : nada
 * @return : void
*/
void
CGrafo_3Dby2D_M4::ReorganizarCmxCmy ()
{
  // O tamanho de cmx e cmy é o mesmo do grafo inicial
  // mas agora o grafo esta menor, por isso percorro o novo grafo (sem os ramos mortos)
  unsigned int cont = 0;
  for (unsigned long int i = 0; i < objeto.size (); i++)
    {
      // Os objetos remanescentes preservaram até agora, os seus rótulos
      // copio para cmx(cont) o valor que estava em cmx[objeto[i]->rotulo]
      cmx[cont] = cmx[objeto[i]->rotulo];
      cmy[cont] = cmy[objeto[i]->rotulo];
      cmz[cont] = cmz[objeto[i]->rotulo];
      cont++;
    }
  // Abaixo é novo, agora deleta o restando do vetor cmx e cmy
  cmx.resize (cont);
  cmy.resize (cont);
  cmz.resize (cont);
}
