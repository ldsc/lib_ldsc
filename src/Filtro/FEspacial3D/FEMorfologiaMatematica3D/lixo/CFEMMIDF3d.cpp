// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDF3D.cpp
Nome da classe:      CFEMMIDF3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa as funções da classe CFEMMIDF3D.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMIDF3D.h"// Classe base
#include "Geometria/Bola/BCDiscreta3D/CBCDiscreta3D.h"	// Cria objeto

/*
============================================================================
Atributos estaticos da classe
============================================================================
*/
// Se verdadeira após cada processamento recalcula a idf
bool  CFEMMIDF3D::atualizaIDF = 0;	// por default, não recalcula a idf apos dilatacao ou fechamento

/*
============================================================================
Documentacao construtor
============================================================================
Executa o construtor da classe base, o TMatriz3D(_pm)
que realiza a copia dos dados.
Depois copia o endereco da imagem no atributo pm.
Assim pm aponta para a matriz recebida e data3D para a matriz IDF
*/
// Construtor sobrecarregado, recebe também o tamanho do raio máximo
CFEMMIDF3D::CFEMMIDF3D (TMatriz3D * &matriz, unsigned int _tamanhoMascara,
		      unsigned int _raioMax):
CFEMorfologiaMatematica3D (matriz, _tamanhoMascara),
TMatriz3D (matriz->GetNX (), matriz->GetNY (), matriz->GetNZ ()),
raioBola (_tamanhoMascara),
raioMaximo (_raioMax),
indiceAtivo (1),
indiceInativo (0)
{
}

/*
============================================================================
Função    ExecutadaPorGo
============================================================================
Função usada exclusivamente por Go:

1-Quando o usuário quer calcular a idf, chama a funcao Go (pm).
	No caso da imagem pm passada ter dimemsões diferentes da pm passada para o construtor, devo
	desalocar a matris idf, definir os novos valores de nx e ny e realocar a idf.
	Se as dimensões da matriz passada e minhas forem iguais posso continuar.

2-Copia valores da matriz passada para a matriz idf, usando data2D[i][j]=pm->data2D[i][j]

	Esta função foi incluída porque o usuário programador pode chamar Go(pm)
	sendo pm diferente daquela passada par o construtor.	Ou seja cria-se uma segurança a mais.

	Outro fato que justifica esta função é que antes, criava objeto idf, executava Go, e depois
	podia executar a erosao, mas sempre sobre a idf da imagem passada para o construtor
	da classe. Agora posso passar uma nova matriz para a função Go,
	diferente da passada para o construtor.

	O terceiro motivo é que no caso de se realizar uma sequencia de erosoes com a mesma
	mascara, eu precisava criar um objeto idf, calcular go e calcular a erosao,
	criar nova idf (sobre pm erodida), executar go e então realizar a erosao...
	Agora posso recalcular Go a qualquer momento, sem recriar o objeto idf.
	Exemplo:
			cria idf      		pfmm=new TMMIDF(pm,tamanhoMascara);
			executa go    		pfmm->Go(pm,tamanhoMascara);
			executa erosao    pfmm->Erosao(pm,raioBola);       // erosao sobre imagem original
			executa go    		pfmm->Go(pm,tamanhoMascara);
			executa erosao    pfmm->Erosao(pm,raioBola);	// erosao sobre imagem já erodida

Assim, toda funcao Go herdeira da CFEMMIDF deve chamar esta funcao:
	 ExecutadaPorGo(matriz);

*/
// Funcao chamada exclusivamente por Go, ou seja depois de executada a idf vai ser recalculada.
void
CFEMMIDF3D::ExecutadaPorGo (TMatriz3D * &matriz)	// ,unsigned int _tamanhoMascara)
{
  pm = matriz;			// armazena endereço matriz
  // verifica se a matriz tem as mesmas dimensoes da idf(this)
  if (this->nx != matriz->GetNX () || this->ny != matriz->GetNY ()
      || this->nz != matriz->GetNZ ())
    {
      Desaloca ();		// desaloca a matriz existende e depois
      this->nx = matriz->GetNX ();	// define novo tamanho
      this->ny = matriz->GetNY ();
      this->nz = matriz->GetNZ ();
      Aloca ();			// aloca de acordo com novo tamanho
    }
  // Agora tenho de armazenar valores de pm na idf

  // substituir por this=matriz;
  register int mi = Getmi ();
  for (int i = 0; i < nx; i++)
    for (int j = 0; j < ny; j++)
      for (int k = 0; k < nz; k++)
	if (matriz->data3D[i][j][k] != 0)	// como a imagem recebida por ser uma outra idf
	  this->data3D[i][j][k] = mi;	// define this com 0 e 1
  // // // // // // // // // // // // // 
	else			// AQUI AQUI AQUI AQUI: trocar 1 por mi
	  this->data3D[i][j][k] = 0;
  // this->data2D[i][j]=matriz->data2D[i][j];
// Write("ExecutadaPorGo.txt");
}

