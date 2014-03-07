// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

#include "AnaliseImagem/Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMDeslocamento3D.h"

#include "AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/CFEMMDeslocamento.h"
#include "AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCEuclidiana.h"

/*
============================================================================
Documentacao      CriaMascara
============================================================================
Descri��o:
			operacao da morfologia matematica usando metodo do deslocamento
			Se a mascara ja existe deve ser eliminada

   Programador:      Andre Duarte Bueno
*/ /* verificar se � mesmo coment�rio nesta funcao
      void   CFEMMDeslocamento::CriaMascara(unsigned int _tamanhoMascara)
      {
      if(mask)                                   // se existe uma mascara
      {
      if(mask->NX()==_tamanhoMascara)        // e � do mesmo  tamanho
      return;                           // sai
      delete mask;                      // se  n�o � do mesmo tamanho apaga
      }                                   // e abaixo cria uma nova

      mask=new CMCEuclidiana(_tamanhoMascara);          // se n�o existe a mascara, cria uma nova
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
Descri��o:
Operacao da morfologia matematica usando metodo classico, deslocamento do EE.
Realiza operacao classica de erosao, percorre a imagem e compara com o EE (a mascara)
Se a multiplicacao da mascara pela imagem der algum valor zero, deve erodir

Programador:      Andre Duarte Bueno
*/
 CMatriz2D * CFEMMDeslocamento::Erosao (CMatriz2D * &matriz,
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
  CMatriz2D
  rImg (*pm);			// Cria uma copia da imagem
  unsigned int
    raioMascaraX = mask->RaioX ();	// variaveis auxiliares
  unsigned int
    raioMascaraY = mask->RaioY ();
  // Procede a opera��o de eros�o desconsiderando a borda
  // Normal, centro da imagem
  for (i = raioMascaraX; i < pm->NX () - raioMascaraX; i++)
    for (j = raioMascaraY; j < pm->NY () - raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)	// se o ponto � poro >0, verifica se � para erodir
	  for (k = 0; k < mask->NX (); k++)
	    for (l = 0; l < mask->NY (); l++)
	      // Se um ponto da mascara estiver ativo
	      // e na imagem inativo, erodir o ponto central
	      if (mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == 0)	// <1
		{		// o mesmo que if( !rImg->data2D[i+k-rX][j+l-rY])
		  pm->data2D[i][j] = 0;
		  goto PulaForMask1;
		}
      PulaForMask1:
	NULL;
      }
  // Percorre a Borda superior
  for (i = raioMascaraX; i < pm->NX () - raioMascaraX; i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = 0; k < mask->NX (); k++)
	    for (l = raioMascaraY; l < mask->NY (); l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask2;
		}
      PulaForMask2:
	NULL;
      }
  // Percorre a Borda inferior
  for (i = raioMascaraX; i < pm->NX () - raioMascaraX; i++)
    for (j = pm->NY () - raioMascaraY; j < pm->NY (); j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = 0; k < mask->NX (); k++)
	    for (l = 0; l < mask->NY () - raioMascaraY; l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask3;
		}
      PulaForMask3:
	NULL;
      }
  // Percorre a Borda esquerda
  for (i = 0; i < raioMascaraX; i++)
    for (j = raioMascaraY; j < pm->NY () - raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = raioMascaraX; k < mask->NX (); k++)
	    for (l = 0; l < mask->NY (); l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask4;
		}
      PulaForMask4:
      	NULL;
}
  // Percorre a Borda direita
  for (i = pm->NX () - raioMascaraX; i < pm->NX (); i++)
    for (j = raioMascaraY; j < pm->NY () - raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = 0; k < mask->NX () - raioMascaraX; k++)
	    for (l = 0; l < mask->NY (); l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask5;
		}
      PulaForMask5:
   	NULL;
   }
  // Percorre o Canto superior  esquerdo
  for (i = 0; i < raioMascaraX; i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = raioMascaraX; k < mask->NX (); k++)
	    for (l = raioMascaraY; l < mask->NY (); l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask6;
		}

  PulaForMask6:
    	NULL;
      }
  // Percorre o Canto superior   direito
  for (i = pm->NX () - raioMascaraX; i < pm->NX (); i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = 0; k < mask->NX () - raioMascaraX; k++)
	    for (l = raioMascaraY; l < mask->NY (); l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask7;
		}
      PulaForMask7:
      	NULL;
    }
  // Percorre o Canto  inferior esquerdo
  for (i = 0; i < raioMascaraX; i++)
    for (j = pm->NY () - raioMascaraY; j < pm->NY (); j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = raioMascaraX; k < mask->NX (); k++)
	    for (l = 0; l < mask->NY () - raioMascaraY; l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask8;
		}
      PulaForMask8:
      	NULL;
    }
  // Percorre o Canto inferior   direito
  for (i = pm->NX () - raioMascaraX; i < pm->NX (); i++)
    for (j = pm->NY () - raioMascaraY; j < pm->NY (); j++)
      {
	if (rImg.data2D[i][j] > 0)
	  for (k = 0; k < mask->NX () - raioMascaraX; k++)
	    for (l = 0; l < mask->NY () - raioMascaraY; l++)
	      if (mask->data2D[k][l] > 0
		  && rImg.data2D[i + k - raioMascaraX][j + l -
						       raioMascaraY] == 0)
		{
		  pm->data2D[i][j] = 0;
		  goto PulaForMask9;
		}
      PulaForMask9:
        	NULL;
  }
  return pm;
}


