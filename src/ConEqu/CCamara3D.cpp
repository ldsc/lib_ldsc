
/*
----------------------------------------------------------------------------
PROJETO:			LIB_LDSC
				Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi,	Marcos Damiani,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CCamara3D.h
Nome da classe:      CCamara3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/
#include <ConEqu/CCamara3D.h>

/**
 * Funcao que recebe um ponteiro para a imagem, e cria a camara
 * primeiro definindo as dimensoes da camara, depois calculando as dimensoes
 * finais da imagem, a seguir aloca a matriz de dados e entao,
 * define os valores do interior da camara a partir dos dados da imagem e dos
 * indices das paredes.

 * @param img 
 * @return 
 */
TCMatriz3D<int> *CCamara3D::CriaCamara(TCMatriz3D<int> * &img) // ,int raioMaximo)
{
  // dimensoesPadroes(raioMaximo);  
  // função que define as dimesões padrões da camara em funcao do raioMaximo

  // Armazena as informações das dimensoes da imagem do meio poroso
  nxImg = img->NX ();
  nyImg = img->NY ();
  nzImg = img->NZ ();

  // Definição das dimensões das paredes da camara
  // (chamada pelo construtor, ou posteriormente pelo usuário)
  // dimensoesParedesCamara();

  // Calculo das dimensões finais da imagem da camara
  // considerando as paredes + a imagem do meio poroso
  nx = paredeEsquerda + nxImg + paredeDireita;
  ny = camaraInferior + membranaInferior + nyImg + membranaSuperior +    camaraSuperior;
  nz = paredeEsquerda + nzImg + paredeDireita;

  path = img->path;
  // Aloca memoria para a camara
  // Potencial bug: Vazamento de memória: se data3D ja existe, então deveria deletar e setar com NULL.
  data3D = AlocaMatriz3D (nx, ny, nz);

  // Preenchimento das paredes da camara com o valor indice=FUNDO (PRETO)
  // Preenchimento das membranas...
  DefineCamara ( /*indiceParedes */ );

  // Copia das informações da imagem do meio poroso para a camara
  DefineImagem (img);
  return this;
}

/**
 * // |*|*****************|*|
 * // |*|-----------------|*|          z
 * // |*|                 |*|        /\
 * // |*|                 |*|    y  .
 * // |*|                 |*|   /|\.
 * // |*|-----------------|*|    |.
 * // |$$$$$$$$$$$$$$$$$$$$$|    --> x
  
 */
void CCamara3D::DefineCamaraInferior ( /*int indice */ )
{
  for (int i = 0; i < nx; i++)
    for (int j = 0; j < camaraInferior && j < ny; j++)
      for (int k = 0; k < nz; k++)
	   data3D[i][j][k] = indiceCamaraInferior;
}

/**
|$$$$$$$$$$$$$$$$$$$$$|
|*|-----------------|*|          z
|*|                 |*|        /\
|*|                 |*|    y  .
|*|                 |*|   /|\.
|*|-----------------|*|    |.
|*********************|    --> x

 */
void CCamara3D::DefineCamaraSuperior ( /*int indice */ )
{
  for (int i = 0; i < nx; i++)	// j=ny-membranaSuperior
    for (int j = camaraInferior + membranaInferior + nyImg + membranaSuperior; j < ny; j++)
      for (int k = 0; k < nz; k++)
	   data3D[i][j][k] = indiceCamaraSuperior;
}

