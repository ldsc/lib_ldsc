/* ----------------------------------------------------------------------------
 * PROJETO:	LIB_LDSC
 * ----------------------------------------------------------------------------
 * Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico - LENEP-CCT-UENF
 * Programadores: Leandro Puerari, Andre Duarte Bueno.
 * Copyright @2011:    Todos os direitos reservados.
 * Nome deste arquivo: CBaseDistribuicao.h
 * Nome da classe:     CBaseDistribuicao
 * --------------------------------------------------------------------------*/

#ifndef CBaseDistribuicao_h
#define CBaseDistribuicao_h

#include <fstream>
#include <string>
#include <set>
#include <vector>

/**
 * @brief Declara a classe CBaseDistribuicao, que é a classe base para as
 * classes CDistribuicao e CDistribuicao3D, as quais possibilitam determinar
 * a distribuição de tamanho dos objetos (poros, gargantas ou sólidos), em imagens
 * binárias 2D e 3D respectivamente.
*/

using namespace std;

class CBaseDistribuicao
{
   //Atributos
public:
   /// Enumera os tipos de distribuição que podem ser calculadas.
	 enum Tipos { dtp, dts, dtg };

   /// Caminho onde o arquivo distribuição será salvo.
   string path;

protected:
   /// Vetor distribuição
   vector<double> distribuicao;

   /// Área dos objetos (poros ou sólidos)
   double objetos;

   Tipos tipo;

   // Construtores / Destrutor
public:
	/// Construtor default
   CBaseDistribuicao () : path("./") {
		distribuicao.clear();
      objetos = 0.0;
	}

	/// Destrutor
   ~ CBaseDistribuicao () {}

   // Métodos
public:
	/// Recebe lista de arquivos distribuição, cálcula a média das curvas e armazena os valores no vetor distribuição
   bool Media (set<string> filesName, Tipos _tipo=dts);

   /// Salva em disco os dados de distribuição (.dtp ou .dts).
   bool Write (string fileName);

protected:
   /// Salva em disco os dados de distribuição de tamanho de poros (.dtp)
   void WriteDTP (ofstream & fout);

   /// Salva em disco os dados de distribuição de tamanho de sólidos (.dts)
   void WriteDTS (ofstream & fout);

	 /// Salva em disco os dados de distribuição de tamanho de gargantas (.dtg)
	 void WriteDTG (ofstream & fout);

};

#endif
