/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEspacial.cpp
Nome da classe:      CFEspacial
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
	 BIBLIOTECAS
----------------------------------------------------------------------------
*/
#include "Filtro/FEspacial/CFEspacial.h"

#include "Mascara/CMPassaBaixa.h"	// O filtro espacial atua sobre a mascara do passa baixa todos os valores=1 Outros filtros usam outras mascaras.

/*
==================================================================================
Documentacao      CriaMascara
==================================================================================
Descrição: 	Se a mascara ja existe deve ser eliminada
Programador:    Andre Duarte Bueno
*/
template<typename T>
void CFEspacial<T>::CriaMascara (unsigned int _tamanhoMascara) {
	if (mask)
		delete mask;
	mask = new CMPassaBaixa (_tamanhoMascara);	// CMPAssaBaixa é a default, faz a média
}

/*
==================================================================================
Documentacao      Go
==================================================================================
Descrição: Deve percorrer a imagem somente onde a mascara cabe // depois mudar
Programador:      Andre Duarte Bueno
*/

/*TCMatriz2D< int > *CFEspacial::Go( TCMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
{
 tamanhoMascara=_tamanhoMascara;
	 return   Go(matriz);
} */
template<typename T>
TCMatriz2D<T> * CFEspacial<T>::Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara) {
	tamanhoMascara = _tamanhoMascara;
	CriaMascara (tamanhoMascara);	// Cria a mascara adequada

	// A funcao de preenchimento da mascara já calculada o peso da mascara
	CFiltro<T>::pm = matriz;
	TCMatriz2D< T > *rImg = new TCMatriz2D< T > (*CFiltro<T>::pm);	// rImg é uma matriz cópia da matriz pm passada

	unsigned int i, j, k, l;			// variáveis auxiliares

	int raioMascaraX = mask->RaioX ();		// temporaria aumenta performance
	int raioMascaraY = mask->RaioY ();
	float soma;

	// Percorre a matriz imagem, exceto a borda
	for (i = (raioMascaraX); i < (CFiltro<T>::pm->NX () - raioMascaraX); i++) {
		for (j = (raioMascaraY); j < (CFiltro<T>::pm->NY () - raioMascaraY); j++) {
			soma = 0.0;
			for (k = 0; k < mask->NX (); k++) {	// percorre a mascara
				for (l = 0; l < mask->NY (); l++) {	// realiza convolução da mascara com a imagem
					soma += mask->data2D[k][l] * rImg->data2D[i + k - raioMascaraX][j + l - raioMascaraY];
				}
			}
			// devo garantir que nao seja valor negativo
			// (cor nao realizavel)
			// o fator de peso da mascara ou é 1 ou
			// um numero maior que 1
			if (soma > 0)
				CFiltro<T>::pm->data2D[i][j] = soma / mask->Peso ();
			else
				CFiltro<T>::pm->data2D[i][j] = 0;
		}
	}
	delete rImg;  // deleta objeto matriz auxiliar
	return CFiltro<T>::pm;
}

