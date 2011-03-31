#ifndef  CConfiguracoesEquilibrio2D_h
#define  CConfiguracoesEquilibrio2D_h

/*
----------------------------------------------------------------------------
PROJETO:		LIB_LDSC
			Bibliotecas de Objetos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
			Liang Zirong, Paulo C. Philippi,
			Marcos Damiani,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CConfiguracoesEquilibrio2D.h
Nome da classe:      CConfiguracoesEquilibrio2D
Arquivos de documentacao do projeto em: path/documentacao/, path/Help
TAREFAS:		Criar classe CConfiguracoesEquillibrio comum aos 
			modelos 2D e 3D.
----------------------------------------------------------------------------
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas C++
//  ----------------------------------------------------------------------------
#ifndef __IOSTREAM_H
#include <iostream>
#endif

//  ----------------------------------------------------------------------------
//  Bibliotecas LIB_LDSC
//  ----------------------------------------------------------------------------
/*
#ifndef CFiltroEspacial_h
#include "Filtro/CFiltroEspacial.h"
#endif
*/
#ifndef CMFluido_h
#include "Material/CMFluido.h"
#endif

#ifndef CCamara2D_h
#include "ConEqu/CCamara2D.h"
#endif

#ifndef CConectividade2D_h
#include "Rotulador/CConectividade2D.h"
#endif

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDF.h"

/**
 * @brief A classe CConfiguracoesEquilibrio2D determina configuracoes equilibrio em imagens 2D.
 *
 * Processo default intrusao de mercurio mb = 0 ca = 0
 * 
 * OBS: Lembre-se, quando executa conectividade->Go() 
 * cria um objeto de rotulagem (uma matriz inteira extra).
 */
class CConfiguracoesEquilibrio2D	//  :CFiltroEspacial
{
  /**
   * Funcao utilizada para marcar Yi da imagem anterior na camara
   * para deixar igual ao codigo do Fabio (e diferente do artigo).
   * @param imagem 
   */
  void DiferencaEmRelacaoArtigo (CMatriz2D * &imagem);

  std::string fileName;	/// Nome do(s) arquivo(s) de disco

  bool visualizar;	/// Se verdadeiro, mostra resultados

  std::ostream * os;	/// Aponta para stream de saída

public:

  CCamara2D *camara; 	/// É a imagem da camara (2D)

  CMFluido *fluidoB;	/// Representa o fluido B

  CMFluido *fluidoA;	/// Representa o fluido A

 protected:
  bool salvarResultadosParciaisDisco; 		/// Se verdadeiro, salva resultados parciais em disco (default = false).
	
  bool salvarResultadosFinaisDisco; 		/// Se verdadeiro, salva resultados finais em disco (default = true).

  int contadorPassosExecutados; 		/// Utilizado para dar nomes aos arquivos de disco em ordem crescente (inicializado com valor 0).

  int indiceParcialB;				/// Indice da regiao B após cada processo

  CFEMMIDF *idf; 				/// É uma copia da imagem com a IDF (é o filtro que gera e armazena a idf)

  CConectividade2D *rotulador; 			/// Objeto conectividade, realiza a verificacao da conectividade (herdeiro de rotulador2D, é uma imagem).
  
  int raioMaximo;				/// Raio maximo da operacao
  
  int raioMinimo;				/// Raio minimo da operacao
  
  int raio; 					/// Utilizado pelos métodos Next e Go para controle do loop.
  
  // Tarefa: transformar em enumeracao.
  int tipoIDF;					/// Define o tipo de idf a ser criada: métricas d4, d8, d34, d5711, d345, e1
  
  int B0;   					/// Índice da região B0
  
  int A0;   					/// Índice da região A0
  
  int Ai;   					/// Índice da região Ai
  
  int G;    					/// Índice da região G
  
  int G_;   					/// Índice da região G_
  
  int Yi;   					/// Índice da região Yi
  
  int KGB0; 					/// Índice da região KGB0
  
  int wbG__U_KGB0;      			/// Índice da região wbG__U_KGB0
  
  int KwbG__U_KGB0B0;   			/// Índice da região KwbG__U_KGB0B0
  
  int B;   					/// Índice da região B
  
  int A;   					/// Índice da região A
	//int Y0;
	//Métodos
 protected:

  void DefineAtributos ();							/// Define os valores de cor das regiões e demais atributos
  
  void CriaCamara (CMatriz2D * &imagem);					/// Cria a camara a partir de imagem 2D
  
