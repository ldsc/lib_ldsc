
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMascara.cpp
Nome da classe:      CMascara
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//   ----------------------------------------------------------------------------
//                         BIBLIOTECAS
//   ----------------------------------------------------------------------------
#include <Mascara/CMascara.h>

/*
==================================================================================
Documentacao      Construtor .
==================================================================================
Descrição:        Passa atributo tamanhoMascara para classe base TMatriz2D< int >,
		observe que o tamanhoMascara é passado para as dimensoes x e y
				da matriz.
				Define-se ainda o peso da mascara como sendo 1.
				Em algumas mascaras o peso=1, nao sendo necessário chamar a funcao
				de calculo do peso.
Programador:      Andre Duarte Bueno
*/
CMascara::CMascara (unsigned int tamanhoMascara)
        : TMatriz2D< int > (tamanhoMascara, tamanhoMascara),  peso (1.0)
{

    //   As herdeiras da CMascara devem preencher a mascara chamando a funcao
    //   NomeClasse::PreencheMascara(); nos seus construtores
}

/*
==================================================================================
Documentacao      Destrutor
==================================================================================
Descrição:
Programador:      Andre Duarte Bueno
*/
CMascara::~CMascara ()
{
}

//   Todo construtor deve chamar a preenche mascara
//   da propria classe
/*
void CMascara::PreencheMascara()
{
//   Cte(0);	//   deve preencher a mascara
}
*/

/*
==================================================================================
Documentacao      CalculaPeso
==================================================================================
Descrição:
		Existem dois tipos de mascaras, uma com peso=1 e outras com peso>1
		Se a mascara tiver peso maior que 1 esta funcao deve ser chamada
		Ou o peso deve ser calculado diretamente na funcao de preenchimento da mascara
Programador:   Andre Duarte Bueno
*/
float CMascara::CalculaPeso () {
    peso = 0;
    for (unsigned int i = 0; i < NX (); i++) {	//   percorre a mascara
        for (unsigned int j = 0; j < NY (); j++) {
            peso += data2D[i][j];	//   calcula peso acumulado
        }
    }
    if (peso == 0) {
        peso = 1;			//   o peso é utilizado no filtro numa divisao
	}
    return peso;			//   e nao pode assumir o valor 0
}