/*
============================================================================
Função    VerificaImagem
============================================================================
Usada pelas funções Erosao,Dilatacao, fechamento e abertura para verificar se é a mesma imagem.
Se for outra imagem recalcula Go.
Tambem armazena endereço da imagem em pm.
*/
void
CFEMMIDF3D::VerificaImagem (TMatriz3D * &matriz)
{				// se for a mesma imagem e tiver o mesmo tamanho sai
  // pm=matriz;
  if (pm == matriz && nx == matriz->GetNX () && ny == matriz->GetNY ()
      && nz == matriz->GetNZ ())
    return;			// sai
  else
    Go (matriz);		// senão chama Go, que redefine o tamanho da imagem
}				// e calcula valores idf


/*
============================================================================
Função    Erosao.
============================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
TMatriz3D *
CFEMMIDF3D::Erosao (TMatriz3D * &matriz, unsigned int _RaioBola)	// 
{
  // -->Padrao
  VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
  tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
  // Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara (tamanhoMascara);	// Cria a mascara adequada, do tamanho de tamanhoMascara

  CBCDiscreta3D *maskd = dynamic_cast < CBCDiscreta3D * >(mask);	// Cria ponteiro para mascara com acesso a GetraioBolaTangente

  // Processamento da erosao em si
  int raioBolaInclusa = maskd->GetraioBolaInclusa ();
  for (int k = 0; k < nz; k++)
    for (int j = 0; j < ny; j++)
      for (int i = 0; i < nx; i++)
	{
	  // se o ponto da idf for maior que a bola tangente, faz ponto=1
	  if (data3D[i][j][k] > raioBolaInclusa)
	    pm->data3D[i][j][k] = indiceAtivo;	// seta ponto ativo
	  else			//    ,indiceAtivo(1)   ,indiceInativo(0)

	    pm->data3D[i][j][k] = indiceInativo;	// seta ponto inativo
	}
  return pm;			// pm é a matriz erodida
}

/*
============================================================================
Função    Abertura
============================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
TMatriz3D *
CFEMMIDF3D::Abertura (TMatriz3D * &matriz, unsigned int _RaioBola)	// 
{
  // -->Padrao
  VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
  tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
  // Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara (tamanhoMascara);	// Cria a mascara adequada,do tamanho de tamanhoMascara

  CBCDiscreta3D *maskd = dynamic_cast < CBCDiscreta3D * >(mask);	// Cria ponteiro para mascara com acesso a GetraioBolaTangente

  // Processamento da abertura em si
  // imagemModificada=0;
  pm->Constante (indiceInativo);	// zera a matriz imagem

  // Otimizacao Mascara (bola)
  int raio = maskd->GetraioX ();
  int raioBolaTangente = maskd->GetraioBolaTangente ();
  int raioBolaInclusa = maskd->GetraioBolaInclusa ();
  int imx;			// i mais x, i+x
  int rmx;			// raio mais x, raio+x
  int jmy;			// 
  int rmy;			// 
  int kmz;			// 
  int rmz;			// 
  // Variáveis para SIMETRIA Bola
  int posxe, posxd;		// x esquerda e x direita
  int posys, posyn;		// y sul e y norte
  int poszb, poszf;		// z back e z front
  int xx, yy, zz;		// posicoes xx e yy da bola
  // falta verificar o contorno
  int ***pmdata3D = pm->GetData3D ();
  for (int k = raio; k < nz - raio; k++)
    for (int j = raio; j < ny - raio; j++)
      for (int i = raio; i < nx - raio; i++)
	{
	  if (data3D[i][j][k] > raioBolaTangente)	// se for maior que a bola tangente vai permanecer
	    {
	      pmdata3D[i][j][k] = indiceAtivo;
	      // imagemModificada=1;
	    }
	  // se for maior que a inclusa e menor que a tangente pintar a bola
	  else if (data3D[i][j][k] > raioBolaInclusa)
	    // {imagemModificada=1;

/*
// // // // // // // // // // // // // // // // // // // 
// PINTA A BOLA NAO OTIMIZADO: NAO CONSIDERA SIMETRIA
		   for (zz=-raio; zz<=raio; zz++)           // percorre a mascara
         {
         kmz= k + zz;
         rmz= raio + zz;
			for (yy=-raio; yy<=raio; yy++)
           {
             jmy= j + yy;
             rmy= raio + yy;                          // usar simetria
				 for (xx=-raio; xx <=raio;xx++)
             	{
               // imx=i+x;  // rmx=raio+x;
             	if(maskd->data3D[raio+xx][rmy] [rmz]!=0)
						   pm->data3D[i+xx]   [jmy] [kmz]=indiceAtivo;  // pinta na imagem
             	}
           }
         }
// // // // // // // // // // // // // // // // // // // 
*/
// // // // // // // // // // // // // // // // // // // 
// PINTA A BOLA OTIMIZADO: CONSIDERA SIMETRIA
	    for (zz = 0; zz <= raio; zz++)	// percorre a mascara
	      {
		poszb = k - zz;
		poszf = k + zz;
		rmz = raio + zz;
		for (yy = 0; yy <= raio; yy++)
		  {
		    posyn = j + yy;
		    posys = j - yy;
		    rmy = raio + yy;
		    for (xx = 0; xx <= raio; xx++)
		      {
			posxe = i - xx;	// por ser simétrica
			posxd = i + xx;
			rmx = raio + xx;
			if (maskd->data3D[rmx][rmy][rmz] != 0)
			  pmdata3D[posxe][posyn][poszf] =
			    pmdata3D[posxe][posys][poszf] =
			    pmdata3D[posxd][posyn][poszf] =
			    pmdata3D[posxd][posys][poszf] =
			    pmdata3D[posxe][posyn][poszb] =
			    pmdata3D[posxe][posys][poszb] =
			    pmdata3D[posxd][posyn][poszb] =
			    pmdata3D[posxd][posys][poszb] = indiceAtivo;
		      }
		  }
	      }
