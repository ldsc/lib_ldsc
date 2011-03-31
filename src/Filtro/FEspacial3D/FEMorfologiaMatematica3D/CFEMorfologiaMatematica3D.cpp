// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

// #define TFiltroDLL           // define como sendo uma dll
#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMorfologiaMatematica3D.h"
#include "Geometria/Bola/BCDiscreta3D/CBCEuclidiana3D.h"

/*
==================================================================================
Documentacao      CriaMascara
==================================================================================
Descricao:	operacao da morfologia matematica usando metodo do deslocamento
		Se a mascara ja existe deve ser eliminada
 Obs: Acrescentado controle para chamada das funções que criam os elementos
  estruturantes, b1 e b2 que sao utilizados nas funções de afinamento 
e espessamento, depois repensar forma de uso
Programador:      Andre Duarte Bueno
*/
void CFEMorfologiaMatematica3D::CriaMascara (unsigned int _tamanhoMascara)
{
// Abaixo dependendo do flag chama função de criação dos elementos estruturantes
// b1 e b2
  if (EEHitMiss1){
      CriaEEB1 (_tamanhoMascara);
      return;
  } else if (EEHitMiss2) {
      CriaEEB2 (_tamanhoMascara);
      return;
  }
  if (mask) {			// se existe uma mascara
     if (mask->NX () == _tamanhoMascara)	// e é do mesmo  tamanho
		return;			// sai
     delete mask;		// se  nao é do mesmo tamanho apaga
  }				// e abaixo cria uma nova
  mask = new CBCEuclidiana3D (_tamanhoMascara);	// se nao existe a mascara, cria uma nova
// testar alocacao.. TOperacao::TestaAlocacao(mask,"ponteiro para mask, Funcao CriaMascara");
}

// obs: b1 e b2 poderiam ser lidos do disco
void CFEMorfologiaMatematica3D::CriaEEB1 (unsigned int _tamanhoMascara)
{				// parametro nao usado
/*delete mask;
mask=new CMCEuclidiana3D(3);
if(mask)
	{
	mask->Constante(0);
	mask->data3D[1][1][0]=1;
	mask->data3D[0][2][0]=1;
	mask->data3D[1][2][0]=1;
	mask->data3D[2][2][0]=1;
   }
EEHitMiss1=EEHitMiss2=false;  */
}

// obs: b1 e b2 poderiam ser lidos do disco
void CFEMorfologiaMatematica3D::CriaEEB2 (unsigned int /*_tamanhoMascara*/ )
{				// parametro nao usado
/*delete mask;
mask=new CMCEuclidiana3D(3);
if(mask)
	{
	mask->Constante(0);
	mask->data3D[0][0][0]=1;
	mask->data3D[1][0][0]=1;
	mask->data3D[2][0][0]=1;
   }
EEHitMiss1=EEHitMiss2=false; */
}

/*

==================================================================================
Documentacao      Erosao
==================================================================================
Descricao:
Operacao da morfologia matematica usando metodo classico, deslocamento do EE.
Realiza operacao classica de erosao, percorre a imagem e compara com o EE (a mascara)
Se a multiplicacao da mascara pela imagem der algum valor zero, deve erodir

Programador:      Andre Duarte Bueno
*/

CMatriz3D * CFEMorfologiaMatematica3D::Erosao (CMatriz3D * & /*matriz */ ,
					       unsigned int /*_RaioBola*/ )
  
