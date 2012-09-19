//  itens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
    Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:    Laboratorio de Desenvolvimento de Software Cientifico
 dos Materiais.
Programadores:   Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  Todos os direitos reservados.
Nome deste arquivo:CFEMMIDFdij.cpp
Nome da classe:      CFEMMIDFdij
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a funcao CriaMascara da classe CFEMMIDFdij.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
// using namespace std;
#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdij.h"
//  Classe base
#include "Geometria/Bola/BCDiscreta/CBCdij.h"

/*
==================================================================================
Documentacao 		CriaMascara
==================================================================================
Descricao:        Funcao que cria a mascara de chanfro adequada.
  O filtro CFEMMIDF recebe a imagem pm, e o tamanho da mascara.
  E a funcao Go recebe o raio maximo, define a variavel raioMaximo
  e chama cria mascara.
Pre-condicoes:
Excessoes:        tipos de excessoes
Concorrencia:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
void CFEMMIDFdij::CriaMascara ( unsigned int _tamanhoMascara ) {
   if ( mask ) {		//  se existe uma mascara
      if ( mask->NX () == _tamanhoMascara ) {	//  e o do mesmo  tamanho
         return;			//  sai
		}
      delete mask;		//  se  nao é do mesmo tamanho apaga objeto mask
   }				//  e abaixo cria uma nova
   //  se nao existe a mascara, cria uma nova
   mask = new CBCdij ( _tamanhoMascara, mi, mj, raioBase );
}

/*
==================================================================================
Documentacao      Go
==================================================================================
Descricao:
//  Substitui o uso da mascara, que usava loop's e indices confusos
//  pelo acesso direto aos valores da mascara.
//  Ao lado do codigo o esboco do ponto da mascara que esta sendo considerado.
//  Da forma como esta o codigo fica mais rapido e compreensivo.
Programador:      Andre Duarte Bueno
*/
//  TCMatriz2D< int > * CFEMMIDFdij::Go(TCMatriz2D< int > * matriz )
TCMatriz2D< int > * CFEMMIDFdij::Go ( TCMatriz2D< int > * &matriz, unsigned int /*_tamanhoMascara*/ ) {
   ExecutadaPorGo ( matriz );	//  armazena valores da matriz e _tamanhoMascara
   // IDFNosPlanosDeContorno(mi);    //  mi=3
   int x, y; // Indices para percorrer a matriz

   //adicionei esta inversão para poder criar imagem IDF informando quem é fundo.
   InverterSeNecessario();

   //  ida   MinimoIda
   //  IDFNosPlanosDeContornoIDA ( mi );
   for ( y = 1; y < ny - 1; y++ ) {	//  NY() e igual a ny, ny da matriz idf
      for ( x = 1; x < nx - 1; x++ ) {
			if ( data2D[x][y] != 0 ) {	//  Testa a imagem, se nao for solido entra
            minimo = raioMaximo;
            //  -----------------------------------------------------------
            min ( data2D[x-1][y] + mi );	/*ponto[x][y] */
            min ( data2D[x-1][y-1] + mj );
            min ( data2D[x][y-1] + mi );
            min ( data2D[x+1][y-1] + mj );
            //  -----------------------------------------------------------
            data2D[x][y] = minimo;
         }
		}
	}
   //  volta    MinimoVolta
   //  IDFNosPlanosDeContornoVOLTA(mi);
   for ( y = ny - 2; y > 0; y-- ) {	//  -2 pois comeca do zero e a mascara tem tamanho 1
      for ( x = nx - 2; x > 0; x-- ) {
         if ( data2D[x][y] != 0 ) {	//  Se nao for solido
            minimo = data2D[x][y];	//  Armazena valor minimo da ida
            //  -----------------------------------------------------------
            min ( data2D[x - 1][y + 1] + mj );
            min ( data2D[x][y + 1] + mi );
            min ( data2D[x + 1][y + 1] + mj );
            min ( data2D[x + 1][y] + mi );
            //  -----------------------------------------------------------
            data2D[x][y] = minimo;
         }
		}
	}
   //Write ("CFEMMIDFdij-Go.pgm");
   //pm->Write ("pmEmGo.pgm");
   return pm;
}

