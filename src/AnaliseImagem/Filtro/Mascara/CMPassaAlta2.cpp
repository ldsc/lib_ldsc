
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMPassaAlta2.cpp
Nome da classe:      CMPassaAlta2
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//   ----------------------------------------------------------------------------
//                         BIBLIOTECAS
//   ----------------------------------------------------------------------------
#include <AnaliseImagem/Filtro/Mascara/CMPassaAlta2.h>

/*
==================================================================================
Documentacao      PreencheMascara
==================================================================================
Descrição:        Preenche a mascara com os valores adequados
Para mascara 3*3 obtem-se
//    0  -1   0
//   -1   5  -1
//    0  -1   0
Para mascara 5*5 obtem-se
//    0  0 -1   0   0
//    0  0 -1   0   0
//   -1 -1  9  -1  -1
//    0  0 -1   0   0
//    0  0 -1   0   0

Programador:      Andre Duarte Bueno
*/
void
CMPassaAlta2::PreencheMascara ()
{
  for (unsigned int i = 0; i < NX (); i++)
    for (unsigned int j = 0; j < NY (); j++)
      {
	if ((i == j) && (i == RaioX ()))
	  data2D[i][j] = NX () + NY () - 1;	//   para mascara 3*3,  3+3-1=5;
	else if (i == RaioX () || j == RaioY ())
	  data2D[i][j] = -1;
	else
	  data2D[i][j] = 0;
      }
//   Nao precisa calcular o peso, pois o peso=1,
//   e foi assim definido no construtor da classe CMascara
//   CalculaPeso();
}
