//�tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef  TFMMIDF3D_cpp
#define  TFMMIDF3D_cpp
/*
  ----------------------------------------------------------------------------
  PROJETO:		Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	TFMMIDF3D.cpp
  Nome da classe:      TFMMIDF3D
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  Descricao:	 Implementa as fun��es da classe TFMMIDF3D.
*/

//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------

#include "TFiltro\TFEspacial3D\TFEMorfologiaMatematica3D\TFMMIDF3D.h"  //Classe base
#include "TMascara\TMCDiscreta3D\TBCDiscreta3D.h"  		   	//Cria objeto

/*
  ============================================================================
  Atributos estaticos da classe
  ============================================================================
*/
//Se verdadeira ap�s cada processamento recalcula a idf
bool TFMMIDF3D::atualizaIDF=0;		//por default, n�o recalcula a idf apos dilatacao ou fechamento

/*
  ============================================================================
  Documentacao construtor
  ============================================================================
  Executa o construtor da classe base, o TMatriz3D(_pm)
  que realiza a copia dos dados.
  Depois copia o endereco da imagem no atributo pm.
  Assim pm aponta para a matriz recebida e data3D para a matriz IDF
*/
//Construtor sobrecarregado, recebe tamb�m o tamanho do raio m�ximo
TFMMIDF3D::TFMMIDF3D(TMatriz3D *& matriz, unsigned int _tamanhoMascara, unsigned int _raioMax)
  :TFMorfologiaMatematica3D(matriz,_tamanhoMascara)
  ,TMatriz3D(matriz->GetNX(),matriz->GetNY(),matriz->GetNZ()),raioBola(_tamanhoMascara-1/2),raioMaximo(_raioMax)
{
}

/*
  ============================================================================
  Fun��o    ExecutadaPorGo
  ============================================================================
  Fun��o usada exclusivamente por Go:

  1-Quando o usu�rio quer calcular a idf, chama a funcao Go (pm).
  No caso da imagem pm passada ter dimems�es diferentes da pm passada para o construtor, devo
  desalocar a matris idf, definir os novos valores de nx e ny e realocar a idf.
  Se as dimens�es da matriz passada e minhas forem iguais posso continuar.

  2-Copia valores da matriz passada para a matriz idf, usando data2D[i][j]=pm->data2D[i][j]

  Esta fun��o foi inclu�da porque o usu�rio programador pode chamar Go(pm)
  sendo pm diferente daquela passada par o construtor.	Ou seja cria-se uma seguran�a a mais.

  Outro fato que justifica esta fun��o � que antes, criava objeto idf, executava Go, e depois
  podia executar a erosao, mas sempre sobre a idf da imagem passada para o construtor
  da classe. Agora posso passar uma nova matriz para a fun��o Go,
  diferente da passada para o construtor.

  O terceiro motivo � que no caso de se realizar uma sequencia de erosoes com a mesma
  mascara, eu precisava criar um objeto idf, calcular go e calcular a erosao,
  criar nova idf (sobre pm erodida), executar go e ent�o realizar a erosao...
  Agora posso recalcular Go a qualquer momento, sem recriar o objeto idf.
  Exemplo:
  cria idf      		pfmm=new TMMIDF(pm,tamanhoMascara);
  executa go    		pfmm->Go(pm,tamanhoMascara);
  executa erosao    pfmm->Erosao(pm,raioBola);       //erosao sobre imagem original
  executa go    		pfmm->Go(pm,tamanhoMascara);
  executa erosao    pfmm->Erosao(pm,raioBola);			//erosao sobre imagem j� erodida

  Assim, toda funcao Go herdeira da TFMMIDF deve chamar esta funcao:
  ExecutadaPorGo(matriz);

*/
//Funcao chamada exclusivamente por Go, ou seja depois de executada a idf vai ser recalculada.
void TFMMIDF3D::ExecutadaPorGo(TMatriz3D*& matriz)//,unsigned int _tamanhoMascara)
{
  pm=matriz;                        //armazena endere�o matriz
  //verifica se a matriz tem as mesmas dimensoes da idf(this)
  if(this->nx != matriz->GetNX() || this->ny != matriz->GetNY() || this->nz != matriz->GetNZ())
    {
      Desaloca();		 //desaloca a matriz existende e depois
      this->nx = matriz->GetNX(); //define novo tamanho
      this->ny = matriz->GetNY();
      this->nz = matriz->GetNZ();
      Aloca();                    //aloca de acordo com novo tamanho
    }			 //Agora tenho de armazenar valores de pm na idf

  //substituir por this=matriz;
  for( int i=0; i< nx; i++)
    for( int j=0; j < ny; j++)
      for( int k=0; k< nz; k++)
        if(matriz->data3D[i][j][k]!=0)  //como a imagem recebida por ser uma outra idf
	  this->data3D[i][j][k]=1;     //define this com 0 e 1
  //////////////////////////
	else                               //AQUI AQUI AQUI AQUI: trocar 1 por mi
	  this->data3D[i][j][k]=0;
  //this->data2D[i][j]=matriz->data2D[i][j];
  //Write("ExecutadaPorGo.txt");
}
/*
  ============================================================================
  Fun��o    VerificaImagem
  ============================================================================
  Usada pelas fun��es Erosao,Dilatacao, fechamento e abertura para verificar se � a mesma imagem.
  Se for outra imagem recalcula Go.
  Tambem armazena endere�o da imagem em pm.
*/
void TFMMIDF3D::VerificaImagem(TMatriz3D*& matriz)
{                                     //se for a mesma imagem e tiver o mesmo tamanho sai
  //pm=matriz;
  if(pm==matriz && nx==matriz->GetNX() && ny==matriz->GetNY() && nz==matriz->GetNZ() )
    return;						     //sai
  else
    Go(matriz);				  //sen�o chama Go, que redefine o tamanho da imagem
}					  //e calcula valores idf


