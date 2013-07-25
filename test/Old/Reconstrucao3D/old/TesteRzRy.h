#ifndef TesteRzRy_h
#define TesteRzRy_h

// -----------------------------------------------------------------------------
// 										BIBLIOTECAS
// -----------------------------------------------------------------------------
// Bibliotecas do lmpt
#include    <cstdio>
#include    <fstream>

/*
// ------------------------------------------------------------------------------
// Objeto TesteRzRy
// ------------------------------------------------------------------------------
Descri��o:
Classe criada esclusivamente para teste da rela��o entre Cz e Ry
calcula Cz usando um fun��o exponencial fun��o da porosidade (isto � yc) e
do comprimento de correla��o.
*/
class TesteRzRy
{

// Atributos
protected:
    // int fatorAmplificacaoNumeroPontos;
    // fator de redu��o, se =1 pega todos os pontos
    // se =2 pega a metade dos pontos.
    // numero total de dados na funcao correlacao
    int numeroPontosCz;

    // porosidade
    double porosidade;

    double porosidadeMinima , porosidadeMaxima,	incrementoPorosidade;

    int comprimentoCorrelacao;// novo

    int compCorMin,compCorMax, incrementoCorrelacao;

    unsigned int gt;		// se gt=1 gaussiana truncada 1, se gt2 gaussiana truncada 2

    // vetor autocovariancia normalizada Ry// usar TVetor
    double *Ry;

    // vetor correla��o Cz// usar TVetor
    double *Cz;

    // limite utilizado para definicao da funcao de faze
    double yc;

    // contadores
    int i,j,k;

    // numero de pontos realmente lidos do arquivo de disco
    // int numeroPontosLidosArquivoCz;

    // nome arquivo covariograma z
    // char nomeArquivoCz[200];
    // nome arquivo autocorrelacao ry
    char nomeArquivoRy[200];

// Metodos
protected:
    // novo calcula Cz usando fun��o exponencial
    void Calculo_Cz();

    // Calcula Ry a partir de Rz
    void Calculo_Ry();

    void SaidaDados();

public:
    // Construtor
    TesteRzRy();

    // Destrutor
    virtual ~TesteRzRy();

    // executa o teste
    virtual int Go();
};
#endif



