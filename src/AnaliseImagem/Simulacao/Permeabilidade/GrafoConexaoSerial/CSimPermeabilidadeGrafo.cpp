/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
===============================================================================
Desenvolvido por:
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:        Andre Duarte Bueno
@file:          CSimPermeabilidadeGrafo.cpp
@begin:         Sat Sep 16 2000
copyright:      (C) 2000 by Andre Duarte Bueno
@email:         andreduartebueno@gmail.com
*/

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
#include <AnaliseImagem/Simulacao/Permeabilidade/GrafoConexaoSerial/CSimPermeabilidadeGrafo.h>
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_SOR.h>
#include <MetNum/Contorno/CContornoCentro.h>

using namespace std;

// -------------------------------------------------------------------------
// Funcao: Construtor
// -------------------------------------------------------------------------
/**
@short  : Construtor da classe CSimPermeabilidadeGrafo
@author : Andre Duarte Bueno
	Construtores
@param  : objetos fluido,solver,grafo, nx,ny,nz, fator amplificacao, tamanhoDoPixel, numeroDePixeisDaBorda
*/
CSimPermeabilidadeGrafo::CSimPermeabilidadeGrafo (
		CMFluido*& _fluido,
		CSolverMatrizDiagonalDominante*& _solver,
		CGrafoConexaoSerial*& _grafo,
		unsigned long int _nx,
		unsigned long int _ny,
		unsigned long int _nz,
		unsigned long int _fatorAmplificacao,
		long double _dimensaoPixel,
		unsigned long int _numeroPixeisBorda
		) :
	fluido ( _fluido ),
	solver ( _solver ),
	grafo ( _grafo ),
	nx ( _nx ),
	ny ( _ny ),
	nz ( _nz ),
	fatorAmplificacao ( _fatorAmplificacao ),
	dimensaoPixel ( _dimensaoPixel ),
	numeroPixeisBorda ( _numeroPixeisBorda )
{
	cout.setf ( ios::left );
	cout.precision ( 18 );
}

/*
-------------------------------------------------------------------------
Funcao:		Construtor de copia
-------------------------------------------------------------------------
@short  : Constróe objeto  igual ao passado como parâmetro
@author : Andre Duarte Bueno
	Construtor de cópia
@param  : CSimPermeabilidadeGrafo&
@return : nada
*/
/*
CSimPermeabilidadeGrafo::CSimPermeabilidadeGrafo(const CSimPermeabilidadeGrafo& obj )
{
	fluido 	=	new 	CMFluido(*(obj.fluido));
	assert(fluido);

	// Abaixo cria objeto SMDiagonalDominante (uma heranca)
	solver 	=	new  	SMDiagonalDominante (*(obj.solver));
	assert(solver);

	grafo 	=	new 	Tgrafo  (*(obj.grafo));
	assert(grafo);
	nx			=	obj.nx;
	ny			=	obj.ny;
	nz			=	obj.nz;
	fatorAmplificacao	=	obj.fatorAmplificacao;
	dimensaoPixel					=	obj.dimensaoPixel;
	numeroPixeisBorda	=	obj.numeroPixeisBorda;
	fluxoFronteira		=	obj.fluxoFronteira;

	// Da class CSimPermeabilidade
	// this = (CSimPermeabilidade) obj;
	permeabilidade = obj.permeabilidade;

	// Da classe TSimulação
	sistemaResolvido = obj.sistemaResolvido;
}
*/

// -------------------------------------------------------------------------
// Funcao:       Destrutor
// -------------------------------------------------------------------------
/**
@short  : Destrói o objeto e deleta objetos agregados
@author :	Andre Duarte Bueno
	Destrutores
@param  : nada
@return : nada
*/
CSimPermeabilidadeGrafo::~CSimPermeabilidadeGrafo ()
{
	/* RETIRADO EM 14/10/2009 com Leandro - verificar se pode deletar, se
	 não tem dois ponteiros para mesmo bloco, esta deletando o objeto, setando o ponteiro local como null,
	 mas o ponteiro externo continua diferente de null,
	 e vai deletar novamente o ponteiro externo.

	 if ( fluido != nullptr )
		{ delete fluido; fluido = nullptr; }
	 if ( solver != nullptr )
		{ delete solver; solver = nullptr; }
	 if ( grafo != nullptr )
		{ delete grafo; grafo = nullptr; }
		*/
}

