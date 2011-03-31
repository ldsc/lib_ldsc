#if !defined(TACFractal_h)
#define TACFractal_h
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TACFractal.h
Nome da classe:      TACFractal
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao CLASSE:		TACFractal
============================================================================
Nome Classe:         TACFractal
Assunto:             Caracterizacao
Superclasse:         ---
Descrição:           Declara a classe TACFractal.
Acesso:
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
*/
//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------

#include <TMatriz/TVetor.h>
#include <TMatriz/TMatriz2D.h>

//----------------------------------------------------------------------------
//Classe: TACFractal
//----------------------------------------------------------------------------
class TACFractal
{
//Atributos novos
public:
  TVetor * vetorDados;		//vetor com dados para calculo da dimensão fractal
protected:
  float dimensaoFractal;	//dimensao fractal calculada
  //Construtor e destrutor
public:
  //Construtor
    TACFractal ();
    virtual ~ TACFractal ();	//Destrutor

  //Métodos virtuais herdados                                   //{{TACFractalVIRTUAL_BEGIN}}
protected:
public:
    virtual float CalculaDimensaoFractal (TMatriz2D * &pm) = 0;	//calcula a dimensão fractal
  virtual TVetor *CalculaVetorDados (TMatriz2D * &pm) = 0;	//calcula vetor de dados
  float GetDimensaoFractal ()
  {
    return dimensaoFractal;
  };				//retorna a dimensão fractal
  //{{TACFractalVIRTUAL_END}}
  //Métodos virtuais associados a tabela de resposta de TACFractal
  //{{TACFractalRSP_TBL_BEGIN}}
protected:
  //{{TACFractalRSP_TBL_END}}
//DECLARE_RESPONSE_TABLE(TACFractal);
};				//{{}}
#endif // TACFractal_h sentry.