{
/*	pm=matriz;
	tamanhoMascara= _RaioBola*2+1;
	CriaMascara(tamanhoMascara);            // Se o tamanho da mascara foi alterado vai cria nova mascara
	unsigned int i,j,k,l;
// 	CMatriz3D rImg (pm);    					 // Cria uma copia da imagem
	unsigned int raioMascaraX= mask->RaioX();     // variaveis auxiliares
	unsigned int raioMascaraY= mask->RaioY();
	unsigned int raioMascaraZ= mask->GetRaioZ();


	// Procede a operação de erosao desconsiderando a borda
	// Normal, centro da imagem
	for (  i=raioMascaraX; i <  pm->NX()-raioMascaraX; i++ )
	 for (  j=raioMascaraY; j < pm->NY()-raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j] > 0)        		 // se o ponto é poro >0, verifica se é para erodir
	  for( k=0;k<mask->NX();k++)
		 for( l=0;l<mask->NY();l++)
														 // Se um ponto da mascara estiver ativo
														 // e na imagem inativo, erodir o ponto central
			if( mask->data3D[k][l] > 0 && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]==0) // <1
			 { 										 // o mesmo que if( !rImg->data3D[i+k-rX][j+l-rY])
			  pm->data3D[i][j]=0;
			  goto PulaForMask1;
			 }
	 PulaForMask1:
	 }
	// Percorre a Borda superior
	for (  i=raioMascaraX; i <  pm->NX()-raioMascaraX; i++ )
	 for (  j=0; j < raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j] > 0)
	  for( k=0;k<mask->NX();k++)
		 for( l=raioMascaraY;l<mask->NY();l++)
			if( mask->data3D[k][l] > 0 && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
			 {
			  pm->data3D[i][j]=0;
			  goto PulaForMask2;
			 }
	 PulaForMask2:
	 }
	// Percorre a Borda inferior
	for (  i=raioMascaraX; i <  pm->NX()-raioMascaraX; i++ )
	 for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
	 {
	 if(rImg.data3D[i][j] > 0)
	  for( k=0;k<mask->NX();k++)
		 for( l=0;l<mask->NY()-raioMascaraY;l++)
			if( mask->data3D[k][l] > 0 && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
			 {
			  pm->data3D[i][j]=0;
			  goto PulaForMask3;
			 }
	 PulaForMask3:
	 }
	// Percorre a Borda esquerda
	for (  i=0; i <  raioMascaraX; i++ )
	 for (  j=raioMascaraY; j < pm->NY()-raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j] > 0)
	  for( k=raioMascaraX;k<mask->NX();k++)
		 for( l=0;l<mask->NY();l++)
			if( mask->data3D[k][l] > 0 && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
			 {
			  pm->data3D[i][j]=0;
			  goto PulaForMask4;
			 }
	 PulaForMask4:
	 }
	// Percorre a Borda direita
	for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
	 for (  j=raioMascaraY; j < pm->NY()-raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j] > 0)
	  for( k=0;k<mask->NX()-raioMascaraX;k++)
		 for( l=0;l<mask->NY();l++)
			if( mask->data3D[k][l] > 0 && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
			 {
			  pm->data3D[i][j]=0;
			  goto PulaForMask5;
			 }
	 PulaForMask5:
	 }
	// Percorre o Canto superior  esquerdo
	for (  i=0; i <  raioMascaraX; i++ )
	 for (  j=0; j < raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j] > 0)
	  for( k=raioMascaraX;k<mask->NX();k++)
		 for( l=raioMascaraY;l<mask->NY();l++)
			if( mask->data3D[k][l] > 0 && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
			 {
			  pm->data3D[i][j]=0;
			  goto PulaForMask6;
			 }
	 PulaForMask6:
	 }
	// Percorre o Canto superior   direito
	for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
	 for (  j=0; j < raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j] > 0)
	  for( k=0;k<mask->NX()-raioMascaraX;k++)
		 for( l=raioMascaraY;l<mask->NY();l++)
			if( mask->data3D[k][l] > 0 && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
			 {
			  pm->data3D[i][j]=0;
			  goto PulaForMask7;
			 }
	 PulaForMask7:
	 }
	// Percorre o Canto  inferior esquerdo
	for (  i=0; i <  raioMascaraX; i++ )
	 for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
	 {
	 if(rImg.data3D[i][j] > 0)
	  for( k=raioMascaraX;k<mask->NX();k++)
		 for( l=0;l<mask->NY()-raioMascaraY;l++)
			if( mask->data3D[k][l] > 0 && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
			 {
			  pm->data3D[i][j]=0;
			  goto PulaForMask8;
			 }
	 PulaForMask8:
	 }
	// Percorre o Canto inferior   direito
	for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
	 for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
	 {
	 if(rImg.data3D[i][j] > 0)
	  for( k=0;k<mask->NX()-raioMascaraX;k++)
		 for( l=0;l<mask->NY()-raioMascaraY;l++)
			if( mask->data3D[k][l] > 0 && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]==0)
			 {
			  pm->data3D[i][j]=0;
			  goto PulaForMask9;
			 }
	 PulaForMask9:
	 }*/
  return pm;
}


