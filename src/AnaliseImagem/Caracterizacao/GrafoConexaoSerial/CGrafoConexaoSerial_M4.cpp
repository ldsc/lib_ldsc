/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          Andre Duarte Bueno
@file:             CGrafoConexaoSerial_M4.cpp
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
#ifndef	CGrafoConexaoSerial_M4_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M4.h>
#endif

#ifndef CObjetoRede_Sitio_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio.h>
#endif

#ifndef CObjetoRede_Sitio_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio.h>
#endif

// #ifndef CObjetoRede_Sitio_h
// #include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio.h>
// #endif
//
// #ifndef CObjetoRede_Sitio_WEST_h
// #include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio_WEST.h>
// #endif
//
// #ifndef CObjetoRede_Sitio_EST_h
// #include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio_EST.h>
// #endif


// -------------------------------------------------------------------------
// Funcao     AdicionarObjetos
// -------------------------------------------------------------------------
/** @short  : Funcao que adiciona ao grafo os objetos deste plano.
 *	Redefinida em CGrafoConexaoSerial_M6, para armazenar a informação cmx e cmy
 *  no próprio objeto.
 *	Observe que cmx e cmy é um vector de float pertencente a this.
 *	Ou seja, aqui cmx e cmy estão sendo armazenada no grafo e não nos objetos,
 *  sendo redimensionadas para size zero logo após o uso.
 *  Sendo assim, o consumo extra de memória é temporário.
 *
 *	PS: a vantagem de armazenar cmx no grafo é deletar
 *	todo o vetor cmx logo após o calculo das condutâncias.
 *
 * @author :	Andre Duarte Bueno
 * @see    :
 * @param  : Recebe a imagem rotulada com os objetos a serem incluídos,	o número do ultimo
 * rótulo utilizado e o tipo de objeto (identifica o objeto a ser criado:
 * @return : void
*/
void
CGrafoConexaoSerial_M4::AdicionarObjetos
( CRotulador2DCm* rotulador,
  unsigned long int ultimoRotuloUtilizado,
  ETipoObjetoGrafo tipoObjeto )
{
   // Chama função da classe base
   CGrafoConexaoSerial::AdicionarObjetos ( rotulador, ultimoRotuloUtilizado, tipoObjeto );

   // Calcula o centro de massa dos objetos da imagem rotulada
   rotulador->CentroMassaObjetos ();

   // ---------------------------------------------------------------------------------------
   // Percorre todos os objetos novos deste plano (rotulador)
   for ( unsigned long int cont = 1; cont <= rotulador->RotuloFinal (); cont++ ) {
         // Adiciona ao vetor cmx a informação do cmx do objeto (long double)
         // o primeiro cmx tem índice 0
         cmx.push_back ( rotulador->CMXObjeto ( cont ) );
         cmy.push_back ( rotulador->CMYObjeto ( cont ) );
         cmz.push_back ( plano );
      }
}

