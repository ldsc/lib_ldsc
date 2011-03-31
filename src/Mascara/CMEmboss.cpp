
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMEmboss.cpp
Nome da classe:      CMEmboss
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//   ----------------------------------------------------------------------------
//                          BIBLIOTECAS
//   ----------------------------------------------------------------------------
#include <Mascara/CMEmboss.h>

/*

==================================================================================
Documentacao      PreencheMascara
==================================================================================
Descrição:        Preenche a mascara com os valores adequados
Programador:      Andre Duarte Bueno
3*3

raio=(3-1)/2=1

//          0       1   2  <-Posicao na matriz
//   	-1	 0   1
//   	-1	 1   1
//   	-1	 0   1

*/

void
CMEmboss::PreencheMascara ()
{
  for (unsigned int i = 0; i < NX (); i++)
    for (unsigned int j = 0; j < NY (); j++)
      {
	if (i < RaioX ())
	  data2D[i][j] = -1;
	else if (i > RaioX ())
	  data2D[i][j] = 1;
	else
	  data2D[i][j] = 0;
      }
  data2D[RaioX ()][RaioY ()] = 1;	//   Coloca o 1 central
  //   o peso=1, não precisa calcular
//   CalculaPeso();                       //   calcula o peso da mascara e armazena no atributo peso
}
