
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMGaussiano.cpp
Nome da classe:      CMGaussiano
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//   ----------------------------------------------------------------------------
//                         BIBLIOTECAS
//   ----------------------------------------------------------------------------
#include <Mascara/CMGaussiano.h>
//   #include <Matriz/fatorial.cpp>

/*

==================================================================================
Documentacao      PreencheMascara
==================================================================================
Descrição:      Preenche a mascara com os valores adequados.
		Código adaptado para melhor performance, percorre só metade da mascara, usa simetria
		calcula valores 1,4,6 para mascara  5*5

//   Para uma Mascara 5*5, os valores sao dados por
//   	1	4	6	4	1
//   	4	16	24	16	4
//   	6	24	36	24	6
//   	4	16	24	16	4
//    1  4  6  4  1
Programador:      Andre Duarte Bueno
*/

void
CMGaussiano::PreencheMascara ()
{
  unsigned int i, j;
  double *fat = new double[ NX () ];	//   Cria vetor com valores de fatorias de i
  double acumulado = 1;
  fat[0] = 1.0;
  for (i = 1; i < NX (); i++)	//   Calcula os fatorias e armazena em fat[i]
    {
      acumulado *= i;           // USAR CFatorial ?????
      fat[i] = acumulado;
    }

  int *v = new int [RaioX () + 1];	//   metade da mascara  (5-1)/2=2  +1=3  0,1,2
  
  //   calcula fatores para primeira linha
  float ftemp = fat[NX () - 1];	//   Pega fatorial de nx-1,pois a matriz comeca do zero
  for (i = 0; i < RaioX () + 1; i++)	//   preenche vetor temporario v
    v[i] = ftemp / (fat[i] * fat[NX () - 1 - i]);

  int t = NX () - 1;		//   Para mascara 5*5 t=4, -1 pois começa do zero
  for (i = 0; i < RaioX () + 1; i++)	//   calcula valores da mascara
    for (j = 0; j < RaioX () + 1; j++)	//   RaioX=RaioY
      data2D[i][j] = data2D[i][t-j]=data2D[t-i][j] =data2D[t-i][t-j] = v[i]*v[j];

  delete [] v;			//   apaga vetores temporarios
  delete [] fat;
  CalculaPeso ();		//   calcula o peso da mascara e armazena no atributo peso
}

/*
Código normal
int i,j;

float *fat=new float [NX()];     //   Cria vetor com valores de fatorias de i
float acumulado=1;
fat[0]=1;
for( i=1; i<NX() ; i++)       	//   Calcula os fatorias e armazena em fat[i]
     {
     acumulado*=i;
     fat[i]=acumulado;
     }

float* v= new float[NX()];			//   cria vetor para NX elementos
                                    //   calcula fatores para primeira linha
float ftemp=fat[NX()-1];   		//   Pega fatorial de nx-1,pois a matriz comeca do zero
for( i=0; i<NX() ; i++)       	//   v[i]= NX()-1!  /  (   i! * (NX()-1-i)!  );
     v[i]= ftemp  /  (   fat[i] * fat[NX()-1-i]  );

for( i=0; i<NX() ; i++)       	//   calcula valores da mascara
 for( j=0; j<NY() ; j++)
       {
       data2D[i][j]=v[i]*v[j];
       }
delete []v;
delete []fat;
CalculaPeso();            			   //   calcula o peso da mascara e armazena no atributo peso
*/
/*
float fact(int n)
{   return  (n>1)? n*fact(n-1):1;	}
*/