// -------------------------------------------------------------------------
// Funcao:   CalcularCondutancias
// -------------------------------------------------------------------------
/** @short  : Redefinida, em relação a CGrafoConexaoSerial_M3
 *	Faz o calculo da condutancia de forma semelhante aquela definida em CGrafoConexaoSerial_M3.
 *	A diferença é adicionar um fator de correção das condutâncias,
 *	que considera a distância efetiva entre os sítios conexos.
 *	A função AdicionarObjetos foi redefinida, de forma a armazenar no vetor
 *	this->cmx a informação do centro de massa na direção x do sítio.
 *	PS1: Como o modelo considera a interação entre planos consecutivos,
 *	nao é necessário armazenar o cmz pois a distância na direção z
 *	vai ser sempre de 1 pixel.
 *
 * @author :	Andre Duarte Bueno
 * @see    : Condutância
 * @param  : nada
 * @return : void
*/
void CGrafoConexaoSerial_M4::CalcularCondutancias ( long double _viscosidade,
      long double _dimensaoPixel,  unsigned long int _fatorAmplificacao )
{
   // Chama função da classe base que calcula as condutâncias
   CGrafoConexaoSerial_M3::CalcularCondutancias ( _viscosidade, _dimensaoPixel,
         _fatorAmplificacao );

   // Inicio do calculo da correção das condutâncias
   // Centro de massa na direção x,y do sitio
   float    cmxSitio {0.0};
   float    cmySitio {0.0};

   // Centro de massa na direção x,y do sitio conexo
   float    cmxSitioConexo {0.0};
   float    cmySitioConexo {0.0};

   // Distancia dx entre os dois sítios
   float    dx {0.0};
   float    dy {0.0};

   // Fator de correção da condutancia (distancia total entre os dois sitios)
   long double    fatorCorrecaoDistancias {0.0};

   // Percorre todos os objetos do grafo
   for ( unsigned long int k = 0; k < objeto.size (); k++ ) {
         // Obtem a informação do cmx e cmy do sitio atual (k)
         cmxSitio = cmx[k];
         cmySitio = cmy[k];

         // Percorre todas as conexões do sitio atual
         for ( unsigned int link = 0; link < objeto[k]->conexao.size (); link++ ) {
               // Recupera a informação  do centro de massa na direção x do sitio conexo
               // Pega o rotulo do sitio conexo, e recupera o cmx do objeto
               cmxSitioConexo = cmx[objeto[k]->conexao[link]->rotulo];
               cmySitioConexo = cmy[objeto[k]->conexao[link]->rotulo];

               // Determina a distância dx e dy entre o sítio e o sitio conexo
               // Correção Bueno/23/1/2014 - como faz dx*dx não precisa achar módulo
               dx = cmxSitio - cmxSitioConexo ;
               dy = cmySitio - cmySitioConexo ;

               // Calcula o fator de correção da condutancia em função  da distância entre os sitios
               fatorCorrecaoDistancias = sqrt ( 1.0 + dx * dx + dy * dy );

               // Corrige a condutancia, considerando o fator de correção
               objeto[k]->condutancia[link] /= fatorCorrecaoDistancias;
            }
      }

// /*
// DEBUG   SALVA vetores cmx e cmy em disco
   {
      ofstream fout ( "vetor_cmx.txt" );

      if ( fout.fail () )
         return;

      fout << "size=" << cmx.size () << endl;

      for ( unsigned int i = 0; i < cmx.size (); i++ )
         fout << "cmx[" << i << "]=" << cmx[i] << endl;

      fout.close ();
      // Como não vai mais usar o cmy, redimensiona o vetor cmy para zero (0), para economizar memória.
      cmx.clear();
   }
   {
      ofstream fout ( "vetor_cmy.txt" );

      if ( fout.fail () )
         return;

      fout << "size=" << cmy.size () << endl;

      for ( unsigned int i = 0; i < cmy.size (); i++ )
         fout << "cmy[" << i << "]=" << cmy[i] << endl;

      fout.close ();
      // Como não vai mais usar o cmy, redimensiona o vetor cmy para zero (0), para economizar memória.
      cmy.clear();
   }
   return;
}

// -------------------------------------------------------------------------
// Funcao:   Write
// -------------------------------------------------------------------------
// Salva o grafo em disco
// Salva adicionalmente a informação dos centros de massa
void CGrafoConexaoSerial_M4::Write ( std::ostream & out ) const
{
  CGrafoConexaoSerial::Write( out );
  
   // Adiciona informação do centro de massa (cmz sempre == 1)
   for ( unsigned long int k = 0; k < objeto.size (); k++ ) {
         out << ' ' << setw ( 5 ) << cmx[k] << ' ' << cmy[k] << ' ' << cmz[k] << '\n';
      }
}

// -------------------------------------------------------------------------
// Funcao:   CalcularCondutancias
// -------------------------------------------------------------------------
/** @short  : Chamada ao final de EliminarRamosMortos.
 * Após a eliminação dos ramos mortos, um conjunto de objetos foi destruído;
 * Como os vetores cmx e cmy estão separados dos objetos, preciso reorganizar
 * cmx e cmy, de forma a levarem em conta a nova situação do grafo.
 * Observe que se as informações de cmx e cmy estivessem nos próprios objetos
 * isto nao seria necessário.
 * Ou seja, o modelo 4 otimiza memória quando comparado com modelo 6;
 * mas tem esta função extra.
 * @author :	Andre Duarte Bueno
 * @see    : Condutância
 * @param  : nada
 * @return : void
*/
void
CGrafoConexaoSerial_M4::ReorganizarCmxCmy ()
{
   // O tamanho de cmx e cmy é o mesmo do grafo inicial
   // mas agora o grafo esta menor, por isso percorro o novo grafo (sem os ramos mortos)
   unsigned int cont = 0;

   for ( unsigned long int i = 0; i < objeto.size (); i++ ) {
         // Os objetos remanescentes preservaram até agora, os seus rótulos
         // copio para cmx(cont) o valor que estava em cmx[objeto[i]->rotulo]
         cmx[cont] = cmx[objeto[i]->rotulo];
         cmy[cont] = cmy[objeto[i]->rotulo];
         cmz[cont] = cmz[objeto[i]->rotulo];
         cont++;
      }

   // Abaixo é novo, agora deleta o restando do vetor cmx e cmy
   cmx.resize ( cont );
   cmy.resize ( cont );
   cmz.resize ( cont );
}
