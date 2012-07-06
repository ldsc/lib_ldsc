
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
   Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMPassaBaixa.cpp
Nome da classe:      CMPassaBaixa
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
----------------------------------------------------------------------------
   BIBLIOTECAS
----------------------------------------------------------------------------
*/
#include <Mascara/CMPassaBaixa.h>
/*
==================================================================================
Documentacao      PreencheMascara
==================================================================================
Descrição:      Preenche a mascara com os valores adequados
  Para a mascara passa baixa, todos os valores são unitários
Programador:    Andre Duarte Bueno
*/
// 1  1  1
// 1  1  1
// 1  1  1
void CMPassaBaixa::PreencheMascara () {
   for (unsigned int i = 0; i < NX (); i++)
      for (unsigned int j = 0; j < NY (); j++)
         data2D[i][j] = 1;
   peso = NX() * NY();  //   armazena no atributo peso
}
