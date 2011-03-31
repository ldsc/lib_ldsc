// Inclusao de bibliotecas de C
#include  <cmath>
#include  <cstring>
#include  <string>
#include  <cstdlib>
// #include  <fstream>
// #include  <iostream>
#include  <iomanip>
#include  <cassert>

// Inclusao de arquivos da LIB_LDSC
// #include <Base/COperacao.h>                             	// Objeto COperacoes
#include <Matriz/CMatriz2D.h>					// Matriz normal da libldsc
#include <FFT/CFFT.h>						// Objeto calculo transformada fourier
#include <Interpolacao/Interpolacao2D/CInt2DBilinear.h> 	// Objeto de interpolacao

// Arquivo cabecalho
#include <Correlacao/CCorrelacaoFFT.h>

using namespace std;

// -----------------------------------------------------------------------
// Construtor:
// -----------------------------------------------------------------------
CCorrelacaoFFT::CCorrelacaoFFT ( int dimensao_vetor_correlacao ) 
	: CCorrelacao ( dimensao_vetor_correlacao ),
	Re_data(0), 
	Im_data(0), 
	NX(0), 
	NY(0)
{
}

// -----------------------------------------------------------------------
// Destrutor:
// -----------------------------------------------------------------------
CCorrelacaoFFT::~CCorrelacaoFFT () 
{ 
 	if ( Im_data )
 		delete [] Im_data;


	if ( Re_data )
		delete [] Re_data;
}

// -----------------------------------------------------------------------
// Funcao: GoMain
// -----------------------------------------------------------------------
int CCorrelacaoFFT::GoMain (int argc, char **argv)
{
  int _NX;
  int _NY;	
  cout << "\nFormato de uso (equivalente ao codigo original do Liang):\n" 
       << "./RzFFT imagem NX NY\n"
       << "\nOs parametros default são:\n"
       << " NX = 640 NY = 480\n"
       << "o arquivo imagem deve ter somente dados \n"
       << "a extensão é adicionada automaticamente e deve ser .dat" << endl;

  string fileName;
  if (argc > 1)          // argv[0]=nomePrograma
    fileName = argv[1];  // primeiro argumento nome imagem
  if (argc > 2)
    _NX = atoi (argv[2]); // segundo argumento NX
  if (argc > 3)
    _NY = atoi (argv[3]); // terceiro argumento NY
  
  cout << "\nParametros passados para o programa:\n"
       << "Nome do arquivo de entrada com a extensão = " << fileName << ".dat\n"
       << "NX = " << _NX << " NY = " << _NY << endl;

  // ----------------------------------
  // Alocacao da matriz parte real
	// Se Re_data nunca foi criada é NULL, então vai ser alocada neste bloco.
	// Se Re_data já existe, mas tem dimensões diferentes, então é realocada.
	// Se não é nula e as dimensões são as mesmas, então já foi alocada, e não
	// precisa realocar, é só zerar.
	//   if ( Re_data == NULL or (NX*NY) != (_NX*_NY) )
	//   {
	// 	// Se NULL delete não tem efeito
	// 	delete [] Re_data;
	// 	Re_data = new float [ _NX * _NY ];
	// 
	// 	  if ( Re_data == NULL )
	// 	  {
	// 		  cerr << "Erro alocacao de Im_data\n";
	// 		  exit(0);
	// 	  }
	//   }
	// 
	//   NX = _NX;
	//   NY = _NY;
	RealocarMatrizSeNecessario(Re_data, _NX, _NY);
  
  // Método estatico da classe matriz2d que lê matriz no formato antigo (somente dados)
  bool leituraArquivo = CMatriz2D::Read2D ( (fileName + ".dat").c_str (), Re_data, NX, NY ); 

  if ( leituraArquivo == 0 )
  {
      cerr << "\nFalha leitura do arquivo, encerrando funcao.";
      return 0;
  }

  // Chama funcao de calculo da autocorrelacao
  Go ( Re_data, NX, NY );

  // Saída da funcao autocorrelacao
  Write ( fileName );

  // Saída da funcao autocorrelacao e dados adicionais
  Writerzf ( fileName );

  // Re_data é destruido no destrutor, o objetivo e evitar realocacoes a cada novo calculo.
  return 1;
}

