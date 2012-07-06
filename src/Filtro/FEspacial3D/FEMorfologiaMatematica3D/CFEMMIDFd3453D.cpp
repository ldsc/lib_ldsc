// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ].

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFd3453D.cpp
Nome da classe:      CFEMMIDFd3453D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a função CriaMascara da classe CFEMMIDFd3453D.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMIDFd3453D.h"	// Classe base
// #include "TMascara\CMCDiscreta3D\CBCd345.h"          // ponteiro para
// #include "TMascara\TMascMascDiscretad34.h"           // Cria objeto


/*
==================================================================================
Documentacao 		CriaMascara
==================================================================================
Descrição:        Funcao que cria a mascara de chanfro adequada.
						O filtro CFEMMIDF recebe a imagem pm, e o tamanho da mascara.
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
/*
void CFEMMIDFd3453D::CriaMascara(unsigned int _tamanhoMascara)
{
 if(mask)										// se existe uma mascara
	{
	 if(mask->NX()==_tamanhoMascara)	// e é do mesmo  tamanho
		  return;                      	// sai
	 delete mask;                     	// se  não é do mesmo tamanho apaga
	}                                   // e abaixo cria uma nova

 mask=new CBCd345(_tamanhoMascara); 		// se não existe a mascara, cria uma nova
} */

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
/*
CMatriz3D *CFEMMIDFd3453D::Go( CMatriz3D *& matriz, unsigned int _tamanhoMascara )
{
 InicializaIDF(matriz,_tamanhoMascara);		// armazena valores da matriz e _tamanhoMascara
						// verifica se pm->data2D e this->data2D tem as mesmas dimensoes
						// preenche os valores de data2D com os valores de pm
 // int register auxiliar;              		// 
 // int register minimo;				// atributo da classe por causa funcao inline
 int x,y,z;					// Indices para percorrer a matriz
 IDFNosPlanosDeContorno(3);
 // -------------------------
 // MinimoIda
	for (z=1; z < nz-1 ;z++)		// inicio do 1 pois já considerou planos 0 acima
	 for (y=1; y < ny-1 ;y++)
    	for (x=1; x < nx-1 ;x++)
			if (data3D[x][y][z]!=0)	// Testa a imagem, se nao for solido entra
				{                          // 
				minimo = raioMaximo;	// assume valor maximo
				//  4  3  4	z=0
				// (3) x  3
				// (4)(3)(4)
				min(data3D[x-1][y]  [z] + 3);
				min(data3D[x-1][y-1][z] + 4); min(data3D[x][y-1][z] + 3); min(data3D[x+1][y-1][z] + 4);
				// (5)(4)(5)	z=-1
				// (4)(3)(4)
				// (5)(4)(5)
				min(data3D[x-1][y+1][z-1] + 5);   min(data3D[x][y+1][z-1] + 4);	min(data3D[x+1][y+1][z-1] + 5);
            min(data3D[x-1][y]  [z-1] + 4);   min(data3D[x][y]  [z-1] + 3);   min(data3D[x]  [y]  [z-1] + 4);
				min(data3D[x-1][y-1][z-1] + 5);   min(data3D[x][y-1][z-1] + 4);	min(data3D[x+1][y-1][z-1] + 5);
				data3D[x][y][z]=minimo;
				}

 // -------------------------
 // MinimoVolta
 for (z = nz-2; z > 0; z--)						  // -2 pois já considerou plano z-1 acima
	 for (y = ny-2; y > 0; y--)
		for (x = nx-2; x > 0; x--)
			if (data3D[x][y][z]!=0)               // Se nao for solido
			  {
			  minimo = data3D[x][y][z];           // Armazena valor minimo da ida
				// (4)(3)(4)	z=0
				// (3) x  3
				//  4  3  4
			  min(data3D[x-1][y+1][z] + 4);  min(data3D[x][y+1][z]+3);  	min(data3D[x+1][y+1][z] + 4);// bug
								      min(data3D[x+1][y]  [z] + 3);
   			// (5)(4)(5)	z=+1
				// (4)(3)(4)
				// (5)(4)(5)
			  min(data3D[x-1][y+1][z+1]+5);   min(data3D[x][y+1][z+1]+4);  min(data3D[x+1][y+1][z+1]+5);
			  min(data3D[x-1][y  ][z+1]+4);	 min(data3D[x][y]  [z+1]+3);	min(data3D[x+1][y]  [z+1]+4);
			  min(data3D[x-1][y-1][z+1]+5);   min(data3D[x][y-1][z+1]+4);  min(data3D[x+1][y-1][z+1]+5);

			  data3D[x][y][z]=minimo;
			  }

return pm;
}
*/

