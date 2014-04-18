#include <AnaliseImagem/Simulacao/Permeabilidade/RedePercolacao/CSimPermeabilidadeRede.h>

#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>

#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_SOR.h>
#include <MetNum/Contorno/CContornoCentro.h>

using namespace std;

// Construtor
CSimPermeabilidadeRede::CSimPermeabilidadeRede (
		CMFluido * &_fluido,
		CSolverMatrizDiagonalDominante *& _solver,
		CContornoRedePercolacao *&_rede,
		unsigned int _nx,
		unsigned int _ny,
		unsigned int _nz,
		unsigned int _fatorAmplificacao,
		long double _dimensaoPixel,
		unsigned int _numeroPixeisBorda
		) :
	fluido (_fluido),
	solver (_solver),
	rede (_rede),
	nx (_nx),
	ny (_ny),
	nz (_nz),
	fatorAmplificacao (_fatorAmplificacao),
	dimensaoPixel (_dimensaoPixel),
	numeroPixeisBorda (_numeroPixeisBorda)
{
	cout.setf (ios::right);
	cout.precision (18);
}

// Destrutor
CSimPermeabilidadeRede::~CSimPermeabilidadeRede () {
	/* Analizar se pode deletar, se não tem dois ponteiros para mesmo bloco, esta deletando
	 * o objeto, setando o ponteiro local como null, mas o ponteiro externo continua diferente
	 * de null, e vai deletar novamente o ponteiro externo.
	if ( fluido != nullptr )
		{ delete fluido; fluido = nullptr; }
	if ( solver != nullptr )
		{ delete solver; solver = nullptr; }
	if ( rede != nullptr )
		{ delete rede; rede = nullptr; }
	*/
}

//	operator<<
ostream & operator<< (ostream & os, const CSimPermeabilidadeRede & obj) {
	os << "\n=====Dados Fluido=====\n";
	os << * ( obj.fluido );
	os << "\n=====Dados Solver=====\n";
	os << * ( obj.solver ); //aqui -> implementar!! solver não esta mostrando dados na tela!
	os << "\n=====Dados grafo=====\n";
	os << * ( obj.rede );
	os << "\n=====Dados nx, ny, nz=====\n";
	os << obj.nx << endl;
	os << obj.ny << endl;
	os << obj.nz << endl;
	os << "\n=====Dados fatorAmplificacao; dimensaoPixel; numeroDePixeisDaBorda; fluxoFronteira=====\n";
	os << obj.fatorAmplificacao << endl;
	os << obj.dimensaoPixel << endl;
	os << obj.numeroPixeisBorda << endl;
	os << obj.fluxoFronteira << endl;
	return os;
}

void CSimPermeabilidadeRede::CriarObjetosAgregados (){

}

/* ------------------------ Usando o units:---------------------
		You have: 1 atm - > You want: Pa
		* 101325
		/ 9.8692327e-06
		Logo: 1 atm = 101325 Pa
*/
// Define as condicoes de contorno
void CSimPermeabilidadeRede::DefinirCondicoesContorno () {
	// Uma atmosfera
	long double pressao_face_esquerda { 1.0 };
	long double pressao_face_direita { 0.0 };

	// Criando contorno esquerdo = 0;
	CContornoCentro *contorno_esquerdo = new CContornoCentro ();
	assert (contorno_esquerdo);
	contorno_esquerdo->ValorContorno ( pressao_face_esquerda );
	rede->contorno.push_back ( contorno_esquerdo );

	// Calculando parâmetros para contorno de centro
	// O objeto contorno de centro, tem uma funcao Go que estima os valores iniciais (de pressão)
	// de rede->objeto[i]->x usando uma reta, agora passo os coeficientes desta reta.
	// Coeficiente a da reta y = a + b.x
	long double a = pressao_face_esquerda;

	// Coeficiente b da reta y = a + b.x
	// Para calcular b preciso das pressoes a esquerda e a direita e do valor do maior plano pmax,
	// o ultimo objeto tem armazenado o valor de pmax em x
	// determina o umtimo objeto
	unsigned long int ultimoObjeto = rede->ptrMatObjsRede->matrizObjetos.size() - 1;

	// determina o valor de pmax
	long double pmax = (rede->ptrMatObjsRede->matrizObjetos[ultimoObjeto].x);

	// determina o valor de b
	long double b = (pressao_face_direita - pressao_face_esquerda) / pmax;

	// Criando contorno de centro = 1
	CContornoCentro *contorno_centro = new CContornoCentro (a, b);
	assert (contorno_centro);
	rede->contorno.push_back (contorno_centro);
	*contorno_centro = pressao_face_esquerda;	// vai ser calculado com a chamada a Go(k)

	// Criando contorno direito = 2
	CContornoCentro *contorno_direito = new CContornoCentro ();
	assert (contorno_direito);
	rede->contorno.push_back (contorno_direito);
	*contorno_direito = pressao_face_direita;
}