// // // // // // // // // // // // // // // // // // // 


	}
  return pm;			// pm é a matriz abertura
}

/*
// Considerando simetria
   unsigned int posxe,posxd;                          // x esquerda e x direita
   unsigned int posys,posyn;	                   	// y sul e y norte
   unsigned int poszf,poszb;                   		// z front , e z back
		   for (int z=0; z<=raio; z++)      	// percorre a mascara
         {
         rmz= raio + z;					// posicao na mascara
         poszf= k + z;					// posicao k da imagem mais z da mascara
         poszb= k - z;
			for (int y=0; y<=raio; y++)
             {
             rmy= raio + y;                           // posicao na mascara
		   	 posys= j - y;			// posicao j da imagem e yy da mascara
	   		 posyn= j + y;
				 for (int x=0; x <=raio;x++)
             	{
               rmx=raio+x;				   // posicao na mascara
				   posxe= i - x;         // posicao i da imagem mais x da mascara
				   posxd= i + x;
             	if(maskd->data2D[rmx][rmy] [rmz]!=0)
		              pm->data3D[posxe][posyn][poszf]=
                    pm->data3D[posxe][posys][poszf]=
      		        pm->data3D[posxe][posyn][poszb]=
                    pm->data3D[posxe][posys][poszb]=

                    pm->data3D[posxd][posyn][poszf]=
                    pm->data3D[posxd][posys][poszf]=
                    pm->data3D[posxd][posyn][poszb]=
                    pm->data3D[posxd][posys][poszb]=indiceAtivo;
             	}
         }
*/


