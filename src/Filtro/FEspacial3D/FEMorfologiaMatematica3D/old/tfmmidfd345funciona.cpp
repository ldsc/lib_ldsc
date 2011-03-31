//Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef  TFMMIDFd345_cpp
#define  TFMMIDFd345_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
				Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:		Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TFMMIDFd345.cpp
Nome da classe:      TFMMIDFd345
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a função CriaMascara da classe TFMMIDFd345.
*/

//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------
#define TFiltroDLL		//define como sendo uma dll
#include "TFiltro\TFEspacial3D\TFEMorfologiaMatematica3D\TFMMIDFd345.h"	//Classe base
#include "TMascara\TMascara3D\TMCd345.h"	//ponteiro para
//#include "TMascara\TMascMascDiscretad34.h"            //Cria objeto

/*
============================================================================
Documentacao construtor
============================================================================
Executa o construtor da classe base, o TFMMIDF(_pm)
que realiza a copia dos dados.
*/
TFMMIDFd345::TFMMIDFd345 (TMatriz3D * &matriz, unsigned int _tamanhoMascara,
			  unsigned int _raioMax):
TFMMIDF3D (matriz, _tamanhoMascara, _raioMax)
{
}
TFMMIDFd345::TFMMIDFd345 (TMatriz3D * &matriz, unsigned int _tamanhoMascara):
TFMMIDF3D (matriz, _tamanhoMascara)
{
}

TFMMIDFd345::TFMMIDFd345 (TMatriz3D * &matriz):TFMMIDF3D (matriz)
{
}

/*
============================================================================
Documentacao 		CriaMascara
============================================================================
Descrição:        Funcao que cria a mascara de chanfro adequada.
						O filtro TFMMIDF recebe a imagem pm, e o tamanho da mascara.
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
void
TFMMIDFd345::CriaMascara (unsigned int _tamanhoMascara)
{
  if (mask)			//se existe uma mascara
    {
      if (mask->GetNX () == _tamanhoMascara)	//e é do mesmo  tamanho
	return;			//sai
      delete mask;		//se  não é do mesmo tamanho apaga
    }				//e abaixo cria uma nova

  mask = new TMCd345 (_tamanhoMascara);	//se não existe a mascara, cria uma nova
}

/*{ if(mask)
	delete mask;
mask=new TMCd34(tamanhoMascara);
} */

