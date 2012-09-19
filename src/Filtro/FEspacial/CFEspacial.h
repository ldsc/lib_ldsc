#ifndef CFEspacial_h
#define CFEspacial_h

// inclue a biblioteca de filtros
#ifndef CFiltro_h
#include <Filtro/CFiltro.h>
#endif

// inclue a biblioteca de mascaras
#ifndef CMascara_h
#include <Mascara/CMascara.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
				Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEspacial.h
Nome da classe:      CFEspacial
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Classe básica para os filtros espaciais.
 *
 * Representa a superclasse para os filtros que atuam sobre
 * o espaço discreto da imagem.
 *
 * Ou seja atua sobre o espaço geométrico da imagem,
 * enquanto os filtros de amplitude atuam sobre o espaço de cor.
 * em uma variável inteira que define o tamanho da mascara a ser utilizada
 * e um ponteiro para um objeto da hierarquia das mascaras.
 * Como a classe base CFiltro tem um ponteiro para uma matriz imagem, eu tenho
 * acesso a imagem através de pm, e a mascara através de mask.
 * A função cria mascara é utilizada para criação da mascara
 * e a função Go executa o filtro em sí.
*/

class CFEspacial : public CFiltro
{
// Atributos
protected:
    unsigned int tamanhoMascara; ///< Tamanho da macara

public:
    CMascara * mask;	///< Ponteiro para mascara

// Métodos
public:

    /// Construtor, recebe ponteiro para imagem, e tamanho da mascara.
    CFEspacial (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara) : CFiltro (matriz), tamanhoMascara (_tamanhoMascara)
    {
        mask = NULL;
    }

    /// Destrutor
    ~CFEspacial ()
    {
        if (mask)
            delete mask;
    }

    /// Obtem tamanhoMascara.
    virtual unsigned int TamanhoMascara () const
    {
        return tamanhoMascara;
    }

    /// Seta o tamanhoMascara.
    virtual void TamanhoMascara (unsigned int _tamanhoMascara)
    {
        tamanhoMascara = _tamanhoMascara;
    }

    /// Realiza o processamento da filtragem
    virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara =0);


protected:
    /// Cria a mascara adequada
    virtual void CriaMascara (unsigned int _tamanhoMascara);

};
#endif // CFEspacial_h
