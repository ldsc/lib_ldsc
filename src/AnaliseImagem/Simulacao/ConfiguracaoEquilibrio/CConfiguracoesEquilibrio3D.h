#ifndef CConfiguracoesEquilibrio3D_h
#define CConfiguracoesEquilibrio3D_h
/**
----------------------------------------------------------------------------
PROJETO:		LIB_LDSC
			Bibliotecas de Objetos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 * Liang Zirong, Paulo C. Philippi, Damiani,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CConfiguracoesEquilibrio3D.h
Nome da classe:     CConfiguracoesEquilibrio3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas C++
//  ----------------------------------------------------------------------------
#ifndef __IOSTREAM_H
#include <iostream>
#endif

//  ----------------------------------------------------------------------------
//  Bibliotecas LIB_LDSC
//  ----------------------------------------------------------------------------
/*
#ifndef TFiltroEspacial_h
#include "TFiltro\TFiltroEspacial.h"
#endif
*/

#include <Amostra/Material/CMFluido.h>
#include <AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CCamara3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FEConectividade/CConectividade3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDF3D.h>

using namespace std;

//  Processo default intrusao de mercurio mb=0 ca=0
//  OBS: Lembre-se, quando executa conectividade->Go() cria um
//  objeto de rotulagem (uma matriz inteira extra).
//  A idf também vai consumir uma imagem inteira.

/**
@brief   	Determina as configurações de equilíbrio de imagens tridimensionais.
@author  	Andre Duarte Bueno
@see		http://www.lenep.uenf.br/~bueno
*/
class CConfiguracoesEquilibrio3D
{
    string nomeArquivo;					/// Nome do arquivo de disco

    bool visualizar;					/// flag, se ativo mostra na tela saídas parciais

    ostream *os;						/// Saídas parciais são enviadas para esta ostream (ex: cout)

    int contadorPassosExecutados;				/// Utilizado para dar nomes aos arquivos de disco em ordem crescente. O nome do arquivo de disco é montado levando em conta a operação realizada e este contador.

    // Tarefa: verificar utilizada de indiceParcialB? nao lembro
    int indiceParcialB;					/// Índice da regiao B após cada processo

public:
    bool salvarResultadosParciaisDisco;		/// flag, se ativo os resultados são salvos em disco. É setado no construtor, default=true

    bool salvarResultadosFinaisDisco; 		/// Se verdadeiro, salva resultados finais em disco (default = true).

    bool corrigirAbertura;					/// Se verdadeiro, corrige erro físico na imagem abertura (default = true).

    CCamara3D * camara;					/// É a imagem da camara (3D)

    CMFluido *fluidoB;					/// Representa o fluido B

    CMFluido *fluidoA;					/// Representa o fluido A

protected:
    //  objeto de interacao entre dois fluidos(confirmar), TInteracaoFluidosAB* interacaoFluidos;

		TCFEMMIDF3D<int> * idf;					/// É uma cópia da imagem com a IDF (3D)

    CConectividade3D *rotulador;				/// Objeto de calculo da conectividade (3D) realiza a verificacao da conectividade.

    int raioMaximo;					/// raioMaximo da operacao

    int raioMinimo;					/// raioMinimo da operacao

    int raio;						/// Utilizado pelos métodos Next e Go para controle do loop.

    int tipoIDF;						/// define o tipo de idf a ser criada 4,8,34,5711,345,e1

    int B0;   						/// Índices da região B0

    int A0;   						/// Índices da região A0

    int Ai;   						/// Índices da região Ai

    int G;    						/// Índices da região G

    int G_;   						/// Índices da região G_

    int Y0;   						/// Índices da região Y0 (não usado em 2D - verificar)

    int Yi;   						/// Índices da região Yi

    int KGB0; 						/// Índices da região KGB0

    int wbG__U_KGB0;  					/// Índices da região wbG__U_KGB0

    int KwbG__U_KGB0B0;					/// Índices da região KwbG__U_KGB0B0

    int B;   						/// Índices da região B

    int A;   						/// Índices da região A

    double saturacaoA;					/// Saturacao da fase A

    double saturacaoB;					/// Saturacao da fase B

//  Métodos
protected:
    void DefineAtributos ();				/// Método que define os atributos da classe

    void CriaCamara (TCMatriz3D<int> * &imagem);		/// Método que  cria a camara

    void CriaIDF ();					/// Método que cria a imagem idf. Uma idf se autocalcula

    void CriaRotulador ();				/// Método que cria a imagem rotulada uma rotulador se auto calcula

    void CalculaAbertura (int &raio);			///  Método que para cada raio irá executar a operação de abertura

    void ConectividadeKGB0 ();				///  Método que determina a conectividade KGB0

    void UniaoG__U_KGB0 ();				///  Método que determina a união de G__U_KGB0

    void ConectividadeKwbG__U_KGB0B0 ();			///  Método que determina a conectividade KwbG__U_KGB0B0

    void SolucaoOmega (int &indiceRegiaoB, TCMatriz3D<int> * &imagem);	///  Método que determina a soluãoo Omega

