// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
				Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:		Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFdj.cpp
Nome da classe:      CFMMIDFdj
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a fun��o CriaMascara da classe CFMMIDFdj.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
using namespace std;

#include "TFiltro\TFEspacial\TFEMorfologiaMatematica\CFMMIDFdj.h"	// Classe base
#include "TMascara\TMCDiscreta\CBCdj.h"	// ponteiro para

/*
============================================================================
Documentacao 		CriaMascara
============================================================================
Descri��o:        Funcao que cria a mascara de chanfro adequada.
Pr�-condi��es:
Excess�es:        tipos de excessoes
Concorr�ncia:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
void
CFMMIDFdj::CriaMascara (unsigned int _tamanhoMascara)
{
  if (mask)			// se existe uma mascara
    {
      if (mask->NX () == _tamanhoMascara)	// e � do mesmo  tamanho
	return;			// sai
      delete mask;		// se  n�o � do mesmo tamanho apaga
    }				// e abaixo cria uma nova

  mask = new CBCdj (_tamanhoMascara, mi, raioBase);	// se n�o existe a mascara, cria uma nova
}


// Mascara de chanfro d8
//       1  1  1
//  1  p  1
//  1  1  1


/*
============================================================================
Documentacao      Go
============================================================================
Descri��o:
// Substitui o uso da mascara da mascara, que usava loop's e �ndices confusos
// pelo acesso direto aos valores da mascara.
// Ao lado do c�digo o esbo�o do ponto da mascara que esta sendo considerado.
// Da forma como esta o c�digo fica mais r�pido e compreensivo.
Programador:      Andre Duarte Bueno
*/
CMatriz2D *
CFMMIDFdj::Go (CMatriz2D * &matriz, unsigned int /*_tamanhoMascara */ )
{
  ExecutadaPorGo (matriz);	// armazena valores da matriz e _tamanhoMascara

//  IDFNosPlanosDeContorno(mj);     // verifica planos de contorno

  int x, y;			// ,i;  // Indices para percorrer a matriz

  // ida   MinimoIda
//       IDFNosPlanosDeContornoIDA(mi);
  // Da forma como esta n�o percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
  for (y = 1; y < ny - 1; y++)	// NY() � igual a ny, ny da matriz idf
    for (x = 1; x < nx - 1; x++)
      if (data2D[x][y] != 0)	// Testa a imagem, se nao for solido entra
	{			// 
	  minimo = raioMaximo;
	  // -----------------------------------------------------------
	  min (data2D[x - 1][y] + mi);	// ponto[x][y]
	  min (data2D[x - 1][y - 1] + mi);
	  min (data2D[x][y - 1] + mi);
	  min (data2D[x + 1][y - 1] + mi);
	  // -----------------------------------------------------------

	  data2D[x][y] = minimo;
	}
  // volta    MinimoVolta
  // Da forma como esta n�o percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
//       IDFNosPlanosDeContornoVOLTA(mi);
  for (y = ny - 2; y > 0; y--)	// -2 pois come�a do zero e a mascara tem tamanho 1
    for (x = nx - 2; x > 0; x--)
      if (data2D[x][y] != 0)	// Se nao for solido
	{
	  minimo = data2D[x][y];	// Armazena valor minimo da ida
	  // -----------------------------------------------------------
	  min (data2D[x - 1][y + 1] + mi);
	  min (data2D[x][y + 1] + mi);
	  min (data2D[x + 1][y + 1] + mi);
	  /*ponto[x][y] */ min (data2D[x + 1][y] + mi);

	  // -----------------------------------------------------------
	  data2D[x][y] = minimo;
	}
  return pm;
}
