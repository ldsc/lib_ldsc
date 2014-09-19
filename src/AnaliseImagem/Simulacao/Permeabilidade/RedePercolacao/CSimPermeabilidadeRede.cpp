// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Simulacao/Permeabilidade/RedePercolacao/CSimPermeabilidadeRede.h>
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_SOR.h>
#include <MetNum/Contorno/CContornoCentro.h>

using namespace std;

// Construtor
CSimPermeabilidadeRede::CSimPermeabilidadeRede (
		CMFluido * &_fluido,
		CSolverMatrizDiagonalDominante *& _solver,
		CContornoRedeDePercolacao *&_rede,
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
	std::cout << "\n==>Definindo condições de contorno..." << std::endl;
	rede->contorno.clear();
	// Uma atmosfera
	long double pressao_face_esquerda { 1.0 };
	long double pressao_face_direita { 0.0 };

	std::cout << "--->Definindo coeficientes da reta y = (a + b) * x" << std::endl;
	// Calculando parâmetros para contorno de centro
	// O objeto contorno de centro, tem uma funcao Go que estima os valores iniciais (de pressão)
	// de rede->objeto[i]->x usando uma reta, agora passo os coeficientes desta reta.
	// Coeficiente a da reta y = a + b.x
	long double a = pressao_face_esquerda;

	// Coeficiente b da reta y = a + b.x
	// Para calcular b preciso das pressoes a esquerda e a direita e do valor do maior plano pmax,
	// o ultimo objeto (sítio) tem armazenado o valor de pmax em x
	long double pressaoMaxima = rede->ptrMatObjsRede->matrizObjetos.rbegin()->second.pontoCentral.x;

	// determina o valor de b
	long double b = (pressao_face_direita - pressao_face_esquerda) / pressaoMaxima;

	std::cout << "\n--->a = " << a;
	std::cout << "\n--->b = " << b;
	std::cout << "\n--->PressaoMaxima = " << pressaoMaxima;
	std::cout << "\n--->PressaoFaceEsquerda = " << pressao_face_esquerda;
	std::cout << "\n--->PressaoFaceDireita = " << pressao_face_direita << std::endl;

	// Criando contorno esquerdo (contorno[0])
	CContornoCentro *contorno_esquerdo = new CContornoCentro ();
	assert (contorno_esquerdo);
	contorno_esquerdo->ValorContorno ( pressao_face_esquerda );
	rede->contorno.push_back ( contorno_esquerdo );

	// Criando contorno de centro  (contorno[1] é o centro)
	CContornoCentro *contorno_centro = new CContornoCentro (a, b);
	assert (contorno_centro);
	contorno_centro->ValorContorno ( pressao_face_esquerda );
	rede->contorno.push_back (contorno_centro);

	// Criando contorno direito (contorno[2])
	CContornoCentro *contorno_direito = new CContornoCentro ();
	assert (contorno_direito);
	contorno_direito->ValorContorno ( pressao_face_direita );
	rede->contorno.push_back (contorno_direito);
}

// Definicao de Valores Iniciais
void CSimPermeabilidadeRede::DefinirCondicoesIniciais () {
	std::cout << "\\n==>Definindo condicoes iniciais..." << std::endl;
	std::cout << "--->Percorre todos os objetos do rede, e define valores iniciais de x (pressao)" << std::endl;
	// CContorno::WEST     CContorno::CENTER;      CContorno::EST;
	for (auto &objeto : rede->ptrMatObjsRede->matrizObjetos) {
		// Para os objetos do centro chama Go, que usa uma reta para estimar valor inicial de x (pressão).
		if (objeto.second.Contorno() == CContorno::ETipoContorno::CENTER)	// 1
			objeto.second.x = rede->contorno[1]->Go(objeto.second.pontoCentral.x);
		else if (objeto.second.Contorno () == CContorno::ETipoContorno::WEST)// 0 - Se contorno=CContorno::WEST  objeto esta na esquerda
			objeto.second.x = rede->contorno[0]->ValorContorno();
		else 	// 2 - Se contorno=CContorno::EST objeto esta na direita
			objeto.second.x = rede->contorno[2]->ValorContorno();
		/*
		std::cout << "Obj: " << objeto.first;
		std::cout << " x: " << objeto.second.pontoCentral.x;
		std::cout << " y: " << objeto.second.pontoCentral.y;
		std::cout << " z: " << objeto.second.pontoCentral.z;
		std::cout << " t: " << objeto.second.Tipo();
		std::cout << " p: " << objeto.second.x << "\n";
		*/
	} //CObjetoRedeDePercolacao::Tipo()
	std::cout << std::endl;

	// Determina parâmetros necessários ao calculo da permeabilidade
	diferencaPressao = rede->contorno[0]->ValorContorno() -  rede->contorno[2]->ValorContorno();
	dimensaoX = rede->nx * fatorAmplificacao * dimensaoPixel;
	dimensaoY = rede->ny * fatorAmplificacao * dimensaoPixel;
	dimensaoZ = rede->nz * fatorAmplificacao * dimensaoPixel;

	//comprimento_z = (nz - 1 - (2 * numeroPixeisBorda)) * fatorAmplificacao * dimensaoPixel;
	comprimento_z = dimensaoZ;
	area = dimensaoX * dimensaoY;
	iteracoes = 0;
	std::cout << "--->diferencaPressao: " << diferencaPressao << std::endl;
	std::cout << "--->dimensaoX: " << dimensaoX << std::endl;
	std::cout << "--->dimensaoY: " << dimensaoY << std::endl;
	std::cout << "--->dimensaoZ: " << dimensaoZ << std::endl;
	std::cout << "--->comprimento_z: " << comprimento_z << std::endl;
	std::cout << "--->area: " << area << std::endl;
}

