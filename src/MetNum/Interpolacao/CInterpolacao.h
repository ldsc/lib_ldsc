#ifndef CInterpolacao_h
#define CInterpolacao_h

///////////////////////////////////////////////////////////////////////////
//                          Framework Interpolacao
///////////////////////////////////////////////////////////////////////////
/**
 * @brief Classe de interpolação.
 *
 * O que é:
 * é uma framework de classes de Interpolacao.
 * 
 * Basicamente uma interpolação consiste em obter o valor de uma
 * propriedade em um ponto cujo valor é desconhecido,
 * mas conhecido em suas adjacencias.
 * Geralmente você tem um conjunto discreto de valores (vetor,matriz), e quer
 * avaliar a propriedade numa posição intermediária.
 * 
 * A interpolação pode ser para o valor da propriedade,
 * ou para a derivada.
 * 
 * Informacoes de montagem:
 * Este módulo foi inicialmente desenvolvido como
 * parte integrante do programa de simulacao numérica s3dmp.
 * A versão inicial tinha as funções de interpolação descritas
 * no livro de métodos numéricos do Prof. C.R.Maliska, adapatadas
 * usando uma formulação orientada a objeto.
 * Realizam a interpolacao da derivada entre dois pontos
 * baseada em diferentes critérios.
 * 
 * Posteriormente tentei combinar, os métodos de interpolação
 * descritos no livro de métodos numéricos de Namir C Shammas,
 * e que são da forma y=f(x). A maioria dos métodos é baseado
 * na expansão de Taylor.
 * 
 * Descrição de coisas feitas, interessantes do ponto de vista da AOO:
 * A classe CInterpolacao foi montada diretamente, onde incluí
 * duas funções que eram comuns aos diferentes métodos.
 * Acrescentei a função Go padrão para a hierarquia .
 * (encapsulamento direto com a geração de função padrão)
 * 
 * A classe CIntLagrange e CIntEDBarycentric foram diretamente encapsuladas
 * pois tem o formato exato da função Go adotada como default.
 * (encapsulamento direto)
 * 
 * Para a classe CIntNewtonDivDiff criei atributos internos que são calculados
 * uma única ves  (pela função GeraTabelaAuxiliar que monta a tabela de valores auxiliares),
 * de forma que a função  Go ficou da mesma forma que a da classe base.
 * (criação de atributos internos usados especificamente por este métodos)
 * 
 * Para a classe CIntNewtonDiff, fiz uma heranca da classe CIntNewtonDivDiff,
 * herdando principalmente os mesmos atributos.
 * Também tem uma função GeraTabelaAuxiliar.
 * Como os atributos recebidos pela função original éram um pouco diferentes,
 * incluí o calculo dos dois novos atributos na função Go.
 * (herança de atributos da classe base e obtenção de formato padrão para Go, por mecanismo
 * de adaptação coerente das entradas)
 * 
 * Para a classe CIntCubSpline foram criados atributos internos, específicos
 * para este método, e que são recebidos no construtor.
 * 
 * Para a classe CIntBarycentric foram criados atributos auxiliares
 * e função de calculo auxiliar. Para conseguir Go padrão.
 * 
 * Exemplo de uso:
 * CInterpolacao* interpolacao = new  CIntNewtonDivDiff();
 * assert(interpolacao);
 * interpolacao->GeraTabelaAuxiliar(vx,vy,n);
 * double resultado = interpolacao->Go(vx,vy,n,x);
 * delete interpolacao;
 * 
 * PS: necessita criar o Tipo
 * TFuncaoDiscreta
 * TFuncaoDiscreta,  	Genérico
 * TFuncaoDiscretaLinear	Vetor y, xInicial ,xFinal e dx
 * TFuncaoDiscretaXY	Que tem dois vetores x e y.
 * 
 * O esboço abaixo mostra a estrutura antiga desta framework:
 * As ligacoes com seta sao estruturas todo-parte.
 * As ligacoes emendadas sao estruturas generalizacao-especializacao

			.----------------------------.
			|       Interpolacao         |
			.----------------------------.
			|        | |   | |       | |
      .--------------------.     | |   | |       | .--------.
      |                          | |   | |       |          |
      |                 .--------. |   | .----.  |          |
      |                 |          |   |      |  |          |
      |                 |          |   |      |  |          |
      |                 |          |   |      |  |          |
      |                 |  .--------.  |      |  |      .------.
      |                 |  | Upwind |  |      |  |      | Exata |
      |                 |  .--------.  | .-----.|      .-------.
      |                 |              | | Wuds ||
.---------------------. |              | .------.|
| Diferencas Centrais | |              |         |
.---------------------. |              |         |
			|              |         |
			|              |         |
		.------------. .------------. .--------.
		| Quadratica | | Quadratica | | Newton |
		.------------. .------------. .--------.

 * Funcionamento com o programa de simulacao numérica:
 * A funcao Main cria um objeto Simulador, este cria objetos No_Coeficientes.
 * Os objetos No_Coeficientes tem um ponteiro para uma único objeto interpolacao,
 * assim todos os objetos No_Coeficientes acessam o mesmo objeto interpolacao.
 * O objeto Interpolacao  criado a partir da chamada da funcao
 * f_selecao_tipo_interpolacao.
 *
 * A classe Interpolacao tem uma funcao de calculo chamada f_calcula_alfa_beta
 * esta funcao calculo os coeficientes alfa e beta.
 * Estes coeficientes sao publicos podendo ser acessados dentro da funcao
 * de calculo do No_Coeficientes.
 * 
 * A funcao f_simulador chama a funcao de calculo dos coeficientes dos nós.
 * A funcao de calculo dos coeficientes chama a funcao de calculo de alfa e beta,
 * e depois usa os valores de alfa e beta.
 * 
 * Maiores informacoes entre em contato com;
 * Eng. Andre Duarte Bueno
 * Rua Luiz Boiteux Piazza, N 3192 
*/

