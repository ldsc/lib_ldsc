#include <iostream>
#include <string>

#include <Integral/CIntegral.h>
#include <Integral/CISimpson.h>
#include <Integral/CIRomberg.h>
#include <Integral/CIGChebyshevQuadrature.h>
#include <Integral/CIGHermiteQuadrature.h>
#include <Integral/CIGLaguerreQuadrature.h>
#include <Integral/CIGLegendreQuadrature.h>

#include <Base/CFuncao.h>

using namespace std;

/// Cria classe herdeira de CFuncao, uma reta
class CFTeste : public CFuncao
{
public:
    virtual double Go (double x, double y = 0)
    {
        return 1.0 + 5.0 * x;
    }
};

/**
 * Exemplo simples de uso da biblioteca integracao.
 *
 * Exercício: Gerar saida em modo texto e gerar gráficos no gnuplot.
 */
int
main (int argc, char *argv[])
{
    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./integral"
          << "\n================================================================================" << endl;

    CFTeste fteste;
    cout << "\nTestando a função y = 1.0 + 5.0 * x; para x=10, y=" << fteste(10) << endl;

    CISimpson simpson(&fteste);
    simpson.NumeroPontos(100);

    cout << "Area no intervalo 0->10 = " << simpson.Go(10,0) << endl;

    cout << "\n\nPrecisa arrumar arquivos de Integral, alguns estão danificados."  << endl;
    /*
    	cout << "Qual filtro deseja testar? \n"
    	 << " CISimpson              (1)\n"
    	 << " CIRomberg              (2)\n"
    	 << " CIGChebyshevQuadrature (3)\n"
    	 << " CIGHermiteQuadrature   (4)\n"
    	 << " CIGLaguerreQuadrature  (5)\n:"
    	 << " CIGLegendreQuadrature  (6)\n:";
    	 int opcao;
    	cin >> opcao;

    	CIntegral* integral;
    	CFuncao* funcao = &fteste;
    	cout << "Criando método integração selecionado.\n";
    	switch(opcao)
    	{
    	case 1 : integral = new CISimpson(funcao);  		break;
    	case 2 : integral = new CIRomberg(funcao);		break;
    	case 3 : integral = new CIGChebyshevQuadrature(funcao);break;
    	case 4 : integral = new CIGHermiteQuadrature(funcao); 	break;
    	case 5 : integral = new CIGLaguerreQuadrature(funcao);	break;
    	case 6 : integral = new CIGLegendreQuadrature(funcao);	break;
    	}
    	cout << "Area no intervalo 0->10 = " << integral->Go(10,0) << endl;
    */
    return 0;
}
