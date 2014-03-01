#if !defined(TACFraFunCov_h)
#define TACFraFunCov_h
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TACFraFunCov.h
Nome da classe:      TACFraFunCov
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:		TACFraFunCov
============================================================================
Nome Classe:          TACFraFunCov
Assunto:              Caracterizacao
Superclasse:
Descri��o:            Implementa a classe TACFraFunCov.  E' derivada da classe TACFractal.
Acesso:
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
*/
//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------

#include <TFractal/TACFractal.h>

//----------------------------------------------------------------------------
//Classe: TACFraFunCov
//----------------------------------------------------------------------------
class TACFraFunCov:public TACFractal
{
//Atributos novos
public:
protected:

  //Construtor e destrutor
public:
  //Construtor
  TACFraFunCov ();
  virtual ~ TACFraFunCov ();	//Destrutor

  //M�todos virtuais herdados               //{{TACFraFunCovVIRTUAL_BEGIN}}
protected:

public:
  virtual float CalculaDimensaoFractal (TMatriz2D * &pm);	//calcula a dimens�o fractal
  virtual TVetor *CalculaVetorDados (TMatriz2D * &pm);	//calcula vetor de dados

  //{{TACFraFunCovVIRTUAL_END}}
  //M�todos virtuais associados a tabela de resposta de TACFraFunCov
  //{{TACFraFunCovRSP_TBL_BEGIN}}
protected:
  //{{TACFraFunCovRSP_TBL_END}}
//DECLARE_RESPONSE_TABLE(TACFraFunCov);
};				//{{}}
#endif // TACFraFunCov_h sentry.
