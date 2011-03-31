#if !defined(TACFraCaixas_h)
#define TACFraCaixas_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TACFraCaixas.h
Nome da classe:      TACFraCaixas
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao CLASSE:		TACFraCaixas
============================================================================
Nome Classe:        TACFraCaixas
Assunto:            Caracterizacao
Superclasse:
Descrição:          	 Implementa a classe TACFraCaixas.  E'derivada da classe TACFractal.
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
//Classe:       TACFraCaixas
//----------------------------------------------------------------------------
class TACFraCaixas:public TACFractal
{
//Atributos novos
public:
protected:

  //Construtor e destrutor
public:
  //Construtor
  TACFraCaixas ();
  virtual ~ TACFraCaixas ();	//Destrutor

  //Métodos virtuais herdados                                   //{{TACFraCaixasVIRTUAL_BEGIN}}
protected:

public:
  virtual float CalculaDimensaoFractal (TMatriz2D * &pm);	//calcula dimensão fractal
  virtual TVetor *CalculaVetorDados (TMatriz2D * &pm);	//calcula vetor de dados

  //{{TACFraCaixasVIRTUAL_END}}
  //Métodos virtuais associados a tabela de resposta de TACFraCaixas
  //{{TACFraCaixasRSP_TBL_BEGIN}}
protected:
  //{{TACFraCaixasRSP_TBL_END}}
//DECLARE_RESPONSE_TABLE(TACFraCaixas);
};				//{{}}
#endif // TACFraCaixas_h sentry.