  void CriaIDF ();								/// Cria imagem idf
  
  void CriaRotulador ();							/// Cria rotulador (objeto que verifica a conectividade)
  
  void CalculaAbertura (int &raio);						/// Calcula a abertura
  
  void ConectividadeKGB0 ();							/// Determina a conectividade KGB0 
  
  void UniaoG__U_KGB0 ();							/// Determina a UniaoG__U_KGB0 
  
  void ConectividadeKwbG__U_KGB0B0 ();						/// Determina a Conectividade KwbG__U_KGB0B0
  
  void SolucaoOmega (int &indiceRegiaoB, CMatriz2D * &imagem);			/// Determina a SolucaoOmega
  
  void CorrecaocaxYi (CMatriz2D * &imagem);					/// Determina a CorrecaocaxYi
  
  void SolucaoFinal (CMatriz2D * &imagem); 					/// Determina a SolucaoFinal 
  
  void RestabeleceIDFPositiva ();		 				/// Restabelece IDF Positiva
  
  /**
   * Salva imagem em disco, recebe a imagem e nome do arquivo
   * @param imagem 
   * @param msg 
   */
  void Salvar (CMatriz2D * &imagem, std::string msg);
 
public:

  /// Construtor default, recebe uma ostream
  CConfiguracoesEquilibrio2D ( /*CMatriz2D* imagem */ std::ostream & out = std::cout);

  /// Destrutor
  virtual ~ CConfiguracoesEquilibrio2D ();
  
  /// Sobrecarga do operador <<. (IMPLEMENTAR)
  friend std::ostream & operator<< (std::ostream & os, CConfiguracoesEquilibrio2D & configuracoesEquilibrio2D)
  {
  	// configuracoesEquilibrio2D = configuracoesEquilibrio2D; //evitar warning;
	  // IMPLEMTENTAR
    	return os;
  }
  
  /// Sobrecarga do operador >>. (IMPLEMENTAR)
  friend std::istream & operator>> (std::istream & is, CConfiguracoesEquilibrio2D & configuracoesEquilibrio2D)
  {
  	// configuracoesEquilibrio2D = configuracoesEquilibrio2D; //evitar warning;
	// IMPLEMTENTAR
	return is;
  }
  
  ///  Executa passo-a-passo a determinação das configurações de equilíbrio.
  virtual bool Next (CMatriz2D * &imagem);
  
  ///  Executa toda a determinação das configurações de equilíbrio.
  virtual void Go (CMatriz2D * &imagem);
  
  /// Aloca uma CMatriz2D e preenche a mesma com 1 se a cor da região informada equivale a cor extraída da câmara. 
  /// Deve ser chamada após Next ou Go.
  /// A imagem alocada deve ser destruida por quem a receber.
  // ou seja, retorna imagem da região informada.
  CMatriz2D * GetImagem ( const int regiao ) const;
  
  /// Preenche a matriz com 1 se a cor da região informada equivale a cor extraída da câmara.
  /// Deve ser chamada após Next ou Go.
  // ou seja, retorna imagem da região informada.
  CMatriz2D * GetImagem( CMatriz2D* imagem, const int regiao ) const;

  ///  Define o fluido A
  void FluidoA (CMFluido * &fa)
  {
    if (fluidoA)
      delete fluidoA;
    fluidoA = fa;
  }
  
  ///  Define o fluido B
  void FluidoB (CMFluido * &fb)
  {
    if (fluidoB)
      delete fluidoB;
    fluidoB = fb;
  }
  
  /// Determina o tipo de idf.
  void TipoIDF (const int &_tipoIDF)
  {
    tipoIDF = _tipoIDF;
  }
  
  /// Retorna this
  CConfiguracoesEquilibrio2D *This()
  {
    return this;
  }
  
  /// Retorna o valor da variável salvarResultadosParciaisDisco (bool)
  bool SalvarResultadosParciaisDisco() const
  { 
  	return salvarResultadosParciaisDisco; 
  }
  
  /// Seta a variável bool salvarResultadosParciaisDisco
  void SalvarResultadosParciaisDisco(bool s) 
  { 
  	salvarResultadosParciaisDisco = s;
  }
  
  /// Retorna indice da regiao A
  int IndiceRegiaoA()	const { return A; }

  /// Retorna indice da regiao B
  int IndiceRegiaoB()	const { return B; }

};

#endif