// ----------------------------------------------------------------------------
// OPerador igualdade (atribuição)
// ----------------------------------------------------------------------------
/*
CSimPermeabilidadeGrafo& CSimPermeabilidadeGrafo::operator= (const CSimPermeabilidadeGrafo&
aCSimPermeabilidadeGrafo)
{
	if (this == &aCSimPermeabilidadeGrafo) return *this;
	// int i = 0;
	CSimPermeabilidade::operator= (aCSimPermeabilidadeGrafo);	// ancestral
	fluido = 	aCSimPermeabilidadeGrafo.fluido;
	fatorAmplificacao = aCSimPermeabilidadeGrafo.fatorAmplificacao;
	numeroPixeisBorda = aCSimPermeabilidadeGrafo.numeroPixeisBorda;
	comprimentoPixel  = aCSimPermeabilidadeGrafo.comprimentoPixel;
	solver = 	aCSimPermeabilidadeGrafo.solver;

	fluido 	=	new 	CMFluido(*(obj.fluido));
	assert(fluido);
	// Abaixo cria objeto SMDiagonalDominante, mas o obj.solver pode ser um CSolverMatrizDiagonal_SOR
	// ou seja, tem de criar funcao em todos os objeto com hierarquia com o nome
	// GetCopy, uso: obj->GetCopy();
	solver 	=	new  	SMDiagonalDominante (*(obj.solver));
	assert(solver);
	grafo 	=	new 	Tgrafo  (*(obj.grafo));
	return *this;
}
*/

// ----------------------------------------------------------------------------
// OPerador comparação igualdade
// ----------------------------------------------------------------------------
/*int CSimPermeabilidadeGrafo::operator== (const CSimPermeabilidadeGrafo& aCSimPermeabilidadeGrafo) const
{
 return (
 // Verifica classe base
 (CSimPermeabilidade::operator== (aCSimPermeabilidadeGrafo)) &&

 // Verifica atributos simples
 (fatorAmplificacao ==  aCSimPermeabilidadeGrafo.fatorAmplificacao) &&
 (numeroPixeisBorda == 		aCSimPermeabilidadeGrafo.numeroPixeisBorda) &&
 (comprimentoPixel == 	aCSimPermeabilidadeGrafo.comprimentoPixel) &&
 // Verifica os objetos agregados, usando == de cada objeto
 (*fluido == 				*aCSimPermeabilidadeGrafo.fluido) &&
 (*solver == 		*aCSimPermeabilidadeGrafo.solver) &&
 (*grafo == 	*aCSimPermeabilidadeGrafo.grafo)
 );
}
*/