// -----------------------------------------------------------------------
// Funcao:
// -----------------------------------------------------------------------
// @short  :
// @author  :Andre Duarte Bueno
// Funcao de cálculo da funcao correlacao
// Recebe uma imagem 2D (vetor) e as dimensoes nx e ny
bool CCorrelacaoFFT::Go ( float * Re_data, int _NX, int _NY ) 
{
	// Se Im_data nunca foi criada é NULL, então vai ser alocada neste bloco.
	// Se Im_data já existe, mas tem dimensões diferentes, então é realocada.
	// Se não é nula e as dimensões são as mesmas, então já foi alocada, e não
	// precisa realocar, é só zerar.
/*	if ( Im_data == NULL or (NX*NY) != (_NX*_NY) )
	{
		delete [] Im_data;
		Im_data = new float [_NX * _NY];
		
		if ( Im_data == NULL )
		{
			cerr << "Erro alocacao de Im_data\n";
			exit(0);
		}
	}
	
	NX = _NX;
	NY = _NY;*/
	RealocarMatrizSeNecessario(Im_data, _NX, _NY);
	
	// Zera Parte Imaginaria
	// usar memcpy é + rápido
	for ( int j = 0; j < NY; j++ ) 
	{
		for ( int i = 0; i < NX; i++ ) 
		{
			MatrizImag(i, j) = 0.0;
		}
	}
	
	// ----------------------------------
	// Criacao do ponteiro para objeto de calculo da FFT
	CFFT * objetoFourier; 

	// Definicao do vetor com as dimensões usadas no CFFT
	int dims[2];
	dims[0] = NX;
	dims[1] = NY;
	
	// ----------------------------------
	// 2D fft
	// Transformada de Fourier sobre imagem 2D (ida)
	// Entra com a imagem lida (Re_data e sai com Re_data  e Im_data)
	//  ret = fftn (2 /*dimensao*/, dims/*vetor dimensao*/, Re_data/*img real*/, Im_data/*img imaginaria*/, 1, -1);// PROCESSA
	//    if (ret) return 1;
	int ret;
	
	objetoFourier = new CFFT ( 2, dims, -1 );

	// Calcula a transformada de fourier, muda Re_data e Im_data
	if ( objetoFourier->Go ( Re_data, Im_data ) ) 
		return 1;						// se retorno==1 sai

	// ----------------------------------
	// Calculo de R = R * R  + I * I, e I = 0
	//	fout << "Calculo de R = R * R  + I * I, e I = 0" << endl;
	for (int j = 0; j < NY; j++)
		for (int i = 0; i < NX; i++) 
		{
			MatrizReal (i, j) = MatrizReal (i, j) * MatrizReal (i, j) + MatrizImag (i, j) *  MatrizImag (i, j);
			MatrizImag (i, j) = 0;
		}
	
	// ----------------------------------
	// 2D inverse fft
	// Transformada de Fourier sobre imagem 2D (volta)
	// Calcula a inversa, depois da composicao acima
	//    printf ("Inverse fft ..."); fflush (stdout);
	//    ret = fftn (2, dims, Re_data, Im_data, -1, 0);
	//    if (ret) return 1;
	objetoFourier->SetScaling (0);
	if ( objetoFourier->Go_Inverse (Re_data, Im_data) ) 		// calcula transformada inversa
		return 1;						// se retorno == 1 sai
	delete objetoFourier;
	// objetoFourier = NULL;
	
	// ----------------------------------
	// Determina o fmin, valor minimo de R
	//{
	//	float fmin;
	//	fmin = MatrizReal (0, 0);
	//	for (int j = 0; j < NY; j++)
	//		for (int i = 0; i < NX; i++)
	//			if (MatrizReal (i, j) < fmin)
	//				fmin = MatrizReal (i, j);
	//	fout << "fmin = " << fmin << endl;
	//}
	
	// ----------------------------------
	// bug: não é bug, mas para o caso FFT não precisa considerar apenas NX/2!!!
	// Definicao do intervalo de calculo da correlacao (deslocamento u) como sendo NX / 2 
	// futuramente considerar extensao/reflexao da imagem
	// Se a dimensao da imagem mudou, muda a dimensao do vetor correlacao e realoca o vetor.
	// Se a dimensão não mudou apenas zera o vetor correlacao.
	// 	if ( size() != ( NX / 2 ) )
	// 	{
	// 		// Novo size()
	// 		deslocamentoMaximo = ( NX / 2 );
	// 
	// 		// Destroe correlacao existente e aloca nova
	// 		if ( correlacao ) 
	// 		{
	// 			delete [] correlacao;
	// 			correlacao = NULL;
	// 		}
	// 	
	// 		// Aloca vetor, bug usava new float (deslocamentoMaximo)
	// 		correlacao = new float [deslocamentoMaximo];
	// 		if ( ! correlacao ) 
	// 		{
	// 			cerr << "Erro alocacao de correlacao - bool CCorrelacaoEspacial::Go (float *_Re_data, int _nx, int _ny)." ;
	// 			return 0;
	// 		}
	// 	}
	// 	
	// 	// zera, usar memcpy memset(ptr,valor,size), veja memset e teste funcionamento
	// 	for (int ss = 0; ss < deslocamentoMaximo; ss++)  
	// 		correlacao[ss] = 0;
	RealocarVetorSeNecessarioEZerar( correlacao, NX );

	// Angulo 
	float theta;
	
	// Quando usamos FFT, a correlacao é determinada em toda imagem
	// aqui iremos calcular a correlacao média, para tal, usamos objeto de interpolacao
	// Objeto de interpolacao - faz uma media das correlacoes em todas as direcoes
	CInt2DBilinear * interpolacao = NULL;
	interpolacao = new CInt2DBilinear;
	if ( ! interpolacao ) 
	{
		cerr << "Erro alocacao de interpolacao." ;
		return 0;
	}

	// Faz k de 1 ate deslocamentoMaximo
	// BUG: porque nao comeca de 0, ie, k = 0??? -> por causa da divisao por zero.
	for ( int k = 1; k < (deslocamentoMaximo); k++ ) 
	{
		for ( int l = 0; l <= 2 * k; l++ )
		{
			// Calcula o angulo
			theta = 3.1415926 * l / 4.0 / k;
			// Cálculo da interpolacao
			// correlacao += TLIP(NX, NY, Re_data,k*cos(theta),k*sin(theta));
			correlacao[k] += interpolacao->Go (NX, NY, Re_data, k * cos (theta),  k * sin (theta));
		}
	// divide pela ponderacao
	correlacao[k] /= (2.0 * k + 1.0);
	}
	delete  interpolacao;
	//interpolacao = NULL;
	
	// NOVO, pois acima nao calcula o ponto 0, pois k comeca em 1
	correlacao[0] = MatrizReal(0, 0); 
	
	// Note que Im_data vai ser deletado no destrutor, evitando-se realocacoes de Im_data a cada calculo.
	return true;
}