/*
============================================================================
Documentacao      Go
============================================================================
Descrição:
//Substitui o uso da mascara da mascara, que usava loop's e índices confusos
//pelo acesso direto aos valores da mascara.
//Ao lado do código o esboço do ponto da mascara que esta sendo considerado.
//Da forma como esta o código fica mais rápido e compreensivo.
 //TIDF
 //Por uma questão de performance, deixar como esta.
 //Mas criar na classe base a mesma funcao mas com codigo generico
 //usando coisas do tipo  min (data3D[x][y][z] + masc->data3D[x][y][z])
 //Funcao IDFNosPlanosDeContorno();//preenche os planos de contorno com valor base
Programador:      Andre Duarte Bueno
*/
TMatriz3D *
TFMMIDFd345::Go (TMatriz3D * &matriz, unsigned int _tamanhoMascara)
{
  InicializaIDF (matriz, _tamanhoMascara);	//armazena valores da matriz e _tamanhoMascara
  //verifica se pm->data2D e this->data2D tem as mesmas dimensoes
  //preenche os valores de data2D com os valores de pm
  //int register auxiliar;                       //
  //int register minimo;                                                         //atributo da classe por causa funcao inline
  int x, y, z;			//Indices para percorrer a matriz
  IDFNosPlanosDeContorno (3);

  //-------------------------
  //MinimoIda
  for (z = 1; z < nz - 1; z++)	//inicio do 1 pois já considerou planos 0 acima
    for (y = 1; y < ny - 1; y++)
      for (x = 1; x < nx - 1; x++)
	if (data3D[x][y][z] != 0)	//Testa a imagem, se nao for solido entra
	  {			//
	    minimo = raioMaximo;	//assume valor maximo
	    // 4  3  4      z=0
	    //(3) x  3
	    //(4)(3)(4)
	    min (data3D[x - 1][y][z] + 3);
	    min (data3D[x - 1][y - 1][z] + 4);
	    min (data3D[x][y - 1][z] + 3);
	    min (data3D[x + 1][y - 1][z] + 4);
	    //(5)(4)(5)     z=-1
	    //(4)(3)(4)
	    //(5)(4)(5)
	    min (data3D[x - 1][y + 1][z - 1] + 5);
	    min (data3D[x][y + 1][z - 1] + 4);
	    min (data3D[x + 1][y + 1][z - 1] + 5);
	    min (data3D[x - 1][y][z - 1] + 4);
	    min (data3D[x][y][z - 1] + 3);
	    min (data3D[x][y][z - 1] + 4);
	    min (data3D[x - 1][y - 1][z - 1] + 5);
	    min (data3D[x][y - 1][z - 1] + 4);
	    min (data3D[x + 1][y - 1][z - 1] + 5);
	    data3D[x][y][z] = minimo;
	  }

  //-------------------------
  //MinimoVolta
  for (z = nz - 2; z > 0; z--)	//-2 pois já considerou plano z-1 acima
    for (y = ny - 2; y > 0; y--)
      for (x = nx - 2; x > 0; x--)
	if (data3D[x][y][z] != 0)	//Se nao for solido
	  {
	    minimo = data3D[x][y][z];	//Armazena valor minimo da ida
	    //(4)(3)(4)     z=0
	    //(3) x  3
	    // 4  3  4
	    min (data3D[x - 1][y + 1][z] + 4);
	    min (data3D[x][y + 1][z] + 3);
	    min (data3D[x + 1][y + 1][z] + 4);	//bug
	    min (data3D[x + 1][y][z] + 3);
	    //(5)(4)(5)     z=+1
	    //(4)(3)(4)
	    //(5)(4)(5)
	    min (data3D[x - 1][y + 1][z + 1] + 5);
	    min (data3D[x][y + 1][z + 1] + 4);
	    min (data3D[x + 1][y + 1][z + 1] + 5);
	    min (data3D[x - 1][y][z + 1] + 4);
	    min (data3D[x][y][z + 1] + 3);
	    min (data3D[x + 1][y][z + 1] + 4);
	    min (data3D[x - 1][y - 1][z + 1] + 5);
	    min (data3D[x][y - 1][z + 1] + 4);
	    min (data3D[x + 1][y - 1][z + 1] + 5);

	    data3D[x][y][z] = minimo;
	  }

  return pm;
}

//IDFNosPlanosDeContorno(int& base);
void
TFMMIDFd345::IDFNosPlanosDeContorno (int &base)
{
  //-------------------------
  //Percorrer todos os planos, se ativo, fazer=3 (menor distancia)
  //Desta forma posso percorrer o centro da imagem direto,sem me preocupar com acesso a pontos inexistentes.
  //Pensar em : pm->setPlanoX(corte=0,valor=3,condicaoPixel==1);
  int pos;			//posicao intermediaria (temporaria)
  int x, y, z;			//Indices para percorrer a matriz
  for (y = 0; y < ny; y++)	//plano back z=0
    for (x = 0; x < nx; x++)
      if (data3D[x][y][0] != 0)
	data3D[x][y][0] = base;	//generico =masc->data3D[x][y][plano];
  pos = nz - 1;
  for (y = 0; y < ny; y++)	//plano front z=nz-1
    for (x = 0; x < nx; x++)
      if (data3D[x][y][pos] != 0)
	data3D[x][y][pos] = base;

  for (x = 0; x < nx; x++)	//plano sul y=0
    for (z = 0; z < nz; z++)
      if (data3D[x][0][z] != 0)
	data3D[x][0][z] = base;
  pos = ny - 1;
  for (x = 0; x < nx; x++)	//plano norte y=ny-1
    for (z = 0; z < nz; z++)
      if (data3D[x][pos][z] != 0)
	data3D[x][pos][z] = base;

  for (y = 0; y < ny; y++)	//plano leste x=0
    for (z = 0; z < nz; z++)
      if (data3D[0][y][z] != 0)
	data3D[0][y][z] = base;
  pos = nx - 1;
  for (y = 0; y < ny; y++)	//plano oeste x=nx-1
    for (z = 0; z < nz; z++)
      if (data3D[pos][y][z] != 0)
	data3D[pos][y][z] = base;
}

//Funcoes inline
void
TFMMIDFd345::min (int &t)
{
  if (t < minimo)
    minimo = t;
}

#endif
