

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMPassaBaixa3D.cpp
Nome da classe:      CMPassaBaixa3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
----------------------------------------------------------------------------
BIBLIOTECAS
----------------------------------------------------------------------------
*/
#include <AnaliseImagem/Filtro/Mascara/CMPassaBaixa3D.h>
//         1       1       1
//         1       1       1
//         1       1       1

/*

==================================================================================
Documentacao      PreencheMascara
==================================================================================
Descrição:        Preenche a mascara com os valores adequados
		  Para a mascara passa baixa, todos os valores são unitários
Programador:      Andre Duarte Bueno
*/

void
CMPassaBaixa3D::PreencheMascara ()
{
  for (unsigned int i = 0; i < NX (); i++)
    for (unsigned int j = 0; j < NY (); j++)
      for (unsigned int k = 0; k < NZ (); k++)
	data3D[i][j][k] = 1;
  //    CalculaPeso ();		//   calcula o peso da mascara e
  peso = NX()*NY()* NZ(); //   armazena no atributo peso
}
