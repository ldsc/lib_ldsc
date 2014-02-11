
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMPassaAlta.cpp
Nome da classe:      CMPassaAlta
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//   ----------------------------------------------------------------------------
//                         BIBLIOTECAS
//   ----------------------------------------------------------------------------
#include <AnaliseImagem/Filtro/Mascara/CMPassaAlta.h>

/*
==================================================================================
Documentacao      PreencheMascara
==================================================================================
Descrição:        Preenche a mascara com os valores adequados
Para mascara 3*3 obtem-se
	-1	-1	-1
	-1	 9	-1
	-1	-1	-1

Para mascara 5*5 obtem-se
	-1	-1	-1 -1 -1
	-1	-1	-1 -1 -1
	-1	-1      25 -1 -1
	-1	-1	-1 -1 -1
	-1	-1	-1 -1 -1

Programador:      Andre Duarte Bueno
*/
void
CMPassaAlta::PreencheMascara ()
{
  for (unsigned int i = 0; i < NX (); i++)
    for (unsigned int j = 0; j < NY (); j++)
      data2D[i][j] = -1;

  data2D[RaioX ()][RaioY ()] = NX () * NY ();	//   3*3=9 Conforme Corel 7.0
  //   data2D[RaioX()][RaioY()]=NX()*NY()-1;             //   3*3-1=8

//   Nao precisa calcular o peso, pois o peso=0
//   CalculaPeso();
}

/*
//   A mascara era preenchida com os valores abaixo.
Nao tenho esta mascara, peguei os valores do imago.
void CMPassaAlta::PreencheMascara()
{
for (int i=0; i<NX(); i++)
 for(int j=0; j<NY(); j++)
 {
  if( i==NX()/2 && i==j )
    data2D[i][j]=8;
  else
    data2D[i][j]=-1;
         }
} */
