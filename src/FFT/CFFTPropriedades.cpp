
//------------------------------------------------------------------
//Bibliotecas
//------------------------------------------------------------------
#include <cmath>
#include <iostream>

//------------------------------------------------------------------
//Bibliotecas LDSC
//------------------------------------------------------------------
#include    <FFT/CFFTPropriedades.h>
#include    <Estatistica/CERandomico.h>

#define Real_E1(x,y,z)	Re_data[(z)*NY*NX+(y)*NX+(x)]
#define Imag_E1(x,y,z)	Im_data[(z)*NY*NX+(y)*NX+(x)]

//------------------------------------------------------------------------------
//Funcao relacionada a uma propriedade da Transformada de Fourier
//Descrição desta propriedade aproximadamente na pagina 39 da tese
//------------------------------------------------------------------------------
//Tenho |Y|, quero Y:
//Y pode ser obtido do modulo de Y e do angulo de fase, ou seja Y= |Y| + angulo de fase
//Esta função representa uma funcao do objeto transformada de fourier (uma propriedade do objeto fourier)

//Funcinamento desta funcao:
//A matriz resultado e uma matriz de numeros complexos (real + imaginario).
//Só que para alguns setores da matriz os elementos sao só imaginarios e para outros
//imaginario e real.
//Por isso alguns for abaixo preenchem  só a parte real

//Nos setores que tem real e imaginario precisa gerar parte imaginária.
//Alem disso, quando da realizacao da simetria, se for real e só aplicar a simetria,
//se for imaginário, o simetrico tem a parte imaginaria multiplicada por -1 (conjugado).

