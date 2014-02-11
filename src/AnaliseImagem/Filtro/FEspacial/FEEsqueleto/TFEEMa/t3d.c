//Programa para extrair esqueleto 3D de uma imagem binaria 3D
//O programa foi desenvolvido pelo Liang mesmo
//foi baseado no artigo: (lista bibliografica tese Liang)
//								Ma C,M, 3D fully parallel thining alforithin...1995
//								Ma C,M, 3D topology preservation...1994


// xlC -o t3d t3d.C -lm
// usage: t3d inputfile N or
//        t3d inputfile NX NY NZ
//
// input: image file
// output: thinning file
//
// Liang Zhirong
// Aug., 1996, Florianoplis

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream.h>

#include "Matrix3di.h"							//biblioteca de matrizes 3D

#define NumMax    (27)							//numero maximo de vizinhos 26+central
//#define PI4 (2*asin(1)/180)

int i,j,k;											//contador
int NumTempA;                             //numero de templates A,B,C
int NumTempB;                             //Um template e uma estrutura com numero de pontos
int NumTempC;                             //e 4 vetores para x,y,z,p
int ***Image,***Thin;                     //Image e a entrada e Thin e a imagem de saida
int NX=60,NY=60,NZ=60;                    //numero de pontos default
char FileInput[200]="GF2.img",FileOutput[200];//arquivo entrada e saida


//Classe de controle do numero de pontos e dos pontos x,y,z,p
//atraves de vetores
//Conforme pode ser visto na tese, pg 62, existem 3 configuraçoes para templates
//Para configuraçao a)6 casos
//Para configuraçao b)8 casos
//Para configuraçao c)12 casos
//no arquivo de disco tempA.dat contem os vetores que construem
//os 6 casos de a, idem para B e C.
//
class Template
{
public:
	unsigned int Num;
	int *x,*y,*z,*p;

	Template();Template(int);
	~Template();
};
//Construtor aloca os vetores
Template::Template()
{
	x=new int[NumMax];
	y=new int[NumMax];
	z=new int[NumMax];
	p=new int[NumMax];
}
//destrutor destroe os vetores
Template::Template(int Num)
{
	x=new int[Num];
	y=new int[Num];
	z=new int[Num];
	p=new int[Num];
}
//deleta os vetores
Template::~Template()
{
	delete []x;
	delete []y;
	delete []z;
	delete []p;
}
//cria vetores de templates globais
Template TempA[100],TempB[100],TempC[100],TempCube;

int Simple(int,int,int);			//Para verificar se e ponto simples ou nao
int TrueEdgePoint();             //Par verificar se e ponto de edge (contorno)
int TrueTempA();                 //Para verificar se satisfaz o template do tipo A
int TrueTempB();                 //"
int TrueTempC();                 //"