/*
// Modifica��es:
	// 2-Procede a opera��o de eros�o separando a imagem em partes internas e externas.
	// a)Procede a opera��o de eros�o no centro da imagem, desconsiderando a borda
	// 			...........
	// 			...*****...
	// 			...*****...
	// 			...*****...
	// 			...........
	// b)Procede a opera��o de eros�o nas regi�es da borda (superior, inferior, esquerda e direita)
	// 			  Superior
	// 			 ...*****...
	// 			 ***     ***
	// Esquerda***     *** Direita
	// 			 ***     ***
	// 			 ...*****...
	// 				Inferior
	// c)Procede a opera��o de eros�o nos cantos da imagem
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
	(x da imagem)	raioMascaraX; <-------->		pm->NX()-raioMascaraX
	(y da imagem)  raioMascaraY  <-------->		pm->NY()-raioMascaraY
	(x da mascara)	0				  <-------->		mask->NX()
	(y da mascara)	0				  <-------->		mask->NY()

*/

/*
============================================================================
Documentacao      Dilatacao
============================================================================
Descri��o:
operacao da morfologia matematica usando metodo classico, com  deslocamento do EE
dilatacao classica, percorre a imagem com o EE e compara.
			  // Modifica��es:
			  // Pensar em trocar a multiplica��o por uma combina��o do tipo
			  // (mask->data2D[k][l] & rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY])  >0)
			  // o operador & retorna 1 se ambos os lados forem != de 0.

Programador:      Andre Duarte Bueno
*/
CMatriz2D *
CFEMMDeslocamento::Dilatacao (CMatriz2D * &matriz, unsigned int _RaioBola)
{
  pm = matriz;
  tamanhoMascara = 2 * _RaioBola + 1;
  CriaMascara (tamanhoMascara);	// criada na classe CFEMorfologicos
  unsigned int i, j, k, l;	// variaveis auxiliares
  CMatriz2D rImg (*pm);		// copia a imagem
  unsigned int raioMascaraX = mask->RaioX ();	// variavel auxiliar para acelerar processo
  unsigned int raioMascaraY = mask->RaioY ();
  // percorre a matriz imagem, exceto a borda
  for (i = raioMascaraX; i < (pm->NX () - raioMascaraX); i++)
    for (j = raioMascaraY; j < (pm->NY () - raioMascaraY); j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->NX (); k++)
	    for (l = 0; l < mask->NY (); l++)
	      // se houver a interseccao de um ponto da bola
	      // com a imagem, o ponto i,j da imagem � dilatado
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask1;
		}
      PulaForMask1:
     	NULL;
     }
  // Percorre a Borda superior
  // o y da imagem vai de 0 at� raioMascaraY
  // o y da mascara come�a de  raioMascaraY vai at�  mask->NY()
  // ou seja, somente a borda superior
  for (i = raioMascaraX; i < pm->NX () - raioMascaraX; i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->NX (); k++)
	    for (l = raioMascaraY; l < mask->NY (); l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask2;
		}

      PulaForMask2:
    	NULL;
      }
  // Percorre a Borda inferior
  for (i = raioMascaraX; i < pm->NX () - raioMascaraX; i++)
    for (j = pm->NY () - raioMascaraY; j < pm->NY (); j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->NX (); k++)
	    for (l = 0; l < mask->NY () - raioMascaraY; l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask3;
		}
      PulaForMask3:
