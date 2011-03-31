/*VERSAO USADO NAS CONFEQU COM LIB_LMPT
----------------------------------------------------------------------------
PROJETO:					LIB_LMPT
Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores: Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, Marcos Damiani,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CConectividade3D.h
Nome da classe:       CConectividade3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help

----------------------------------------------------------------------------

Funcao: 	  virtual void verificaConectividade(int& ira, int& irb,int& irc);
Objetivo:   Dada uma imagem com duas regiões A e B, determinar a conexão destas
  		  regiões utilizando um objeto de rotulagem.
Parametros: O indice da região A e da região B, e o novo indice da região conexa.
Comentario: O funcionamento da verificacao da conectividade é simples.

1 - 	Pesquisa um ponto da imagem que pertença a regiãoB, e obtem o rotulo deste (rotuloRegiaoB).
2 - 	Pode-se proceder então a verificação da conectividade, varrendo todos os pontos
	da imagem e verificando todos aqueles que pertençam a regiãoB ou a regiãoA e que
	pertençam ao mesmo objeto (rótulo=rotuloRegiaoB) na matriz rotulo.

Obs: Observe que não se fala em camara, nem em regiões da camara, o objeto é todo
	definido de forma genérica. Não se define o sistema para resolver o problema 
	específico de determinação das configurações de equilíbrio.

--------------------------------------------------------------------------------*/
#include <Rotulador/CConectividade3D.h>

void CConectividade3D::VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB, int &indiceRegiaoConexa)
{
	if (rotulado == false)	//verifica se a imagem já foi rotulada
    		Go (pm);			//se ainda não rotulada, rotula...
	DeterminaRotuloRegiaoB (indiceRegiaoB);
	// Realiza a determinacao da conexao das áreas com indices:
  	// indiceRegiaoA  e indiceRegiaoB na matriz pm, e que fazem parte
  	// do objeto de rótulo rotuloRegiaoB na matriz rotulo.
  	// Ou seja: 
  	// se fizer parte do objeto com rotulo: rotuloRegiaoB e
  	// se na matriz imagem tem o indice das regioes A ou B, então pertence a região conexa.
  	for (int i = 0; i < nx; i++)	//Percorre a imagem
    		for (int j = 0; j < ny; j++)
      		for (int k = 0; k < nz; k++)
				if (data3D[i][j][k] == rotuloRegiaoB)
	  				if (pm->data3D[i][j][k] == indiceRegiaoA || pm->data3D[i][j][k] == indiceRegiaoB)
	    					pm->data3D[i][j][k] = indiceRegiaoConexa;	//indiceAtivo=indiceKGB
}

//----------------------------------------------------------------------------
//A região conexa é marcada com valores negativos
void CConectividade3D::VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB)
{
	if (rotulado == false)	//verifica se a imagem já foi rotulada
    		Go (pm);			//se ainda não rotulada, rotula
	DeterminaRotuloRegiaoB (indiceRegiaoB);
	// Se fizer parte do objeto com rotulo: rotuloRegiaoB e
	// se na matriz imagem tem o indice das regioes A ou B, então pertence a região conexa.
	// Pinta a região conexa com os indices só que negativos
  	for (int i = 0; i < nx; i++)	//Percorre a imagem
    		for (int j = 0; j < ny; j++)
      		for (int k = 0; k < nz; k++)
				if (data3D[i][j][k] == rotuloRegiaoB)
	  			{
	    				if (pm->data3D[i][j][k] == indiceRegiaoA)
	      				pm->data3D[i][j][k] = -indiceRegiaoA;
	    				else if (pm->data3D[i][j][k] == indiceRegiaoB)
	      				pm->data3D[i][j][k] = -indiceRegiaoB;
	  			}
}

//----------------------------------------------------------------------------
//A região conexa é marcada com os novos índices passados
void CConectividade3D::VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB, int &novoIndiceA, int &novoIndiceB)
{
	if (rotulado == false)	//verifica se a imagem já foi rotulada
    		Go (pm);			//se ainda não rotulada, rotula
	DeterminaRotuloRegiaoB (indiceRegiaoB);
	// Se fizer parte do objeto com rotulo: rotuloRegiaoB e
  	// se na matriz imagem tem o indice das regioes A ou B, então pertence a região conexa.
  	// Pinta a região conexa com os indices só que negativos
  	for (int i = 0; i < nx; i++)	//Percorre a imagem
    		for (int j = 0; j < ny; j++)
      		for (int k = 0; k < nz; k++)
				if (data3D[i][j][k] == rotuloRegiaoB)
	  			{
	    				if (pm->data3D[i][j][k] == indiceRegiaoA)
	      				pm->data3D[i][j][k] = novoIndiceA;
	    				else if (pm->data3D[i][j][k] == indiceRegiaoB)
	      				pm->data3D[i][j][k] = novoIndiceB;
	  			}
}

//----------------------------------------------------------------------------
//Pesquisa na imagem, ponto com o indice passado (deve ser o indide da regiãoB) e determina o seu rotulo
void CConectividade3D::DeterminaRotuloRegiaoB (int &indice)
{
	for (int i = 0; i < nx; i++)	//Percorre a imagem a procura do rotulo dado a regiãoB
    		for (int j = 0; j < ny; j++)	//o indice da região B só deve ser da região B (Senao falha)
      		for (int k = 0; k < nz; k++)	//ou seja a região B deve ser única e não subdividida.
				if (pm->data3D[i][j][k] == indice)
	  			{
	    				rotuloRegiaoB = data3D[i][j][k];
	    				i = nx;
	    				j = ny;
	    				k = nz;
	  			} 
	//sai do for
}