//calcula o thining
main(int argc, char **argv)
{
	system ("date");
	int ni,nj,nk,kk,ki,kn;			//contador
	int Judge;							//juiz, toma decisao
	int JudgeBlack;					//Para verificar componente preto ()
	int JudgeWhite;               //Para verificar componente branco ()
	int N;								//contador
	long NumeroPontosPretosImagem;//Conta o numero de pontos pretos na imagem original
	long NumeroMarcadosParaDeletar;//Conta o numero de pontos marcados para deletar
	long NumeroPontosDeletados;	//Conta o numero de pontos deletados


	if (argc == 1)						//testa parametros
	{
		printf("\nif you didn't use parameter, default value are:\n");
		printf("input file=GF2.img 60*60*60\n");
		printf("Please use: t3d inputfile N\nor\n");
		printf("Please use: t3d inputfile NX NY NZ\n");
	}

	if (argc > 1) strcpy(FileInput,argv[1]);//le a imagem do disco
	if (argc > 2) NX = NY = NZ = atoi (argv[2]);	/* cube */
	if (argc > 3) NY = NZ = atoi (argv[3]);
	if (argc > 4) NZ = atoi (argv[4]);

	strxfrm(FileOutput, FileInput, strlen(FileInput)-3);//formata nome arquivo
	strcat(FileOutput, ".ske");

	cout <<  FileInput <<  NX << "*" <<  NY << "*"  <<  NZ << "\n" ;
	ifstream infile(FileInput), infileA("tempA.dat"),infileB("tempB.dat"),infileC("tempC.dat");
	ofstream outfile(FileOutput);

	Image=Matrix3di(NX,NY,NZ);						//Aloca a imagem original
	Thin=Matrix3di(NX,NY,NZ);						//aloca imagem thining

//Reading Image
	cout <<"Reading Image ... ... ";
	for(i=0; i < NX; i++)
		for(j=0; j < NY; j++)
			for(k=0; k < NY; k++)
				infile >> Image[i][j][k];			//le imagem do disco
	cout << "Done." << endl;

//
//The first two and last two borders equal 1 in i direction, otherwise 0.
//Para colocar a fronteira,
//os dois primeiros planos e os dois ultimos
	for(i=0; i < NX; i++)
		for(j=0; j < NY; j++)
			Image[i][j][0]=Image[i][j][NZ-1]=Image[i][j][1]=Image[i][j][NZ-2]=0;

	for(i=0; i < NX; i++)
		for(k=0; k < NZ; k++)
			Image[i][0][k]=Image[i][NY-1][k]=Image[i][1][k]=Image[i][NY-2][k]=0;

	for(j=0; j < NY; j++)
		for(k=0; k < NZ; k++)
			Image[0][j][k]=Image[NX-1][j][k]=Image[1][j][k]=Image[NX-2][j][k]=1;

	for(i=0; i < NX; i++)
		for(j=0; j < NY; j++)
			for(k=0; k < NZ; k++)
				Thin[i][j][k]=Image[i][j][k];		//copia para thin

//To read the data of Template A
//Aqui le o arquivo tempA do disco
	kn=0;
	while(infileA)
	{
		for(ki=0; ki < 11; ki++)
		{
			infileA >> TempA[kn].x[ki];
			infileA >> TempA[kn].y[ki];
			infileA >> TempA[kn].z[ki];
			infileA >> TempA[kn].p[ki];
		}
		kn++;
	}

	NumTempA=kn-1;										//numeros de tempA-1

//To read the data of Template B
//le arquivo TempB do disco
	kn=0;
	while(infileB)
	{
		for(ki=0; ki < 11; ki++)
		{
			infileB >> TempB[kn].x[ki];
			infileB >> TempB[kn].y[ki];
			infileB >> TempB[kn].z[ki];
			infileB >> TempB[kn].p[ki];
		}
		kn++;
	}
	NumTempB=kn-1;											//numeros de tempB-1

//To read the data of Template C
	kn=0;
	while(infileC)
	{
		for(ki=0; ki < 12; ki++)
		{
			infileC >> TempC[kn].x[ki];
			infileC >> TempC[kn].y[ki];
			infileC >> TempC[kn].z[ki];
			infileC >> TempC[kn].p[ki];
		}
		kn++;
	}

	NumTempC=kn-1;

//The main Iteration
//Iteracao para calculo do thining
	cout << "Thinning ... ... " << endl;
	do
	{
	Judge=0;
	//To judge satisfy all the set T

	for(i=2; i < NX-2; i++)						//Percorre toda a imagem
		for(j=2; j < NY-2; j++)
			for(k=2; k < NZ-2; k++)
				if(Image[i][j][k] == 1)			//se for preto, verificar se os vizinhos satisfazem as condicoes de template
				if(TrueTempA() ||  TrueTempB() ||  TrueTempC() )//se satisfaz entra
														//as 4 linhas abaixo fazem as verificacoes listadas na pg 62
														//da tese Liang
				if(!TrueEdgePoint() )			//Verifica se e ponto de edge pg  , definicao 4.1
				if((Image[i+1][j][k] == 1 || (Image[i+1][j][k] == 0 && Image[i-2][j][k] == 1)))//verifica 3 condicoes
				if((Image[i][j+1][k] == 1 || (Image[i][j+1][k] == 0 && Image[i][j-2][k] == 1)))//
				if(Image[i][j][k+1] ==  1 || ((Image[i][j][k+1] == 0 && Image[i][j][k-2] == 1)))
				{
					Judge++;							//acumula
					Thin[i][j][k]=2;           //Marca o ponto como um ponto que satisfaz todas as condicoes acima
				}

	//Rule to preserve the points
	//ACima marcou os pontos
	//Agora vai verificar quais os pontos essenciais
	for(i=2; i < NX-2; i++)
		for(j=2; j < NY-2; j++)
			for(k=2; k < NZ-2; k++)				//percorre a imagem
			{
				if(Thin[i][j][k] == 2)			//se for ponto marcado
				{
					///Abaixo a rotina vai verificar quais os pontos dos planos
					//do cubo que nao sao necessarios.(Desnecessarios)
					//---------------
					//Plano 1
					//---------------
					if(Thin[i+1][j][k]==2)		//se os vizinhos forem marcados
					if(Thin[i][j][k+1]==2)
					if(Thin[i+1][j][k+1]==2)
					{
						Thin[i+1][j][k]=0;		//zera os vizinhos
						Thin[i][j][k+1]=0;
						Thin[i+1][j][k+1]=0;
						kk=0;
						for(ni=-1; ni <= 1; ni++)		//percorre os vizinhos
							for(nj=-1; nj <= 1; nj++)
								for(nk=-1; nk <= 1; nk++)
								{
									if(!(ni ==0 && nj ==0 && nk==0))	//exceto centro
										{									//Em torno do ponto de análise, verifica os pontos
																			//que devem ser testados.
										TempCube.x[kk]=ni;			//se for um ponto a ser testado
										TempCube.y[kk]=nj;         //inclue na estrutura TempCube
										TempCube.z[kk]=nk;         //
										TempCube.p[kk]=Thin[i+ni][j+nj][k+nk];

										if(TempCube.p[kk] != 0)		//se for diferente de 0
											TempCube.p[kk]=1;			//marca como sendo 1
										//cout << Cube[ni+1][nj+1][nk+1] << endl;
										kk++;
									}
						}
						JudgeBlack=Simple(1,26,3);					//aquichama a funcao de teste dos pontos incluidos
																			//na estrutura, vai testar somente os pontos pretos=1
																			//lá dentro da Simple vai rotular e testar
																			//Traduzindo a funcao Simple vai retornar o numero do cluster

						kk=0;
						for(ni=-1; ni <= 1; ni++)					//repete o procedimento acima
							for(nj=-1; nj <= 1; nj++)				//a diferenca e que elimina os pontos diagonais
								for(nk=-1; nk <= 1; nk++)
								{
									if(!(ni ==0 && nj ==0 && nk==0))//exceto o central
									if(!(abs(ni) ==1 && abs(nj) ==1 && abs(nk) ==1))//exceto os diagonais maiores
									{                                               //exceto a diagonal do cubo
										TempCube.x[kk]=ni;
										TempCube.y[kk]=nj;
										TempCube.z[kk]=nk;
										TempCube.p[kk]=Thin[i+ni][j+nj][k+nk];

										if(TempCube.p[kk] != 0)
											TempCube.p[kk]=1;
										//cout << Cube[ni+1][nj+1][nk+1] << endl;
										kk++;
									}

								}
						JudgeWhite=Simple(0,18,2);						//Verifica pontos brancos

						if( JudgeWhite ==1 && (Thin[i-1][j][k] == 0 ||Thin[i+1][j][k] == 0 ||
							Thin[i][j-1][k] ==0 || Thin[i][j+1][k] == 0 ||		//se o ponto foi juldago e aceito e
							Thin[i][j][k-1] == 0 || Thin[i][j][k+1] == 0))     //se os vizinhos sao 0
							JudgeWhite =1;													//entao e aceito
						else
							JudgeWhite =0;													//se um dos vizinhos nao e zero,
																								//nao e aceito

						Thin[i+1][j][k]=2;
						Thin[i][j][k+1]=2;
						Thin[i+1][j][k+1]=2;

						if(!(JudgeBlack == 1 && JudgeWhite ==1))
						{
							  Image[i][j][k]=2;
							  continue;
						}
					}

					//---------------
					//Plano 2
					//---------------
					if(Thin[i+1][j][k]==2)
					if(Thin[i][j+1][k]==2)
					if(Thin[i+1][j+1][k]==2)
					 {
						Thin[i+1][j][k]=0;
						Thin[i][j+1][k]=0;
						Thin[i+1][j+1][k]=0;
						kk=0;
						for(ni=-1; ni <= 1; ni++)
							for(nj=-1; nj <= 1; nj++)
								for(nk=-1; nk <= 1; nk++)
								{
									if(!(ni ==0 && nj ==0 && nk==0))
										{
										TempCube.x[kk]=ni;
										TempCube.y[kk]=nj;
										TempCube.z[kk]=nk;
										TempCube.p[kk]=Thin[i+ni][j+nj][k+nk];

										if(TempCube.p[kk] != 0)
											TempCube.p[kk]=1;
										kk++;
										}
								}

						JudgeBlack=Simple(1,26,3);

						kk=0;
						for(ni=-1; ni <= 1; ni++)
							for(nj=-1; nj <= 1; nj++)
								for(nk=-1; nk <= 1; nk++)
									{
									if(!(ni ==0 && nj ==0 && nk==0))
									if(!(abs(ni) ==1 && abs(nj) ==1 && abs(nk) ==1))
										{
										TempCube.x[kk]=ni;
										TempCube.y[kk]=nj;
										TempCube.z[kk]=nk;
										TempCube.p[kk]=Thin[i+ni][j+nj][k+nk];

										if(TempCube.p[kk] != 0)
											TempCube.p[kk]=1;
										//cout << Cube[ni+1][nj+1][nk+1] << endl;
										kk++;
										}

									}
					  JudgeWhite=Simple(0,18,2);

						if( JudgeWhite ==1 && (Thin[i-1][j][k] == 0 ||Thin[i+1][j][k] == 0 ||
							Thin[i][j-1][k] ==0 || Thin[i][j+1][k] == 0 ||
							Thin[i][j][k-1] == 0 || Thin[i][j][k+1] == 0))
								  JudgeWhite =1;
						else
								  JudgeWhite =0;

						Thin[i+1][j][k]=2;
						Thin[i][j+1][k]=2;
						Thin[i+1][j+1][k]=2;

						if(!(JudgeBlack == 1 && JudgeWhite ==1))
						{
							  Image[i][j][k]=2;
							  continue;
						}
					}

					//---------------
					//Plano 3
					//---------------
					if(Thin[i][j+1][k]==2)
					if(Thin[i][j][k+1]==2)
					if(Thin[i][j+1][k+1]==2)
					 {
						Thin[i][j+1][k]=0;
						Thin[i][j][k+1]=0;
						Thin[i][j+1][k+1]=0;
						kk=0;
						for(ni=-1; ni <= 1; ni++)
							for(nj=-1; nj <= 1; nj++)
								for(nk=-1; nk <= 1; nk++)
								{
									if(!(ni ==0 && nj ==0 && nk==0))
										{
										TempCube.x[kk]=ni;
										TempCube.y[kk]=nj;
										TempCube.z[kk]=nk;
										TempCube.p[kk]=Thin[i+ni][j+nj][k+nk];

										if(TempCube.p[kk] != 0)
											TempCube.p[kk]=1;
										kk++;
									}
								}

						JudgeBlack=Simple(1,26,3);
						kk=0;
						for(ni=-1; ni <= 1; ni++)
							for(nj=-1; nj <= 1; nj++)
								for(nk=-1; nk <= 1; nk++)
								{
									if(!(ni ==0 && nj ==0 && nk==0))
									if(!(abs(ni) ==1 && abs(nj) ==1 && abs(nk) ==1))
									{
										TempCube.x[kk]=ni;
										TempCube.y[kk]=nj;
										TempCube.z[kk]=nk;
										TempCube.p[kk]=Thin[i+ni][j+nj][k+nk];

										if(TempCube.p[kk] != 0)
											TempCube.p[kk]=1;
										//cout << Cube[ni+1][nj+1][nk+1] << endl;
										kk++;
									}
								}
					  JudgeWhite=Simple(0,18,2);

						if( JudgeWhite ==1 && (Thin[i-1][j][k] == 0 ||Thin[i+1][j][k] == 0 ||
							Thin[i][j-1][k] ==0 || Thin[i][j+1][k] == 0 ||
							Thin[i][j][k-1] == 0 || Thin[i][j][k+1] == 0))
							  JudgeWhite =1;
						else
							  JudgeWhite =0;

						Thin[i][j+1][k]=2;
						Thin[i][j][k+1]=2;
						Thin[i][j+1][k+1]=2;

						if(!(JudgeBlack == 1 && JudgeWhite ==1))
						{
							  Image[i][j][k]=2;
						}
					}
				}
				}

	//Show the numbers of black points NumeroPontosDeletados, points to be deleted NumeroPontosPretosImagem and points deleted NumeroMarcadosParaDeletar
	//Só para mostrar na tela os resultados dos pontos pretos na imagem, amrcados e deletados
	NumeroPontosPretosImagem=NumeroMarcadosParaDeletar=NumeroPontosDeletados=0;
	for(i=2; i < NX-2; i++)
		for(j=2; j < NY-2; j++)
			for(k=2; k < NZ-2; k++)
			{
				if(Image[i][j][k] > 0)
					NumeroPontosDeletados++;
				if(Thin[i][j][k] == 2)
					NumeroPontosPretosImagem++;
				if(Thin[i][j][k] == 2 && Image[i][j][k] != 2)
				{
					NumeroMarcadosParaDeletar++;
					Image[i][j][k]=0;
					}
				Thin[i][j][k]=Image[i][j][k];
			}

	cout << NumeroPontosDeletados << "\t" << NumeroPontosPretosImagem << "\t" << NumeroMarcadosParaDeletar << endl;


	}while(Judge != 0);

	cout << "Please wait a while. Thining more ... " << endl;
	int Nmin=26;
	for(i=2; i < NX-2; i++)									//Rotina para calcular o numero de vizinhos minimos
		for(j=2; j < NY-2; j++)								//LENTO
			for(k=2; k < NZ-2; k++)                   //percorre a imagem
				if(Thin[i][j][k] == 1)						//se for ponto preto (preto e do esqueleto)
				{	N=-Image[i][j][k];						//
					for(ni=-1; ni <= 1; ni++)           //percorre vizinhos
						for(nj=-1; nj <= 1; nj++)
							for(nk=-1; nk <= 1; nk++)
					N+=Image[i+ni][j+nj][k+nk];    		//calcula soma dos pontos 1 entre os vizinhos
					if(N < Nmin)								//se for menor que o minimo
						Nmin=N;									//define novo minimo (minimo e usado para determinar o numero)
				}													//minimo de pontos que garantem a conectividade
																	//

	do
	{
	//Indicating what points will be deleted
	Judge=0;
	for(i=2; i < NX-2; i++)									//depois de calculado o numero minimo de pontos
		for(j=2; j < NY-2; j++)								//verifica se este ponto tem numero de vizinho
			for(k=2; k < NZ-2; k++)							//igual ao minimo
				if(Image[i][j][k] == 1)
				  {
					N=-Image[i][j][k];
					for(ni=-1; ni <= 1; ni++)
						for(nj=-1; nj <= 1; nj++)
							for(nk=-1; nk <= 1; nk++)
					N+=Image[i+ni][j+nj][k+nk];
					if(N == (Nmin))                     //se igual
							Thin[i][j][k]=2;  //Judge++;	//marcar,e abaixo vai verificar todos os vizinhos 3D
				}

	//Verifying the point									//conferidos se os pontos satisfazem a regra do minimo
	for(i=2; i < NX-2; i++)									//percorro toda a imagem
		for(j=2; j < NY-2; j++)
			for(k=2; k < NZ-2; k++)
				if(Thin[i][j][k] == 2)						//aquitodos os pontos que podem vir a ser esqueleto
					{												//estao marcados
						kk=0;										//preciso pegar os vizinhos, corrigir
						for(ni=-1; ni <= 1; ni++)
							for(nj=-1; nj <= 1; nj++)
								for(nk=-1; nk <= 1; nk++)
									{
									if(!(ni ==0 && nj ==0 && nk==0))//exceto centro
									{
									TempCube.x[kk]=ni;					//pega os dados
									TempCube.y[kk]=nj;
									TempCube.z[kk]=nk;
									TempCube.p[kk]=Thin[i+ni][j+nj][k+nk];

									if(TempCube.p[kk] != 0)
										TempCube.p[kk]=1;
										kk++;
									}
						}

						JudgeBlack=Simple(1,26,3);			//retorna o numero de cluster do tipo 1 preto
																	//o numero 3 e a distancia maxima ao quadrado 26 vizinhos
						kk=0;
						for(ni=-1; ni <= 1; ni++)
							for(nj=-1; nj <= 1; nj++)
								for(nk=-1; nk <= 1; nk++)
									{
									if(!(ni ==0 && nj ==0 && nk==0))
									if(!(abs(ni) ==1 && abs(nj) ==1 && abs(nk) ==1))
										{
										TempCube.x[kk]=ni;
										TempCube.y[kk]=nj;
										TempCube.z[kk]=nk;
										TempCube.p[kk]=Thin[i+ni][j+nj][k+nk];

										if(TempCube.p[kk] != 0)
											TempCube.p[kk]=1;
										kk++;
										}
									}
						  JudgeWhite=Simple(0,18,2);			//retorna numero de cluster do tipo branco
																		//para branco a distancia maxima e 2, 18 vizinhos
						if( JudgeWhite ==1 && (Thin[i-1][j][k] == 0 ||Thin[i+1][j][k] == 0 ||
							Thin[i][j-1][k] ==0 || Thin[i][j+1][k] == 0 ||
							Thin[i][j][k-1] == 0 || Thin[i][j][k+1] == 0))
								  JudgeWhite =1;					//
						else
								  JudgeWhite =0;              //
						if((JudgeBlack == 1 && JudgeWhite ==1))
							{
							  Thin[i][j][k]=0;  Judge++;
							 }
						else
							Thin[i][j][k]=1;
					}

	//mostra na tela, o numero de pontos totais, e numero de dpontos deletados
	if(Judge != 0)
	{
	NumeroPontosPretosImagem=NumeroPontosDeletados=0;aqui
	 Nmin=26;
	for(i=2; i < NX-2; i++)
		for(j=2; j < NY-2; j++)
			for(k=2; k < NZ-2; k++)
			{
				if(Image[i][j][k] == 1)
					NumeroPontosPretosImagem++;

				Image[i][j][k]=Thin[i][j][k];

				if(Image[i][j][k] ==1)
				{
					NumeroPontosDeletados++;
					N=-Image[i][j][k];
					for(ni=-1; ni <= 1; ni++)
						for(nj=-1; nj <= 1; nj++)
							for(nk=-1; nk <= 1; nk++)
								N+=Image[i+ni][j+nj][k+nk];
					if(N < Nmin)
						Nmin=N;
				}
			}
	  }
	else
		Nmin++;
	}while(Nmin < 26);
	cout << "Done.\n";

	//Save skeleton in a file								//salva o esqueleto no disco
	cout << "Writing in the file... ... ";
	for(i=0; i < NX; i++)
	{
		for(j=0; j < NY; j++)
		{
			for(k=0; k < NZ; k++)
				outfile << Image[i][j][k] << " ";
			outfile << endl;
		}
		outfile << endl;
	}
		cout << "Done.\n";

	DeleteMatrix3di(Image);
	DeleteMatrix3di(Thin);

	system ("date");
		cout << "End all.\n";
}

