
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMRotacaoVerticaleDiferenca.cpp
Nome da classe:         CMRotacaoVerticaleDiferenca
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//   ----------------------------------------------------------------------------
//                         BIBLIOTECAS
//   ----------------------------------------------------------------------------
#include <Mascara/CMRotacaoVerticaleDiferenca.h>

/*
==================================================================================
Documentacao      PreencheMascara
==================================================================================
Descrição:        Preenche a mascara com os valores adequados
                  Abaixo o exemplo para uma mascara 3*3
Programador:      Andre Duarte Bueno
//   	0      -1	0
//   	0	1	0
//   	0	0	0
*/

void
CMRotacaoVerticaleDiferenca::PreencheMascara ()
{
  Constante (0);		//   chama função de preenchimento com valor constante
  data2D[RaioX ()][RaioY ()] = 1;
  if (RaioY () > 0)
    data2D[RaioX ()][RaioY () - 1] = -1;
  peso = 1;			//   o peso é colocado como sendo=1
}				//   se o peso=0 causa divisao por zero no filtro espacial
