// VERSAO USADO NAS CONFEQU COM LIB_LDSC
/*
  ----------------------------------------------------------------------------
  PROJETO:					LIB_LDSC
  Bibliotecas de Objetos
  ----------------------------------------------------------------------------

  Desenvolvido por:		Laboratorio de Desenvolvimento de Software Cientifico dos Materiais.
  Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, Marcos Damiani,...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CConectividade2D.h
  Nome da classe:      	CConectividade2D
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  ----------------------------------------------------------------------------
*/
#include <Rotulador/CConectividade2D.h>
/*
  Funcao: 	virtual void verificaConectividade(int& ira, int& irb,int& irc);
  Objetivo: 	Dada uma imagem com duas regiões A e B, determinar a conexão destas regiões utilizando um objeto de rotulagem.
  Parametros: 	O indice da região A e da região B, e o novo indice da região conexa.
  Comentario: 	O funcionamento da verificacão da conectividade é simples.
	
  1-Pesquisa um ponto da imagem que pertença a regiãoB, e obtem o rotulo	deste (rotuloRegiaoB).
  2-Pode-se proceder então a verificação da conectividade, varrendo todos os pontos da imagem e verificando todos aqueles que pertençam
    a regiãoB ou a regiãoA e que pertençam ao mesmo objeto (rótulo=rotuloRegiaoB) na matriz rotulo.

  Obs: Observe que não se fala em camara, nem em regiões da camara, o objeto é todo
  definido de forma genérica. Não se define o sistema para resolver o problema específico
  de determinação das configurações de equilíbrio.
 ----------------------------------------------------------------------------
*/
void CConectividade2D::VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB, int &indiceRegiaoConexa)
{
  if(rotulado == false)	// verifica se a imagem já foi rotulada
    Go(pm);			// se ainda não rotulada, rotula
  
  DeterminaRotuloRegiaoB (indiceRegiaoB);

  // ------------------------------------------------------------------------
  // Realiza a determinacao da conexao das áreas com indices:
  // indiceRegiaoA  e indiceRegiaoB na matriz pm, e que fazem parte
  // do objeto de rótulo rotuloRegiaoB na matriz rotulo.
  // Ou seja:
  // se fizer parte do objeto com rotulo: rotuloRegiaoB e
  // se na matriz imagem tem o indice das regioes A ou B, então pertence a região conexa.
  // pinta a região conexa com o indice indiceRegiaoConexa
  for (int i = 0; i < nx; i++){	// Percorre a imagem
    for (int j = 0; j < ny; j++){
      if (data2D[i][j] == rotuloRegiaoB){
	   if (pm->data2D[i][j] == indiceRegiaoA || pm->data2D[i][j] == indiceRegiaoB){
	     pm->data2D[i][j] = indiceRegiaoConexa;
	   }
	 }
    }
  }
}

// ----------------------------------------------------------------------------
// A região conexa é marcada com valores negativos
void CConectividade2D::VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB){
  if (rotulado == false)	// verifica se a imagem já foi rotulada
    Go (pm);			// se ainda não rotulada, rotula

  DeterminaRotuloRegiaoB (indiceRegiaoB);

  // ------------------------------------------------------------------------
  // Se fizer parte do objeto com rotulo: rotuloRegiaoB e
  // se na matriz imagem tem o indice das regioes A ou B, então pertence a região conexa.
  // Pinta a região conexa com os indices só que negativos
  for (int i = 0; i < nx; i++){	// Percorre a imagem
    for (int j = 0; j < ny; j++){
      if (data2D[i][j] == rotuloRegiaoB){
	   if (pm->data2D[i][j] == indiceRegiaoA){
	     pm->data2D[i][j] = -indiceRegiaoA;
	   }else if (pm->data2D[i][j] == indiceRegiaoB){
	     pm->data2D[i][j] = -indiceRegiaoB;
	   }
	 }
    }
  }
}

// ----------------------------------------------------------------------------
// A região conexa é marcada com os novos índices passados
void CConectividade2D::VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB, int &novoIndiceA, int &novoIndiceB)
{
  if (rotulado == false)	// verifica se a imagem já foi rotulada
    Go (pm);			// se ainda não rotulada, rotula

  DeterminaRotuloRegiaoB (indiceRegiaoB);

  // ------------------------------------------------------------------------
  // Se fizer parte do objeto com rotulo: rotuloRegiaoB e
  // se na matriz imagem tem o indice das regioes A ou B, então pertence a região conexa.
  // Pinta a região conexa com os indices só que negativos
  for (int i = 0; i < nx; i++){	// Percorre a imagem
    for (int j = 0; j < ny; j++){
      if (data2D[i][j] == rotuloRegiaoB){
	   if (pm->data2D[i][j] == indiceRegiaoA){
	     pm->data2D[i][j] = novoIndiceA;
	   }else if (pm->data2D[i][j] == indiceRegiaoB){
	     pm->data2D[i][j] = novoIndiceB;
	   }
	 }
    }
  }
}

// ----------------------------------------------------------------------------
// Pesquisa na imagem ponto com o indice passado e determina o seu rotulo
void CConectividade2D::DeterminaRotuloRegiaoB (int &indice)
{
  for (int i = 0; i < nx; i++){	    // Percorre a imagem a procura do indice dado a regiãoB
    for (int j = 0; j < ny; j++){	    // o indice da região B só deve ser da região B (Senao falha)
      if (pm->data2D[i][j] == indice){ // ou seja a região B deve ser única e não subdividida.
	   rotuloRegiaoB = data2D[i][j];
	   i = nx;
	   j = ny; // deve sair do for
	 }
    }
  }
}