//Calulating the number of componets (black or white)
//calcula o numero de componentes preto e brancos
int Simple(int sign,int M, int d)
{
	int n,kk,ki,dx,dy,dz,d2,Judge1;
	n=0;
	for(kk=0; kk < M; kk++)
		if(TempCube.p[kk] == sign)
			n++;

	if(n == 0)
		return(1);

	for(kk=0; kk < M; kk++)
	{
		if(TempCube.p[kk] == sign)
		{
			Judge1=1;
			TempCube.p[kk] =-1;
		}

		if(TempCube.p[kk] == -1)
			break;
	}

	for(kk=0; kk < M; kk++)
		if(TempCube.p[kk] == -1)
		{
			for(ki=0; ki < M; ki++)
			{
				if(TempCube.p[ki] == sign)
				{
					dx=(TempCube.x[kk]-TempCube.x[ki])*(TempCube.x[kk]-TempCube.x[ki]);
					dy=(TempCube.y[kk]-TempCube.y[ki])*(TempCube.y[kk]-TempCube.y[ki]);
					dz=(TempCube.z[kk]-TempCube.z[ki])*(TempCube.z[kk]-TempCube.z[ki]);
						  d2=dx+dy+dz;
					if(dx <=1 && dy <= 1 && dz <=1 && d2 <= d)
						TempCube.p[ki] =-1;
				}
			}
		}

	for(kk=0; kk < M; kk++)
		if(TempCube.p[kk] == sign)
			return(0);
  return(Judge1);
}

