//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
          Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:      	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:         	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFdj.cpp
Nome da classe:      CFEMMIDFdj
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a função CriaMascara da classe CFEMMIDFdj.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
//using namespace std;
#ifndef CFEMMIDFdj_h
#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdj.h"	//  Classe base
#endif
#include "Geometria/Bola/BCDiscreta/CBCdj.h"	//  ponteiro para

/*
==================================================================================
Documentacao 		CriaMascara
==================================================================================
Descrição:        Funcao que cria a mascara de chanfro adequada.
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
template<typename T>
void CFEMMIDFdj<T>::CriaMascara (unsigned int _tamanhoMascara) {
	 if (this->mask) {			//  se existe uma mascara
			if (this->mask->NX () == _tamanhoMascara)	//  e é do mesmo  tamanho
         return;			//  sai
			delete this->mask;		//  se  não é do mesmo tamanho apaga objeto mask
   }				//  e abaixo cria uma nova
	 this->mask = new CBCdj (_tamanhoMascara, mi, raioBase);	//  se não existe a mascara, cria uma nova
}


//  Mascara de chanfro d8
//   1  1  1
//   1  p  1
//   1  1  1

/*
==================================================================================
Documentacao      Go
==================================================================================
Descrição:
//  Substitui o uso da mascara da mascara, que usava loop's e índices confusos
//  pelo acesso direto aos valores da mascara.
//  Ao lado do código o esboço do ponto da mascara que esta sendo considerado.
//  Da forma como esta o código fica mais rápido e compreensivo.
Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * CFEMMIDFdj<T>::Go (TCMatriz2D<T> * &matriz, unsigned int /*_tamanhoMascara */ ) {
	 this->ExecutadaPorGo (matriz);	//  armazena valores da matriz e _tamanhoMascara
	 this->InverterSeNecessario();
   // IDFNosPlanosDeContorno(mj);     //  verifica planos de contorno
   int x, y; // Indices para percorrer a matriz
   // ida   MinimoIda
   // IDFNosPlanosDeContornoIDA(mi);
   // Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
	 for (y = 1; y < this->ny - 1; y++)	//  NY() é igual a ny, ny da matriz idf
			for (x = 1; x < this->nx - 1; x++)
				 if (this->data2D[x][y] != 0)	//  Testa a imagem, se nao for solido entra
         {			//
						this->minimo = this->raioMaximo;
            //  -----------------------------------------------------------
						this->min (this->data2D[x - 1][y] + mi);	//  ponto[x][y]
						this->min (this->data2D[x - 1][y - 1] + mi);
						this->min (this->data2D[x][y - 1] + mi);
						this->min (this->data2D[x + 1][y - 1] + mi);
            //  -----------------------------------------------------------

						this->data2D[x][y] = this->minimo;
         }
   //  volta    MinimoVolta
   //  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
   //        IDFNosPlanosDeContornoVOLTA(mi);
	 for (y = this->ny - 2; y > 0; y--)	//  -2 pois começa do zero e a mascara tem tamanho 1
			for (x = this->nx - 2; x > 0; x--)
				 if (this->data2D[x][y] != 0)	//  Se nao for solido
         {
						this->minimo = this->data2D[x][y];	//  Armazena valor minimo da ida
            //  -----------------------------------------------------------
						this->min (this->data2D[x - 1][y + 1] + mi);
						this->min (this->data2D[x][y + 1] + mi);
						this->min (this->data2D[x + 1][y + 1] + mi);
						this->min (this->data2D[x + 1][y] + mi);

            //  -----------------------------------------------------------
						this->data2D[x][y] = this->minimo;
         }
	 return this->pm;
}