// Solucao do Sistema de Equações
void CSimPermeabilidadeRede::SolucaoSistemaEquacoes () {
	std::cout << "\n==>Solucionando sistema de equacoes..." << std::endl;
	//if (setouVetorObjetos == false) { // se ainda não setou o vetor de objetos que será passado para o solver
		int sizeOfMatObjs = rede->ptrMatObjsRede->matrizObjetos.size();
		objs.clear();
		objs.resize( sizeOfMatObjs );
		std::cout << "--->Objetos enviados para o solver:" << std::endl;
		for ( int i = 0; i < sizeOfMatObjs; ++i ) {
			objs[i] = &(rede->ptrMatObjsRede->matrizObjetos[i+1]);
			std::cout << "\n---->Obj: " << i << " x = " << objs[i]->X();
		}
		std::cout << std::endl;
		setouVetorObjetos = true;
	//}
	std::cout << "--->Executando solver->Go()..." << std::endl;
	long double erroSolver = solver->Go(&objs);

	// Mostra estado atual do sistema de solução da permeabilidade.
	std::cout << "SolucaoSistemaEquacoes() [Pressões]:\n"
						<< "solver->Iteracoes["			<< std::setw (  5 ) 	<< solver->Iteracoes ()
						<< "] solver->LimiteErro["	<< std::setw ( 10 ) 	<< solver->LimiteErro ()
						<< "] solver->Erro["				<< std::setw ( 10 ) 	<< solver->Erro ()
						<< "] solver->Go["					<< erroSolver 	<< "]" << endl;
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
	// 0-Solucao do sistema como um todo
	// 1-Incrementa o numero de iteracoes
	++iteracoes;
	// 2-Processo iterativo, determina o erro em funcao dos fluxos esquerdo e direito
	long double permEsq {0.0};
	long double permDir {0.0};
	{	// 2.1-Chamando funcao de solução do sistema de equacoes
		//SolucaoSistemaEquacoes ();

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
		cout << "Next() [Vazões e permeabilidade]:\n"
				 << "Qe["							<< setw (  5 ) << fluxoe
				 << "] Qd["						<< setw (  5 ) << fluxod;
		cout.precision ( 18 );
		cout  << "] p(mD)["				<< setw ( 12 ) << permeabilidade
					<< "] erroRelativoPer["	<< setw (  5 ) << erroRelativo << "% "
					<< "] Iteracoes["					<< setw (  5 ) << iteracoes << "]" << endl;
	}
	return permeabilidade;
}

// Go - Chama Next até que o sistema esteja resolvido ou iteracoes tenha ultrapassado o limite
long double CSimPermeabilidadeRede::Go () {
	// Se o sistema ainda não foi resolvido, chama SolucaoSistema(); que chama os métodos:
	// CriarObjetosAgregados (); DefinirCondicoesContorno (); DefinirCondicoesIniciais (); SolucaoSistemaEquacoes ();
	if ( sistemaResolvido == false )
		SolucaoSistema();						// Resolve todo o sistema, incluindo as pressões, faz sistemaResolvido=true

	Next ();											// Calcula vazões e permeabilidade.

	while ( erroRelativo > limiteErroRelativo && iteracoes < limiteIteracoes ) { // Se ainda não convergiu.
		SolucaoSistemaEquacoes ();	// Refina cálculo das pressões
		Next ();										// Recalcula e verifica vazões e pressões.
	}

	return permeabilidade;
}

// FluxoFronteira - Determina o fluxo nos sítios da fronteira
// Pode e deve ser otimizada, pois não precisa varrer todo a rede.
// Durante o calculo da rede, anotar os nós de cada face, e criar funcao que retorna lista dos nós de cada face.
long double CSimPermeabilidadeRede::FluxoFronteira (CContorno::ETipoContorno tipoFronteira) {
	long double fluxo_fronteira {0.0};
	// Para todos os objetos da rede
	for ( auto objeto_i : rede->ptrMatObjsRede->matrizObjetos ) {
		// verificar se é um objeto com a fronteira solicitada
		if ( objeto_i.second.Contorno () == tipoFronteira )
			// se afirmativo, calcula o fluxo na fronteira e acumula
			fluxo_fronteira += objeto_i.second.Fluxo (&(rede->ptrMatObjsRede->matrizObjetos));
	}

	return fluxo_fronteira;
}

//	operator<<
ostream & operator<< (ostream & os, const CSimPermeabilidadeRede & obj) {
	os << "\n=====Dados Fluido=====\n";
	os << * ( obj.fluido );
	os << "\n=====Dados Solver=====\n";
	os << * ( obj.solver ); //aqui -> implementar!! solver não esta mostrando dados na tela!
	os << "\n=====Dados Cálculo Permeabilidade (Lei Darcy)=====\n";
	os << "Fluxo fronteira: " << obj.fluxoFronteira << endl;
	os << "Diferença pressão: " << obj.diferencaPressao << endl;
	os << "DimensaoX: " << obj.dimensaoX << endl;
	os << "DimensaoY: " << obj.dimensaoY << endl;
	os << "DimensaoZ: " << obj.dimensaoZ << endl;
	os << "Comprimento_z: " << obj.comprimento_z << endl;
	os << "Area: " << obj.area << endl;
	os << "\n=====Dados Imagem=====\n";
	os << "nx: " << obj.nx << endl;
	os << "ny: " << obj.ny << endl;
	os << "nz: " << obj.nz << endl;
	os << "Fator amplificação: " << obj.fatorAmplificacao << endl;
	os << "Dimensão pixel: " << obj.dimensaoPixel << endl;
	os << "Num. pixeis borda: " << obj.numeroPixeisBorda << endl;
	os << "\n=====Dados Rede=====\n";
	os << * ( obj.rede );
	return os;
}