/*
  ============================================================================
  Fun��o    Erosao.
  ============================================================================
  Obs: A sequ�ncia de execucao desta fun��o n�o deve ser alterada sem uma analise detalhada
*/
TMatriz3D*  TFMMIDF3D::Erosao(TMatriz3D*& matriz,unsigned int _RaioBola)    //
{
  //-->Padrao
  VerificaImagem(matriz);    				//verifica se � a mesma imagem (se diferente recalcula Go)
  tamanhoMascara=2*_RaioBola+1;         //Define o tamanho da mascara
  //Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara(tamanhoMascara);				//Cria a mascara adequada, do tamanho de tamanhoMascara

  TBCDiscreta3D* maskd     = dynamic_cast<TBCDiscreta3D*>(mask);//Cria ponteiro para mascara com acesso a GetraioBolaTangente

  //Processamento da erosao em si
  int raioBolaInclusa=maskd->GetraioBolaInclusa();
  for( int k=0; k < nz; k++)
    for( int  j=0; j < ny; j++)
      for(int  i=0; i< nx; i++)
	{
	  //se o ponto da idf for maior que a bola tangente, faz ponto=1
	  if (data3D[i][j][k] >  raioBolaInclusa)
	    pm->data3D[i][j][k]=1;  //seta ponto ativo
	  else
	    pm->data3D[i][j][k]=0;  //seta ponto inativo
	}
  return pm;                             //pm � a matriz erodida
}

/*
  ============================================================================
  Fun��o    Abertura
  ============================================================================
  Obs: A sequ�ncia de execucao desta fun��o n�o deve ser alterada sem uma analise detalhada
*/
TMatriz3D*  TFMMIDF3D::Abertura(TMatriz3D*& matriz,unsigned int _RaioBola)	//
{
  //-->Padrao
  VerificaImagem(matriz);    			//verifica se � a mesma imagem (se diferente recalcula Go)
  tamanhoMascara=2*_RaioBola+1;         //Define o tamanho da mascara
  //Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara(tamanhoMascara);				//Cria a mascara adequada,do tamanho de tamanhoMascara

  TBCDiscreta3D* maskd     = dynamic_cast<TBCDiscreta3D*>(mask);//Cria ponteiro para mascara com acesso a GetraioBolaTangente

  //Processamento da abertura em si
  pm->Constante(0);							//zera a matriz imagem
  int raio=maskd->GetraioX();
  int raioBolaTangente=maskd->GetraioBolaTangente();
  int raioBolaInclusa=maskd->GetraioBolaInclusa();
  //int imx;						//i mais x, i+x
  //int rmx;						//raio mais x, raio+x
  int jmy;							//
  int rmy;							//
  int kmz;							//
  int rmz;							//
  //falta verificar o contorno
  for(  int k=raio; k < nz-raio; k++)
    for(  int j=raio; j < ny-raio; j++)
      for(  int i=raio; i < nx-raio; i++)
	{
	  if (data3D[i][j][k] >  raioBolaTangente)	//se for maior que a bola tangente vai permanecer
	    pm->data3D[i][j][k]=1;
	  //se for maior que a inclusa e menor que a tangente pintar a bola
	  else if (data3D[i][j][k] > raioBolaInclusa)
	    //Aqui poderia considerar simetria no que se refere a mascara
	    for (int z=-raio; z<=raio; z++)           //percorre a mascara
	      {
		kmz= k + z;
		rmz= raio + z;
		for (int y=-raio; y<=raio; y++)
		  {
		    jmy= j + y;
		    rmy= raio + y;                          //usar simetria
		    for (int x=-raio; x <=raio;x++)
		      {
			//imx=i+x;
			//rmx=raio+x;
			if(maskd->data3D[raio+x][rmy] [rmz]!=0)
			  pm->data3D[i+x]   [jmy] [kmz]=1;  //pinta na imagem
		      }
		  }
	      }
	}
  return pm;                             					 //pm � a matriz abertura
}
/*
//Considerando simetria
unsigned int posxe,posxd;                       //x esquerda e x direita
unsigned int posys,posyn;	                //y sul e y norte
unsigned int poszf,poszb;                   	//z front , e z back
for (int z=0; z<=raio; z++)           		//percorre a mascara
{
mz= raio + z;					//posicao na mascara
poszf= k + z;					//posicao k da imagem mais z da mascara
poszb= k - z;
for (int y=0; y<=raio; y++)
{
rmy= raio + y;                                   //posicao na mascara
posys= j - y;					 //posicao j da imagem e yy da mascara
posyn= j + y;
for (int x=0; x <=raio;x++)
{
rmx=raio+x;					 //posicao na mascara
posxe= i - x;                    	         //posicao i da imagem mais x da mascara
posxd= i + x;
if(maskd->data2D[rmx][rmy] [rmz]!=0)
pm->data3D[posxe][posyn][poszf]=
pm->data3D[posxe][posys][poszf]=
pm->data3D[posxe][posyn][poszb]=
pm->data3D[posxe][posys][poszb]=

pm->data3D[posxd][posyn][poszf]=
pm->data3D[posxd][posys][poszf]=
pm->data3D[posxd][posyn][poszb]=
pm->data3D[posxd][posys][poszb]=1;
}
}
*/


