// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CBCEuclidiana_h
#define CBCEuclidiana_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCEuclidiana.h
Nome da classe:      CBCEuclidiana
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao Classe:		CBCEuclidiana
==================================================================================
Nome Classe:           CBCEuclidiana
Assunto:               Mascaras e filtros
Superclasse:           CMascara
Descrição:             Implementa as funções da classe CBCEuclidiana.
                       A CBCEuclidiana é uma matriz bidimensional cujos valores,
                       são 0 se a distância do ponto x,y for maior que o raio, e
                       1 se menor.
                       A função PreencheMascara calcula a distância de cada ponto x,y
                       e determina se data2D[x][y]=0 ou 1.
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
*/
// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
// #include "CMascara\CMascara.h"
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdij.h>


// ----------------------------------------------------------------------------
// Classe:       CBCEuclidiana
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando métrica dij.
 * E' derivada da classe base CBCdij.
 */
class CBCEuclidiana:public CBCdij	//  public CMascara
{
// Metodos
public:

    /// Construtor, chama PreencheMascara
    CBCEuclidiana (unsigned int tm);

    /// Destrutor
    virtual ~ CBCEuclidiana ()
    {
    }

    //   virtual unsigned int CalculaRaioBola();
};
#endif