/**
          k                     k+1
|*********************| |*********************|
|*|-----------------|*| |*|-----------------|*|
|*|                 |*| |*|                 |*|
|*|                 |*| |*|                 |*|
|*|                 |*| |*|                 |*|
|*|$ $ $ $ $ $ $ $ $|*| |*| $ $ $ $ $ $ $ $ |*|<-Observe que são alternados
|*********************| |*********************|

i  :  01010101010101
i+1:  10101010101010

Obs: dimensao nx e nz par:
Quando uso i+2, (e k+2) pula de 2 em 2, preenchendo corretamente o espaço.
    0123456
    012345 	 0ababab?
    1ababab      1bababa?
    2bababa      2ababab?
    3ababab      3bababa? <- Linha vertical nao preenchida
    4bababa      4ababab?
    5ababab      5bababa?
    6??????? < - linha 0 < x < nx, z = nz-1 nao preenchida

    Obs: dimensao nx e nz impar:
    Usa i+2, e k+2, pula de 2 em 2, NAO preenchendo o ultimo ponto.
    1234567
    010101?
    Em funcao desta possibilidade, o ultimo ponto é preenchido após o for.
*/
// Membrana de 2 em 2 pixel's
// o primeiro e "pintado" com a cor da parede e o outro com a cor da camara inferior
void CCamara3D::DefineMembranaInferior ( /*int indice */ )
{
  for (int i = paredeEsquerda; i < nx - paredeDireita - 1; i += 2)	// -1 porque abaixo usa +1
    for (int j = camaraInferior; j < camaraInferior + membranaInferior && j < ny; j++)	// 
      for (int k = paredeEsquerda; k < nz - paredeDireita - 1; k += 2)
	{
	  data3D[i][j][k] = indiceParedes;
	  data3D[i + 1][j][k] = indiceCamaraInferior;

	  data3D[i + 1][j][k + 1] = indiceParedes; // obs:invertido
	  data3D[i][j][k + 1] = indiceCamaraInferior;
	}
	
// Write("DefineCamaraInferior-apos-forforfor.txt");
  // Pintura dos pontos finais no caso de imagens com dimensao impar.
  // Varrendo a ultima linha na direcao x (z=nz-1)
  int k = nz - 1;
  for (int i = paredeEsquerda; i < nx - paredeDireita - 1; i += 2)	// -1 porque abaixo usa +1
    for (int j = camaraInferior; j < camaraInferior + membranaInferior && j < ny; j++)
      {
	data3D[i][j][k] = indiceParedes;
	data3D[i + 1][j][k] = indiceCamaraInferior;
      }
  // Varrendo a ultima linha na direcao z (x=nx-1)
  int i = nx - 1;
  for (int j = camaraInferior; j < camaraInferior + membranaInferior && j < ny; j++)
    for (int k = paredeEsquerda; k < nz - paredeDireita - 1; k += 2)
      {
	data3D[i][j][k] = indiceParedes;
	data3D[i][j][k + 1] = indiceCamaraInferior;
      }
}

/**
          k                     k+1
|*********************| |*********************|
|*|$ $ $ $ $ $ $ $ $|*| |*| $ $ $ $ $ $ $ $ |*|<-Observe que são alternados
|*|                 |*| |*|                 |*|
|*|                 |*| |*|                 |*|
|*|                 |*| |*|                 |*|
|*|-----------------|*| |*|-----------------|*|
|*********************| |*********************|
*/
void CCamara3D::DefineMembranaSuperior ( /*int indice */ )
{
  for (int i = paredeEsquerda; i < nx - paredeDireita - 1; i += 2)
    for (int j = camaraInferior + membranaInferior + nyImg; j < camaraInferior + membranaInferior + nyImg + membranaSuperior && j < ny; j++)
      for (int k = paredeEsquerda; k < nz - paredeDireita - 1; k += 2)
	{
	  data3D[i][j][k] = indiceParedes;
	  data3D[i + 1][j][k] = indiceCamaraSuperior;

	  data3D[i][j][k + 1] = indiceCamaraSuperior;	// Obs:Alternado
	  data3D[i + 1][j][k + 1] = indiceParedes;
	}
	
  // Pintura dos pontos finais no caso de imagens com dimensao impar.
  // Varrendo a ultima linha na direcao x (z=nz-1)
  int k = nz - 1;
  for (int i = paredeEsquerda; i < nx - paredeDireita - 1; i += 2)	// -1 porque abaixo usa +1
    for (int j = camaraInferior; j < camaraInferior + membranaInferior && j < ny; j++)
      {
	data3D[i][j][k] = indiceParedes;
	data3D[i + 1][j][k] = indiceCamaraSuperior;
      }
      
  // Varrendo a ultima linha na direcao z (x=nx-1)
  int i = nx - 1;
  for (int j = camaraInferior; j < camaraInferior + membranaInferior && j < ny; j++)
    for (int k = paredeEsquerda; k < nz - paredeDireita - 1; k += 2) {
	 data3D[i][j][k] = indiceParedes;
	 data3D[i][j][k + 1] = indiceCamaraSuperior;
    }
}


/**
Parede na direcao x ( 0 <= x < paredeEsquerda ), 0 <= z < nz
 
|$********************|
|$|-----------------|*|          y
|$|                 |*|        /\
|$|                 |*|    z  .
|$|                 |*|   /|\.
|$|-----------------|*|    |.
|$********************|    -----> x
  
Parede na direcao z
 * 
|*********************|
|*|-----------------|*|          x
|*|                 |*|        /\
|*|                 |*|    z  .
|*|                 |*|   /|\.
|*|-----------------|*|    |.
|**$$$$$$$$$$$$$$$$$**|    --> x
 */
