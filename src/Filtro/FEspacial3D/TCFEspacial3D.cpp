/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEspacial3D.cpp
Nome da classe:      TCFEspacial3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
 */

#ifndef TCFEspacial3D_h
#include "Filtro/FEspacial3D/TCFEspacial3D.h"
#endif

#include "Mascara/CMPassaBaixa3D.h"	// O filtro espacial atua sobre a mascara   

// do passa baixa todos os valores=1	// outros filtros usam outras mascaras.
/*
==================================================================================
Documentacao      CriaMascara
==================================================================================
Descrição:
				Se a mascara ja existe deve ser eliminada

Programador:      Andre Duarte Bueno
*/
template <typename T>
void TCFEspacial3D<T>::CriaMascara (unsigned int _tamanhoMascara) {
	if (mask)
		delete mask;				// Apaga objeto mask
	mask = new CMPassaBaixa3D (_tamanhoMascara);	// TMPAssaBaixa é a default, faz a média
}

/*
==================================================================================
Documentacao      Go
==================================================================================
Descrição:
				Deve percorrer a imagem somente onde a mascara cabe // depois mudar

Programador:      Andre Duarte Bueno
*/

/*TCMatriz3D<int> *TCFEspacial3D::Go( TCMatriz3D<int> *& matriz, unsigned int _tamanhoMascara )
{
	tamanhoMascara=_tamanhoMascara;
	 return   Go(matriz);
} */
template <typename T>
TCMatriz3D<T> * TCFEspacial3D<T>::Go (TCMatriz3D<T> * &matriz, unsigned int _tamanhoMascara) {
	CriaMascara (tamanhoMascara);	// Cria a mascara adequada
	// A funcao de preenchimento da mascara
	// já calculada o peso da mascara
	this->pm = matriz;

	TCMatriz3D<T> *rImg = new TCMatriz3D<T> (*this->pm);	// rImg é uma matriz cópia da matriz pm passada

	unsigned int i, j, k;		// percorre imagem

	unsigned int ii, jj, kk;	// percorre mascara

	int raioMascaraX = mask->RaioX ();	// temporaria aumenta performance

	int raioMascaraY = mask->RaioY ();

	int raioMascaraZ = mask->RaioZ ();

	float soma;

	// Percorre a matriz imagem, exceto a borda
	for (i = (raioMascaraX); i < (this->pm->NX () - raioMascaraX); i++)
		for (j = (raioMascaraY); j < (this->pm->NY () - raioMascaraY); j++)
			for (k = (raioMascaraZ); k < (this->pm->NZ () - raioMascaraZ); k++) {
				soma = 0.0;
				for (ii = 0; ii < mask->NX (); ii++)	// percorre a mascara
					for (jj = 0; jj < mask->NY (); jj++)
						for (kk = 0; kk < mask->NZ (); kk++) {		// realiza convolução da mascara com a imagem
							soma += mask->data3D[ii][jj][kk] * rImg->data3D[i + ii - raioMascaraX][j + jj - raioMascaraY][k + kk - raioMascaraZ];
						}
				// devo garantir que nao seja valor negativo
				// (cor nao realizavel)
				// o fator de peso da mascara ou é 1 ou
				// um numero maior que 1
				if (soma > 0)
					this->pm->data3D[i][j][k] = soma / mask->Peso ();
				else
					this->pm->data3D[i][j][k] = 0;
			}
	delete rImg;				// Apaga objeto rImg
	return this->pm;
}
