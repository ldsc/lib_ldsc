#include <fstream>

#include <cmath>

#include "MetNum/Funcao/CFNaoLinearRzRy.h"	// classe

#include "MetNum/Funcao/CFPHermiteRec.h"	// objetos inclusos

#include "MetNum/Funcao/CFSomatorioCm2RyM.h"

#include "MetNum/Integral/CISimpson.h"

#include "MetNum/Solver/Funcao/CSFBissecao.h"

// nao usa y
double
CFNaoLinearRzRy::Go (double x, double y)
{
  // double& rz=x;
  double rz = x;		// armazena valor recebido x como rz
  double ry;			// valor de ry
  // --------------------------------------------------------------------------
  // 1-Calcula os coeficientes Cm, criando objeto funcao e objeto de integracao
  // --------------------------------------------------------------------------
  if (CoeficientesCmJaCalculados == false)	// com o flag só calcula os coeficientes uma única vez
    {

      CoeficientesCmJaCalculados = true;

      // cout<<"\nCriando Polinomio Hermite";
      // CFPHermite* funcaoHermite=new CFPHermite(porosidade,yc,numeroPontosSerie);
      CFPHermiteRec *funcaoHermite =
	new CFPHermiteRec (porosidade, yc, numeroPontosSerie);

      // cout<<" ok,";
      // cout<<", integral ";
      CISimpson simpson (dynamic_cast < CFuncao * >(funcaoHermite));	// cria objeto de integracao, ja passa a funcao a ser utilizada

      // simpson.SetLimiteInferior(-10); // agora sao passados na funcao Go
      // simpson.SetLimiteSuperior(10);

      simpson.NumeroPontos (200);	// 100
      // cout<<" ok.";
      // simpsom->Parametros(...);
      // cout<<" Calculando cm..";
      // ofstream fout("valorCm.cm2");// novo


      for (unsigned int j = 0; j <= numeroPontosSerie; j++)
	{
	  // cout<<"["<<j<<"]=";
	  funcaoHermite->M (j);	// a funcao Hermite usa 2 parametros m e x

	  // Go(limiteSuperior=1,limiteInferior=0)
	  // verificar a alteracao da linha acima, para a de baixo
	  cm[j] = simpson.Go (10, -10);	// calcula a integral numerica, passa x

	  // cout<<cm[j]<<" ";
	  // fout<<"\ncm["<<j<<"]="<<cm[j];
	}
      // cout<<", Deletando Polinomio Hermite...";

      delete funcaoHermite; 		// deleta objeto funcao
      // cout<<" ok.";
      // fout.close();// novo
    }

  // --------------------------------------------------------------------------
  // 2-Calculo de Ry a partir de Rz
  // --------------------------------------------------------------------------
  // Ry pode ser calculado a partir da equacao 3.20 (serie de polinomios de Hermite)
  // ou por uma funcao empirica 3.28. Abaixo determina coeficiente alfa1 da equacao empirica
  double rz90 = 0;

  for (int i = 0; i < numeroPontosSerie; i++)	// quando rz for maior que 0,9 pode-se calcular ry pela equacao empirica 3.28 pg38
    rz90 += (cm[i] * cm[i]) * pow (0.9, i);	// calcula rz90, usado para determinar alfa1

  if ((rz <= 1.0 && rz > rz90)	// se esta no intervalo da funcao empirica
      &&			// novo
      gt == 1			// e estamos usando gaussiana truncada 1     // novo
    )
    {
      // cout<<", ry(alfa1)=";
      // Estamos calculando uma curva.
      // No final da curva, quando rz > rz90, o autor original (gt1) fazia o ajuste abaixo.
      // Na época os computadores tinham pouca memória, e as maiores imagens tinham 100 ao cubo.
      // Quando foram analisar os dados encontraram muitos erros, e após fazerem analise, resolveram
      // adicionar o ajuste abaixo. Que, para imagens pequenas se justifica.
      // Acho que este ajuste estava errado, e éra aplicado porque os computadores tinham pouca memória.
      // Com o aumento da memória podemos construir imagens maiores.
      // gt2 simplesmente elimina este ajuste.
      
      // Com gaussiana truncada 2
       
      double alfa1 = (1.0 - rz90) / sqrt (0.1);	// calcula coeficiente auxiliar alfa1
      ry = 1.0 - (1.0 - rz) * (1.0 - rz) / alfa1 / alfa1;	// calcula ry a partir de rz usando funcao empirica
      // cout<<ry;
    }
  else				// senao calcula Ry usando interpolacao (quando Rz menor que 0.9)
    {
      // cout<<"\nVai criar função CFuncaoSomatorioCm2RyM ...";
      CFSomatorioCm2RyM *funcaoSomatorioCm2RyM =
	new CFSomatorioCm2RyM (rz, numeroPontosSerie, cm);

      // CFuncao* funcaoSomatorioCm2RyM=new CFuncaoSomatorioCm2RyM(rz,numeroPontosSerie,cm);
      // cout<<"...funcao CFuncaoSomatorioCm2RyM criada.";
      // cout<<"\nVai criar TSolverBissecao bissecao ...";
      // TSBissecao bissecao(dynamic_cast<CFuncao *>(funcaoSomatorioCm2RyM)); aqui
      CSFBissecao bissecao (dynamic_cast < CFuncao * >(funcaoSomatorioCm2RyM));

      // TSolverBissecao bissecao(funcaoSomatorioCm2RyM);
      bissecao.LimiteInferior (0.0000000001);

      bissecao.LimiteSuperior (1.0);

      bissecao.LimiteIteracoes (100);

      bissecao.LimiteErro (1.0e-8);

      // cout<<" ...usando bissecao... ";
      // SetParametros(_a,  _b, _h, _limiteErro,_limiteIteracoes,_limiteInferior, _limiteSuperior)
      // bissecao.SetParametros(-1,1,0.01, 1e-8,50,-1,1);
      // cout<<"\nCalculando ry=";

      ry = bissecao.Go (0.8);	// passa uma semente
      // cout<<ry;
      // cout<<"\nVai deletar funcaoSomatorioCm2RyM....";

      delete funcaoSomatorioCm2RyM; 		// deleta objeto funcao
      // cout<<"\n... funcaoSomatorioCm2RyM deletada";
    }

  return (ry);			// retorna ry calculado
}
