/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
									Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico
										[LDSC].
@author:          Andre Duarte Bueno
@file:             CPermeabilidadeGrafo.cpp
@begin:            Sat Sep 16 2000
copyright:        (C) 2000 by Andre Duarte Bueno
@email:            andreduartebueno@gmail.com
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
#include <AnaliseImagem/Simulacao/Permeabilidade/GrafoConexaoSerial/CPermeabilidadeGrafo.h>

#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDSOR.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoContorno.h>
#include <MetNum/Contorno/CContorno.h>
#include <MetNum/Contorno/CContornoCentro.h>

using namespace std;

// -------------------------------------------------------------------------
// Funcao: Construtor
// -------------------------------------------------------------------------
/**
@short  : Construtor da classe CPermeabilidadeGrafo
@author : Andre Duarte Bueno
@see    : Construtores
@param  : objetos fluido,solver,grafo, nx,ny,nz, fator amplificacao, tamanhoDoPixel, numeroDePixeisDaBorda
@return : nada
*/
CPermeabilidadeGrafo::CPermeabilidadeGrafo (	CMFluido * &_fluido, 
					  CSMDiagonalDominante *& _solver,
						CGra3Dby2D *& _grafo,
						unsigned long int _nx,
							unsigned long int _ny,
																							unsigned long int _nz,
																							unsigned long int _fatorAmplificacao,
																							long double _sizePixel,
																							unsigned long int _numeroPixelsBorda )
	:
		fluido (_fluido),
		solver (_solver),
		grafo (_grafo),
		nx (_nx),
		ny (_ny),
		nz (_nz),
		fatorAmplificacao (_fatorAmplificacao),
		sizePixel (_sizePixel),
		numeroPixelsBorda (_numeroPixelsBorda),
		fluxoFronteira (0)
{
	cout.setf (ios::right);
	cout.precision (18);
}

