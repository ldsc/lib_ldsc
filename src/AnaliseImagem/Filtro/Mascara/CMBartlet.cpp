
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMBartlet.cpp
Nome da classe:      CMBartlet
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
----------------------------------------------------------------------------
			BLIOTECAS
----------------------------------------------------------------------------
*/

#include <AnaliseImagem/Filtro/Mascara/CMBartlet.h>
/*
==================================================================================
Documentacao      PreencheMascara
==================================================================================
Descrição:	Preenche a mascara com os valores adequados
		Código adaptado para melhor performance, percorre só metade da mascara, usa simetria
//   Para uma Mascara 5*5, os valores sao dados por
//   	1	2	3	2	1         
//   	2	4	6	4	2
//   	3	6	9	6	3
//   	2	4	6	4	2
//   	1	2	3	2	1

Programador:      Andre Duarte Bueno
*/

void
CMBartlet::PreencheMascara ()
{
  int *v = new int [RaioX () + 1];	//   metade da mascara  5/2=3  0,1,2

  for (unsigned int ii = 0; ii < RaioX () + 1; ii++)	//   preenche vetor temporario v
    v[ii] = ii + 1;

  int t = NX () - 1;		//   tamanho da mascara -1 (comeca do zero)
  for (unsigned int i = 0; i < RaioX () + 1; i++)	//   calcula valores da mascara
    for (unsigned int j = 0; j < RaioX () + 1; j++)	//   a mascara é quadrada  RaioY=RaioX
      data2D[i][j] = data2D[i][t - j] = data2D[t - i][j] =data2D[t - i][t - j] = v[i] * v[j];
  delete [] v;
  //   calcula o peso da mascara e armazena
  CalculaPeso ();		//   no atributo peso
}

/*
//   codigo normal
int* v= new int [NX()];

for(int i=0; i < NX() ; i++)
     v[i]= i+1;

int t=NX()-1;
for(int i=0; i<NX() ; i++)
 for(int j=0; j<NY() ; j++)
       data2D[i][j]=v[i]*v[j];
delete []v;
CalculaPeso();
*/