/*
// Modificações:
	// 2-Procede a operação de erosao separando a imagem em partes internas e externas.
	// a)Procede a operação de erosao no centro da imagem, desconsiderando a borda
	// 			...........
	// 			...*****...
	// 			...*****...
	// 			...*****...
	// 			...........
	// b)Procede a operação de erosao nas regiões da borda (superior, inferior, esquerda e direita)
	// 			  Superior
	// 			 ...*****...
	// 			 ***     ***
	// Esquerda***     *** Direita
	// 			 ***     ***
	// 			 ...*****...
	// 				Inferior
	// c)Procede a operação de erosao nos cantos da imagem
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
==================================================================================
Documentacao      Dilatacao
==================================================================================
Descricao:
operacao da morfologia matematica usando metodo classico, com  deslocamento do EE
dilatacao classica, percorre a imagem com o EE e compara.
			  // Modificações:

			  // Pensar em trocar a multiplicação por uma combinação do tipo

			  // (mask->data3D[k][l] & rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY])  >0)
			  // o operador & retorna 1 se ambos os lados forem != de 0.

Programador:      Andre Duarte Bueno
*/
CMatriz3D *
CFEMorfologiaMatematica3D::Dilatacao (CMatriz3D * &matriz,
				     unsigned int _RaioBola)
{
/*	pm=matriz;
	tamanhoMascara= _RaioBola*2+1;
	CriaMascara(tamanhoMascara);              	 // criada na classe CFEMorfologicos
	unsigned int i,j,k,l;                            // variaveis auxiliares
	CMatriz3D rImg(pm ); 						 // copia a imagem
	unsigned int raioMascaraX= mask->RaioX();     // variavel auxiliar para acelerar processo
	unsigned int raioMascaraY= mask->RaioY();	 // percorre a matriz imagem, exceto a borda
	for (  i=raioMascaraX; i <  ( pm->NX()-raioMascaraX ); i++ )
	 for (  j=raioMascaraY; j < ( pm->NY()-raioMascaraY ); j++ )
	 {
	 if(rImg.data3D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
	 for(  k=0;k<mask->NX();k++)
		 for(  l=0;l<mask->NY();l++)
							// se houver a interseccao de um ponto da bola
							// com a imagem, o ponto i,j da imagem é dilatado
		  if( mask->data3D[k][l] && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]  >0)
			  {
			  pm->data3D[i][j]=1;
			  goto PulaForMask1;
			  }
	 PulaForMask1:
	 }
	// Percorre a Borda superior
	// o y da imagem vai de 0 até raioMascaraY
	// o y da mascara começa de  raioMascaraY vai até  mask->NY()
	// ou seja, somente a borda superior
	for (  i=raioMascaraX; i <  pm->NX()-raioMascaraX; i++ )
	 for (  j=0; j < raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
	  for( k=0;k<mask->NX();k++)
		 for( l=raioMascaraY;l<mask->NY();l++)
		  if(mask->data3D[k][l] && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
			{
			pm->data3D[i][j]=1;
			goto PulaForMask2;
			}
	  PulaForMask2:
	  }
	// Percorre a Borda inferior
	for (  i=raioMascaraX; i <  pm->NX()-raioMascaraX; i++ )
	 for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
	 {
	 if(rImg.data3D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
	  for( k=0;k<mask->NX();k++)
		 for( l=0;l<mask->NY()-raioMascaraY;l++)
		  if(mask->data3D[k][l] && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
			{
			pm->data3D[i][j]=1;
			goto PulaForMask3;
			}
	  PulaForMask3:
	  }

	// Percorre a Borda esquerda
	for (  i=0; i <  raioMascaraX; i++ )
	 for (  j=raioMascaraY; j < pm->NY()-raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
	  for( k=raioMascaraX;k<mask->NX();k++)
		 for( l=0;l<mask->NY();l++)
		  if(mask->data3D[k][l] && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
			{
			pm->data3D[i][j]=1;
			goto PulaForMask4;
			}
	  PulaForMask4:
	  }

	// Percorre a Borda direita
	for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
	 for (  j=raioMascaraY; j < pm->NY()-raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
	  for( k=0;k<mask->NX()-raioMascaraX;k++)
		 for( l=0;l<mask->NY();l++)
		  if(mask->data3D[k][l] && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
			{
			pm->data3D[i][j]=1;
			goto PulaForMask5;
			}
	  PulaForMask5:
	  }

	// Percorre o Canto superior  esquerdo
	for (  i=0; i <  raioMascaraX; i++ )
	 for (  j=0; j < raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
	  for( k=raioMascaraX;k<mask->NX();k++)
		 for( l=raioMascaraY;l<mask->NY();l++)
		  if(mask->data3D[k][l] && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
			{
			pm->data3D[i][j]=1;
			goto PulaForMask6;
			}
	  PulaForMask6:
	  }
	// Percorre o Canto superior   direito
	for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
	 for (  j=0; j < raioMascaraY; j++ )
	 {
	 if(rImg.data3D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
	  for( k=0;k<mask->NX()-raioMascaraX;k++)
		 for( l=raioMascaraY;l<mask->NY();l++)
		  if(mask->data3D[k][l] && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
			{
			pm->data3D[i][j]=1;
			goto PulaForMask7;
			}
	  PulaForMask7:
	  }
	// Percorre o Canto  inferior esquerdo
	for (  i=0; i <  raioMascaraX; i++ )
	 for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
	 {
	 if(rImg.data3D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
	  for( k=raioMascaraX;k<mask->NX();k++)
		 for( l=0;l<mask->NY()-raioMascaraY;l++)
		  if(mask->data3D[k][l] && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
			{
			pm->data3D[i][j]=1;
			goto PulaForMask8;
			}
	  PulaForMask8:
	  }
	// Percorre o Canto inferior   direito
	for (  i=pm->NX()-raioMascaraX; i <  pm->NX(); i++ )
	 for (  j=pm->NY()-raioMascaraY; j < pm->NY(); j++ )
	 {
	 if(rImg.data3D[i][j]==0)              // se o ponto for solido(branco), pode ser dilatado
	  for( k=0;k<mask->NX()-raioMascaraX;k++)
		 for( l=0;l<mask->NY()-raioMascaraY;l++)
		  if(mask->data3D[k][l] && rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY]>0)
			{
			pm->data3D[i][j]=1;
			goto PulaForMask9;
			}
	  PulaForMask9:
	  }
	// delete rImg;*/
  return pm;
}