/*
-------------------------------------------------------------------------
Funcao:		Construtor de copia
-------------------------------------------------------------------------
@short  : Constróe objeto  igual ao passado como parâmetro
@author : Andre Duarte Bueno
@see    : Construtor de cópia
@param  : CPermeabilidadeGrafo&
@return : nada
*/
/*
CPermeabilidadeGrafo::CPermeabilidadeGrafo(const CPermeabilidadeGrafo& obj )
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
	sizePixel					=	obj.sizePixel;
	numeroPixelsBorda	=	obj.numeroPixelsBorda;
	fluxoFronteira		=	obj.fluxoFronteira;

	// Da class CPermeabilidade
	// this = (CPermeabilidade) obj;
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
@see    : Destrutores
@param  : nada
@return : nada
*/
CPermeabilidadeGrafo::~CPermeabilidadeGrafo ()
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
CPermeabilidadeGrafo& CPermeabilidadeGrafo::operator= (const CPermeabilidadeGrafo&
aCPermeabilidadeGrafo)
{
	if (this == &aCPermeabilidadeGrafo) return *this;
	// int i = 0;
	CPermeabilidade::operator= (aCPermeabilidadeGrafo);	// ancestral
	fluido = 	aCPermeabilidadeGrafo.fluido;
	fatorAmplificacao = aCPermeabilidadeGrafo.fatorAmplificacao;
	numeroPixelsBorda = aCPermeabilidadeGrafo.numeroPixelsBorda;
	comprimentoPixel  = aCPermeabilidadeGrafo.comprimentoPixel;
	solver = 	aCPermeabilidadeGrafo.solver;

	fluido 	=	new 	CMFluido(*(obj.fluido));
	assert(fluido);
	// Abaixo cria objeto SMDiagonalDominante, mas o obj.solver pode ser um CSMDSOR
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
/*int CPermeabilidadeGrafo::operator== (const CPermeabilidadeGrafo& aCPermeabilidadeGrafo) const
{
 return (
 // Verifica classe base
 (CPermeabilidade::operator== (aCPermeabilidadeGrafo)) &&

 // Verifica atributos simples
 (fatorAmplificacao ==  aCPermeabilidadeGrafo.fatorAmplificacao) &&
 (numeroPixelsBorda == 		aCPermeabilidadeGrafo.numeroPixelsBorda) &&
 (comprimentoPixel == 	aCPermeabilidadeGrafo.comprimentoPixel) &&
 // Verifica os objetos agregados, usando == de cada objeto
 (*fluido == 				*aCPermeabilidadeGrafo.fluido) &&
 (*solver == 		*aCPermeabilidadeGrafo.solver) &&
 (*grafo == 	*aCPermeabilidadeGrafo.grafo)
 );
} 
*/

/*
-------------------------------------------------------------------------
Funcao:  	operator<<
-------------------------------------------------------------------------
@short  :	Sobrecarga operador saída stream
@author :	Andre Duarte Bueno
@see    :
@param  :	ostream& os, const CPermeabilidadeGrafo& obj
@return :	ostream&
*/
ostream & operator<< (ostream & os, const CPermeabilidadeGrafo & obj)
{
	os << *(obj.fluido);
	os << *(obj.solver);
	os << *(obj.grafo);
	os << obj.nx << endl;
	os << obj.ny << endl;
	os << obj.nz << endl;
	os << obj.fatorAmplificacao << endl;
	os << obj.sizePixel << endl;
	os << obj.numeroPixelsBorda << endl;
	os << obj.fluxoFronteira << endl;
	return os;
}

// -------------------------------------------------------------------------
// Funcao:               operator>>
// -------------------------------------------------------------------------
/**
@short  :		Sobrecarga operador entrada streams
@author :		Andre Duarte Bueno
@see    :
@param  : 	istream& is, CPermeabilidadeGrafo& p
@return :		istream&
*/
/*istream& operator>> (istream& is, CPermeabilidadeGrafo& p)
{
	is >> p.fatorAmplificacao;
	is >> p.numeroPixelsBorda;
	is >> p.sizePixel;
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
@short  : Lê atributos do objeto do arquivo de disco
		Recebe nome do arquivo de disco
@author : Andre Duarte Bueno
@see    :
@param  : char* fileName
@return : void
*/
/*
void CPermeabilidadeGrafo::Read(string fileName)
{
	ifstream fin(fileName.c_str());	// Abre o arquivo de disco
	if(fin)                               // se ok
		fin>>*this;                     // Envia dados para este objeto
 //  else
 //   {
 //  	  	cout << "\nEnter Object Attribute Values or 0 - Class CPermeabilidadeGrafo";
 // 		cout<<"\nEnter fatorAmplificacao: ";
 // 		cin>>fatorAmplificacao;
 // 	   cout<<"\nEnter numeroPixelsBorda: ";
 //  		cin>>numeroPixelsBorda;
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
// Funcao:       CriacaoObjetosAgregados ()
// -------------------------------------------------------------------------
/**
@short  :		Cria objetos agregados, herdada da TSimulacao e obrigatoriamente
					definida  (é virtual na classe base).
								Não esta fazendo nada, pois todos os objetos agregados
					foram recebidos no construtor da classe.
@author :	Andre Duarte Bueno
@see    :
@param  :
@return : void
*/
void
CPermeabilidadeGrafo::CriacaoObjetosAgregados ()
{
	/*
	if(fluido)
	delete fluido;
		fluido =new CMFluido(0.001002);
	assert(fluido);
	
	if(solver)
	delete solver;
	solver = new  CSMDSOR ();
	assert(solver);

	if(grafo)
	delete grafo;
	grafo = new CGra3Dby2DAdvanced ();
		assert(grafo);
*/
}

// -------------------------------------------------------------------------
// Funcao:DefinicaoCondicoesContorno()
// -------------------------------------------------------------------------
/**
@short  : Usada para definir as condicoes de contorno
@author : Andre Duarte Bueno
@see    :
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
CPermeabilidadeGrafo::DefinicaoCondicoesContorno ()
{
	//
	// Uma atmosfera
	long double pressao_face_esquerda = 1.0;
	long double pressao_face_direita = 0.0;

	// Criando os objetos de contorno
	// Criando contorno esquerdo = 0
	CContorno *contorno_esquerdo = new CContorno ();
	assert (contorno_esquerdo);
	grafo->contorno.push_back (contorno_esquerdo);
	*contorno_esquerdo = pressao_face_esquerda; // contorno se comporta como um double, veja classe CContorno.

	// Calculando parâmetros para contorno de centro
	// O objeto contorno de centro, tem uma funcao Go que estima os valores iniciais (de pressão)
	// de grafo->objeto[i]->x usando uma reta, agora passo os coeficientes desta reta.
	// Coeficiente a da reta y = a + b.x
	long double a = pressao_face_esquerda;

	// Coeficiente b da reta y = a + b.x
	// Para calcular b preciso das pressoes a esquerda e a direita e do valor do maior plano pmax,
	// o ultimo objeto tem armazenado o valor de pmax em x
	// determina o umtimo objeto
	unsigned long int ultimoObjeto = grafo->objeto.size () - 1;

	// determina o valor de pmax
	long double pmax = (grafo->objeto[ultimoObjeto]->x);

	// determina o valor de b
	long double b = (pressao_face_direita - pressao_face_esquerda) / pmax;

	// Criando contorno de centro = 1
	CContorno *contorno_centro = new CContornoCentro (a, b);
	assert (contorno_centro);
	grafo->contorno.push_back (contorno_centro);
	*contorno_centro = pressao_face_esquerda;	// vai ser calculado com a chamada a Go(k)

	// Criando contorno direito = 2
	CContorno *contorno_direito = new CContorno ();
	assert (contorno_direito);
	grafo->contorno.push_back (contorno_direito);
	*contorno_direito = pressao_face_direita;
}

// -------------------------------------------------------------------------
// Funcao:       DefinicaoValoresIniciais
// -------------------------------------------------------------------------
/**
@short  : Definição dos valores iniciais de pressão
@author : Andre Duarte Bueno
@see    : .
@param  : void
@return : void
*/
void
CPermeabilidadeGrafo::DefinicaoValoresIniciais ()
{
	// Para todos os objetos do grafo associa valores iniciais de pressão
	unsigned long int numeroObjetos = grafo->objeto.size ();

	// Percorre todos os objetos do grafo, de define valores iniciais de x (pressão)
	// CContorno::WEST     CContorno::CENTER;      CContorno::EST;
	for (unsigned long int k = 0; k < numeroObjetos; k++)
	{
		// Para os objetos do centro chama Go, que usa uma reta para estimar valor inicial de x (pressão).
        if (grafo->objeto[k]->Contorno () == CContorno::ETipoContorno::CENTER)	// 1
			grafo->objeto[k]->x = grafo->contorno[1]->Go (grafo->objeto[k]->x);

		// Se contorno=CContorno::WEST  objeto esta na esquerda
        else if (grafo->objeto[k]->Contorno () == CContorno::ETipoContorno::WEST)	// 0
			grafo->objeto[k]->x = (*(grafo->contorno[0]));

		// Se contorno=CContorno::EST objeto esta na direita
		else			// 2
			grafo->objeto[k]->x = (*(grafo->contorno[2]));
	}

	// Transforma as propriedades raioHidraulico em condutancias
	// o calculo das condutancias agora é realizado no proprio grafo
	grafo->CalculoCondutancias (fluido->Viscosidade (), sizePixel, fatorAmplificacao);

	// No grafo ocorrem conjunto de sítios com mais de uma ligação entre sí, posso eliminar
	// os links duplicado, somando as suas condutâncias, o que é feito na funcao EliminarCondutanciasRepetidas
	grafo->EliminarCondutanciasRepetidas ();

	// Determina parâmetros necessários ao calculo da permeabilidade
	diferencaPressao = (*(grafo->contorno[0])) - (*(grafo->contorno[2]));
	dimensaoX = (nx - numeroPixelsBorda) * fatorAmplificacao * sizePixel;
	dimensaoY = (ny - numeroPixelsBorda) * fatorAmplificacao * sizePixel;
	dimensaoZ = (nz - numeroPixelsBorda) * fatorAmplificacao * sizePixel;

	// Como as pressões estão no meio de cada nó,
	// O comprimento a ser considerado deve descontar 1 pixel
	comprimento = (nz - 2 * numeroPixelsBorda - 1) * fatorAmplificacao * sizePixel;
	area = dimensaoY * dimensaoX;
	iteracoes = 1;
}

// -------------------------------------------------------------------------
// Funcao:  SolucaoSistemaEquacoes()
// -------------------------------------------------------------------------
/**
@short  : Definição SolucaoSistema
@author : Andre Duarte Bueno
@see    :
@param  : nada
@return : void
*/
void
CPermeabilidadeGrafo::SolucaoSistemaEquacoes ()
{
 // Pega ponteiro para vetor do tipo CSMParametroSolver*
 // Todo: alterar para dynamic_cast
	vector < CSMParametroSolver * >*ptr_obj = (vector < CSMParametroSolver * >*) & (grafo->objeto);

	// vector<CSMParametroSolver*> * ptr_obj = static_cast<vector<CSMParametroSolver*> *  >( &(grafo->objeto));
	long double erroSolver = solver->Go (ptr_obj);

	//era usado para dar continuidadea simulação no caso de uma interrupcao indesejada.
	//grafo->SalvaVetorPropriedades ();

	cout << "\nIts[" << setw (4) << solver->Iteracoes ()
			 << "] LEs[" << setw (10) << solver->LimiteErro ()
			 << "] Es[" << erroSolver
			 << "] Erro[" << setw (10) << solver->Erro () << "]" << endl;
}

// -------------------------------------------------------------------------
// Funcao:  Next()
// -------------------------------------------------------------------------
/**
@short  :
Calcular a permeabilidade do grafo
	Calculo:
		fluxo = (permeabilidade * area * diferencaPressao)
				/
								 (viscosidade * comprimento)

		permeabilidade =  (fluxo * viscosidade * comprimento)
				/
						 (area * diferencaPressao)

		LIANG:
		// fatorConversao* (fluxo*viscosidade*comprimento)
																/
		(area             *diferencaPressao)
		// Permeability= 1.013*1.0e+15*    (Q    *Viscosity)
																 /
				(Length*(NX-4)*n  *1e5) ;
		// fatorConversaoParaMiliDarcy = 1.013*1e+15

		comprimento =    (nx - 4) * fatorAmplificacao*sizePixel
		area        =    (ny - 4) * fatorAmplificacao*sizePixel * (nz - 4) * fatorAmplificacao * sizePixel
		permeabilidade = (fluxo * viscosidade * comprimento)
					/
												 (area * diferencaPressao)

pg136 Dynamics of fluids do Bear
Permeabilidade[darcy] = fluxo[cm/seg]*viscosidade[centipoise]*dx[cm]
					/
			area[cm]   *  dp [atm]

@author : Andre Duarte Bueno
@see    : permeabilidade
@param  : nada
@return : void
*/
long double
CPermeabilidadeGrafo::Next ()
{
	// 0)Solucao do sistema como um todo
	// 2-Processo iterativo,
	// determina o erro em funcao dos fluxos esquerdo e direito
	long double permEsq = 0.0;
	long double permDir = 0.0;

	{
		// 2.1-Chamando funcao de solução do sistema de equacoes
		SolucaoSistemaEquacoes ();

		// 2.2) Determina o fluxo na fronteira solicitada
        long double fluxoe = FluxoFronteira (CContorno::ETipoContorno::WEST);
        long double fluxod = FluxoFronteira (CContorno::ETipoContorno::EST);

		// 2.3) Calcula a permeabilidade
		permEsq = (fluxoe * fluido->Viscosidade () * comprimento) / (area * diferencaPressao);
		permDir = (fluxod * fluido->Viscosidade () * comprimento) / (area * diferencaPressao);

		// permeabilidade media
		permeabilidade = (permEsq - permDir) / 2.0;	// tem sinais contrarios
		permeabilidade = permeabilidade > 0.0 ? permeabilidade : -permeabilidade;

		// Calcula o erro percentual, que deve ser < 5 %
		long double dp = permDir + permEsq;
		dp = dp > 0 ? dp : -dp;
		erroPermeabilidade = 100.0 * dp / permeabilidade;

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
				 << erroPermeabilidade << "%  Itp=" << iteracoes << endl;
	}
	// 1-Incrementa o numero de iteracoes
	iteracoes++;

	return permeabilidade;
}

// -------------------------------------------------------------------------
// Funcao:    Go
// -------------------------------------------------------------------------
/**
@short  : Chama Next ate que o sistema esteja resolvido ou iteracoes tenha ultrapassado o limite
@author : Andre Duarte Bueno
@see    :
@param  :		
@return :   long double
*/
long double CPermeabilidadeGrafo::Go () {
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
	cout << " limIt[" << limiteIteracoes << "] limErro[" << limiteErro << "]" << endl;

	cout << "Informacoes do SOLVER das pressões" << endl;
	cout << " limIt[" << setw (4) << solver->LimiteIteracoes ()
			 << "] limErro[" << setw (10) << solver->LimiteErro ()
			 << "] " << endl;

	cout << "Chamando SolucaoSistema (solver->Go)..." << endl;

	//int numeroDivergencias = 0;
	long double ErroAnterior;
	do {
		ErroAnterior = erroPermeabilidade;
		Next ();
		//if (erroPermeabiliNextdade > ErroAnterior)
		//	numeroDivergencias++;
	} while (// verifica o erro 5%
					 erroPermeabilidade > limiteErro &&
					 // verifica limiteIteracoes
					 iteracoes < limiteIteracoes
					 // apos 100 passos divergindo, para
					 /*&& numeroDivergencias < 100 */ );

	erro = erroPermeabilidade; // novo, seta o erro final // vai ser usado no Imago

	return permeabilidade;
}

// -------------------------------------------------------------------------
// Funcao:    FluxoFronteira
// -------------------------------------------------------------------------
/**
@short  : Determina o fluxo nos sítios da fronteira grafo->objeto[k] retorna o sítio k
@author : Andre Duarte Bueno
@see    :
@param  : unsigned long int	
@return : long double
Pode e deve ser otimizada, pois não precisa varrer todo o grafo,
durante o calculo do grafo anotar os nós de cada face, e criar funcao que retorna lista dos nós de cada face.
*/
long double CPermeabilidadeGrafo::FluxoFronteira (CContorno::ETipoContorno tipoFronteira)
{
	long double fluxos = 0.0;
	long double fluxoObjeto = 0.0;

	// Para todos os objetos do grafo
	for (unsigned long int k = 0; k < grafo->objeto.size (); k++)
	{
		// verificar se é um objeto com a fronteira solicitada
		if (grafo->objeto[k]->Contorno () == tipoFronteira)
			// se afirmativo, calcula o fluxo na fronteira e acumula
		{
			fluxoObjeto = grafo->objeto[k]->Fluxo ();
			fluxos += fluxoObjeto;
		}
	}
	return fluxos;
}