// Funcao de cálculo da funcao correlacao
// Recebe uma imagem 2D (CMatriz2D)
bool CCorrelacaoFFT::Go ( CMatriz2D *img ) 
{
	if (! img ) 
		return false;
		
	// ----------------------------------
	// bug: não é bug, mas para o caso FFT não precisa considerar apenas NX/2!!!
	// Definicao do intervalo de calculo da correlacao (deslocamento u) como sendo NX / 2 
	// futuramente considerar extensao/reflexao da imagem
	// Se a dimensao da imagem mudou, muda a dimensao do vetor correlacao e realoca o vetor.
	// Se a dimensão não mudou apenas zera o vetor correlacao.
	// 	if ( size() != ( img->NX() / 2 ) )
	// 	{
	// 
	// 		// Novo size()
	// 		deslocamentoMaximo = ( img->NX() / 2 );
	// 
	// 		// Destroe correlacao existente e aloca nova
	// 		if ( correlacao ) 
	// 		{
	// 			delete [] correlacao;
	// 			correlacao = NULL;
	// 		}
	// 	
	// 		// Aloca vetor, bug usava new float (deslocamentoMaximo)
	// 		correlacao = new float [deslocamentoMaximo];
	// 		if ( ! correlacao ) 
	// 		{
	// 			cerr << "Erro alocacao de correlacao - bool CCorrelacaoEspacial::Go (float *_Re_data, int _nx, int _ny)." ;
	// 			return 0;
	// 		}
	// 	}
	RealocarVetorSeNecessarioEZerar( correlacao, img->NX() );

  // ----------------------------------
  // Alocacao da matriz parte real
	// Se Re_data nunca foi criada é NULL, então vai ser alocada neste bloco.
	// Se Re_data já existe, mas tem dimensões diferentes, então é realocada.
	// Se não é nula e as dimensões são as mesmas, então já foi alocada, e não
	// precisa realocar, é só zerar.
/*	if ( Re_data == NULL or (NX*NY) != ( img->NX()*img->NY() ) )
	{
		delete [] Re_data;
		Re_data = new float [ img->NX()*img->NY()  ];
		
		if ( Re_data == NULL )
		{
			cerr << "Erro alocacao de Re_data\n";
			exit(0);
		}
	}
	
	NX = img->NX();
	NY = img->NY();*/
	RealocarMatrizSeNecessario(Re_data, img->NX(), img->NY() );
	
	// Copia dados de img para Re_data usando MatrizReal
	for (int j = 0; j < NY; j++) 
	{
		for (int i = 0; i < NX; i++) 
		{
			MatrizReal(i,j) = img->data2D[i][j];
		}
	}
	
	return Go( Re_data, NX, NY); 
}


