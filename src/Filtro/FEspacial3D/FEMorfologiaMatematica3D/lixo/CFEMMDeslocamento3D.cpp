// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]


#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMDeslocamento.h"
#include "Mascara/CMCEuclidiana.h"

/*
============================================================================
Documentacao      CriaMascara
============================================================================
Descrição:
			operacao da morfologia matematica usando metodo do deslocamento
			Se a mascara ja existe deve ser eliminada

   Programador:      Andre Duarte Bueno
*// *
      void   CFEMMDeslocamento::CriaMascara(unsigned int _tamanhoMascara)
      {
      if(mask)                                   // se existe uma mascara
      {
      if(mask->GetNX()==_tamanhoMascara)        // e é do mesmo  tamanho
      return;                           // sai
      delete mask;                      // se  não é do mesmo tamanho apaga
      }                                   // e abaixo cria uma nova

      mask=new CMCEuclidiana(_tamanhoMascara);          // se não existe a mascara, cria uma nova
      }   */
/*{
 if(mask)
	delete mask;

 mask=new CMCEuclidiana(_tamanhoMascara);
}    */
/*
============================================================================
Documentacao      Erosao
============================================================================
Descrição:
Operacao da morfologia matematica usando metodo classico, deslocamento do EE.
Realiza operacao classica de erosao, percorre a imagem e compara com o EE (a mascara)
Se a multiplicacao da mascara pela imagem der algum valor zero, deve erodir

Programador:      Andre Duarte Bueno
*/
 TMatriz2D * CFEMMDeslocamento::Erosao (TMatriz2D * &matriz,
					unsigned int _RaioBola) 
{
  pm = matriz;
  tamanhoMascara = 2 * _RaioBola + 1;
  CriaMascara (tamanhoMascara);	// criada na classe CFEMorfologicos
  unsigned int
    i,
    j,
    k,
    l;
  TMatriz2D
  rImg (pm);			// Cria uma copia da imagem
  unsigned int
    raioMascaraX = mask->GetRaioX ();	// variaveis auxiliares
  unsigned int
    raioMascaraY = mask->GetRaioY ();
  // Procede a operação de erosão desconsiderando a borda
  // Normal, centro da imagem
  for (i = raioMascaraX; i < pm->GetNX () - raioMascaraX; i++)
    for (j = raioMascaraY; j < pm->GetNY () - raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)	// se o ponto é poro >0, verifica se é para erodir
	  for (k = 0; k < mask->GetNX (); k++)
	    for (l = 0; l < mask->GetNY (); l++)
	      // Se um ponto da mascara estiver ativo
	      // e na imagem inativo, erodir o ponto central
	      if (mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == 0)	// <1
		{		// o mesmo que if( !rImg->data2D[i+k-rX][j+l-rY])
		  pm->data2D[i][j] = 0;
		  goto PulaForMask1;
		}
      PulaForMask1:
      }
  // Percorre a Borda superior
  for (i = raioMascaraX; i < pm->GetNX () - raioMascaraX; i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = 0; k < mask->GetNX (); k++)
	    for (l = raioMascaraY; l < mask->GetNY (); l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask2;
		}
      PulaForMask2:
      }
  // Percorre a Borda inferior
  for (i = raioMascaraX; i < pm->GetNX () - raioMascaraX; i++)
    for (j = pm->GetNY () - raioMascaraY; j < pm->GetNY (); j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = 0; k < mask->GetNX (); k++)
	    for (l = 0; l < mask->GetNY () - raioMascaraY; l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask3;
		}
      PulaForMask3:
      }
  // Percorre a Borda esquerda
  for (i = 0; i < raioMascaraX; i++)
    for (j = raioMascaraY; j < pm->GetNY () - raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = raioMascaraX; k < mask->GetNX (); k++)
	    for (l = 0; l < mask->GetNY (); l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask4;
		}
      PulaForMask4:
      }
  // Percorre a Borda direita
  for (i = pm->GetNX () - raioMascaraX; i < pm->GetNX (); i++)
    for (j = raioMascaraY; j < pm->GetNY () - raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = 0; k < mask->GetNX () - raioMascaraX; k++)
	    for (l = 0; l < mask->GetNY (); l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask5;
		}
      PulaForMask5:
      }
  // Percorre o Canto superior  esquerdo
  for (i = 0; i < raioMascaraX; i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = raioMascaraX; k < mask->GetNX (); k++)
	    for (l = raioMascaraY; l < mask->GetNY (); l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask6;
		}
      PulaForMask6:
      }
  // Percorre o Canto superior   direito
  for (i = pm->GetNX () - raioMascaraX; i < pm->GetNX (); i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = 0; k < mask->GetNX () - raioMascaraX; k++)
	    for (l = raioMascaraY; l < mask->GetNY (); l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask7;
		}
      PulaForMask7:
      }
  // Percorre o Canto  inferior esquerdo
  for (i = 0; i < raioMascaraX; i++)
    for (j = pm->GetNY () - raioMascaraY; j < pm->GetNY (); j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = raioMascaraX; k < mask->GetNX (); k++)
	    for (l = 0; l < mask->GetNY () - raioMascaraY; l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask8;
		}
      PulaForMask8:
      }
  // Percorre o Canto inferior   direito
  for (i = pm->GetNX () - raioMascaraX; i < pm->GetNX (); i++)
    for (j = pm->GetNY () - raioMascaraY; j < pm->GetNY (); j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = 0; k < mask->GetNX () - raioMascaraX; k++)
	    for (l = 0; l < mask->GetNY () - raioMascaraY; l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask9;
		}
      PulaForMask9:
      }
  return pm;
}