//--------------------------------------------------------------------
//                              BIBLIOTECAS
//--------------------------------------------------------------------
#include <Base/CMath.h>
#define INTERP_EPS 1.0e-30	//colocar no CMath?? from C.Shammas

/**
 * @brief Classe base para os objetos de interpolação.
 * 
 * Antigo:
 * A Classe Interpolacao deve calcular os coeficientes alfa e beta.
 * Observe que na funcao de calculo de alfa e beta os parametros
 * sao passados por referencia, isto evita a cópia da variável o que
 * deixa a funcao mais rápida.
*/
class CInterpolacao : virtual public CMath
{
public:
	
	/// Realiza a interpolação, vai ser reescrita nas derivadas.
  virtual double Go (double *xarr, double *yarr, int n, double x) = 0;

protected:
	
/// Métodos auxiliares usados pelas classes derivadas
  int getDerivatives (double *xarr, double *yarr, int n, double *deriv, double *h, double tolerance);

/// Método auxiliar
  int tridiagonal (int n, double *a, double *b, double *c, double *d, double epsilon);
};

#endif


/*
Organizacao da lib CInterpolacao
(por enquanto tirei a parte de métodos numericos, criado diretório a parte)

Funções:
//Parametros:
Vetor x
Vetor y
Numero de pontos n
Posição x
double Lagrange	(double* xarr, double* yarr,int n, double x);
+
Vetor wt (pesos)
flag  calcWtFlag (flag de calculo dos pesos)
double Barycentric(double* xarr, double* yarr,double* wt,int calcWtFlag, int n, double x);
ok
double ED_Barycentric(double* xarr, double* yarr,int n, double x);
+
vetor tabela
flag  buildMatFlag
double NewtonDivDiff(double* xarr, double* yarr,double* diffTable, int buildMatFlag,int n, double x);

+
vetor diffTable
flag  buildMatFlag
double NewtonDiff(double x0, double h, double* yarr,double* diffTable, int buildMatFlag,int n, double x);
double Spline(double* xarr, double* yarr, double* deriv,double* h, int n, double x);

Funcoes auxiliares
------------------
recebe 4 vetores a,b,c,d o tamanho n, e um epsilon
int tridiagonal(int n, double* a, double* b, double* c,double* d, double epsilon);

recebe vetores x,y,
tamanho n
vetores deriv,h
e a tolerancia
int getDerivatives(double* xarr, double* yarr, int n,double* deriv, double* h,double tolerance);
*/
