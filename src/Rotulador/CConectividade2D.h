#ifndef CConectividade2D_h
#define CConectividade2D_h

/*
----------------------------------------------------------------------------
PROJETO:		LIB_LDSC
			Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi,
			Marcos Damiani,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CConectividade2D.h
Nome da classe:      CConectividade2D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/

#ifndef __IOSTREAM__
#include <iostream>
#endif

#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif
#include <Rotulador/TCRotulador2D.h>
/**
 * @brief Determina a conexão de objetos em imagens 3D.
 * 
 * A classe CConectividade2D representa conceitos geométricos.
 * 
 * A classe  CConectividade2D é filha da classe TRotulagem2D. 
 * De forma que recebe uma imagem  no construtor e processa a 
 * rotulagem através da função Go (da mesma forma que na classe pai).
 * 
 * Acrecenta entretanto a função VerificaConectividade, utilizada 
 * para verificar a conectividade  de duas regiões na imagem.
 * 
 * Uma imagem pode ser binária, ou ter vários índices. 
 * Ou seja, pode ter varias regiões
 * (região A,B,C,...). Cada uma sendo diferenciada pelo seu índice
 * (indiceRegiãoA,indiceRegiãoB,indiceRegiãoC,...)
 * 
 * O objetivo do objeto CConectividade2D é verificar os diferentes 
 * tipos   de conexães destas regiões.
 * 
 * A funcao VerificaConectividade(ira,irb,irc) recebe como parametros 
 * os índices das regioes A, B   e irc=indiceRegiaoConexa, utilizado 
 * para "pintar" a região conexa.
 * Ou seja, o que diferencia as duas regiães A e B são seus índices 
 * na matriz imagem.
 * Outro aspecto importante com relacao a funcao VerificaConectividade,
 * é que para determinar o rotulo da região B, a funcao faz uma 
 * pesquisa, a procura de pontos na imagem que tenham o indice
 * indiceRegiaoB, depois verifica na matriz rotulo, o rotulo que 
 * foi dado a região B. 
 * O que implica que a região B deve ser única. Se B for multipla,
 * somente uma delas será considerada.
 * As outras versões da mesma função funcionam da mesma forma, 
 * só que permitem a "pintura" do resultado final de forma diferenciada.
 * 
 * Posteriormente considerar multiplas regiões B.
*/

class CConectividade2D : public TCRotulador2D<int>
{
	
  // Atributos
  TCMatriz2D< int > *pm; ///< Ponteiro para a imagem recebida, usado na função VerificaConectividade.

  // A função verifica conectividade não recebe a imagem, pois é necessária
  // a rotulagem da imagem(funcao Go()).
  // A rotulagem pode ser feita uma única vez, e a
  // verificação da conectividade diversas vezes.
  
  int rotuloRegiaoB; ///< rotulo dado a região B

  // Obs: Futuramente pode ter varias regiões B, (vários rótulos)?

 // Métodos
 protected:
  /// Pesquisa na imagem ponto com o indice passado e determina o seu rotulo
  void DeterminaRotuloRegiaoB (int &indice);	

 public:
  /// Construtor Default
  // CConectividade2D():TRotulador2D(),TCMatriz2D< int >(){};// Exige a definição de pm e data2D posteriormente
		CConectividade2D (TCMatriz2D< int > * &imagem):TCRotulador2D<int> (imagem), pm (imagem)
  {
  }

  /// Destrutor
  virtual ~ CConectividade2D ()
  {
  }

  /// A região conexa é marcada com valores negativos
  void VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB);

  /// A região conexa é marcada com o valor indiceRegiaoConexa
  void VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB,
			      int &indiceRegiaoConexa);

  /// A regiãoo conexa é marcada com os novos índices passados
  void VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB,
			      int &novoIndiceA, int &novoIndiceB);
};

#endif