//antiga função Fy() do codigo do Liang
void
CFFTPropriedades::FFTY_of_ModY (double *Re_data, double *Im_data)	//,int semente=0)
{
  int i, j, k;			//contadores
  double angulo;		//angulo de fase
  int NXdiv2 = NX / 2;		//variavel aceleradora
  int NYdiv2 = NY / 2;		//variavel aceleradora
  int NZdiv2 = NZ / 2;		//variavel aceleradora

  //Variaveis auxiliares
  unsigned long pos;		//novidade
  unsigned long posB;		//novidade

  unsigned long NX_I;		//=NX-i;
  unsigned long NY_J;		//=NY-j;
  unsigned long NZ_K;		//=NZ-k;
  unsigned long NYxNX;
  unsigned long JxNX;		//=j*NX;
  unsigned long KxNYxNX;	//=k*NYxNX;
  unsigned long NY_JxNX;	//=NYxNX-JxNX;
  unsigned long NZxNYxNX;	//=NZxNYxNX;
  unsigned long NZ_KxNYxNX;	//=(NZxNYxNX-KxNYxNX);

  unsigned long NYdiv2xNX;	//=(NYdiv2)*NX;

  //Cria objeto utilizado para gerar numeros randomicos,passa a semente e o multiplicador
  //para obter um numero randomico execute: randomico->Go;
  long int semente = -1;
  CERandomico *randomico = new CERandomico (semente);
  randomico->Maximo (2.0 * M_PI);	//o número randomico fica entre 0->2pi

  //--------------------------------------------------------------------
  //Calculo das variaveis auxiliares
  NYxNX = NY * NX;
  NZxNYxNX = NZ * NY * NX;
  NYdiv2xNX = NYdiv2 * NX;

  //--------------------------------------------------------------------
  //Percorre os planos k=0 k=NZ/2
  //--------------------------------------------------------------------
  for (k = 0; k < NZ; k += NZdiv2)	//
    {
      NZ_K = NZ - k;
      KxNYxNX = k * NYxNX;
      NZ_KxNYxNX = NZxNYxNX - KxNYxNX;
      //------------------------------------------------------------------
      //pontos 0,0,0 e 0,0,NZ/2
      pos = KxNYxNX;
      Re_data[pos] *= cos (randomico->Go ());	//1
      Im_data[pos] = 0;

      //pontos 0,NYdiv2,0 e 0,NYdiv2,NZ/2
      pos = NYdiv2xNX + KxNYxNX;
      Re_data[pos] *= cos (randomico->Go ());	//1
      Im_data[pos] = 0;

      //pontos 0,0,0 e 0,0,NZ/2
      pos = NXdiv2 + KxNYxNX;
      Re_data[pos] *= cos (randomico->Go ());	//1
      Im_data[pos] = 0;

      //pontos 0,0,0 e 0,0,NZ/2
      pos = NXdiv2 + NYdiv2xNX + KxNYxNX;
      Re_data[pos] *= cos (randomico->Go ());	//1
      Im_data[pos] = 0;

      //-------------------------------------------------------------------
      //*******************************************************************
      //Preenche os planos Z=0 e Z=NZ/2 exceto os pontos já calculados acima
      //Poderia mover este for para cima e eliminar o if. Só que o resultado final embora certo vai diferir do Liang
      //por causa da sequencia de geração do número aleatório
      for (i = 0; i <= (NXdiv2); i++)
	{
	  //i
	  //NX_I=NX-i;
	  for (j = 0; j < (NY); j++)
	    {
	      //j
	      //NY_J=NY-j;
	      JxNX = j * NX;
	      //NY_JxNX=NYxNX-JxNX;
	      if (!
		  ((i == 0 && j == 0) || (i == 0 && j == NYdiv2)
		   || (i == NXdiv2 && j == 0) || (i == NXdiv2
						  && j == NYdiv2)))
		{
		  angulo = randomico->Go ();
		  pos = i + JxNX + KxNYxNX;
		  //Imag_E1(i,j,k)=Real_E1(i,j,k)*sin(angulo);//5
		  Im_data[pos] = Re_data[pos] * sin (angulo);	//5
		  //Real_E1(i,j,k)=Real_E1(i,j,k)*cos(angulo);//5
		  Re_data[pos] = Re_data[pos] * sin (angulo);
		}
	    }
	}
      //-------------------------------------------------------------------
      //rebate as linhas y    (vertical) para cima
      for (j = 1; j <= (NYdiv2); j++)
	{
	  //j
	  NY_J = NY - j;
	  JxNX = j * NX;
	  NY_JxNX = NYxNX - JxNX;

	  pos = NY_JxNX + KxNYxNX;
	  posB = JxNX + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//6 //rebate linha y /*First row*/
	  Im_data[pos] = -Im_data[posB];

	  pos = NXdiv2 + NY_JxNX + KxNYxNX;
	  posB = NXdiv2 + JxNX + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//
	  Im_data[pos] = -Im_data[posB];
	}
      //-------------------------------------------------------------------
      //rebate as linhas x    (horizontal) para a direita
      for (i = 1; i <= (NXdiv2); i++)
	{
	  //i
	  NX_I = NX - i;
	  pos = NX_I + KxNYxNX;
	  posB = i + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//
	  Im_data[pos] = -Im_data[posB];

	  pos = NX_I + NYdiv2xNX + KxNYxNX;
	  posB = i + NYdiv2xNX + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//
	  Im_data[pos] = -Im_data[posB];
	}
       /*E*/
	//--------------------------------------------------------------------
	//Cruza as regiões dos planos z=0 e z=NZ/2
	for (i = 1; i <= (NXdiv2); i++)
	for (j = 1; j <= (NYdiv2); j++)
	  {
	    //j
	    NY_J = NY - j;
	    JxNX = j * NX;
	    NY_JxNX = NYxNX - JxNX;
	    //i
	    NX_I = NX - i;

	    pos = NX_I + JxNX + KxNYxNX;
	    posB = i + NY_JxNX + KxNYxNX;
	    Re_data[pos] = Re_data[posB];	//15 //rebate linha y        /*First row*/
	    Im_data[pos] = -Im_data[posB];

	    pos = posB;
	    posB = i + JxNX + KxNYxNX;
	    Re_data[pos] = Re_data[posB];	//16 cruza ptos no plano z=0 e z=NZ/2
	    Im_data[pos] = -Im_data[posB];
	    /*GAMA* */
	  }
    }

  //--------------------------------------------------------------------
  //Percorre os planos k=1 até k<NZ/2             /*k=1-NZdiv2-1   */
  //--------------------------------------------------------------------
  //*******************************************************************
  //Principal for de calculo. Calcula todos os planos de z=1 até z=NZ/2-1
  //Ou seja o ítem 17 somado aos ítens 15 e 16 preenchem metade da matriz 3D
  for (k = 1; k < NZdiv2; k++)
    {
      //k
      //NZ_K=NZ-k;
      KxNYxNX = k * NYxNX;
      //NZ_KxNYxNX=(NZxNYxNX-KxNYxNX);
      for (i = 0; i < NX; i++)
	{
	  //i
	  NX_I = NX - i;
	  for (j = 0; j < NY; j++)
	    {
	      //j
	      //NY_J=NY-j;
	      JxNX = j * NX;	//
	      //NY_JxNX=NYxNX-JxNX;

	      angulo = randomico->Go ();
	      pos = i + JxNX + KxNYxNX;
	      //Imag_E1(i,j,k)=Real_E1(i,j,k)*sin(angulo);//17
	      Im_data[pos] = Re_data[pos] * sin (angulo);	//17
	      //Real_E1(i,j,k)=Real_E1(i,j,k)*cos(angulo);
	      Re_data[pos] *= cos (angulo);
	    }
	}
    }

  //--------------------------------------------------------------------
  //Percorre os planos k=1 até k<NZ/2 e realiza rebatimentos para cima
  //--------------------------------------------------------------------
/*k=NZdiv2+1 -NZ-1*/
  for (k = 1; k < NZdiv2; k++)
    {
      //k
      NZ_K = NZ - k;
      KxNYxNX = k * NYxNX;
      NZ_KxNYxNX = NZxNYxNX - KxNYxNX;
      //rebate linha    vertical para cima
      pos = NZ_KxNYxNX;
      posB = KxNYxNX;
      Re_data[pos] = Re_data[posB];	//
      Im_data[pos] = -Im_data[posB];

      //rebate linha    vertical para cima
      pos = NYdiv2xNX + NZ_KxNYxNX;
      posB = NYdiv2xNX + KxNYxNX;
      Re_data[pos] = Re_data[posB];	// 19
      Im_data[pos] = -Im_data[posB];

      //rebate linha    vertical para cima
      pos = NXdiv2 + NZ_KxNYxNX;
      posB = NXdiv2 + KxNYxNX;
      Re_data[pos] = Re_data[posB];	//20
      Im_data[pos] = -Im_data[posB];

      //rebate linha    vertical para cima
      pos = NXdiv2 + NYdiv2xNX + NZ_KxNYxNX;
      posB = NXdiv2 + NYdiv2xNX + KxNYxNX;
      Re_data[pos] = Re_data[posB];	//21
      Im_data[pos] = -Im_data[posB];

      //--------------------------------------------------------------------
      //Cruza os planos y=0 e y=NY/2
      for (j = 1; j <= NYdiv2; j++)
	{
	  //j
	  NY_J = NY - j;
	  JxNX = j * NX;
	  NY_JxNX = NYxNX - JxNX;

	  pos = JxNX + NZ_KxNYxNX;
	  posB = NY_JxNX + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//22
	  Im_data[pos] = -Im_data[posB];

	  pos = NY_JxNX + NZ_KxNYxNX;
	  posB = JxNX + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//23
	  Im_data[pos] = -Im_data[posB];

	  pos = NXdiv2 + JxNX + NZ_KxNYxNX;
	  posB = NXdiv2 + NY_JxNX + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//24
	  Im_data[pos] = -Im_data[posB];

	  pos = NXdiv2 + NY_JxNX + NZ_KxNYxNX;
	  posB = NXdiv2 + JxNX + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//25

	  Im_data[pos] = -Im_data[posB];
	}

      //--------------------------------------------------------------------
      //Cruza os planos x=0 e x=NX/2
      for (i = 1; i <= NXdiv2; i++)
	{
	  //i
	  NX_I = NX - i;

	  pos = i + NZ_KxNYxNX;
	  posB = NX_I + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//26
	  Im_data[pos] = -Im_data[posB];

	  pos = NX_I + NZ_KxNYxNX;
	  posB = i + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//27
	  Im_data[pos] = -Im_data[posB];

	  pos = i + NYdiv2xNX + NZ_KxNYxNX;
	  posB = NX_I + NYdiv2xNX + KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//28
	  Im_data[pos] = -Im_data[posB];

	  pos = NX_I + NYdiv2xNX + NZ_KxNYxNX;
	  posB = NX_I + NYdiv2xNX + NZ_KxNYxNX;
	  Re_data[pos] = Re_data[posB];	//29
	  Im_data[pos] = -Im_data[posB];
	}

      //--------------------------------------------------------------------
      //********************************************************************
      //Rebate os blocos inferiores para cima, cruzando os mesmos
      //Para os planos 1 < Z < NZ/2
      //k
      NZ_K = NZ - k;
      KxNYxNX = k * NYxNX;
      NZ_KxNYxNX = (NZxNYxNX - KxNYxNX);
      for (i = 1; i <= (NXdiv2); i++)
	{
	  //i
	  NX_I = NX - i;
	  for (j = 1; j <= (NYdiv2); j++)
	    {
	      //j
	      NY_J = NY - j;
	      JxNX = j * NX;
	      NY_JxNX = NYxNX - JxNX;
	      //Real_E1(i,j,NZ-k)=Real_E1(NX-i,NY-j,k);//30
	      //Imag_E1(i,j,NZ-k)=-Imag_E1(NX-i,NY-j,k);
	      pos = NX_I + NY_JxNX + KxNYxNX;
	      posB = i + JxNX + NZ_KxNYxNX;
	      Re_data[posB] = Re_data[pos];
	      Im_data[posB] = -Im_data[pos];

	      //Real_E1(NX-i,NY-j,NZ-k)=Real_E1(i,j,k);//31
	      //Imag_E1(NX-i,NY-j,NZ-k)=-Imag_E1(i,j,k);
	      pos = i + JxNX + KxNYxNX;
	      posB = NX_I + NY_JxNX + NZ_KxNYxNX;
	      Re_data[posB] = Re_data[pos];	//31
	      Im_data[posB] = -Im_data[pos];

	      //Real_E1(i,NY-j,NZ-k)=Real_E1(NX-i,j,k);//32
	      //Imag_E1(i,NY-j,NZ-k)=-Imag_E1(NX-i,j,k);
	      pos = NX_I + JxNX + KxNYxNX;
	      posB = i + NY_JxNX + NZ_KxNYxNX;
	      Re_data[posB] = Re_data[pos];	//32
	      Im_data[posB] = -Im_data[pos];

	      //Real_E1(NX-i,j,NZ-k)=Real_E1(i,NY-j,k); //33
	      //Imag_E1(NX-i,j,NZ-k)=-Imag_E1(i,NY-j,k);
	      pos = i + NY_JxNX + KxNYxNX;
	      posB = NX_I + JxNX + NZ_KxNYxNX;
	      Re_data[posB] = Re_data[pos];	//32
	      Im_data[posB] = -Im_data[pos];
	    }
	}
    }
  delete randomico;		//deleta o objeto de geracao de numeros randomicos
}

/*	//atualização de variáveis auxiliares
	//k
	NZ_K=NZ-k;
	KxNYxNX=k*NYxNX;
	NZ_KxNYxNX=(NZxNYxNX-KxNYxNX);
	//j
	NY_J=NY-j;
	JxNX=j*NX;
	NY_JxNX=NYxNX-JxNX;
	//i
	NX_I=NX-i;
*/
