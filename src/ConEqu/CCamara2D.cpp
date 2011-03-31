/*--------------------------------------------------------------------------
PROJETO:	LIB_LDSC
Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:		Laboratorio de Desenvolvimento de Software Cientifico.
Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, Marcos Damiani,...
Copyright @1997:  		Todos os direitos reservados.
Nome deste arquivo:		CCamara2D.h
Nome da classe:      	CCamara2D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
--------------------------------------------------------------------------*/
#include <ConEqu/CCamara2D.h>

/**
 * Funcao que recebe um ponteiro para a imagem e cria a camara;
 * Primeiro definindo as dimensoes da camara,
 * depois calculando as dimensoes finais da imagem da camara.
 * A seguir aloca a matriz de dados e então,
 * define os valores do interior da camara a partir dos dados da imagem e dos indices das paredes.
 * @param img 
 * @return *CCamara2D
 */
CMatriz2D *CCamara2D::CriaCamara ( CMatriz2D * &img )
{
  // Função que define as dimensões padrões da camara em função do raioMaximo
  // dimensoesPadroes(raioMaximo);       
  
  // Armazena as informações das dimensoes da imagem do meio poroso
  nxImg = img->NX();
  nyImg = img->NY();
  nzImg = 0 ;

  // Definição das dimensões das paredes da camara
  // (chamada pelo construtor, ou posteriormente pelo usuário)
  // dimensoesParedesCamara();

  // Calculo das dimensões finais da imagem da camara
  // considerando as paredes + a imagem do meio poroso
  nx = paredeEsquerda + nxImg + paredeDireita;
  ny = camaraInferior + membranaInferior + nyImg + membranaSuperior + camaraSuperior;
	
  path = img->path;
   
  // Aloca memoria para a camara
  // Potencial bug - perda de memória: deveria verificar se data2D já existe, 
  // caso afirmativo, deletar, e setar com NULL
  data2D = AlocaMatriz2D(  nx ,  ny );
  
  // Preenchimento das paredes da camara com o valor indice=FUNDO (PRETO)
  DefineCamara( /*indiceParedes */ );
  
  // Copia das informacoes da imagem do meio poroso para a camara
  DefineImagem(img);
  
  return this; // retorna a camara
}

/*
Definida na classe base:
void CCamara::DefineCamara(int indice)
{
  DefineCamaraSuperior ();
  DefineMembranaSuperior	();
  DefineParedeEsquerda ();
  DefineParedeDireita ();
  DefineMembranaInferior	();
  DefineCamaraInferior ();
}
*/

/*
|$$$$$$$$$$$$$$$$$$$$$|
|*|-----------------|*|          z
|*|                 |*|        /\
|*|                 |*|    y  .
|*|                 |*|   /|\.
|*|-----------------|*|    |.
|*********************|    --> x
*/
void
CCamara2D::DefineCamaraSuperior ( /*int Índice */ )
{
  for ( int i = 0; i < nx; i++ )	// j=ny-membranaSuperior
    for ( int j = camaraInferior + membranaInferior + nyImg + membranaSuperior ; j < ny ; j++ )
      data2D[i][j] = indiceCamaraSuperior;
/*
   Write("OBJcamara-1_DefineCamaraSuperior.txt");
*/
}

/*
|*********************|
|*|$$$$$$$$$$$$$$$$$|*|          z
|*|                 |*|        /\
|*|                 |*|    y  .
|*|                 |*|   /|\.
|*|-----------------|*|    |.
|*********************|    --> x
*/
// Membrana de 2 em 2 pixel's
// Os pontos da membrana assumem o mesmo indice das paredes da camara (indiceParedes)
// Os outros pontos assumem o indice da camara superior
void CCamara2D::DefineMembranaSuperior ( /*int Índice */ )
{
  for ( int i = paredeEsquerda; i < nx - paredeDireita - 1; i += 2 )
    for ( int j = camaraInferior + membranaInferior + nyImg;	// j=ny-camaraSuperior-membranaSuperior
	 j < camaraInferior + membranaInferior + nyImg + membranaSuperior && j < ny; j++ )	
		  // j < ny-camaraSuperior
      {
		data2D[i][j] = indiceParedes;			// Assume a cor da parede
		data2D[i + 1][j] = indiceCamaraSuperior;	// assume a cor da camara superior
      }
// Write("OBJcamara-2_DefineMembranaSuperior.txt");
}

