/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
@Desenvolvido_por:
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:        André Duarte Bueno
@file:          CGra3Dby2D_M6.cpp
@begin:         Sat Sep 16 2000
@copyright:     (C) 2000 by André Duarte Bueno
@email:         andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>
#include <cmath>
#include <sstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGra3Dby2D_M6.h>

#ifndef COGSitio_CC_CM_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_CC_CM.h>
#endif

#ifndef COGSitio_CC_CM_WEST_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_CC_CM_WEST.h>
#endif

#ifndef COGSitio_CC_CM_EST_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_CC_CM_EST.h>
#endif

#ifndef CMath_h
#include <Base/CMath.h>
#endif
using namespace std;

// -------------------------------------------------------------------------
// Função:       CriarObjetoGrafo
// -------------------------------------------------------------------------
/**
@short  : Cria objeto herdeiro de CObjetoGrafo, de acordo com o tipo solicitado (COGSitio_CC...).
@author : André Duarte Bueno
@see    : grafos
@param  : CContorno::ETipoContorno tipoContorno
@return : Retorna um ponteiro para um sítio novo
*/
CObjetoGrafo *
CGra3Dby2D_M6::CriarObjetoGrafo (CContorno::ETipoContorno tipoContorno)
{
  CObjetoGrafo *data;
  switch (tipoContorno)
    {
    case CContorno::ETipoContorno::CENTER:
      data = new COGSitio_CC_CM ();
      break;
    case CContorno::ETipoContorno::WEST:
      data = new COGSitio_CC_CM_WEST ();
      break;
    case CContorno::ETipoContorno::EST:
      data = new COGSitio_CC_CM_EST ();
      break;
    default:
      data = new COGSitio_CC_CM ();
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
    COGSitio_EST = 0, CSitioCentro = 1,  COGSitio_EST = 2)
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
  // inclue o rotulo final, o objeto final
  for (int rotulo = 1; rotulo <= rotulador->RotuloFinal (); rotulo++)

    {
      // Obtem um sítio novo passando o tipo
      data = CriarObjetoGrafo (tipoContornoObjeto);
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
      COGSitio_CC_CM *    sitio = dynamic_cast < COGSitio_CC_CM * >(data);
      assert (sitio);
      sitio->cx = rotulador->CMXObjeto (rotulo);
      sitio->cy = rotulador->CMYObjeto (rotulo);
      sitio->cz = plano;

      // Insere o objeto criado a lista de objetos do grafo
      objeto.push_back (data);
    }
}

// -------------------------------------------------------------------------
// Função:   CalcularCondutancias
// -------------------------------------------------------------------------
/**
@short  : Redefinida, em relação a CGrafo.
        Adiciona o calculo das condutâncias das ligações
@author :	André Duarte Bueno
@see    : Condutância
@param  : nada
@return : void
*/
void CGra3Dby2D_M6::CalcularCondutancias (long double _viscosidade, long double _dimensaoPixel, unsigned long int _fatorAmplificacao)
{
// ***********NOVO CALCULO TORTUOSIDADE*******
  tortuosidade = 0.0;
  numeroDerivacoesUsadasCalculoTortuosidade = 0;
// FIM NOVO*********

  ofstream saida ((NomeGrafo() + ".fatorCorrecao").c_str());

  // Chama função da classe base que calcula as condutancias
  CGra3Dby2D_M3::CalcularCondutancias (_viscosidade, _dimensaoPixel, _fatorAmplificacao);

  // Inicio do calculo da correção das condutancias
  // Ponteiro para sitio derivado
  COGSitio_CC_CM *
    sitio = nullptr;

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
      // Converte o ponteiro ObjetoGrafo para COGSitio_CC_CM, para ter acesso ao vetor condutancia[link] e cx,cy,cz
      sitio = dynamic_cast < COGSitio_CC_CM * >(objeto[k]);
      assert (sitio);

      // Obtêm a informação do cmx e cmy do sitio atual (k)
      double
    cmxSitio = sitio->cx;
      double
    cmySitio = sitio->cy;

      // Percorre todas as conecções do sitio atual
      COGSitio_CC_CM *
    sitioConexo = nullptr;
      for (unsigned int link = 0; link < sitio->coneccao.size (); link++)
    {
      sitioConexo = dynamic_cast < COGSitio_CC_CM * >(sitio->coneccao[link]);
      assert (sitioConexo);

      // Recupera a informação  do centro de massa na direção x, do sitio conexo
      double
        cmxSitioConexo = sitioConexo->cx;
      double
        cmySitioConexo = sitioConexo->cy;

      // Determina a distância dx e dy entre o sítio e o sitio conexo
        // Correção, como faz dx*dx, não precisa achar módulo!
      dx = cmxSitio - cmxSitioConexo ;
      dy = cmySitio - cmySitioConexo ;

      // Calcula o fator de correção da condutancia em função  da distância entre os sitios
      fatorCorrecaoDistancias = sqrt (1.0 + dx * dx + dy * dy);
      saida << fatorCorrecaoDistancias << endl;
      // Corrige a condutancia, considerando o fator de correção

// ***********NOVO NO MODELO 6 EVITA EXCESSO DE CORRECAO*******
// Este fator precisa ser melhor definido, pois é arbitrário
      if (fatorCorrecaoDistancias < 2.0)
        sitio->condutancia[link] /= fatorCorrecaoDistancias;
      else
        sitio->condutancia[link] /= 2.0;
// FIM NOVO*********
// ***********NOVO CALCULO TORTUOSIDADE*******
      // Para cálculo da tortuosidade vai acumular as distâncias laterais
      // e o número de derivações (número de conecções)
      // e depois dividir pelo número de conecções.
      tortuosidade += fatorCorrecaoDistancias;    // acumula as distâncias
      numeroDerivacoesUsadasCalculoTortuosidade++;// acumula número conecções
// FIM NOVO*********
      // sitio->condutancia[link] /= sqrt (1.0 + dx * dx + dy * dy);
    }			// for link
    }				// for k
// ***********NOVO CALCULO TORTUOSIDADE*******
  tortuosidade /= numeroDerivacoesUsadasCalculoTortuosidade;// corrige a tortuosidade

  ofstream arq_tortuosidade( (NomeGrafo() + ".tortuosidade").c_str() );
  arq_tortuosidade << "A tortuosidade da imagem " << nomeArquivo << " é : " << tortuosidade << "\n";
  arq_tortuosidade.close();

  cout << "\nA tortuosidade da imagem " << nomeArquivo << " é : " << tortuosidade << "\n";

// FIM NOVO*********

  saida.close ();
  return;
}
