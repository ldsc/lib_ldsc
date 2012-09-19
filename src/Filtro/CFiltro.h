// ítens conferidos: 1[] 2[] 3[] 4[] 5[] 6[] 7[] 8[] 9[] 10[]
#ifndef CFiltro_h
#define CFiltro_h

// ponteiro para matriz
#include <Matriz/TCMatriz2D.h>

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez,
Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFiltro.h
Nome da classe:      CFiltro
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief            Classe básica para os filtros.
 * Representa a superclasse para os filtros que podem atuar sobre
 * o espaço discreto da imagem (filtros espaciais) ou filtros que atuam
 * sobre o espaço de cor das imagens (filtros de amplitude).
 * Tem um atributo básico que é um ponteiro para a imagem (matriz imagem)
 * a ser tratada.
 * Ou seja um filtro tem um ponteiro para a imagem podendo atuar sobre éla.
 * Cardinalidade:    1:1    ,de uma maneira geral usa-se um filtro para atuar sobre uma imagem
 *
 * Tarefas de um filtro:
 * -Funcao CanGo () informa se a operacao pode ser realizada
 * -Funcao Go realiza a filtragem da imagem
*/

class CFiltro
{
protected:

    TCMatriz2D< int > * pm;	///< é um ponteiro para a imagem a ser filtrada

public:
/// Construtor, recebe o endereço da matriz a ser processada
    CFiltro (TCMatriz2D< int > * &matriz)
    {
        pm = matriz;
    }

    /// Destrutor
    ~CFiltro ()
    {
    }

    /**
     * @brief Realiza o processamento da filtragem
     * @param matriz ponteiro para imagem a ser processada
     * @param _tamanhoMascara  dimensão da mascara a ser utilizada
     * @return ponteiro para imagem processada
     */
    virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara = 0) = 0;
};

#endif //   CFiltro_h
