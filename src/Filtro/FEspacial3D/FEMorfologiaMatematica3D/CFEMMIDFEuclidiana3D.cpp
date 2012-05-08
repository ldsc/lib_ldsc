// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFEuclidiana3D.cpp
Nome da classe:      CFEMMIDFEuclidiana3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a função CriaMascara da classe CFEMMIDFEuclidiana3D.
*/
// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMIDFEuclidiana3D.h" // Classe base

#include "Geometria/Bola/BCDiscreta3D/CBCEuclidiana3D.h"

/*
==================================================================================
Documentacao 		CriaMascara
==================================================================================
Descrição:      Funcao que cria a mascara de chanfro adequada.
*/

void CFEMMIDFEuclidiana3D::CriaMascara (unsigned int _tamanhoMascara) {
  if (mask)
    delete mask;
  mask = new CBCEuclidiana3D (_tamanhoMascara);	//  valores mi,mj,rb definidos pelo construtor de CBCEuclidiana
}
