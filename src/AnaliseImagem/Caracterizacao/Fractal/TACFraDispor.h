#if !defined(TACFraDisPor_h)
#define TACFraDisPor_h
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TACFraDisPor.h
Nome da classe:      TACFraDisPor
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:		TACFraDisPor
============================================================================
Nome Classe:           TACFraDisPor
Assunto:
Superclasse:
Descri��o:             Implementa a classe TACFraDisPor.  E' derivada da classe TACFractal.
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
//Classe: TACFraDisPor
//----------------------------------------------------------------------------
class TACFraDisPor:public TACFractal
{
//Atributos novos
public:
protected:

  //Construtor e destrutor
public:
  //Construtor
  TACFraDisPor ();
  virtual ~ TACFraDisPor ();	//Destrutor

  //M�todos virtuais herdados                  //{{TACFraDisPorVIRTUAL_BEGIN}}
protected:
public:
  virtual float CalculaDimensaoFractal (TMatriz2D * &pm) = 0;	//calcula a dimens�o fractal
  virtual TVetor *CalculaVetorDados (TMatriz2D * &pm) = 0;	//calcula vetor de dados

  //{{TACFraDisPorVIRTUAL_END}}
  //M�todos virtuais associados a tabela de resposta de TACFraDisPor
  //{{TACFraDisPorRSP_TBL_BEGIN}}
protected:
  //{{TACFraDisPorRSP_TBL_END}}
//DECLARE_RESPONSE_TABLE(TACFraDisPor);
};				//{{}}
#endif // TACFraDisPor_h sentry.