/*
  ============================================================================
  Fun��o    Dilatacao
  ============================================================================
  Obs: A sequ�ncia de execucao desta fun��o n�o deve ser alterada sem uma analise detalhada
*/
TMatriz3D*  TFMMIDF3D::Dilatacao(TMatriz3D*& matriz,unsigned int _RaioBola)
{
  //-->Padrao
  VerificaImagem(matriz);    				//verifica se � a mesma imagem (se diferente recalcula Go)
  tamanhoMascara=2*_RaioBola+1;         //Define o tamanho da mascara
  //Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara(tamanhoMascara);				//Cria a mascara adequada,do tamanho de tamanhoMascara

  TBCDiscreta3D* maskd     = dynamic_cast<TBCDiscreta3D*>(mask);//Cria ponteiro para mascara com acesso a GetraioBolaTangente

  //Processamento da Dilatacao em si
  int raio=maskd->GetraioX();
  int raioBolaTangente=maskd->GetraioBolaTangente();
  int raioBolaInclusa=maskd->GetraioBolaInclusa();
  int imx;							//i mais x, i+x
  int rmx;							//raio mais x, raio+x
  int jmy;							//
  int rmy;							//
  int kmz;							//
  int rmz;							//
  int mi=Getmi();					//
  //falta verificar o contorno

  //pm->Constante(0);							//zera a matriz imagem
  for( int jj=0; jj < ny; jj++)					//percorre toda a idf e
    for( int ii=0; ii< nx; ii++)             //pinta pontos na imagem
      for( int kk=0; kk< nz; kk++)             //pinta pontos na imagem
        if(data3D[ii][jj][kk] >=mi)
	  pm->data3D[ii][jj][kk]=1;
        else
	  pm->data3D[ii][jj][kk]=0;


  for(  int k=raio; k < nz-raio; k++)
    for(  int j=raio; j < ny-raio; j++)
      for(  int i=raio; i< nx-raio; i++)
	{
	  //if(data3D[i][j][k] > mi)
	  //   pm->data3D[i][j][k]=1;
	  //else
	  if (data3D[i][j][k] ==mi )			//usar simetria
	    //alterei para
	    //if (data3D[i][j][k] >=mi || data3D[i][j][k] < raioBolaTangente)

	    //PintaBola(i,j,k,raio)
	    for (int z=-raio; z<=raio; z++)           //percorre a mascara
	      {
		kmz= k + z;
		rmz= raio + z;
		for (int y=-raio; y<=raio; y++)
		  {
		    jmy= j + y;
		    rmy= raio + y;                          //usar simetria
		    for (int x=-raio; x <=raio;x++)
		      {
			//imx=i+x;
			//rmx=raio+x;
			if(maskd->data3D[raio+x][rmy] [rmz]!=0)
			  pm->data3D[i+x]   [jmy] [kmz]=1;  //pinta na imagem
		      }
		  }
	      }
	}
  //verifica atualiza��o idf
  if(atualizaIDF)					//verifica o flag de atualizacao da idf ap�s dilata��o
    Go(pm);						//se ativo recalcula a idf

  return pm;                             //pm � a matriz Dilatacao
}