    void CorrecaocaxYi (TCMatriz3D<int> * &imagem);		///  Método que corrige caxYi,  isto é, recupera os píxeis tomados por Yi

    void SolucaoFinal (TCMatriz3D<int> * &imagem);		///  Método que fornece a solução final

    void RestabeleceIDFPositiva ();			///  Método que reestabelece a idf positiva

    void DiferencaEmRelacaoArtigo (TCMatriz3D<int> * &imagem); 	///  Método que realiza correção, isto é, ajuste para que o método funcione.  Este ajuste não consta do artigo original.

    void Salvar (TCMatriz3D<int> * &imagem, std::string msg);	///  Método que salva em disco as imagens geradas, Utilizado em etapas de teste

public:

    ///  Construtor Default, recebe uma ostream, para a qual enviará a saída.
    CConfiguracoesEquilibrio3D ( /*TCMatriz2D< int >* imagem */ ostream & out = std::cout);

    ///  Destrutor
    virtual ~ CConfiguracoesEquilibrio3D ();

    /// Sobrecarga do operador <<
    // IMPLEMENTAR - Deve enviar para saída os atributos do objeto
    friend ostream & operator<< (ostream & os, CConfiguracoesEquilibrio3D & configuracoesEquilibrio3D) {
        configuracoesEquilibrio3D = configuracoesEquilibrio3D; //evitar warning
        return os;

    }
    /// Sobrecarga do operador >>
    // IMPLEMENTAR - Deve obter da entrada os atributos do objeto
    friend istream & operator>> (istream & is, CConfiguracoesEquilibrio3D & configuracoesEquilibrio3D) {
		configuracoesEquilibrio3D = configuracoesEquilibrio3D; //evitar warning
        return is;
    }

    ///  Executa passo-a-passo a determinação das configurações de equilíbrio.
    virtual bool Next (TCMatriz3D<int> * &imagem);

    ///  Executa toda a determinação das configurações de equilíbrio.
    virtual void Go (TCMatriz3D<int> * &imagem);

    /// Retorna ponteiro para uma CMatriz3D o qual aponta para uma imagem binária extraída da câmara, correspondente a regiao passada como parâmetro. Deve ser chamada após Next ou Go.
// TCMatriz3D<int> * GetImagem ( int indice_regiao ) const;

    /// Altera os valores da matriz passsada como parametro para corresponder a imagem binária referente a regiao passada como parâmetro. Deve ser chamada após Next ou Go.
// bool GetImagem(TCMatriz3D<int> * imagem, int indice_regiao) const;

    /// Retorna o índice da região A
    int GetIndiceRegiaoA() {
        return A;
    }

    /// Retorna o índice da região B
    int GetIndiceRegiaoB() {
        return B;
    }

    /// Retorna o índice da região Yi
    int GetIndiceRegiaoY() {
        return Yi;
    }

    /// Inverte o fluxo na determinação das configurações de equilíbrio. (drenagem <-> embebição)
    void InverterFluxo ( TCMatriz3D<int> * &imagem );

    /// Retorna ponteiro para uma CMatriz3D o qual aponta para uma imagem da região informada (A ou B) extraída da câmara. Deve ser chamada após Next ou Go.
    // Quem chamou deve destruir a imagem que é aqui criada.
    TCMatriz3D<int> * GetImagem( int regiao ) const;

    /// Preenche a imagem recebida como parametro com os pixeis que sao iguais ao indice regiao.
    bool GetImagem(TCMatriz3D<int> * &imagem, int regiao) const;

    /// Método que seta o atributo fluidoA
    void FluidoA (  CMFluido * &fa)
    {
        if (fluidoA)
            delete fluidoA;
        fluidoA = fa;
    }

    /// Método que seta o atributo fluidoB
    void FluidoB ( CMFluido * &fb)
    {
        if (fluidoB)
            delete fluidoB;
        fluidoB = fb;
    }

    /// Método que seta o tipo de idf
    void TipoIDF (const int &_tipoIDF) 		{
        tipoIDF = _tipoIDF;
    }

    /// Método que retorna o tipo de idf
    int TipoIDF () const				{
        return tipoIDF ;
    }

    ///  Somente para teste, retorna this
    const CConfiguracoesEquilibrio3D* This ()const {
        return this;
    }

    /// Get salvarResultadosParciaisDisco
    bool SalvarResultadosParciaisDisco() 	const	{
        return salvarResultadosParciaisDisco;
		}

    /// Set salvarResultadosParciaisDisco
    void SalvarResultadosParciaisDisco(bool s) 	{
        salvarResultadosParciaisDisco = s;
		}

    /// Get salvarResultadosFinaisDisco
    bool SalvarResultadosFinaisDisco() 	const	{
        return salvarResultadosFinaisDisco;
		}

    /// Set salvarResultadosParciaisDisco
    void SalvarResultadosFinaisDisco(bool s) 	{
        salvarResultadosFinaisDisco = s;
		}


    /// Get Saturação da fase A
    double SaturacaoA() const			{
        return saturacaoA;
    }

    /// Get Saturação da fase B
    double SaturacaoB()  const 			{
        return saturacaoB;
    }

};

#endif