/*
==================================================================================
Documentacao      Fechamento
==================================================================================
Descricao:
operacao da morfologia matematica usando metodo classico do deslocamento do EE.
O fechamento é a operacao de dilatacao seguida de erosao
Programador:      Andre Duarte Bueno
*/

CMatriz3D * CFEMorfologiaMatematica3D::Fechamento (CMatriz3D * &matriz,
						   unsigned int _RaioBola) 
{
  
				// dilatacao
    Dilatacao (matriz, _RaioBola);

  // erosao
  Erosao (matriz, _RaioBola);

  return matriz;
}

/*

==================================================================================
Documentacao      Abertura
==================================================================================
Descricao:
Operacao da morfologia matematica usando metodo classoco, do deslocamento do EE
A abertura é a erosao seguida da dilatacao
A operacao de erosao, cria uma copia de pm, cria a mascara e

processa a erosao, devolvendo a pm erodida

A operacao de dilatacao, cria uma copia de pm, cria a mascara

e processa a dilatacao, devolvendo a pm dilatada


Pode-se  incluir diretamente aqui o processamento da erosao seguida da dilatacao

o que deixa o processo mais rapido.

Obs: observe que o objeto TFAbertura nao cria nem usa uma mascara, sao os objetos

erosao e dilatacao que criam e usam uma mascara.

Programador:      Andre Duarte Bueno
*/
CMatriz3D * CFEMorfologiaMatematica3D::Abertura (CMatriz3D * &matriz, unsigned int _RaioBola) 
{
	// erosao
	Erosao (matriz, _RaioBola);
	// dilatacao
    	Dilatacao (matriz, _RaioBola);
	return matriz;
}