//verifica se e ponto edge válido
int TrueEdgePoint()
{

	int ni,nj,nk,N,Nx,Ny,Nz;

	N=-Image[i][j][k];
	for(ni=-1; ni <= 1; ni++)
		for(nj=-1; nj <= 1; nj++)
			for(nk=-1; nk <= 1; nk++)
					N+=Image[i+ni][j+nj][k+nk];

	Nx=-Image[i][j][k];
	for(nj=-1; nj <= 1; nj++)
		for(nk=-1; nk <= 1; nk++)
				Nx+=Image[i][j+nj][k+nk];

	Ny=-Image[i][j][k];
	for(ni=-1; ni <= 1; ni++)
		for(nk=-1; nk <= 1; nk++)
				Ny+=Image[i+ni][j][k+nk];

	Nz=-Image[i][j][k];
	for(ni=-1; ni <= 1; ni++)
		for(nj=-1; nj <= 1; nj++)
				Nz+=Image[i+ni][j+nj][k];

	if(N == 1 || (Nx == 1 && Ny ==1 && Nz ==1) )
		return(1);
	else
		return(0);
}
//verifica se os pontos satisfazem a condicao do templateA
int TrueTempA()
{
	int     kn,ki, Condition;
	for(kn=0; kn <  NumTempA; kn++)
	{
		Condition=0;
		for(ki=0; ki < 11; ki++)
		{
			if(Image[i+TempA[kn].x[ki]][j+TempA[kn].y[ki]][k+TempA[kn].z[ki]] == TempA[kn].p[ki])
				Condition++;
		}
		if(Condition ==11)
			break;
	}

	if(Condition ==11)
		return(1);
	else
		return(0);
}