/*
// Modificações:
	// 2-Procede a operação de erosão separando a imagem em partes internas e externas.
	// a)Procede a operação de erosão no centro da imagem, desconsiderando a borda
	// 			...........
	// 			...*****...
	// 			...*****...
	// 			...*****...
	// 			...........
	// b)Procede a operação de erosão nas regiões da borda (superior, inferior, esquerda e direita)
	// 			  Superior
	// 			 ...*****...
	// 			 ***     ***
	// Esquerda***     *** Direita
	// 			 ***     ***
	// 			 ...*****...
	// 				Inferior
	// c)Procede a operação de erosão nos cantos da imagem
	// 	Canto Superior esquerdo		Canto superior direito
	// 							 ***.....***
	// 			 				 ...     ...
	// 			 				 ...     ...
	// 			 				 ...     ...
	// 			 				 ***.....***
	// Canto inferior esquerdo		canto inferior direito

	// Valores dos intervalos a serem percorridos
	// Normal, centro da imagem
	/*
	(x da imagem)	raioMascaraX; <-------->		pm->GetNX()-raioMascaraX
	(y da imagem)  raioMascaraY  <-------->		pm->GetNY()-raioMascaraY
	(x da mascara)	0				  <-------->		mask->GetNX()
	(y da mascara)	0				  <-------->		mask->GetNY()

*/

