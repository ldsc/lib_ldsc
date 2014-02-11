// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CBCEuclidiana3D_h
#define CBCEuclidiana3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCEuclidiana3D.h
Nome da classe:      CBCEuclidiana3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:		CBCEuclidiana3D
==================================================================================
Nome Classe:           CBCEuclidiana3D
Assunto:               Mascaras e filtros
Superclasse:           CMascara
Descrição:             Implementa as funções da classe CBCEuclidiana3D.
                       A CBCEuclidiana3D é uma matriz bidimensional cujos valores,
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
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdijk3D.h>

// ----------------------------------------------------------------------------
// Classe:       CBCEuclidiana3D
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando métrica pseudo euclidiana.
 * É derivada da classe base CBCdijk3D.
 */

class CBCEuclidiana3D : public CBCdijk3D	//  public CMascara
{

// Metodos
public:

    /// Construtor, chama PreencheMascara
    CBCEuclidiana3D (unsigned int tamanhoMascara);

    /// Destrutor
    virtual ~ CBCEuclidiana3D ()
    {
    }
    // Métodos virtuais herdados

protected:

    /// Calcula o raio da bola
    virtual void CalculaRaioBola ();

};
#endif