//verifica se os pontos satisfazem a condicao do templateB
int TrueTempB()
{
	int     kn,ki,Condition;
  for(kn=0; kn <  NumTempB; kn++)
	{
		Condition=0;
		for(ki=0; ki < 8; ki++)
		{
			if(Image[i+TempB[kn].x[ki]][j+TempB[kn].y[ki]][k+TempB[kn].z[ki]] == TempB[kn].p[ki])
				Condition++;
		}

		if(!(Image[i+TempB[kn].x[8]][j+TempB[kn].y[8]][k+TempB[kn].z[8]] == 1 && Image[i+TempB[kn].x[9]][j+TempB[kn].y[9]][k+TempB[kn].z[9]] == 1 && Image[i+TempB[kn].x[10]][j+TempB[kn].y[10]][k+TempB[kn].z[10]] == 1))
			Condition++;

		if(Condition == 9)
			break;
	}
	if(Condition == 9)
		return(1);
	else
		return(0);
}

//verifica se os pontos satisfazem a condicao do templateC
int TrueTempC()
{
	int     kn,ki, Condition;
	for(kn=0; kn < NumTempC ; kn++)
	{
		Condition=0;
		for(ki=0; ki < 8; ki++)
		{
			if(Image[i+TempC[kn].x[ki]][j+TempC[kn].y[ki]][k+TempC[kn].z[ki]] == TempC[kn].p[ki])
				Condition++;
		}
		if(!(Image[i+TempC[kn].x[8]][j+TempC[kn].y[8]][k+TempC[kn].z[8]] == 1 && Image[i+TempC[kn].x[9]][j+TempC[kn].y[9]][k+TempC[kn].z[9]] == 1))
			Condition++;

		if(!(Image[i+TempC[kn].x[10]][j+TempC[kn].y[10]][k+TempC[kn].z[10]] == 1 && Image[i+TempC[kn].x[11]][j+TempC[kn].y[11]][k+TempC[kn].z[11]] == 1))
			Condition++;

		if(Condition == 10)
			break;
	}

	if(Condition == 10)
		return(1);
	else
		return(0);
}