// -----------------------------------------------------------------------
// Funcao:
// -----------------------------------------------------------------------
// @short  :
// saida .rzf
bool CCorrelacaoFFT::Writerzf (string fileName)
{
 if (correlacao == NULL)
		return 0;
 if (Re_data == NULL)
		return 0;
  
  ofstream frzf ( (fileName + ".rzf").c_str () );
  if ( ! frzf )
    return 0;

  // Escreve cabecalho
  frzf 	            << 0 
	<< setw(15) << MatrizReal (0, 0) 
	<< setw(15) << MatrizReal (0, 0) 
	<< setw(15) << MatrizReal (0, 0) 
	<< setw(15) << MatrizReal (0, 0) << endl;

  // Armazena resultados nos arquivos de disco
  // Faz k de 1 ate deslocamentoMaximo
  for (int k = 1; k < size(); k++)  
   frzf              << k 
	<< setw(15)  << MatrizReal (k, 0) 
	<< setw(15)  << MatrizReal (0, k) 
	<< setw(15)  << (MatrizReal(0, k) + MatrizReal (k, 0)) / 2 
	<< setw(15)  << correlacao[k] << endl; 				// salva correlacao
  frzf.close ();
  return 1;
}

// -----------------------------------------------------------------------
// Funcao:
// -----------------------------------------------------------------------
// @short  :
// @author  :Andre Duarte Bueno
// saida .cor
// bool CCorrelacaoFFT::Write (string fileName)
// {
//   if ( ! correlacao ) 	
// 	  return 0;
//   
//   string arquivo_saida_cor ( fileName + ".cor" );
//   
//   ofstream fcor ( arquivo_saida_cor.c_str () );
//   if (! fcor.is_open() )
//     return 0;
//   
//   // Escreve cabecalho
//   // fcor << 0 << "\t" << MatrizReal(0, 0) << endl; 
//   fcor << 0 << "\t" << correlacao[0] << endl; 
// 
//   // Escreve o vetor correlacao em disco
//   for (int k = 1; k < (deslocamentoMaximo); k++)
//     fcor << k << "\t" << correlacao[k] << endl;
// 
//   // Fecha o arquivo de disco
//   fcor.close ();
//   return 1;
// }

// -----------------------------------------------------------------------
// Funcao:
// -----------------------------------------------------------------------
// @short  :
// Recebe a propria matriz (Im_data ou Re_data) e a nova dimensao,
// a matriz é realocada de o   ponteiro for null ou se houve mudanca de dimensao. 
bool CCorrelacaoFFT::RealocarMatrizSeNecessario( float*& matriz, int novoNX, int novoNY )
{
	if ( matriz == NULL or (NX*NY) != ( novoNX * novoNY ) )
	{
	delete [] matriz;
	matriz = new float [  novoNX * novoNY  ];
		
	if ( matriz == NULL )
		{
		cerr << "Erro alocacao da matriz Re_data ou Im_data\n";
		exit(0);
		}
	}
	
	NX = novoNX;
	NY = novoNY;
	return 1;
}

// -----------------------------------------------------------------------
// Funcao:
// -----------------------------------------------------------------------
// @short  :
// Realoca o vetor se for necessário, sempre zera o vetor correlacao
bool CCorrelacaoFFT::RealocarVetorSeNecessarioEZerar( float*& correlacao, int novoNX)
{
	if ( size() != ( novoNX / 2 ) )
	{
		deslocamentoMaximo = ( novoNX / 2 );

	// Destroe correlacao existente e aloca nova
	if ( correlacao ) 
		{
		delete [] correlacao;
		correlacao = NULL;
		}
	
	// Aloca vetor, bug usava new float (deslocamentoMaximo)
	correlacao = new float [deslocamentoMaximo];
	if ( ! correlacao ) 
		{
		cerr << "Erro alocacao de correlacao." ;
		exit( 0 );
		}
		
	NX = novoNX;	
	}
	
	// zera, usar memcpy memset(ptr,valor,size), veja memset e teste funcionamento
	for (int ss = 0; ss < deslocamentoMaximo; ss++)  
		correlacao[ss] = 0;
 return 1;
}
