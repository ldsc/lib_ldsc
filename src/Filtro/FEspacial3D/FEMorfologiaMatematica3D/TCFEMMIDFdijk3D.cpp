// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ].

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
   Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIDFdijk3D.cpp
Nome da classe:      TCFEMMIDFdijk3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a função CriaMascara da classe TCFEMMIDFdijk3D.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#ifndef TCFEMMIDFdijk3D_h
#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFdijk3D.h"	// Classe base
#endif

#include "Geometria/Bola/BCDiscreta3D/CBCdijk3D.h" // ponteiro para
// #include "TMascara\TMascMascDiscretad34.h"  // Cria objeto


/*
==================================================================================
Documentacao 		CriaMascara
==================================================================================
Descrição:  Funcao que cria a mascara de chanfro adequada.
 O filtro TCFEMMIDF recebe a imagem pm, e o tamanho da mascara.
 E a função Go recebe o raio máximo, define a variável raioMaximo
 e chama cria mascara.
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
template<typename T>
void TCFEMMIDFdijk3D<T>::CriaMascara (unsigned int _tamanhoMascara) {
	 if (this->mask) { // se existe uma mascara
			if (this->mask->NX () == _tamanhoMascara)	// e é do mesmo  tamanho
         return;		// sai
			delete this->mask;	// se  não é do mesmo tamanho apaga
   } // e abaixo cria uma nova

	 this->mask = new CBCdijk3D (_tamanhoMascara, mi, mj, mk, raioBase);	// se não existe a mascara, cria uma nova
}

/*
==================================================================================
Documentacao      Go
==================================================================================
Descrição:
// Substitui o uso da mascara da mascara, que usava loop's e índices confusos
// pelo acesso direto aos valores da mascara.
// Ao lado do código o esboço do ponto da mascara que esta sendo considerado.
// Da forma como esta o código fica mais rápido e compreensivo.
 // TIDF
 // Por uma questão de performance, deixar como esta.
 // Mas criar na classe base a mesma funcao mas com codigo generico
 // usando coisas do tipo  min (data3D[x][y][z] + masc->data3D[x][y][z])
 // Funcao IDFNosPlanosDeContorno();// preenche os planos de contorno com valor base
Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz3D<T> * TCFEMMIDFdijk3D<T>::Go (TCMatriz3D<T> * &matriz, unsigned int /*_tamanhoMascara*/) {
   ExecutadaPorGo (matriz);	// pm=matriz, copia valores, verfica

   //adicionei esta inversão para poder criar imagem IDF informando quem é indice e fundo.
	 this->InverterSeNecessario();

   //  IDFNosPlanosDeContornoIDA(mi);
   // -------------------------
   int x, y, z;			// Indices para percorrer a matriz
   register int xm1;		// x+1
   register int x_1;		// x-1
   register int ym1;		// y+1
   register int y_1;		// y-1
   register int zm1;		// z+1
   register int z_1;		// z-1

   // MinimoIda
	 for (z = 1; z < this->nz - 1; z++) {	// inicio do 1 pois já considerou planos 0 acima
      z_1 = z - 1;
			for (y = 1; y < this->ny - 1; y++) {
         ym1 = y + 1;
         y_1 = y - 1;
				 for (x = 1; x < this->nx - 1; x++) {
						if (this->data3D[x][y][z] != 0) {	// Testa a imagem, se nao for solido entra
               xm1 = x + 1;
               x_1 = x - 1;
							 this->minimo = this->raioMaximo;	// assume valor maximo
               //  4  3  4      z=0
               // (3) x  3
               // (4)(3)(4)
							 this->min (this->data3D[x_1][y][z] + mi);	// ponto[x][y]
							 this->min (this->data3D[x_1][y_1][z] + mj);
							 this->min (this->data3D[x][y_1][z] + mi);
							 this->min (this->data3D[xm1][y_1][z] + mj);
               // (5)(4)(5)     z=-1
               // (4)(3)(4)
               // (5)(4)(5)
							 this->min (this->data3D[x_1][ym1][z_1] + mk);
							 this->min (this->data3D[x][ym1][z_1] + mj);
							 this->min (this->data3D[xm1][ym1][z_1] + mk);
							 this->min (this->data3D[x_1][y][z_1] + mj);
							 this->min (this->data3D[x][y][z_1] + mi);
							 this->min (this->data3D[xm1][y][z_1] + mj);
							 this->min (this->data3D[x_1][y_1][z_1] + mk);
							 this->min (this->data3D[x][y_1][z_1] + mj);
							 this->min (this->data3D[xm1][y_1][z_1] + mk);
							 this->data3D[x][y][z] = this->minimo;
            }
         }
      }
   }
   // -------------------------
   //  IDFNosPlanosDeContornoVOLTA(mi);
   // MinimoVolta
	 for (z = this->nz - 2; z > 0; z--) { // -2 pois já considerou plano z-1 acima
      zm1 = z + 1;
			for (y = this->ny - 2; y > 0; y--) {
         ym1 = y + 1;
         y_1 = y - 1;
				 for (x = this->nx - 2; x > 0; x--) {
						if (this->data3D[x][y][z] != 0) { // Se nao for solido
               xm1 = x + 1;
               x_1 = x - 1;
							 this->minimo = this->data3D[x][y][z];	// Armazena valor minimo da ida
               // (4)(3)(4)     z=0
               // (3) x  3
               //  4  3  4
							 this->min (this->data3D[x_1][ym1][z] + mj);
							 this->min (this->data3D[x][ym1][z] + mi);
							 this->min (this->data3D[xm1][ym1][z] + mj);	// bug
							 /*ponto[x][y] */ this->min (this->data3D[xm1][y][z] + mi);
               // (5)(4)(5)     z=+1
               // (4)(3)(4)
               // (5)(4)(5)
							 this->min (this->data3D[x_1][ym1][zm1] + mk);
							 this->min (this->data3D[x][ym1][zm1] + mj);
							 this->min (this->data3D[xm1][ym1][zm1] + mk);
							 this->min (this->data3D[x_1][y][zm1] + mj);
							 this->min (this->data3D[x][y][zm1] + mi);
							 this->min (this->data3D[xm1][y][zm1] + mj);
							 this->min (this->data3D[x_1][y_1][zm1] + mk);
							 this->min (this->data3D[x][y_1][zm1] + mj);
							 this->min (this->data3D[xm1][y_1][zm1] + mk);

							 this->data3D[x][y][z] = this->minimo;
            }
         }
      }
   }
	 return this->pm;
}
