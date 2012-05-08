// VERSAO USADO NAS CONFEQU COM LIB_LMPT
/*
  ----------------------------------------------------------------------------
  PROJETO:					LIB_LMPT
  Bibliotecas de Objetos
  ----------------------------------------------------------------------------

  Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi,
  Marcos Damiani,...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CFEConectividade2D.h
  Nome da classe:      CFEConectividade2D
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  ----------------------------------------------------------------------------
*/
#include <Rotulador/CFEConectividade2D.h>
/*

//   Funcao: 	virtual   void verificaConectividade(int& ira, int& irb,int& irc);
//   Objetivo:  Dada uma imagem com duas regi�es A e B, determinar a conex�o destas
// 					regi�es utilizando um objeto de rotulagem.
//   Parametros:O indice da regi�o A e da regi�o B, e o novo indice da regi�o conexa.
//   Comentario:O funcionamento da verificacao da conectividade � simples.

1-Pesquisa um ponto da imagem que perten�a a regi�oB, e obtem o rotulo	deste (rotuloRegiaoB).
2-Pode-se proceder ent�o a verifica��o da conectividade, varrendo todos os pontos
da imagem e verificando todos aqueles que perten�am a regi�oB ou a regi�oA e que
perten�am ao mesmo objeto (r�tulo=rotuloRegiaoB) na matriz rotulo.
*/
// Obs: Observe que n�o se fala em camara, nem em regi�es da camara, o objeto � todo
// definido de forma gen�rica. N�o se define o sistema para resolver o problema espec�fico
// de determinac�o das configura��es de equil�brio.
// ----------------------------------------------------------------------------
void
CFEConectividade2D::VerificaConectividade (int &indiceRegiaoA,
        int &indiceRegiaoB,
        int &indiceRegiaoConexa)
{
    if (rotulado == false)	// verifica se a imagem j� foi rotulada
        Go (pm);			// se ainda n�o rotulada, rotula

    DeterminaRotuloRegiaoB (indiceRegiaoB);

    // ------------------------------------------------------------------------
    // Realiza a determinacao da conexao das �reas com indices:
    // indiceRegiaoA  e indiceRegiaoB na matriz pm, e que fazem parte
    // do objeto de r�tulo rotuloRegiaoB na matriz rotulo.
    // Ou seja:
    // se fizer parte do objeto com rotulo: rotuloRegiaoB e
    // se na matriz imagem tem o indice das regioes A ou B, ent�o pertence a regi�o conexa.
    // pinta a regi�o conexa com o indice indiceRegiaoConexa
    for (int i = 0; i < nx; i++)	// Percorre a imagem
        for (int j = 0; j < ny; j++)
            if (data2D[i][j] == rotuloRegiaoB)
            {
                if (pm->data2D[i][j] == indiceRegiaoA
                        || pm->data2D[i][j] == indiceRegiaoB)
                    pm->data2D[i][j] = indiceRegiaoConexa;
            }
}

// ----------------------------------------------------------------------------
// A regi�o conexa � marcada com valores negativos
void
CFEConectividade2D::VerificaConectividade (int &indiceRegiaoA,
        int &indiceRegiaoB)
{
    if (rotulado == false)	// verifica se a imagem j� foi rotulada
        Go (pm);			// se ainda n�o rotulada, rotula

    DeterminaRotuloRegiaoB (indiceRegiaoB);

    // ------------------------------------------------------------------------
    // Se fizer parte do objeto com rotulo: rotuloRegiaoB e
    // se na matriz imagem tem o indice das regioes A ou B, ent�o pertence a regi�o conexa.
    // Pinta a regi�o conexa com os indices s� que negativos
    for (int i = 0; i < nx; i++)	// Percorre a imagem
        for (int j = 0; j < ny; j++)
            if (data2D[i][j] == rotuloRegiaoB)
            {
                if (pm->data2D[i][j] == indiceRegiaoA)
                    pm->data2D[i][j] = -indiceRegiaoA;
                else if (pm->data2D[i][j] == indiceRegiaoB)
                    pm->data2D[i][j] = -indiceRegiaoB;
            }
}

// ----------------------------------------------------------------------------
// A regi�o conexa � marcada com os novos �ndices passados
void
CFEConectividade2D::VerificaConectividade (int &indiceRegiaoA,
        int &indiceRegiaoB,
        int &novoIndiceA, int &novoIndiceB)
{
    if (rotulado == false)	// verifica se a imagem j� foi rotulada
        Go (pm);			// se ainda n�o rotulada, rotula

    DeterminaRotuloRegiaoB (indiceRegiaoB);

    // ------------------------------------------------------------------------
    // Se fizer parte do objeto com rotulo: rotuloRegiaoB e
    // se na matriz imagem tem o indice das regioes A ou B, ent�o pertence a regi�o conexa.
    // Pinta a regi�o conexa com os indices s� que negativos
    for (int i = 0; i < nx; i++)	// Percorre a imagem
        for (int j = 0; j < ny; j++)
            if (data2D[i][j] == rotuloRegiaoB)
            {
                if (pm->data2D[i][j] == indiceRegiaoA)
                    pm->data2D[i][j] = novoIndiceA;
                else if (pm->data2D[i][j] == indiceRegiaoB)
                    pm->data2D[i][j] = novoIndiceB;
            }
}

// ----------------------------------------------------------------------------
// Pesquisa na imagem ponto com o indice passado e determina o seu rotulo
void
CFEConectividade2D::DeterminaRotuloRegiaoB (int &indice)
{
    for (int i = 0; i < nx; i++)	// Percorre a imagem a procura do indice dado a regi�oB
        for (int j = 0; j < ny; j++)	// o indice da regi�o B s� deve ser da regi�o B (Senao falha)
            // ou seja a regi�o B deve ser �nica e n�o subdividida.
            if (pm->data2D[i][j] == indice)
            {
                rotuloRegiaoB = data2D[i][j];

                i = nx;
                j = ny;		// deve sair do for
            }
}