/*
============================================================================
Função    Dilatacao
============================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
TMatriz3D *
CFEMMIDF3D::Dilatacao (TMatriz3D * &matriz, unsigned int _RaioBola)
{
  // -->Padrao
  VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
  tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
  // Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara (tamanhoMascara);	// Cria a mascara adequada,do tamanho de tamanhoMascara

  CBCDiscreta3D *maskd = dynamic_cast < CBCDiscreta3D * >(mask);	// Cria ponteiro para mascara com acesso a GetraioBolaTangente

  // Processamento da Dilatacao em si
  // pm->Constante(0);                                                    // zera a matriz imagem
  int mi = Getmi ();		// 
  int ***pmdata3D = pm->GetData3D ();
  for (int jj = 0; jj < ny; jj++)	// percorre toda a idf e
    for (int ii = 0; ii < nx; ii++)	// pinta pontos na imagem
      for (int kk = 0; kk < nz; kk++)	// pinta pontos na imagem
	if (data3D[ii][jj][kk] >= mi)
	  pmdata3D[ii][jj][kk] = indiceAtivo;	// pm->data3D[ii][jj][kk]=1;
	else
	  pmdata3D[ii][jj][kk] = indiceInativo;	// pm->data3D[ii][jj][kk]=0;

  // Otimizacao Mascara (bola)
  int raio = maskd->GetraioX ();
//   int raioBolaTangente=maskd->GetraioBolaTangente();
//   int raioBolaInclusa=maskd->GetraioBolaInclusa();
  int imx;			// i mais x, i+x
  int rmx;			// raio mais x, raio+x
  int jmy;			// 
  int rmy;			// 
  int kmz;			// 
  int rmz;			// 
  // Variáveis para SIMETRIA Bola
  int posxe, posxd;		// x esquerda e x direita
  int posys, posyn;		// y sul e y norte
  int poszb, poszf;		// z back e z front
  int xx, yy, zz;		// posicoes xx e yy da bola
  // falta verificar o contorno
  for (int k = raio; k < nz - raio; k++)
    for (int j = raio; j < ny - raio; j++)
      for (int i = raio; i < nx - raio; i++)
	{
	  if (data3D[i][j][k] == mi)	// usar simetria

/*
// // // // // // // // // // // // // // // // // // // 
// PINTA A BOLA NAO OTIMIZADO: NAO CONSIDERA SIMETRIA
		   for (zz=-raio; zz<=raio; zz++)           // percorre a mascara
         {
         kmz= k + zz;
         rmz= raio + zz;
			for (yy=-raio; yy<=raio; yy++)
           {
             jmy= j + yy;
             rmy= raio + yy;                          // usar simetria
				 for (xx=-raio; xx <=raio;xx++)
             	{
               // imx=i+x;  // rmx=raio+x;
             	if(maskd->data3D[raio+xx][rmy] [rmz]!=0)
						   pm->data3D[i+xx]   [jmy] [kmz]=indiceAtivo;  // pinta na imagem
             	}
           }
         }
// // // // // // // // // // // // // // // // // // // 
*/

// // // // // // // // // // // // // // // // // // // 
// PINTA A BOLA OTIMIZADO: CONSIDERA SIMETRIA
	    for (zz = 0; zz <= raio; zz++)	// percorre a mascara
	      {
		poszb = k - zz;
		poszf = k + zz;
		rmz = raio + zz;
		for (yy = 0; yy <= raio; yy++)
		  {
		    posyn = j + yy;
		    posys = j - yy;
		    rmy = raio + yy;
		    for (xx = 0; xx <= raio; xx++)
		      {
			posxe = i - xx;	// por ser simétrica
			posxd = i + xx;
			rmx = raio + xx;
			if (maskd->data3D[rmx][rmy][rmz] != 0)
			  pmdata3D[posxe][posyn][poszf] =
			    pmdata3D[posxe][posys][poszf] =
			    pmdata3D[posxd][posyn][poszf] =
			    pmdata3D[posxd][posys][poszf] =
			    pmdata3D[posxe][posyn][poszb] =
			    pmdata3D[posxe][posys][poszb] =
			    pmdata3D[posxd][posyn][poszb] =
			    pmdata3D[posxd][posys][poszb] = indiceAtivo;
		      }
		  }
	      }
// // // // // // // // // // // // // // // // // // // 

	}
  // verifica atualização idf
  if (atualizaIDF)		// verifica o flag de atualizacao da idf após dilatação
    Go (pm);			// se ativo recalcula a idf

  return pm;			// pm é a matriz Dilatacao
}

