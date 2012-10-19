/* ----------------------------------------------------------------------------
 * PROJETO:	LIB_LDSC
 * ----------------------------------------------------------------------------
 * Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico - LENEP-CCT-UENF
 * Programadores: Leandro Puerari, Andre Duarte Bueno.
 * Copyright @2011:    Todos os direitos reservados.
 * Nome deste arquivo: CDistribuicao3D.h
 * Nome da classe:     CDistribuicao3D
 * --------------------------------------------------------------------------*/

#ifndef CDistribuicao3D_h
#define CDistribuicao3D_h

#include <Matriz/TCMatriz3D.h>
//#include <Distribuicao/CBaseDistribuicao.h>
#include <Distribuicao/CDistribuicao.h>

/**
 * @brief Declara a classe CDistribuicao3D, herdeira de CBaseDistribuicao.
 * Esta classe permite determinar a distribuição de tamanho dos objetos
 * (poros) em uma imagem 3D.
*/

using namespace std;

class CDistribuicao3D : public CDistribuicao //public CBaseDistribuicao
{
   //Atributos
private:
   /// Ponteiro para matriz tridimensional (imagem 3D)
	 TCMatriz3D<bool> * pm3D;

public:
   /// Enumera os tipos demétricas que podem ser utilizadas para executar a operação abertrua no cálculo da distribuição em imagens 3D.
   enum Metrica3D {
      espacial,
      d345,
      euclidiana
   };

   // Construtores / Destrutor
public:
	/// Construtor default
   CDistribuicao3D () : CDistribuicao( ), pm3D( NULL ) {
	}

   /// Construtor (recebe CMatriz3D)
	 CDistribuicao3D ( TCMatriz3D<bool> * _pm3D ) : CDistribuicao( ), pm3D(_pm3D) {
	}

	/// Destrutor
   ~ CDistribuicao3D () {}

   // Métodos
public:
   /// Calcula a distribuição de tamanho dos objetos em uma imagem 3D binária. Recebe como parâmetros: Tipo, Metrica3D, indice e fundo.
   bool Go( Tipos _tipo=dts, Metrica3D _metrica=d345, int indice=1, int fundo=0 );

   /// Calcula a distribuição de tamanho dos objetos em uma imagem 3D binária. Recebe como parâmetros: Ponteiro para Matriz 3D, Tipo, Metrica3D, indice e fundo.
	 bool Go( TCMatriz3D<bool> * _pm3D, Tipos _tipo=dts, Metrica3D _metrica=d345, int indice=1, int fundo=0 );

   /// Calcula a distribuição de tamanho dos objetos em uma imagem 3D binária. Recebe como parâmetros: Tipo, Metrica 2D, indice, fundo e o número de planos que serão utilizados para o calculo da média das distribuições.
   /// Se o número de planos informado for menor que 1, o defalt será 10% dos planos.
   bool Go( Tipos _tipo=dts, Metrica _metrica=d34, int indice=1, int fundo=0, int numPlanos=0  );

   /// Calcula a distribuição de tamanho dos objetos em uma imagem 3D binária. Recebe como parâmetros: Ponteiro para Matriz 3D, Tipo, Metrica 2D, indice, fundo e o número de planos que serão utilizados para o calculo da média das distribuições.
   /// Se o número de planos informado for menor que 1, o defalt será 10% dos planos.
	 bool Go( TCMatriz3D<bool> * _pm3D, Tipos _tipo=dts, Metrica _metrica=d34, int indice=1, int fundo=0, int numPlanos=0 );

};

#endif