/*
  ============================================================================
  Fun��o    Fechamento
  ============================================================================
  Obs: A sequ�ncia de execucao desta fun��o n�o deve ser alterada sem uma analise detalhada
*/
TMatriz3D*  TFMMIDF3D::Fechamento(TMatriz3D*& matriz,unsigned int _RaioBola)
{
  bool atualizaIDF_old=atualizaIDF;//armazena valor de atualizaIDF
  atualizaIDF=1;                //ativa, para que a Dilatacao recalcule  a idf
  Dilatacao(matriz,_RaioBola);	//processa a dilata��o, e depois Go
  atualizaIDF=atualizaIDF_old;  //atualizaIDF volta ao estado anterior

  Erosao(matriz,_RaioBola);		//Processa a eros�o, considerando imagem idf atualizada
}
/*
//IDFNosPlanosDeContorno(int& base);
void TFMMIDF3D::IDFNosPlanosDeContorno(int& base)
{
//-------------------------
//Percorrer todos os planos, se ativo, fazer=3 (menor distancia)
//Desta forma posso percorrer o centro da imagem direto,sem me preocupar com acesso a pontos inexistentes.
//Pensar em : pm->setPlanoX(corte=0,valor=3,condicaoPixel==1);
int pos;						//posicao intermediaria (temporaria)
int x,y,z;						//Indices para percorrer a matriz
//----------IDA
for (y=0; y < ny ;y++)					//plano leste x=0
for (z=0; z < nz ;z++)
if (data3D[0][y][z]!=0)
data3D[0][y][z]=base;
for (x=0; x < nx ;x++)           	//plano sul y=0
for (z=0; z < nz ;z++)
if (data3D[x][0][z]!=0)
data3D[x][0][z]=base;
for (y=0; y < ny ;y++)					//plano back z=0
for (x=0; x < nx ;x++)
if (data3D[x][y][0]!=0)
data3D[x][y][0]=base;

//----------VOLTA
pos=nx-1;
for (y=ny-1; y >0 ;y--)				        //plano oeste x=nx-1
for (z=nz-1; z >0 ;z--)
if (data3D[pos][y][z]!=0)
data3D[pos][y][z]=base;
pos=ny-1;
for (x=nx-1; x >0 ;x--)           	                //plano norte y=ny-1
for (z=nz-1; z >0 ;z--)
if (data3D[x][pos][z]!=0)
data3D[x][pos][z]=base;
pos=nz-1;
for (y=ny-1; y >0 ;y--)				        //plano front z=nz-1
for (x=nx-1; x >0 ;x--)
if (data3D[x][y][pos]!=0)
data3D[x][y][pos]=base;
}        */
 //	void IDFNosPlanosDeContornoIda(int& base);       //Calcula a idf nos planos de contorno
 //	void IDFNosPlanosDeContornoVolta(int& base);     //Calcula a idf nos planos de contorno
void TFMMIDF3D::IDFNosPlanosDeContornoIDA(int& base)
{
  int x,y,z;						//Indices para percorrer a matriz
  //----------IDA
  for (z=0; z < nz ;z++)            //plano leste x=0
    for (y=0; y < ny ;y++)
      if (data3D[0][y][z]!=0)
	data3D[0][y][z]=base;
  for (z=0; z < nz ;z++)            //plano sul y=0
    for (x=0; x < nx ;x++)
      if (data3D[x][0][z]!=0)
	data3D[x][0][z]=base;
  for (y=0; y < ny ;y++)				  //plano back z=0
    for (x=0; x < nx ;x++)
      if (data3D[x][y][0]!=0)
	data3D[x][y][0]=base;
}
void TFMMIDF3D::IDFNosPlanosDeContornoVOLTA(int& base)
{
  int x,y,z;						  //Indices para percorrer a matriz
  int pos;						  //posicao intermediaria (temporaria)
  //----------VOLTA
  pos=nx-1;
  for (z=nz-1; z >0 ;z--)                                 //plano oeste x=nx-1
    for (y=ny-1; y >0 ;y--)
      if (data3D[pos][y][z]!=0)
	data3D[pos][y][z]=base;
  pos=ny-1;
  for (z=nz-1; z >0 ;z--)                                 //plano norte y=ny-1
    for (x=nx-1; x >0 ;x--)
      if (data3D[x][pos][z]!=0)
	data3D[x][pos][z]=base;
  pos=nz-1;
  for (y=ny-1; y >0 ;y--)			          //plano front z=nz-1
    for (x=nx-1; x >0 ;x--)
      if (data3D[x][y][pos]!=0)
	data3D[x][y][pos]=base;
}

//Funcoes inline
void TFMMIDF3D::min(int& t)
{
  if (t < minimo)
    minimo = t;
}
#endif