/*
============================================================================
Documentacao      Dilatacao
============================================================================
Descrição:
operacao da morfologia matematica usando metodo classico, com  deslocamento do EE
dilatacao classica, percorre a imagem com o EE e compara.
			  // Modificações:
			  // Pensar em trocar a multiplicação por uma combinação do tipo
			  // (mask->data2D[k][l] & rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY])  >0)
			  // o operador & retorna 1 se ambos os lados forem != de 0.

Programador:      Andre Duarte Bueno
*/
TMatriz2D *
CFEMMDeslocamento::Dilatacao (TMatriz2D * &matriz, unsigned int _RaioBola)
{
  pm = matriz;
  tamanhoMascara = 2 * _RaioBola + 1;
  CriaMascara (tamanhoMascara);	// criada na classe CFEMorfologicos
  unsigned int i, j, k, l;	// variaveis auxiliares
  TMatriz2D rImg (pm);		// copia a imagem
  unsigned int raioMascaraX = mask->GetRaioX ();	// variavel auxiliar para acelerar processo
  unsigned int raioMascaraY = mask->GetRaioY ();
  // percorre a matriz imagem, exceto a borda
  for (i = raioMascaraX; i < (pm->GetNX () - raioMascaraX); i++)
    for (j = raioMascaraY; j < (pm->GetNY () - raioMascaraY); j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->GetNX (); k++)
	    for (l = 0; l < mask->GetNY (); l++)
	      // se houver a interseccao de um ponto da bola
	      // com a imagem, o ponto i,j da imagem é dilatado
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask1;
		}
      PulaForMask1:
      }
  // Percorre a Borda superior
  // o y da imagem vai de 0 até raioMascaraY
  // o y da mascara começa de  raioMascaraY vai até  mask->GetNY()
  // ou seja, somente a borda superior
  for (i = raioMascaraX; i < pm->GetNX () - raioMascaraX; i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->GetNX (); k++)
	    for (l = raioMascaraY; l < mask->GetNY (); l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask2;
		}
      PulaForMask2:
      }
  // Percorre a Borda inferior
  for (i = raioMascaraX; i < pm->GetNX () - raioMascaraX; i++)
    for (j = pm->GetNY () - raioMascaraY; j < pm->GetNY (); j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->GetNX (); k++)
	    for (l = 0; l < mask->GetNY () - raioMascaraY; l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask3;
		}
      PulaForMask3:
      }

  // Percorre a Borda esquerda
  for (i = 0; i < raioMascaraX; i++)
    for (j = raioMascaraY; j < pm->GetNY () - raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = raioMascaraX; k < mask->GetNX (); k++)
	    for (l = 0; l < mask->GetNY (); l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask4;
		}
      PulaForMask4:
      }

  // Percorre a Borda direita
  for (i = pm->GetNX () - raioMascaraX; i < pm->GetNX (); i++)
    for (j = raioMascaraY; j < pm->GetNY () - raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->GetNX () - raioMascaraX; k++)
	    for (l = 0; l < mask->GetNY (); l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask5;
		}
      PulaForMask5:
      }

  // Percorre o Canto superior  esquerdo
  for (i = 0; i < raioMascaraX; i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = raioMascaraX; k < mask->GetNX (); k++)
	    for (l = raioMascaraY; l < mask->GetNY (); l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask6;
		}
      PulaForMask6:
      }
  // Percorre o Canto superior   direito
  for (i = pm->GetNX () - raioMascaraX; i < pm->GetNX (); i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->GetNX () - raioMascaraX; k++)
	    for (l = raioMascaraY; l < mask->GetNY (); l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask7;
		}
      PulaForMask7:
      }
  // Percorre o Canto  inferior esquerdo
  for (i = 0; i < raioMascaraX; i++)
    for (j = pm->GetNY () - raioMascaraY; j < pm->GetNY (); j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = raioMascaraX; k < mask->GetNX (); k++)
	    for (l = 0; l < mask->GetNY () - raioMascaraY; l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask8;
		}
      PulaForMask8:
      }
  // Percorre o Canto inferior   direito
  for (i = pm->GetNX () - raioMascaraX; i < pm->GetNX (); i++)
    for (j = pm->GetNY () - raioMascaraY; j < pm->GetNY (); j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->GetNX () - raioMascaraX; k++)
	    for (l = 0; l < mask->GetNY () - raioMascaraY; l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask9;
		}
      PulaForMask9:
      }
  // delete rImg;
  return pm;
}

/*
============================================================================
Documentacao      Fechamento
============================================================================
Descrição:
operacao da morfologia matematica usando metodo classico do deslocamento do EE.
O fechamento é a operacao de dilatacao seguida de erosao
Programador:      Andre Duarte Bueno
*/
TMatriz2D * CFEMMDeslocamento::Fechamento (TMatriz2D * &matriz,
					   unsigned int _RaioBola) 
{
  				// dilatacao
    Dilatacao (matriz, _RaioBola);	// 

  // erosao
  Erosao (matriz, _RaioBola);	// 

  return matriz;
}

/*
============================================================================
Documentacao      Abertura
============================================================================
Descrição:
Operacao da morfologia matematica usando metodo classoco, do deslocamento do EE
A abertura é a erosao seguida da dilatacao
A operacao de erosao, cria uma copia de pm, cria a mascara e
processa a erosao, devolvendo a pm erodida
A operacao de dilatacao, cria uma copia de pm, cria a mascara
e processa a dilatacao, devolvendo a pm dilatada

Pode-se  incluir diretamente aqui o processamento da erosao seguida da dilatacao
o que deixa o processo mais rapido.
Obs: observe que o objeto TFAbertura não cria nem usa uma mascara, são os objetos
erosao e dilatacao que criam e usam uma mascara.

Programador:      Andre Duarte Bueno
*/
TMatriz2D * CFEMMDeslocamento::Abertura (TMatriz2D * &matriz,
					 unsigned int _RaioBola) 
{
  				// erosao
    Erosao (matriz, _RaioBola);

  // dilatacao
  Dilatacao (matriz, _RaioBola);

  return matriz;
}

TMatriz2D *
CFEMMDeslocamento::DeteccaoContorno (TMatriz2D * &matriz,
				    unsigned int _RaioBola)
{
  return matriz;
}

TMatriz2D *
CFEMMDeslocamento::HitMiss (TMatriz2D * &matriz, unsigned int _RaioBola)
{
  return matriz;
}

TMatriz2D *
CFEMMDeslocamento::Esqueleto (TMatriz2D * &matriz, unsigned int _RaioBola)
{
  return matriz;
}

#endif


