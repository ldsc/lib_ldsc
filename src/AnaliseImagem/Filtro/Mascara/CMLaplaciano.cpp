
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMLaplaciano.cpp
Nome da classe:      CMLaplaciano
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
*/
#include <AnaliseImagem/Filtro/Mascara/CMLaplaciano.h>

/*
==================================================================================
Documentacao      PreencheMascara
==================================================================================
Descrição:
		O filtro laplaciano pode ser obtido subtraindo-se da imagem original
		a imagem original filtrada com um filtro de passa alta
		laplaciano(img) = img - passaAlta(img)

		Aqui a mascara é implementada considerando um tamanho padrao 3*3
		Para tamanhos de mascara diferentes, alterar o codigo, deixar generico
Programador:      Andre Duarte Bueno
0  1  0
1 -4  1
0  1  0
*/

void
CMLaplaciano::PreencheMascara ()
{
  for (unsigned int i = 0; i < NX (); i++)
    for (unsigned int j = 0; j < NY (); j++)
      {
	if ((i == j) && (i == RaioX ()))
	  data2D[i][j] = -4;
	else if (i == RaioX () || j == RaioY ())
	  data2D[i][j] = +1;
	else
	  data2D[i][j] = 0;
      }
//   Nao precisa calcular o peso, pois o peso=0
//   CalculaPeso();
}