void CCamara3D::DefineParedeEsquerda ( /*int indice */ )
{
  int i, j, k;
/*
  Parede na direcao x (0<=x<paredeEsquerda), 0<=z<nz
 
|$********************|
|$|-----------------|*|          y
|$|                 |*|        /\
|$|                 |*|    z  .
|$|                 |*|   /|\.
|$|-----------------|*|    |.
|$********************|    --> x
*/

for (i = 0; i < paredeEsquerda && i < nx; i++)
    for (j = camaraInferior; j < ny - camaraSuperior; j++)
      for (k = 0; k < nz; k++)
	   data3D[i][j][k] = indiceParedes;

// Parede na direcao z
/*
|*********************|
|*|-----------------|*|          x
|*|                 |*|        /\
|*|                 |*|    z  .
|*|                 |*|   /|\.
|*|-----------------|*|    |.
|**$$$$$$$$$$$$$$$$$**|    --> x
*/
  for (i = paredeEsquerda; i < nx - paredeEsquerda; i++)
    for (j = camaraInferior; j < ny - camaraSuperior; j++)
      for (k = 0; k < paredeEsquerda; k++)
	   data3D[i][j][k] = indiceParedes;
}

/**
Parede na direcao x
|********************$|
|*|-----------------|$|          y
|*|                 |$|        /\
|*|                 |$|    z  .
|*|                 |$|   /|\.
|*|-----------------|$|    |.
|********************$|    --> x

Parede na direcao z
|**$$$$$$$$$$$$$$$$$**|
|*|-----------------|*|          y
|*|                 |*|        /\
|*|                 |*|    z  .
|*|                 |*|   /|\.
|*|-----------------|*|    |.
|*********************|    --> x

 */
void CCamara3D::DefineParedeDireita ( /*int indice */ )
{
  int i, j, k;
  
  // Parede na direcao x
/*
|********************$|
|*|-----------------|$|          y
|*|                 |$|        /\
|*|                 |$|    z  .
|*|                 |$|   /|\.
|*|-----------------|$|    |.
|********************$|    --> x
*/
  for (i = paredeEsquerda + nxImg; i < nx; i++)
    for (j = camaraInferior; j < ny - camaraSuperior && j < ny; j++)
      for (k = 0; k < nz; k++)
	   data3D[i][j][k] = indiceParedes;

// Parede na direcao z
/*
|**$$$$$$$$$$$$$$$$$**|
|*|-----------------|*|          y
|*|                 |*|        /\
|*|                 |*|    z  .
|*|                 |*|   /|\.
|*|-----------------|*|    |.
|*********************|    --> x
*/
  for (i = paredeEsquerda; i < nx - paredeEsquerda; i++)
    for (j = camaraInferior; j < ny - camaraSuperior; j++)
      for (k = paredeEsquerda + nzImg; k < nz; k++)
	   data3D[i][j][k] = indiceParedes;
}

/**
 Desenha a imagem no centro da camara
 Copia a imagem do meio poroso para a camara
 Permite trocar a imagem localizada na camara sem reconstruir toda a camara
 Desde que a imagem tenha as mesmas dimensoes.
 Se a imagem tiver outras dimensoes é necessario redefinir a camara.void

|*********************|
|*|-----------------|*|          z
|*|SSSSSSSSSSSSSSSSS|*|        /\
|*|SSSSSSSSSSSSSSSSS|*|    y  .
|*|SSSSSSSSSSSSSSSSS|*|   /|\.
|*|-----------------|*|    |.
|*********************|    --> x
*/
void CCamara3D::DefineImagem (TCMatriz3D<int> *&img)
{
  int minx = (img->NX () > this->nxImg) ? this->nxImg : img->NX ();
  int miny = (img->NY () > this->nyImg) ? this->nyImg : img->NY ();
  int minz = (img->NZ () > this->nzImg) ? this->nzImg : img->NZ ();

  for (int x = 0; x < minx; x++)	// Percorre a imagem
    for (int y = 0; y < miny; y++)	// Copia imagem para centro da camara
      for (int z = 0; z < minz; z++)
	   if (img->data3D[x][y][z] != 0)
	     data3D[paredeEsquerda + x][camaraInferior + membranaInferior + y][paredeEsquerda + z] = indiceCamaraSuperior;	// =img->data3D[x][y][z];
	   else
	     data3D[paredeEsquerda + x][camaraInferior + membranaInferior + y][paredeEsquerda + z] = 0;

/* Ja localiza imagem:
  for(int x=paredeEsquerda; x < minx+paredeEsquerda; x++)
	 for(int y=camaraInferior + membranaInferior; y < camaraInferior + membranaInferior+miny; y++)
	  for(int z=paredeEsquerda; z < minz+paredeEsquerda; z++)
         data3D[x][y][z]=indiceCamaraSuperior;	// =img->data3D[x][y][z];
*/
}

// Pensar em criar funcao redefineImagem(imagem)