/*
============================================================================
Função    Fechamento
============================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
TMatriz3D *
CFEMMIDF3D::Fechamento (TMatriz3D * &matriz, unsigned int _RaioBola)
{
  bool atualizaIDF_old = atualizaIDF;	// armazena valor de atualizaIDF
  atualizaIDF = 1;		// ativa, para que a Dilatacao recalcule  a idf
  Dilatacao (matriz, _RaioBola);	// processa a dilatação, e depois Go
  atualizaIDF = atualizaIDF_old;	// atualizaIDF volta ao estado anterior

  Erosao (matriz, _RaioBola);	// Processa a erosão, considerando imagem idf atualizada
  return matriz;
}

/*
// IDFNosPlanosDeContorno(int& base);
void CFEMMIDF3D::IDFNosPlanosDeContorno(int& base)
{
 // -------------------------
 // Percorrer todos os planos, se ativo, fazer=3 (menor distancia)
 // Desta forma posso percorrer o centro da imagem direto,sem me preocupar com acesso a pontos inexistentes.
 // Pensar em : pm->setPlanoX(corte=0,valor=3,condicaoPixel==1);
 int pos;										// posicao intermediaria (temporaria)
 int x,y,z;										// Indices para percorrer a matriz
    // ----------IDA
	 for (y=0; y < ny ;y++)					// plano leste x=0
	  	for (z=0; z < nz ;z++)
			 if (data3D[0][y][z]!=0)
				data3D[0][y][z]=base;
	 for (x=0; x < nx ;x++)           	// plano sul y=0
		for (z=0; z < nz ;z++)
			 if (data3D[x][0][z]!=0)
				data3D[x][0][z]=base;
	 for (y=0; y < ny ;y++)					// plano back z=0
		for (x=0; x < nx ;x++)
			 if (data3D[x][y][0]!=0)
				data3D[x][y][0]=base;

    // ----------VOLTA
	 pos=nx-1;
	 for (y=ny-1; y >0 ;y--)				// plano oeste x=nx-1
	  	for (z=nz-1; z >0 ;z--)
			 if (data3D[pos][y][z]!=0)
				data3D[pos][y][z]=base;
	 pos=ny-1;
	 for (x=nx-1; x >0 ;x--)           	// plano norte y=ny-1
	  	for (z=nz-1; z >0 ;z--)
			 if (data3D[x][pos][z]!=0)
				data3D[x][pos][z]=base;
	 pos=nz-1;
	 for (y=ny-1; y >0 ;y--)				// plano front z=nz-1
      for (x=nx-1; x >0 ;x--)
			 if (data3D[x][y][pos]!=0)
				data3D[x][y][pos]=base;
}        */
//       void IDFNosPlanosDeContornoIda(int& base);     // Calcula a idf nos planos de contorno
//       void IDFNosPlanosDeContornoVolta(int& base);     // Calcula a idf nos planos de contorno
/*
void CFEMMIDF3D::IDFNosPlanosDeContornoIDA(int& base)
{
 int x,y,z;										// Indices para percorrer a matriz
    // ----------IDA
	 for (z=0; z < nz ;z++)            // plano leste x=0
		 for (y=0; y < ny ;y++)
			 if (data3D[0][y][z]!=0)
				data3D[0][y][z]=base;
	 for (z=0; z < nz ;z++)            // plano sul y=0
		 for (x=0; x < nx ;x++)
			 if (data3D[x][0][z]!=0)
				data3D[x][0][z]=base;
	 for (y=0; y < ny ;y++)				  // plano back z=0
		for (x=0; x < nx ;x++)
			 if (data3D[x][y][0]!=0)
				data3D[x][y][0]=base;
}
void CFEMMIDF3D::IDFNosPlanosDeContornoVOLTA(int& base)
{
 int x,y,z;									  // Indices para percorrer a matriz
 int pos;									  // posicao intermediaria (temporaria)
    // ----------VOLTA
	 pos=nx-1;
	  	for (z=nz-1; z >0 ;z--)         // plano oeste x=nx-1
		 for (y=ny-1; y >0 ;y--)
			 if (data3D[pos][y][z]!=0)
				data3D[pos][y][z]=base;
	 pos=ny-1;
	  	for (z=nz-1; z >0 ;z--)         // plano norte y=ny-1
		 for (x=nx-1; x >0 ;x--)
			 if (data3D[x][pos][z]!=0)
				data3D[x][pos][z]=base;
	 pos=nz-1;
	 for (y=ny-1; y >0 ;y--)			  // plano front z=nz-1
      for (x=nx-1; x >0 ;x--)
			 if (data3D[x][y][pos]!=0)
				data3D[x][y][pos]=base;
}
*/
// Funcoes inline
void
CFEMMIDF3D::min (int &t)
{
  if (t < minimo)
    minimo = t;
}

