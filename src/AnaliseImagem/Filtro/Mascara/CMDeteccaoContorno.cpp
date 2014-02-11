
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMDeteccaoContorno.cpp
Nome da classe:      CMDeteccaoContorno
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//   ----------------------------------------------------------------------------
//                         BIBLIOTECAS
//   ----------------------------------------------------------------------------
#include <AnaliseImagem/Filtro/Mascara/CMDeteccaoContorno.h>

/*
==================================================================================
Documentacao      PreencheMascara
==================================================================================
Descrição:        Preenche a mascara com os valores adequados
Para mascara 3*3 obtem-se
	1	1	1
	1      -8       1
	1	1	1
Programador:      Andre Duarte Bueno
*/
void
CMDeteccaoContorno::PreencheMascara ()
{
  for (unsigned int i = 0; i < NX (); i++)
    for (unsigned int j = 0; j < NY (); j++)
      data2D[i][j] = 1;

  data2D[RaioX ()][RaioY ()] = -(int) NX () * NY () + 1;

//   Nao precisa calcular o peso, pois o peso=0, assume então peso=1
//   CalculaPeso();
}