/*
|*********************|
|$|-----------------| |          z
|$|                 | |        /\
|$|                 | |    y  .
|$|                 | |   /|\.
|$|-----------------| |    |.
|*********************|    --> x
*/
void CCamara2D::DefineParedeEsquerda ( /*int Índice */ )
{
  for (int i = 0; i < paredeEsquerda && i < nx; i++)	// Parede esquerda
    for (int j = camaraInferior; j < ny - camaraSuperior; j++)
      data2D[i][j] = indiceParedes;
// Write("OBJcamara-3_DefineParedeEsquerda.txt");
}


/*
|*********************|
| |-----------------|$|          z
| |                 |$|        /\
| |                 |$|    y  .
| |                 |$|   /|\.
| |-----------------|$|    |.
|*********************|    --> x
*/
void CCamara2D::DefineParedeDireita ( /*int Índice */ )
{
  for (int i = paredeEsquerda + nxImg; i < nx; i++)	// Parede Direita
    for (int j = camaraInferior; j < ny - camaraSuperior; j++)
      data2D[i][j] = indiceParedes;
// Write("OBJcamara-4_DefineParedeDireita.txt");
}

/*
|*********************|
|*|-----------------|*|          z
|*|                 |*|        /\
|*|                 |*|    y  .
|*|                 |*|   /|\.
|*|$$$$$$$$$$$$$$$$$|*|    |.
|*********************|    --> x
*/
/*
 Membrana de 2 em 2 pixel's
 Os pontos da membrana assumem o mesmo indice das paredes da camara (indiceParedes)
 Os outros pontos assumem o indice da camara inferior
*/
void CCamara2D::DefineMembranaInferior ( /*int Índice */ )
{
  for (int i = paredeEsquerda; i < nx - paredeDireita - 1; i += 2)	
	  							// -1 porque abaixo +1
    for (int j = camaraInferior; j < camaraInferior + membranaInferior && j < ny; j++)	// 
    {
		data2D[i][j] = indiceParedes;			// Um pixel assume cor da parede
		data2D[i + 1][j] = indiceCamaraInferior;	// e o outro da membrana inferior
    }
// Write("OBJcamara-5_DefineMembranaInferior.txt");
}

/*
|*|*****************|*|
|*|-----------------|*|          z
|*|                 |*|        /\
|*|                 |*|    y  .
|*|                 |*|   /|\.
|*|-----------------|*|    |.
|$$$$$$$$$$$$$$$$$$$$$|    --> x
*/
// "Pinta" a camara inferior
void
CCamara2D::DefineCamaraInferior ( /*int indice */ )
{
  for ( int i = 0; i < nx; i++ )
    for ( int j = 0; j < camaraInferior && j < ny; j++ )
      data2D[i][j] = indiceCamaraInferior;
/*
   Write("OBJcamara-6_DefineCamaraInferior.txt");
*/
}

/*
|*********************|
|*|-----------------|*|          z
|*|SSSSSSSSSSSSSSSSS|*|        /\
|*|SSSSSSSSSSSSSSSSS|*|    y  .
|*|SSSSSSSSSSSSSSSSS|*|   /|\.
|*|-----------------|*|    |.
|*********************|    --> x
*/
/*
 Copia a imagem do meio poroso para a camara
 Permite trocar a imagem localizada na camara sem reconstruir toda a camara
 Desde que a imagem tenha as mesmas dimensoes.
 Se a imagem tiver outras dimensoes e necessario redefinir a camara chamando CriaCamara novamente.
 Desenha a imagem no centro da camara
 */
void CCamara2D::DefineImagem (CMatriz2D * &img)
{
// Obtem os menores valores, para evitar estouro de pilha
  int minx = (img->NX () > this->nxImg) ? this->nxImg : img->NX ();
  int miny = (img->NY () > this->nyImg) ? this->nyImg : img->NY ();

  for (int x = 0; x < minx; x++)	// Percorre a imagem
    for (int y = 0; y < miny; y++)	// Copia imagem para centro da camara
      if (img->data2D[x][y] != 0)
		data2D[paredeEsquerda + x][camaraInferior + membranaInferior + y] = indiceCamaraSuperior;
      else
		data2D[paredeEsquerda + x][camaraInferior + membranaInferior + y] = 0;
// Write("OBJcamara-7_DefineImagem.txt");
}

// Pensar em criar funcao RedefineImagem( imagem )

