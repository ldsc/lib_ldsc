// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEPassaAlta.cpp
Nome da classe:      TCFEPassaAlta
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
*/
#ifndef TCFEPassaAlta_h
#include "AnaliseImagem/Filtro/FEspacial/TCFEPassaAlta.h"
#endif
#include "AnaliseImagem/Filtro/Mascara/CMPassaAlta.h"

/*
==================================================================================
Documentacao      CriaMascara
==================================================================================
Descrição:	Se a mascara ja existe deve ser eliminada
Programador:      Andre Duarte Bueno
*/
template<typename T>
void TCFEPassaAlta<T>::CriaMascara (unsigned int _tamanhoMascara) {
	this->tamanhoMascara = _tamanhoMascara;
	if (this->mask)
		delete this->mask;
	this->mask = new CMPassaAlta (_tamanhoMascara);
}

