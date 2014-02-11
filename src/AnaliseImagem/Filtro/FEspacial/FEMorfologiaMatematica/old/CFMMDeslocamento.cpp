// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFMMDeslocamento_cpp
#define CFMMDeslocamento_cpp


#include "TFiltro\TFEspacial\TFEMorfologiaMatematica\CFMMDeslocamento.h"
#include "TMascara\TMCDiscreta\TBCEuclidiana.h"

/*
  ============================================================================
  Documentacao      CriaMascara
  ============================================================================
  Descri��o:
  operacao da morfologia matematica usando metodo do deslocamento
  Se a mascara ja existe deve ser eliminada

  Programador:      Andre Duarte Bueno
*// *
void   CFMMDeslocamento::CriaMascara(unsigned int _tamanhoMascara)
{
  if(mask)  // se existe uma mascara
    {
      if(mask->NX()==_tamanhoMascara)   // e � do mesmo  tamanho
	return;                         // sai
      delete mask;                      // se  n�o � do mesmo tamanho apaga
    }                                   // e abaixo cria uma nova

  mask=new TBCEuclidiana(_tamanhoMascara);  // se n�o existe a mascara, cria uma nova
}   */
    /*{
      if(mask)
      delete mask;

      mask=new TBCEuclidiana(_tamanhoMascara);
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

 CMatriz2D * CFMMDeslocamento::Erosao (CMatriz2D * &matriz,
					 unsigned int /*_RaioBola*/ )
{
  pm = matriz;
  /*   tamanhoMascara=2*_RaioBola+1;
       CriaMascara(tamanhoMascara);          	 // criada na classe TFMorfologicos
       unsigned int i,j,k,l;
       CMatriz2D rImg (pm);    					 // Cria uma copia da imagem
       unsigned int raioMascaraX= mask->GetRaioX();     // variaveis auxiliares
       unsigned int raioMascaraY= mask->GetRaioY();
       // Procede a opera��o de eros�o desconsiderando a borda
       // Normal, centro da imagem
       for (  i=raioMascaraX; i <  pm->NX()-raioMascaraX; i++ )
       for (  j=raioMascaraY; j < pm->NY()-raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j] > 0)        	 // se o ponto � poro >0, verifica se � para erodir
       for( k=0;k<mask->NX();k++)
       for( l=0;l<mask->NY();l++)
       // Se um ponto da mascara estiver ativo
       // e na imagem inativo, erodir o ponto central
       if( mask->data2D[k][l] > 0 && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]==0) // <1
       { 				 // o mesmo que if( !rImg->data2D[i+k-rX][j+l-rY])
       pm->data2D[i][j]=0;
       goto PulaForMask1;
       }
       PulaForMask1:
       }
       // Percorre a Borda superior
       for (  i=raioMascaraX; i <  pm->NX()-raioMascaraX; i++ )
       for (  j=0; j < raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j] > 0)
       for( k=0;k<mask->NX();k++)
       for( l=raioMascaraY;l<mask->NY();l++)
       if( mask->data2D[k][l] > 0 && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
       {
       pm->data2D[i][j]=0;
       goto PulaForMask2;
       }
       PulaForMask2:
       }
       // Percorre a Borda inferior
       for (  i=raioMascaraX; i <  pm->NX()-raioMascaraX; i++ )
       for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
       {
       if(rImg.data2D[i][j] > 0)
       for( k=0;k<mask->NX();k++)
       for( l=0;l<mask->NY()-raioMascaraY;l++)
       if( mask->data2D[k][l] > 0 && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
       {
       pm->data2D[i][j]=0;
       goto PulaForMask3;
       }
       PulaForMask3:
       }
       // Percorre a Borda esquerda
       for (  i=0; i <  raioMascaraX; i++ )
       for (  j=raioMascaraY; j < pm->NY()-raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j] > 0)
       for( k=raioMascaraX;k<mask->NX();k++)
       for( l=0;l<mask->NY();l++)
       if( mask->data2D[k][l] > 0 && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
       {
       pm->data2D[i][j]=0;
       goto PulaForMask4;
       }
       PulaForMask4:
       }
       // Percorre a Borda direita
       for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
       for (  j=raioMascaraY; j < pm->NY()-raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j] > 0)
       for( k=0;k<mask->NX()-raioMascaraX;k++)
       for( l=0;l<mask->NY();l++)
       if( mask->data2D[k][l] > 0 && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
       {
       pm->data2D[i][j]=0;
       goto PulaForMask5;
       }
       PulaForMask5:
       }
       // Percorre o Canto superior  esquerdo
       for (  i=0; i <  raioMascaraX; i++ )
       for (  j=0; j < raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j] > 0)
       for( k=raioMascaraX;k<mask->NX();k++)
       for( l=raioMascaraY;l<mask->NY();l++)
       if( mask->data2D[k][l] > 0 && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
       {
       pm->data2D[i][j]=0;
       goto PulaForMask6;
       }
       PulaForMask6:
       }
       // Percorre o Canto superior   direito
       for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
       for (  j=0; j < raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j] > 0)
       for( k=0;k<mask->NX()-raioMascaraX;k++)
       for( l=raioMascaraY;l<mask->NY();l++)
       if( mask->data2D[k][l] > 0 && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
       {
       pm->data2D[i][j]=0;
       goto PulaForMask7;
       }
       PulaForMask7:
       }
       // Percorre o Canto  inferior esquerdo
       for (  i=0; i <  raioMascaraX; i++ )
       for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
       {
       if(rImg.data2D[i][j] > 0)
       for( k=raioMascaraX;k<mask->NX();k++)
       for( l=0;l<mask->NY()-raioMascaraY;l++)
       if( mask->data2D[k][l] > 0 && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
       {
       pm->data2D[i][j]=0;
       goto PulaForMask8;
       }
       PulaForMask8:
       }
       // Percorre o Canto inferior   direito
       for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
       for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
       {
       if(rImg.data2D[i][j] > 0)
       for( k=0;k<mask->NX()-raioMascaraX;k++)
       for( l=0;l<mask->NY()-raioMascaraY;l++)
       if( mask->data2D[k][l] > 0 && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
       {
       pm->data2D[i][j]=0;
       goto PulaForMask9;
       }
       PulaForMask9:
       }*/
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
	// 			 ***.....***
	// 			 ...     ...
	// 			 ...     ...
	// 			 ...     ...
	// 			 ***.....***
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
CFMMDeslocamento::Dilatacao (CMatriz2D * &matriz, unsigned int /*_RaioBola*/ )
{
  pm = matriz;
  /*   tamanhoMascara=2*_RaioBola+1;
       CriaMascara(tamanhoMascara);          	 // criada na classe TFMorfologicos
       unsigned int i,j,k,l;                            // variaveis auxiliares
       CMatriz2D rImg(pm ); 						 // copia a imagem
       unsigned int raioMascaraX= mask->GetRaioX();     // variavel auxiliar para acelerar processo
       unsigned int raioMascaraY= mask->GetRaioY();
       // percorre a matriz imagem, exceto a borda
       for (  i=raioMascaraX; i <  ( pm->NX()-raioMascaraX ); i++ )
       for (  j=raioMascaraY; j < ( pm->NY()-raioMascaraY ); j++ )
       {
       if(rImg.data2D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
       for(  k=0;k<mask->NX();k++)
       for(  l=0;l<mask->NY();l++)
       // se houver a interseccao de um ponto da bola
       // com a imagem, o ponto i,j da imagem � dilatado
       if( mask->data2D[k][l] && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]  >0)
       {
       pm->data2D[i][j]=1;
       goto PulaForMask1;
       }
       PulaForMask1:
       }
       // Percorre a Borda superior
       // o y da imagem vai de 0 at� raioMascaraY
       // o y da mascara come�a de  raioMascaraY vai at�  mask->NY()
       // ou seja, somente a borda superior
       for (  i=raioMascaraX; i <  pm->NX()-raioMascaraX; i++ )
       for (  j=0; j < raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
       for( k=0;k<mask->NX();k++)
       for( l=raioMascaraY;l<mask->NY();l++)
       if(mask->data2D[k][l] && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
       {
       pm->data2D[i][j]=1;
       goto PulaForMask2;
       }
       PulaForMask2:
       }
       // Percorre a Borda inferior
       for (  i=raioMascaraX; i <  pm->NX()-raioMascaraX; i++ )
       for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
       {
       if(rImg.data2D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
       for( k=0;k<mask->NX();k++)
       for( l=0;l<mask->NY()-raioMascaraY;l++)
       if(mask->data2D[k][l] && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
       {
       pm->data2D[i][j]=1;
       goto PulaForMask3;
       }
       PulaForMask3:
       }

       // Percorre a Borda esquerda
       for (  i=0; i <  raioMascaraX; i++ )
       for (  j=raioMascaraY; j < pm->NY()-raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
       for( k=raioMascaraX;k<mask->NX();k++)
       for( l=0;l<mask->NY();l++)
       if(mask->data2D[k][l] && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
       {
       pm->data2D[i][j]=1;
       goto PulaForMask4;
       }
       PulaForMask4:
       }

       // Percorre a Borda direita
       for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
       for (  j=raioMascaraY; j < pm->NY()-raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
       for( k=0;k<mask->NX()-raioMascaraX;k++)
       for( l=0;l<mask->NY();l++)
       if(mask->data2D[k][l] && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
       {
       pm->data2D[i][j]=1;
       goto PulaForMask5;
       }
       PulaForMask5:
       }

       // Percorre o Canto superior  esquerdo
       for (  i=0; i <  raioMascaraX; i++ )
       for (  j=0; j < raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
       for( k=raioMascaraX;k<mask->NX();k++)
       for( l=raioMascaraY;l<mask->NY();l++)
       if(mask->data2D[k][l] && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
       {
       pm->data2D[i][j]=1;
       goto PulaForMask6;
       }
       PulaForMask6:
       }
       // Percorre o Canto superior   direito
       for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
       for (  j=0; j < raioMascaraY; j++ )
       {
       if(rImg.data2D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
       for( k=0;k<mask->NX()-raioMascaraX;k++)
       for( l=raioMascaraY;l<mask->NY();l++)
       if(mask->data2D[k][l] && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
       {
       pm->data2D[i][j]=1;
       goto PulaForMask7;
       }
       PulaForMask7:
       }
       // Percorre o Canto  inferior esquerdo
       for (  i=0; i <  raioMascaraX; i++ )
       for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
       {
       if(rImg.data2D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
       for( k=raioMascaraX;k<mask->NX();k++)
       for( l=0;l<mask->NY()-raioMascaraY;l++)
       if(mask->data2D[k][l] && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
       {
       pm->data2D[i][j]=1;
       goto PulaForMask8;
       }
       PulaForMask8:
       }
       // Percorre o Canto inferior   direito
       for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
       for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
       {
       if(rImg.data2D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
       for( k=0;k<mask->NX()-raioMascaraX;k++)
       for( l=0;l<mask->NY()-raioMascaraY;l++)
       if(mask->data2D[k][l] && rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
       {
       pm->data2D[i][j]=1;
       goto PulaForMask9;
       }
       PulaForMask9:
       }
       // delete rImg;
       */
  return pm;
}
#endif