NULL;
      }

  // Percorre a Borda esquerda
  for (i = 0; i < raioMascaraX; i++)
    for (j = raioMascaraY; j < pm->NY () - raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = raioMascaraX; k < mask->NX (); k++)
	    for (l = 0; l < mask->NY (); l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask4;
		}
    	      PulaForMask4:
NULL;
      }

  // Percorre a Borda direita
  for (i = pm->NX () - raioMascaraX; i < pm->NX (); i++)
    for (j = raioMascaraY; j < pm->NY () - raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->NX () - raioMascaraX; k++)
	    for (l = 0; l < mask->NY (); l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask5;
		}
      PulaForMask5:
NULL;
      }

  // Percorre o Canto superior  esquerdo
  for (i = 0; i < raioMascaraX; i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = raioMascaraX; k < mask->NX (); k++)
	    for (l = raioMascaraY; l < mask->NY (); l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask6;
		}
      PulaForMask6:
NULL;
      }
  // Percorre o Canto superior   direito
  for (i = pm->NX () - raioMascaraX; i < pm->NX (); i++)
    for (j = 0; j < raioMascaraY; j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->NX () - raioMascaraX; k++)
	    for (l = raioMascaraY; l < mask->NY (); l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask7;
		}
      PulaForMask7:
NULL;
      }
  // Percorre o Canto  inferior esquerdo
  for (i = 0; i < raioMascaraX; i++)
    for (j = pm->NY () - raioMascaraY; j < pm->NY (); j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = raioMascaraX; k < mask->NX (); k++)
	    for (l = 0; l < mask->NY () - raioMascaraY; l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask8;
		}
      PulaForMask8:
NULL;
      }
  // Percorre o Canto inferior   direito
  for (i = pm->NX () - raioMascaraX; i < pm->NX (); i++)
    for (j = pm->NY () - raioMascaraY; j < pm->NY (); j++)
      {
	if (rImg.data2D[i][j] == 0)	// se o ponto for solido(branco), pode ser dilatado
	  for (k = 0; k < mask->NX () - raioMascaraX; k++)
	    for (l = 0; l < mask->NY () - raioMascaraY; l++)
	      if (mask->data2D[k][l]
		  && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] >
		  0)
		{
		  pm->data2D[i][j] = 1;
		  goto PulaForMask9;
		}
      PulaForMask9:
NULL;
      }
  // delete rImg;
  return pm;
}

/*
============================================================================
Documentacao      Fechamento
============================================================================
Descri��o:
operacao da morfologia matematica usando metodo classico do deslocamento do EE.
O fechamento � a operacao de dilatacao seguida de erosao
Programador:      Andre Duarte Bueno
*/
CMatriz2D * CFEMMDeslocamento::Fechamento (CMatriz2D * &matriz,
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
Descri��o:
Operacao da morfologia matematica usando metodo classoco, do deslocamento do EE
A abertura � a erosao seguida da dilatacao
A operacao de erosao, cria uma copia de pm, cria a mascara e
processa a erosao, devolvendo a pm erodida
A operacao de dilatacao, cria uma copia de pm, cria a mascara
e processa a dilatacao, devolvendo a pm dilatada

Pode-se  incluir diretamente aqui o processamento da erosao seguida da dilatacao
o que deixa o processo mais rapido.
Obs: observe que o objeto TFAbertura n�o cria nem usa uma mascara, s�o os objetos
erosao e dilatacao que criam e usam uma mascara.

Programador:      Andre Duarte Bueno
*/
CMatriz2D * CFEMMDeslocamento::Abertura (CMatriz2D * &matriz, unsigned int _RaioBola) 
{
  // erosao
  Erosao (matriz, _RaioBola);

  // dilatacao
  Dilatacao (matriz, _RaioBola);

  return matriz;
}

CMatriz2D *
CFEMMDeslocamento::DeteccaoContorno (CMatriz2D * &matriz, unsigned int _RaioBola)
{
  return matriz;
}

CMatriz2D *
CFEMMDeslocamento::HitMiss (CMatriz2D * &matriz, unsigned int _RaioBola)
{
  return matriz;
}

CMatriz2D *
CFEMMDeslocamento::Esqueleto (CMatriz2D * &matriz, unsigned int _RaioBola)
{
  return matriz;
}