/*
-------------------------------------------------------------------------
Funcao:  	operator<<
-------------------------------------------------------------------------
@short  :	Sobrecarga operador saída stream
@author :	Andre Duarte Bueno

@param  :	ostream& os, const CSimPermeabilidadeGrafo& obj
@return :	ostream&
*/
ostream& operator<< ( ostream& os, const CSimPermeabilidadeGrafo& obj )
{
	os << "\n=====Dados Fluido=====\n";
	os << * ( obj.fluido );
	os << "\n=====Dados Solver=====\n";
	os << * ( obj.solver ); //aqui -> implementar!! solver não esta mostrando dados na tela!
	os << "\n=====Dados grafo=====\n";
	os << * ( obj.grafo );
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

// -------------------------------------------------------------------------
// Funcao:               operator>>
// -------------------------------------------------------------------------
/**
@short  :		Sobrecarga operador entrada streams
@author :		Andre Duarte Bueno

@param  : 	istream& is, CSimPermeabilidadeGrafo& p
@return :		istream&
*/
/*istream& operator>> (istream& is, CSimPermeabilidadeGrafo& p)
{
	is >> p.fatorAmplificacao;
	is >> p.numeroPixeisBorda;
	is >> p.dimensaoPixel;
	is >> p.fluxoFronteira;

	// p.fluido->Read("fluido.dat");
	// Lê os dados do objeto do arquivo de disco
	// 	ifstream fin("fluido.dat");
	//   fin >> (*p.fluido);
	// 	fin.close();
	// p.solver->Read("solver.dat");
	// p.grafo->Read("grafo.dat");

	ou usar:
	ifstream fin();
	fin.open("fluido.dat");  fin >> *p.fluido ;  			fin.close();
	fin.open("solver.dat");  fin >> *p.solver ;  			fin.close();
	fin.open("grafo.dat");   fin >> *p.grafo ; fin.close();
	return is;
}
*/

// -------------------------------------------------------------------------
// Funcao:               Read
// -------------------------------------------------------------------------
/**
@short  : Lê atributos do objeto do arquivo de disco;
		Recebe nome do arquivo de disco
@author : Andre Duarte Bueno

@param  : char* nomeArquivo
@return : void
*/
/*
void CSimPermeabilidadeGrafo::Read(string nomeArquivo)
{
	ifstream fin(nomeArquivo.c_str());	// Abre o arquivo de disco
	if(fin)                               // se ok
		fin>>*this;                     // Envia dados para este objeto
 //  else
 //   {
 //  	  	cout << "\nEnter Object Attribute Values or 0 - Class CSimPermeabilidadeGrafo";
 // 		cout<<"\nEnter fatorAmplificacao: ";
 // 		cin>>fatorAmplificacao;
 // 	   cout<<"\nEnter numeroPixeisBorda: ";
 //  		cin>>numeroPixeisBorda;
 // 		cout<<"\nEnter comprimentoPixel: ";
 //  		cin>>comprimentoPixel;
 //      cin.get();
			// falta ler informacoes objetos agregados
// 	  fluido->Read("fluido.dat");
// Lê os dados do objeto do arquivo de disco
// 	  solver->Read("solver.dat");
// 	  grafo->Read();

//    }
}
*/

// -------------------------------------------------------------------------
// Funcao:       CriarObjetosAgregados ()
// -------------------------------------------------------------------------
/**
@short  :	Cria objetos agregados, herdada da CSimulacao e obrigatoriamente definida  (é virtual na classe base).
			Não esta fazendo nada, pois todos os objetos agregados foram recebidos no construtor da classe.
@author :	Andre Duarte Bueno

@param  :
@return : void
*/
void
CSimPermeabilidadeGrafo::CriarObjetosAgregados ()
{
	/*
	 if(fluido)
	 delete fluido;
		fluido =new CMFluido(0.001002);
	 assert(fluido);

	 if(solver)
	 delete solver;
	 solver = new  CSolverMatrizDiagonal_SOR ();
	 assert(solver);

	 if(grafo)
	 delete grafo;
	 grafo = new CGrafoConexaoSerial ();
		assert(grafo);
	 */
}

// -------------------------------------------------------------------------
// Funcao:DefinirCondicoesContorno()
// -------------------------------------------------------------------------
/**
@short  : Usada para definir as condições de contorno
@author : Andre Duarte Bueno

@param  : nada
@return : void
*/
/*
	------------------------ Usando o units:---------------------
		You have: 1 atm - > You want: Pa
		* 101325
		/ 9.8692327e-06
		Logo: 1 atm = 101325 Pa
*/
void
CSimPermeabilidadeGrafo::DefinirCondicoesContorno ()
{
	// Uma atmosfera
	long double pressao_face_esquerda { 1.0 };
	long double pressao_face_direita { 0.0 };

	// Criando os objetos de contorno:
	// Criando contorno esquerdo e adicionando o mesmo ao grafo (contorno[0] é a face esquerda).
	CContornoCentro* contorno_esquerdo = new CContornoCentro ();
	assert ( contorno_esquerdo );
	contorno_esquerdo->ValorContorno ( pressao_face_esquerda );
	grafo->contorno.push_back ( contorno_esquerdo );

	// Calculando parâmetros para contorno de centro.
	// O objeto contorno_centro tem uma funcao Go() que estima os valores iniciais (de pressão)
	// para os objetos do grafo que estão localizados no centro.
	// Ou seja, o valor inicial de pressão em grafo->objeto[i]->x é estimado usando uma reta;
	// Aqui eu defino os valores dos coeficientes a e b desta reta (y = a + b.x).
	long double a = pressao_face_esquerda;

	// Coeficiente b da reta y = a + b.x
	// Para calcular b preciso das pressoes a esquerda e a direita e do valor do maior plano pressaoMaxima,
	// o ultimo objeto tem armazenado o valor de pressaoMaxima em x, determina o ultimo objeto.
	unsigned long int ultimoObjeto = grafo->objeto.size () - 1;

	// determina o valor de pressaoMaxima a partir do valor do ultimo objeto do grafo. ?
	long double pressaoMaxima = ( grafo->objeto[ultimoObjeto]->x );
	cerr << "\npressaoMaxima em DefinirCondicoesContorno(): " << pressaoMaxima << endl;

	// determina o valor de b
	long double b = ( pressao_face_direita - pressao_face_esquerda ) / pressaoMaxima;

	// Criando contorno de centro  (contorno[1] é o centro)
	CContornoCentro* contorno_centro = new CContornoCentro ( a, b );
	assert ( contorno_centro );
	contorno_centro->ValorContorno ( pressao_face_esquerda );
	grafo->contorno.push_back ( contorno_centro );

	// Criando contorno direito = 2 (grafo assume controle, i.e, vai deltar os contornos)
	//  (contorno[2] é a face direita)
	CContornoCentro* contorno_direito = new CContornoCentro ();
	assert ( contorno_direito );
	contorno_direito->ValorContorno ( pressao_face_direita );
	grafo->contorno.push_back ( contorno_direito );
}

// -------------------------------------------------------------------------
// Funcao:       DefinirCondicoesIniciais
// -------------------------------------------------------------------------
/**
@short  : Para todos os objetos do grafo associa valores iniciais de pressão
@author : Andre Duarte Bueno
@param  : void
@return : void
*/
void
CSimPermeabilidadeGrafo::DefinirCondicoesIniciais ()
{
	// Para todos os objetos do grafo associa valores iniciais de pressão
	unsigned long int numeroObjetos = grafo->objeto.size ();

	// Percorre todos os objetos do grafo e define valores iniciais de x (pressão)
	// CContorno::WEST     CContorno::CENTER;      CContorno::EST;
	for ( unsigned long int k = 0; k < numeroObjetos; k++ ) {
		// Para os objetos localizados no centro do grafo chama contorno[1]->Go(),
		// que usa uma reta para estimar valor inicial de x (pressão).
		if ( grafo->objeto[k]->Contorno () == CContorno::ETipoContorno::CENTER )
			// Passa o valor de grafo->objeto[k]->x, que é o plano do objeto na imagem 3D
			// para função Go() do objeto de contorno, que vai calcular um valor inicial
			// para o grafo->objeto[k]->x.
			grafo->objeto[k]->x = grafo->contorno[1]->Go ( grafo->objeto[k]->x );

		// Se contorno=CContorno::WEST  objeto esta na face esquerda, valor inicial igual da face esquerda
		else if ( grafo->objeto[k]->Contorno () == CContorno::ETipoContorno::WEST )
			grafo->objeto[k]->x = ( * ( grafo->contorno[0] ) );

		// Se contorno=CContorno::EST objeto esta na face direita, valor inicial igual da face direita
		else
			grafo->objeto[k]->x = ( * ( grafo->contorno[2] ) );
	}

	// Pré-processamento
	// Transforma as propriedades raioHidraulico em condutâncias
	// o cálculo das condutâncias agora é realizado no proprio grafo
	grafo->CalcularCondutancias ( fluido->Viscosidade (), dimensaoPixel, fatorAmplificacao );

	// No grafo ocorrem conjunto de sítios com mais de uma ligação entre sí, posso eliminar
	// os links duplicado, somando as suas condutâncias, o que é feito na funcao EliminarConexoesParalelo_SomarCondutancias
	grafo->EliminarConexoesParalelo_SomarCondutancias ();

	// Determina parâmetros necessários ao cálculo da permeabilidade
	diferencaPressao = grafo->contorno[0]->ValorContorno() -  grafo->contorno[2]->ValorContorno();
	dimensaoX = ( nx - numeroPixeisBorda ) * fatorAmplificacao * dimensaoPixel;
	dimensaoY = ( ny - numeroPixeisBorda ) * fatorAmplificacao * dimensaoPixel;
	dimensaoZ = ( nz - numeroPixeisBorda ) * fatorAmplificacao * dimensaoPixel;

	// Como as pressões estão no meio de cada nó, o comprimento a ser considerado deve descontar 1 pixel
	// 1/2 píxel da face esquerda e 1/2 píxel da face direita.
	// também devo descontar os píxies das duas bordas
	comprimento_z = ( nz - 1 - ( 2 * numeroPixeisBorda ) ) * fatorAmplificacao * dimensaoPixel;
	area = dimensaoY * dimensaoX;
	iteracoes = 0;

	// Vai ler dados do arquivo de disco, se não conseguir abrir arquivo disco usa valores padrões.
	ifstream finsolver_permeabilidade ( "input_data/solver_permeabilidade.info" );
	string msg = " ...não abriu o arquivo de disco, usando valores default";

	if ( finsolver_permeabilidade.good () ) {
		finsolver_permeabilidade >> limiteIteracoes;
		finsolver_permeabilidade >> limiteErroRelativo;
		finsolver_permeabilidade.close ();
		msg += " ...done";
	}

	// debug
	//     ofstream fout ( "perm1_aposDefinirCondicoesIniciais.grafo" );
	//     fout << *this;
	//     fout.close();
}

// -------------------------------------------------------------------------
// Funcao:  SolucaoSistemaEquacoes()
// -------------------------------------------------------------------------
/**
@short  : Solução do Sistema de Equações, resolve as pressões.
@author : Andre Duarte Bueno

@param  : nada
@return : void
*/
void
CSimPermeabilidadeGrafo::SolucaoSistemaEquacoes ()
{
	// Pega ponteiro para vetor do tipo CSolverMatriz_ParametroSolver*
	/// @todo: Não permite static_cast nem dynamic_cast pois tirei métodos virtuais
	/// da classe CSolverMatriz_ParametroSolver. Ver como resolver!
	/// 1 - Recolocar CSolverMatriz_ParametroSolver como virtual.
	//aqui aqui aqui
	//    PROBLEMA NÃO RESOLVE NADA vector < CSolverMatriz_ParametroSolver* >* ptr_obj =    ( vector < CSolverMatriz_ParametroSolver* >* ) & ( grafo->objeto );
	vector < CSolverMatriz_ParametroSolver* > obj ( grafo->objeto.size() );
	for(int i = 0; i < grafo->objeto.size(); i++)
		obj[i] = grafo->objeto[i];

	//    vector<CSolverMatriz_ParametroSolver*>* ptr_obj = dynamic_cast<vector<CSolverMatriz_ParametroSolver*> *  > ( & ( grafo->objeto ) );

	// Resolve o sistema para as pressões.
	//    long double erroSolver = solver->Go ( ptr_obj );
	long double erroSolver = solver->Go ( &obj );

	// Usado para dar continuidade a simulação no caso de uma interrupção indesejada de energia.
	if ( salvarDadosParciaisPressaoDisco )
		grafo->SalvarVetorPropriedades_x ();

	// Mostra estado atual do sistema de solução da permeabilidade.
	cout << "SolucaoSistemaEquacoes() [Pressões]:\n"
			 << "solver->Iteracoes[" 	<< setw ( 5 ) 	<< solver->Iteracoes ()
			 << "] solver->LimiteErro[" 	<< setw ( 10 ) 	<< solver->LimiteErro ()
			 << "] solver->Erro[" 		<< setw ( 10 ) 	<< solver->Erro ()
			 << "] solver->Go[" 			<< erroSolver 	<< endl << "]" 		<< endl;
	// debug
	//    ofstream fout ( "perm2_aposSolucaoSistemaEquacoes.grafo" );
	//    fout << *this;
	//    fout.close();
}

// -------------------------------------------------------------------------
// Funcao:  Next()
// -------------------------------------------------------------------------
/**
@short  :
Calcula a permeabilidade da imagem usando o grafo.
	Calculo:
		fluxo = (permeabilidade * area * diferencaPressao)
								/
					 (viscosidade * comprimento_z)

		permeabilidade =  (fluxo * viscosidade * comprimento_z)
									/
						 (area * diferencaPressao)

		LIANG:
		// fatorConversao* (fluxo*viscosidade*comprimento_z)
									/
						(area * diferencaPressao)
		// Permeability= 1.013*1.0e+15*    (Q    *Viscosity)
									 /
							(Length*(NX-4)*n  *1e5) ;
		// fatorConversaoParaMiliDarcy = 1.013*1e+15

		comprimento_z =  (nx - 4) * fatorAmplificacao*dimensaoPixel
		area        =    (ny - 4) * fatorAmplificacao*dimensaoPixel * (nz - 4) * fatorAmplificacao * dimensaoPixel
		permeabilidade = (fluxo * viscosidade * comprimento_z)
									/
						 (area * diferencaPressao)

pg136 Dynamics of fluids do Bear
Permeabilidade[darcy] = fluxo[cm/seg]*viscosidade[centipoise]*dx[cm]
									/
							area[cm]   *  dp [atm]

@author : Andre Duarte Bueno
	permeabilidade
@param  : nada
@return : void
*/
long double
CSimPermeabilidadeGrafo::Next ()
{
	// 0)Solucao do sistema como um todo
	// 2-Processo iterativo
	// Determina o erro em função dos fluxos esquerdo e direito
	long double permEsq { 0.0 };
	long double permDir { 0.0 };
	{
		// 2.2) Determina o fluxo na fronteira solicitada
		long double fluxoe = FluxoFronteira ( CContorno::ETipoContorno::WEST );
		long double fluxod = FluxoFronteira ( CContorno::ETipoContorno::EST );

		// 2.3) Calcula a permeabilidade -> Lei de Darcy
		permEsq = ( fluxoe * fluido->Viscosidade () * comprimento_z ) / ( area * diferencaPressao );
		permDir = ( fluxod * fluido->Viscosidade () * comprimento_z ) / ( area * diferencaPressao );

		// permeabilidade média
		permeabilidade = ( permEsq - permDir ) / 2.0;	// tem sinais contrários
		permeabilidade = permeabilidade > 0.0 ? permeabilidade : -permeabilidade; // valor positivo

		// Calcula o erro percentual, que deve ser < 5 % (menor que valor definido pelo usuário).
		long double dp = permDir + permEsq;   // tem sinais contrários
		dp = dp > 0 ? dp : -dp; 				// valor positivo
		erroRelativo = 100.0 * dp / permeabilidade;

		/*
				 2.4) Conversoes, converte para miliDarcy.
				 A permeabilidade foi calculada usando valores do sistema internacional, logo, a permeabilidade esta em m.
				 Para converter para o formato padrão da área, isto é, miliDarcy
				 ------------------------ Usando programa units:-------------------------
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
		cout.precision ( 4 );
		cout << "Next() [Vazões e permeabilidade]:\n"
				 << "Qe[" 		<< setw ( 5 ) 	<< fluxoe
				 << "] Qd[" 		<< setw ( 5 ) 	<< fluxod;
		cout.precision ( 18 );
		cout  << "] p(mD)[" 	<< setw ( 12 ) 	<< permeabilidade
					<< "] erroRelPer[" << setw ( 5 ) 	<< erroRelativo << "% "
					<< "] Itp[" 		<< setw ( 5 ) 	<< iteracoes << endl;
	}
	// 1-Incrementa o numero de iteracoes
	iteracoes++;

	return permeabilidade;
}

// -------------------------------------------------------------------------
// Funcao:    Go
// -------------------------------------------------------------------------
/**
@short  : Go() chama Next() até que o sistema esteja resolvido ou iterações tenha ultrapassado o limite
@author : Andre Duarte Bueno

@param  :
@return :   long double
*/
long double CSimPermeabilidadeGrafo::Go ()
{
	// Se o sistema ainda não foi resolvido, chama SolucaoSistema(); que chama os métodos:
	// CriarObjetosAgregados (); DefinirCondicoesContorno (); DefinirCondicoesIniciais (); SolucaoSistemaEquacoes ();
	if ( sistemaResolvido == false )
		SolucaoSistema();				// Resolve todo o sistema, incluindo as pressões, faz sistemaResolvido=true

	Next ();							// Calcula vazões e permeabilidade.

	while ( erroRelativo > limiteErroRelativo &&  	// Verifica o erro relativo 1% cálculo permeabilidade.
					iteracoes < limiteIteracoes   			// Verifica limiteIteracoes.
					)   {										// Se ainda não convergiu.
		SolucaoSistemaEquacoes ();					// Refina cálculo das pressões
		Next ();										// Recalcula e verifica vazões e pressões.
	}

	return permeabilidade;
}

// -------------------------------------------------------------------------
// Funcao:    FluxoFronteira
// -------------------------------------------------------------------------
/**
@short  : Determina o fluxo nos sítios da fronteira grafo->objeto[k] retorna o fluxo.
@author : Andre Duarte Bueno
@param  : unsigned long int
@return : long double
Pode e deve ser otimizada, pois não precisa varrer todo o grafo,
durante o cálculo do grafo anotar os nós de cada face, e criar funcao que retorna lista dos nós de cada face.
*/
long double CSimPermeabilidadeGrafo::FluxoFronteira ( CContorno::ETipoContorno tipoFronteira )
{
	long double fluxo_fronteira { 0.0 };

	// Para todos os objetos do grafo
	for ( auto objeto_i : grafo->objeto )
		// verificar se é um objeto da fronteira solicitada
		if ( objeto_i ->Contorno () == tipoFronteira )
			// se afirmativo, calcula o fluxo na fronteira e acumula
			fluxo_fronteira += objeto_i->Fluxo ();

	return fluxo_fronteira;
}
