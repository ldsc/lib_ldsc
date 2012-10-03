/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
   Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEPassaBaixa.cpp
Nome da classe:      TCFEPassaBaixa
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
----------------------------------------------------------------------------
   BIBLIOTECAS
----------------------------------------------------------------------------
*/
#ifndef TCFEPassaBaixa_h
#include "Filtro/FEspacial/TCFEPassaBaixa.h"
#endif

#include "Mascara/CMPassaBaixa.h"

/*
==================================================================================
Documentacao      CriaMascara
==================================================================================
Descrição:	Se a mascara ja existe deve ser eliminada o construtor da mascara chama
a funcao de preenchimento da mascara e de calculo do peso da mascara
Programador:    Andre Duarte Bueno
*/
template<typename T>
void TCFEPassaBaixa<T>::CriaMascara (unsigned int _tamanhoMascara) {
	 this->tamanhoMascara = _tamanhoMascara;
	 if (this->mask)
			delete this->mask;
	 this->mask = new CMPassaBaixa (_tamanhoMascara);
}