CMatriz3D * CFEMorfologiaMatematica3D::DeteccaoContorno (CMatriz3D * &matriz, unsigned int /*_RaioBola*/ )
{
  return matriz;
}

/*
==================================================================================
Documentacao      HitMiss
==================================================================================
Descricao:
	realiza operação Hit-Mis ou HOM.
	Faz uma copia da imagem, depois processa erosao sobre a matriz original com
	elemento estruturante b1, depois acha o complemento da matriz
   e realiza erosao sobre copia com EE b2,
	depois realiza a intersecção das duas matrizes

Programador:      Andre Duarte Bueno
*/

CMatriz3D * CFEMorfologiaMatematica3D::HitMiss (CMatriz3D * &matriz, unsigned int /*_RaioBola*/ )
{
/*	CMatriz3D* matriz2=new CMatriz3D(matriz);	// Cria copia da matriz
	matriz2->Inverter();                   // e transforma na complementar

	EEHitMiss1=true;                       // ativa criação da mascara para bi
	CFEMorfologiaMatematica3D::Erosao(matriz,_RaioBola);// Erosao sobre a matriz com Bi

	EEHitMiss2=true;                       // ativa criação da mascara para be
	CFEMorfologiaMatematica3D::Erosao(matriz2,_RaioBola);// Erosao sobre a matriz2 com be

   matriz->Intersecao(matriz2);				// intersecção entre matriz e matriz2
   delete matriz2;
                 */
  return matriz;
}

CMatriz3D *
CFEMorfologiaMatematica3D::Afinamento (CMatriz3D * &matriz,
				      unsigned int _RaioBola)
{
  /*     CMatriz3D* matriz2=new CMatriz3D(matriz);       // Cria copia da matriz
     HitMiss(matriz2,  _RaioBola);                       // Realiza operação HitMiss
     matriz2->Inverter();                               // o mesmo que matriz2->Complementar();
     matriz->Intersecao(matriz2);                    // intersecção entre matriz e matriz2
     delete matriz2; */
  return matriz;
}

CMatriz3D *
CFEMorfologiaMatematica3D::Espessamento (CMatriz3D * &matriz,
					unsigned int /*_RaioBola*/ )
{				/*
				   CMatriz3D* matriz2=new CMatriz3D(matriz); // Cria copia da matriz
				   HitMiss(matriz2,  _RaioBola);    // Realiza operação HitMiss
				   matriz->Uniao(matriz2);          // união entre matriz e matriz2
				   delete matriz2;
				 */
  return matriz;
}

CMatriz3D *
CFEMorfologiaMatematica3D::Esqueleto (CMatriz3D * &matriz,
				     unsigned int /*_RaioBola*/ )
{
  return matriz;
}