// Definicao de Valores Iniciais
void CSimPermeabilidadeRede::DefinirCondicoesIniciais () {
	// Para todos os objetos do rede associa valores iniciais de pressão
	unsigned long int numeroObjetos = rede->ptrMatObjsRede->matrizObjetos.size();

	// Percorre todos os objetos do rede, de define valores iniciais de x (pressão)
	// CContorno::WEST     CContorno::CENTER;      CContorno::EST;
	for (unsigned long int k = 0; k < numeroObjetos; ++k) {
		// Para os objetos do centro chama Go, que usa uma reta para estimar valor inicial de x (pressão).
		if (rede->ptrMatObjsRede->matrizObjetos[k].Contorno() == CContorno::ETipoContorno::CENTER)	// 1
			rede->ptrMatObjsRede->matrizObjetos[k].x = rede->contorno[1]->Go(rede->ptrMatObjsRede->matrizObjetos[k].x,0.0);
		// Se contorno=CContorno::WEST  objeto esta na esquerda	// 0
		else if (rede->ptrMatObjsRede->matrizObjetos[k].Contorno () == CContorno::ETipoContorno::WEST,0.0)
			rede->ptrMatObjsRede->matrizObjetos[k].x = (*(rede->contorno[0]));
		// Se contorno=CContorno::EST objeto esta na direita	// 2
		else
			rede->ptrMatObjsRede->matrizObjetos[k].x = (*(rede->contorno[2]));
	}

	// Transforma as propriedades raioHidraulico em condutancias
	// o calculo das condutancias agora é realizado na propria rede
	//rede->CalculoCondutancias (fluido->Viscosidade (), dimensaoPixel, fatorAmplificacao);

	// No rede ocorrem conjunto de sítios com mais de uma ligação entre sí, posso eliminar
	// os links duplicado, somando as suas condutâncias, o que é feito na funcao EliminarCondutanciasRepetidas
	//rede->EliminarCondutanciasRepetidas ();

	// Determina parâmetros necessários ao calculo da permeabilidade
	diferencaPressao = (*(rede->contorno[0])) - (*(rede->contorno[2]));
	dimensaoX = (nx - numeroPixeisBorda) * fatorAmplificacao * dimensaoPixel;
	dimensaoY = (ny - numeroPixeisBorda) * fatorAmplificacao * dimensaoPixel;
	dimensaoZ = (nz - numeroPixeisBorda) * fatorAmplificacao * dimensaoPixel;

	// Como as pressões estão no meio de cada nó,
	// O comprimento a ser considerado deve descontar 1 pixel
	comprimento_z = (nz - 2 * numeroPixeisBorda - 1) * fatorAmplificacao * dimensaoPixel;
	area = dimensaoY * dimensaoX;
	iteracoes = 1;
}

// Solucao do Sistema de Equações
void CSimPermeabilidadeRede::SolucaoSistemaEquacoes ()
{ // Pega ponteiro para vetor do tipo CSolverMatriz_ParametroSolver*
	vector< CSolverMatriz_ParametroSolver * > * ptr_obj = new vector< CSolverMatriz_ParametroSolver * >();
	for ( auto e : rede->ptrMatObjsRede->matrizObjetos ){
		ptr_obj->push_back( static_cast< CSolverMatriz_ParametroSolver * > (&(e.second)) );
	}
	long double erroSolver = solver->Go (ptr_obj);

	cout << "\nIts[" << setw (4) << solver->Iteracoes ()
			 << "] LEs[" << setw (10) << solver->LimiteErro ()
			 << "] Es[" << erroSolver
			 << "] Erro[" << setw (10) << solver->Erro () << "]" << endl;
}

