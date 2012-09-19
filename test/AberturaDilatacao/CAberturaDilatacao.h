
#ifndef CAberturaDilatacao_H
#define CAberturaDilatacao_H

#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMorfologiaMatematica.h>
#include <Matriz/TMatriz2D.h>
#include <Matriz/CVetor.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

using namespace std;

/**
 * Classe para determinacao da distribuicao de s�tios e ligacoes usando m�todo da abertura-dilatacao.
	@author André Duarte Bueno <bueno@lenep.uenf.br>
	@author Rafael Borgate <borgate@gmail.com>
*/
class CAberturaDilatacao
{

protected:
    /// Ponteiro para objeto filtro morfologia matematica
    CFEMorfologiaMatematica* pfmf;

    /// Vetor distribuicao total = distTotal = distPoros + distLigacoes
    CVetor* distribuicaoTotalPoros;

    /// Vetor dist. ligacoes
    CVetor* distribuicaoLigacoes;

    /// vetor dist. poros
    CVetor* distribuicaoSitios;

    /// Matriz original (ponteiro para matriz original)
    TMatriz2D< int >* pm;

    /// Matriz rotulada
    TMatriz2D< int >* matrizRotulo;

    /// Numero de objetos identificados na última rotulagem (Não é acumulado)
    int numeroObjetos;

    /// Vetor �rea dos objetos
    CVetor* areaObjetos;

    /// Vetor perimetro dos objetos
    CVetor* perimetroObjetos;

    /// Vetor raio hidraulico dos objetos
    CVetor* raioHidraulicoObjetos;

    /// Porosidade
    double porosidade;

    /// Nome da imagem que esta processando (usado para salvar resultado final)
    std::string nomeImagem;

    /// fator usado como crit�rio de parada
    int raioMaximoElementoEstruturante ;

    /// fator usado para reducao do Raio do Elemento Estruturante no processamento da dilatacao.
    int fatorReducaoRaioElemEst ;

    /// Valor do incremento do raio do elemento estruturante
    int incrementoRaioElementoEstruturante ;

    /// Numero do modelo de calculo
    int modelo;

    /// Se ativo salva os resultados parciais
    static bool salvarResultadosParciais ;
public:
    /// Construtor
    CAberturaDilatacao();

    /// Construtor sobrecarregado
    CAberturaDilatacao(TMatriz2D< int >* &matriz, std::string _nomeImagem = "");

    /// Destrutor
    ~CAberturaDilatacao();

    /// Calculo da porosidade
    double Porosidade(TMatriz2D< int >*& pm);

    /// Rotula a imagem
    void RotulaImagem();

    /// Rotula a imagem de forma sequencial  ( os rotulos ficarão sequênciais e iniciarão em rotuloInicial )
    // Normalmente rotuloInicial = 0, usase- um rotulo inicial diferente de zero quando
    // deseja-se trabalhar com multiplas rotulagens sobre uma mesma imagem.
    void RotulaImagemSequencial( int rotuloInicial , int FUNDO = 0 );

    /// Determina distribuicao Total de Poros (m�todo normal)
    void DistTotalPoros();

    /// Determina distribuicao de sitios e ligacoes (m�todo novo) modelo 0 (Anaimp)
    void DistSitiosLigacoes_Modelo_0();

    /// Determina distribuicao de sitios e ligacoes (m�todo novo, modelo 1)
    // void DistSitiosLigacoes_Modelo_1_old_usaCVetor(); // Usa CVetor
    void DistSitiosLigacoes_Modelo_1();               // Usa vector

    /// Determina distribuicao de sitios e ligacoes (m�todo novo, modelo 2)
    void DistSitiosLigacoes_Modelo_2();

    /// Determina distribuicao de sitios e ligacoes (m�todo novo, modelo 3)
    void DistSitiosLigacoes_Modelo_3();

    /// Determina distribuicao de sitios e ligacoes (m�todo novo, modelo 4)
    void DistSitiosLigacoes_Modelo_4();

    /// Salva vetor em disco (j� tem Write?)
    void Salvar(CVetor* &v, std::string nomeArquivo);
    void Salvar(std::vector<double> v, std::string nomeArquivo);

    void SequenciaAberturaTonsCinza();

    /// Retorna o raio Maximo do Elemento Estruturante
    int RaioMaximoElementoEstruturante () {
        return  raioMaximoElementoEstruturante ;
    }

    /// Seta o raioMaximoElementoEstruturante  - o raio Maximo do Elemento Estruturante
    void RaioMaximoElementoEstruturante ( int _rmee) {
        raioMaximoElementoEstruturante = _rmee;
    }

    /// Retorna o fatorReducaoRaioElemEst
    int FatorReducaoRaioElemEst() {
        return  fatorReducaoRaioElemEst;
    }

    /// Seta o fatorReducaoRaioElemEst ( ex: =2, significa que a dilatacao vai ser realizada com raioElemen = raioElemen / 2 )
    void FatorReducaoRaioElemEst( int _frree) {
        fatorReducaoRaioElemEst = _frree;
    }

    /// Retorna o incrementoRaioElementoEstruturante
    int IncrementoRaioElementoEstruturante () {
        return  incrementoRaioElementoEstruturante;
    }

    /// Seta o incrementoRaioElementoEstruturante
    void IncrementoRaioElementoEstruturante ( int _iree) {
        incrementoRaioElementoEstruturante = _iree;
    }

    /// Retorna flag salvarResultadosParciais
    static bool SalvarResultadosParciais( ) {
        return salvarResultadosParciais;
    }

    /// Seta flag salvarResultadosParciais
    static void SalvarResultadosParciais( bool b ) {
        salvarResultadosParciais = b;
    }
};

#endif
