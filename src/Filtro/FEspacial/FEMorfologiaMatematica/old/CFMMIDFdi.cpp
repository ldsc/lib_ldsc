// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFdi.cpp
Nome da classe:      CFMMIDFdi
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a fun��o CriaMascara da classe CFMMIDFdi.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
//using namespace std;

#include "TFiltro\TFEspacial\TFEMorfologiaMatematica\CFMMIDFdi.h"	// Classe base
#include "TMascara\TMCDiscreta\CBCdi.h"	// ponteiro para

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
CFMMIDFdi::CriaMascara (unsigned int _tamanhoMascara)
{
  if (mask)			// se existe uma mascara
    {
      if (mask->NX () == _tamanhoMascara)// e � do mesmo  tamanho
	return;			// sai
      delete mask;		// se  n�o � do mesmo tamanho apaga
    }				// e abaixo cria uma nova

  mask = new CBCdi (_tamanhoMascara, mi, raioBase);// se n�o existe a mascara, cria uma nova
}

// Mascara de chanfro d4
//     1
//  1  p  1
//     1
// CMatriz2D * CFMMIDFdi::Go( CMatriz2D *& matriz)
// Obs
CMatriz2D *
CFMMIDFdi::Go (CMatriz2D * &matriz, unsigned int /*_tamanhoMascara */ )
{
  ExecutadaPorGo (matriz);	// verifica a matriz e copia dados para data2D

  // IDFNosPlanosDeContorno(mi);                          // verifica planos de contorno


  // ida    MinimoIda
//        IDFNosPlanosDeContornoIDA(mi);                 // verifica planos de contorno
  int y_1;			// otimizacao
  int x, y;			// ,i;           // Indices para percorrer a matriz
  for (y = 1; y < ny - 1; y++)	// NY() � igual a ny, ny da matriz idf
    {
      // y_1=y-1;
      for (x = 1; x < nx - 1; x++)
	if (data2D[x][y] != 0)	// Testa a imagem, se nao for solido entra
	  {			// 
	    minimo = raioMaximo;	// 64000 aqui 65536
	    // -----------------------------------------------------------
	    min (data2D[x - 1][y] + mi);	/*ponto[x][y] */
	    min (data2D[x][y - 1] + mi);
	    // -----------------------------------------------------------
	    data2D[x][y] = minimo;
	  }
    }
  // volta  MinimoVolta
  //      IDFNosPlanosDeContornoVOLTA(mi);
  // int ym1;                              // otimizacao
  for (y = ny - 2; y > 0; y--)	// -2 pois come�a do zero e a mascara tem tamanho 1
    {				// ym1=y+1;
      for (x = nx - 2; x > 0; x--)
	if (data2D[x][y] != 0)	// Se nao for solido, entra
	  {
	    minimo = data2D[x][y];	// armazena valor minimo ida
	    // -----------------------------------------------------------

	    min (data2D[x][y + 1] + mi);
	    /*ponto[x][y] */ min (data2D[x + 1][y] + mi);

	    // -----------------------------------------------------------
	    data2D[x][y] = minimo;
	  }
    }
  return pm;
}