// Next - Calcula a permeabilidade da rede.
/* Calculo:
	fluxo = (permeabilidade * area * diferencaPressao) / (viscosidade * comprimento)
	permeabilidade = (fluxo * viscosidade * comprimento) / (area * diferencaPressao)
	LIANG:
		fatorConversao* (fluxo*viscosidade*comprimento) / (area * diferencaPressao)
		Permeability= 1.013*1.0e+15*    (Q    *Viscosity) / (Length*(NX-4)*n  *1e5) ;
		fatorConversaoParaMiliDarcy = 1.013*1e+15
		comprimento = (nx - 4) * fatorAmplificacao*dimensaoPixel
		area        = (ny - 4) * fatorAmplificacao*dimensaoPixel * (nz - 4) * fatorAmplificacao * dimensaoPixel
		permeabilidade = (fluxo * viscosidade * comprimento) / (area * diferencaPressao)

	Pag. 136 Dynamics of fluids do Bear
	Permeabilidade[darcy] = fluxo[cm/seg]*viscosidade[centipoise]*dx[cm] / area[cm] * dp [atm]
*/
long double CSimPermeabilidadeRede::Next () {
	// 0)Solucao do sistema como um todo
	// 2-Processo iterativo,
	// determina o erro em funcao dos fluxos esquerdo e direito
	long double permEsq = 0.0;
	long double permDir = 0.0;
	{	// 2.1-Chamando funcao de solução do sistema de equacoes
		SolucaoSistemaEquacoes ();

		// 2.2) Determina o fluxo na fronteira solicitada
		long double fluxoe = FluxoFronteira (CContorno::ETipoContorno::WEST);
		long double fluxod = FluxoFronteira (CContorno::ETipoContorno::EST);

		// 2.3) Calcula a permeabilidade
		permEsq = (fluxoe * fluido->Viscosidade() * comprimento_z) / (area * diferencaPressao);
		permDir = (fluxod * fluido->Viscosidade() * comprimento_z) / (area * diferencaPressao);

		// permeabilidade media
		permeabilidade = (permEsq - permDir) / 2.0;	// tem sinais contrarios
		permeabilidade = permeabilidade > 0.0 ? permeabilidade : -permeabilidade;

		// Calcula o erro percentual, que deve ser < 5 %
		long double dp = permDir + permEsq;
		dp = dp > 0 ? dp : -dp;
		erroRelativo = 100.0 * dp / permeabilidade;

		/*
			 2.4) Conversoes
			 // 2.4) Converte para miliDarcy
			 A permeabilidade foi calculada usando valores do sistema internacional,
			 logo, a permeabilidade esta em m.
			 Para converter para o formato padrão da área, isto é, miliDarcy
			 ------------------------ Usando SolucaoSistemaEquacoeso units:---------------------
			 You have: m*m -> You want: mdarcy
			 * 1.01325e+15
			 / 9.8692327e-16
			 Logo: 1 m =  1.01325e+15 miliDarcy
			 Ou seja, precisa multiplicar a permeabilidade por 1.01325e+15 (ou dividir por 9.8692327e-16)
			 fatorConversao *= 1.01325e+15;

			 // antes: long double fatorConversao =  101325.0 /  9.87e-11;// =1.0266e15
		 */
		long double fatorConversao = 1.01325e+15;
		permeabilidade = fatorConversao * permeabilidade;

		// SAIDA RESULTADOS TELA E DISCO
		cout.precision (4);
		cout << " QE=" << setw (5) << fluxoe << " QD=" << setw (5) << fluxod;
		cout.precision (18);
		cout << " p(mD)=" << setw (12) << permeabilidade << " Ep=" << setw (4)
				 << erroRelativo << "%  Itp=" << iteracoes << endl;
	}
	// 1-Incrementa o numero de iteracoes
	iteracoes++;

	return permeabilidade;
}

// Go - Chama Next até que o sistema esteja resolvido ou iteracoes tenha ultrapassado o limite
long double CSimPermeabilidadeRede::Go () {
	// Solicita propriedades do solver relativo a permeabilidade
	unsigned long int limiteIteracoes = 5000;
	long double limiteErro = 5.0; //5%

	ifstream finsolver ("input_data/solver_permeabilidade.info");
	string msg = " ...nÃo abriu o arquivo de disco, usando valores default";
	if (finsolver.good ()) {
		finsolver >> limiteIteracoes;
		finsolver >> limiteErro;
		finsolver.close ();
		msg += " ...done";
	}
	cout << "\nInformacoes do SOLVER das permeabilidades" << endl;
	cout << " limIt[" << setw (4) << limiteIteracoes
			 << "] limErro[" << setw (10) << limiteErro << "]" << endl;

	cout << "Informacoes do SOLVER das pressões" << endl;
	cout << " limIt[" << setw (4) << solver->LimiteIteracoes()
			 << "] limErro[" << setw (10) << solver->LimiteErro() << "]" << endl;

	cout << "Chamando SolucaoSistema (solver->Go)..." << endl;

	//int numeroDivergencias = 0;
	long double ErroAnterior;
	do {
		ErroAnterior = erroRelativo;
		Next ();
		//if (erroPermeabiliNextdade > ErroAnterior)
		//	numeroDivergencias++;
	} while (// verifica o erro 5%
					 erroRelativo > limiteErro &&
					 // verifica limiteIteracoes
					 iteracoes < limiteIteracoes
					 // apos 100 passos divergindo, para
					 /*&& numeroDivergencias < 100 */ );

	erro = erroRelativo; // novo, seta o erro final // vai ser usado no Imago

	return permeabilidade;
}

// FluxoFronteira - Determina o fluxo nos sítios da fronteira
// Pode e deve ser otimizada, pois não precisa varrer todo a rede.
// Durante o calculo da rede, anotar os nós de cada face, e criar funcao que retorna lista dos nós de cada face.
long double CSimPermeabilidadeRede::FluxoFronteira (CContorno::ETipoContorno tipoFronteira) {
	long double fluxos = 0.0;
	// Para todos os objetos da rede
	for ( auto & mo : rede->ptrMatObjsRede->matrizObjetos ) {
		// verificar se é um objeto com a fronteira solicitada
		if (mo.second.Contorno () == tipoFronteira) { // se afirmativo, calcula o fluxo na fronteira e acumula
			fluxos += mo.second.Fluxo ( &(rede->ptrMatObjsRede->matrizObjetos) );
		}
	}
	return fluxos;
}
