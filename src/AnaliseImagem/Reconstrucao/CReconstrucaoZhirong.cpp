
// ------------------------------------------------------------------------------
// Includes:
// -----------------------------------------------------------------------------
#include  <cmath>
#include  <cstdlib>
#include  <ctime>
#include  <fstream>
#include  <iostream>
#include  <iomanip>
using namespace std;

// ------------------------------------------------------------------------------
// Includes do LDSC
// ------------------------------------------------------------------------------
// declaracao da classe
#include  <AnaliseImagem/Reconstrucao/CReconstrucaoZhirong.h>

// objeto CFNaoLinearRzRy
#include  <MetNum/Funcao/CFNaoLinearRzRy.h>

#include  <Estatistica/CEstatistica.h>

#include  <Estatistica/CENormal.h>

// ------------------------------------------------------------------------------
// Construtor
// ------------------------------------------------------------------------------
// Construtor, entrada de dados da linha de comando ou manual,
// leitura do arquivo de disco, alocacao de matrizes,ponteiros arquivos disco
CReconstrucaoZhirong::CReconstrucaoZhirong (std::string nomeArquivo, int fatorAmp, int npcz, double sp, int nx, int ny, int nz) : CReconstrucao () {
   cout << "Parametros enviados para reconstrucao:";
   cout << "\nnomeArquivo:" << nomeArquivo;
   cout << "\nfator de amplificacao:" << fatorAmp;
   cout << "\nnumero de pontos a serem lidos do arquivo de disco:" << npcz;
   cout << "\ndimensao nx:" << nx;
   cout << "\ndimensao ny:" << ny;
   cout << "\ndimensao nz:" << nz;

   numeroPontosLidosArquivoCz = 0;	// numero de pontos realmente lidos do arquivo de disco
   nomeArquivoCz = nomeArquivo;
   nomeArquivo = nomeArquivo;
   fatorAmplificacaoNumeroPontos = fatorAmp;
   numeroPontosCz = npcz;
   sizePixel = sp;
   NX = nx;
   NY = ny;
   NZ = nz;

   // Alocacao de dados das matrizes
   // Ry=new double[numeroPontosCz+1];                   // 30+1
   // Cz=new double[numeroPontosCz+1];
   Ry = new double[numeroPontosCz * fatorAmplificacaoNumeroPontos + 1];	// 30+1
   Cz = new double[numeroPontosCz * fatorAmplificacaoNumeroPontos + 1];	// alterar
   Re_data = new double [NX * NY * NZ];
   Im_data = new double [NX * NY * NZ];

   if (Ry == nullptr || Cz == nullptr || Re_data == nullptr || Im_data == nullptr) {
      /*cerr<<"\nNão conseguiu alocar memoria para matrizes real e imaginaria.\n";cin.get() */ ;
      exit (1);
   }				// return 1;}

   for (k = 0; k < NZ; k++)	// zera a matriz imaginária
      for (j = 0; j < (NY); j++)
         for (i = 0; i < (NX); i++)
            Imag_E1 (i, j, k) = 0; //  usar menset

}

// ----------------------------------------
// Destrutor
// ----------------------------------------
// Fecha arquivos abertos
// libera memoria alocada
CReconstrucaoZhirong::~CReconstrucaoZhirong () {
   // fout.close();
   // fin.close();
   if(Re_data) 		delete [] Re_data;
   if(Im_data) 		delete [] Im_data;
   if(objetoFourier)     delete objetoFourier;
   //  delete Ry; delete Rz; ??????
}

// ----------------------------------------
// LeituraArquivoCz
// ----------------------------------------
// Lê os dados do arquivo com os valores do deslocamento u e da correlação Cz
void CReconstrucaoZhirong::LeituraArquivoCz () {
   // Le do disco a porosidade. Qdo u=0, correlacao(u=0)=porosidade
   ifstream fin (nomeArquivoCz.c_str());	// abre arquivo do disco
   // fin.open(nomeArquivoCz);    // abre arquivo do disco
   if (fin == nullptr) {		// abre arquivo disco
      /*cout<<"\nNao abriu arquivo disco "<<nomeArquivoCz<<", verifique nome do arquivo.\n"; */
      exit (1);
   }
   int u;
   fin >> u >> porosidade;	// Lê a porosidade
   fin.seekg (0, ios::beg);	// posiciona ponteiro leitura novamente no inicio
   numeroPontosLidosArquivoCz = 0;
   // cout<<"\nValores lidos do arquivo de disco"<<endl;
   // cout<<"\nporosidade="<<porosidade<<endl;
   // alteracao coloquei <=
   // diferente:liang lê todo o cz do disco
   for (int i = 0; i <= numeroPontosCz * fatorAmplificacaoNumeroPontos; i++) {	// leitura do arquivo de disco
      fin >> u;		// Lê o valor de u do arquivo de disco
      fin >> Cz[u];		// Lê Cz
      // cout<<"\n"<<numeroPontosLidosArquivoCz<<"\tCz["<<u<<"]="<<Cz[u];// novo
      numeroPontosLidosArquivoCz++;	// acumula o número de pontos realmente lidos do arquivo de disco
      if (fin.eof ())	// se chegou ao fim do arquivo sai do loop
         break;
   }
   fin.close ();
   // cout<<"\nNúmero pontos lidos do arquivo="<<numeroPontosLidosArquivoCz;
}

// ----------------------------------------
// calculo de Ry
// ----------------------------------------
// Obs: Cria objeto externo TFuncaoNaoLinearRzRy equivale a antiga função ry=Rzry(yc,porosidade,rz);
// Determina todos os valores de Ry e salva o resultado em disco
// Obs: Embora utilize o fator de amplificação para definir quais os valores de Ry
// que serão utilizados na reconstrução, todos os valores são determinados e salvos em disco
// isto se justifica se Ry for utilizado posteriormente em outros processos
void CReconstrucaoZhirong::Calculo_Ry1D () {
   // Cria objeto filtro não linear  // que assume numeroPontos=35
   // cout<<"\nCriando objeto  filtroNaoLinearRzRy...";
   CFNaoLinearRzRy *funcaoNaoLinearRzRy =    new CFNaoLinearRzRy (yc, porosidade, 35 );
   if (funcaoNaoLinearRzRy == nullptr) {
      // cout<<" ...erro alocação memória objeto filtroNaoLinearRzRy, encerra.";
      exit (1);
   }
   // else
   // cout<<" ...objeto filtroNaoLinearRzRy criado corretamente.";
   double porosidadeAoQuadrado = porosidade * porosidade;	// acelerador
   double porMenospor2 = porosidade - porosidadeAoQuadrado;	// acelerador
   int i = 0;
   double rz;
   nomeArquivoRy=  "saidaRzRy.rzy";	// Abre arquivo disco para armazenar resultados
   // cout<<"\nVai tentar abrir o arquivo fout (escrita) com nome ="<<nomeArquivoRy;
   ofstream fout (nomeArquivoRy.c_str());	// fout.open(nomeArquivoRy);
   if (fout == nullptr) {
      // cout<<"\nNao abriu arquivo disco "<<nomeArquivoRy<<", verifique o nome do arquivo\n";
      exit (1);
   }
   // else
   //                    cout<<" abriu arquivo corretamente";
   fout.precision (5);		// precisão de 5 digitos na saída para disco
   // diferente:liang calcula rz para todo o cz e salva cz e rz em disco
   for (int i = 0; i < numeroPontosLidosArquivoCz; i++) {
      // calculo de Ry em funcao de Rz
      rz = (Cz[i] - porosidadeAoQuadrado) / (porMenospor2);	// Cz normalizada
      Ry[i] = funcaoNaoLinearRzRy->Go (rz);	// calcula Ry de rz
      // cout<<"\nRz["<<i<<"]="<<rz<<"\tRy["<<i<<"]="<<Ry[i];             // mostra na tela
      fout << setw (4) << i << " " << setw (10) << rz << " " << setw (10) << Ry[i] << "\n";	// salva em disco
   }
   delete funcaoNaoLinearRzRy;	// elimina objeto criado
   // Aqui considera o fator de redução
   fout << "\nValores utilizados de i e Ry considerando fatorReducaoNumeroPontos";
   fout << "\ni\tRy";
   // obs: abaixo ao usar <=, esta usando numeroPontosCz+1 pontos e nao numeroPontosCz
   for (i = 0; i <= numeroPontosCz; i++) {
      Ry[i] = Ry[i * fatorAmplificacaoNumeroPontos];	// usa fator de redução para
   }
   // novo: o calculo do Ry esta todo correto. Fornece exatamente os mesmos resultados do liang
   for (i = 0; i <= numeroPontosCz * fatorAmplificacaoNumeroPontos; i++)
      fout << "\n" << i << "\t" << Ry[i];	// determinar pontos a serem utilizados
   fout.close ();		// fecha ponteiro para arquivo
}

// ----------------------------------------
// Calcula_Ry3D_de_1D()
// ----------------------------------------
// Tenho uma funcao de autocorrelacao Ry(u), preciso converter para 3D Ry(p)
// A funcao Calcula_Ry3D_de_1D gera a matriz 3D Ry(p)
// somente parte real
// usa consideracoes de simetria
void CReconstrucaoZhirong::Calcula_Ry3D_de_1D () {
   int i, j, k;			// contador
   int u;			// deslocamento
   double uu;			// auxiliar
   // coloquei as linhas abaixo
   int NXdiv2 = NX / 2;		// variavel aceleradora
   int NYdiv2 = NY / 2;		// variavel aceleradora
   int NZdiv2 = NZ / 2;		// variavel aceleradora

   // Só calcula 1/8 da imagem 3D(pois tem simetria)
   // abaixo atualiza valores de acordo com a simetria (rebate)
   // alteracao
   double kXk, jXj;		// (alteração)
   for (k = 0; k <= NZdiv2; k++) {	// percorre a matriz de dados
      kXk = k * k;
      for (j = 0; j <= NYdiv2; j++) {
         jXj = j * j;
         for (i = 0; i <= NXdiv2; i++) {
            uu = sqrt (i * i + jXj + kXk);	// calcula deslocamento (alteracao)
            u = (int) uu;	// Para imagem 20*20*20=8000   sqrt=89,44271909999
            // (alterar) Zerar vetor real no construtor, com função + rápida
            if (uu > numeroPontosCz)	// acima do comprimento maximo a matriz de inteiros assume valor nulo
               Real_E1 (i, j, k) = 0.0;	// se for maior que numeroPontosCz assume valor 0
            else		// pega o valor e ja calcula parte real
               Real_E1 (i, j, k) = Ry[u] + (uu - u) * (Ry[u + 1] - Ry[u]);
            // faz uma interpolacao entre Ry[u] e Ry[u+1] ************
         }
      }
   }

   // simetria
   for (k = 0; k <= NZdiv2; k += NZdiv2) {
      for (j = 1; j <= (NYdiv2); j++) {
         Real_E1 (0, NY - j, k) = Real_E1 (0, j, k);	//
         Real_E1 (NXdiv2, NY - j, k) = Real_E1 (NXdiv2, j, k);	//
      }
      for (i = 1; i <= (NXdiv2); i++) {
         Real_E1 (NX - i, 0, k) = Real_E1 (i, 0, k);
         Real_E1 (NX - i, NYdiv2, k) = Real_E1 (i, NYdiv2, k);
      }
      for (i = 1; i <= (NXdiv2); i++)
         for (j = 1; j <= (NYdiv2); j++) {
            Real_E1 (i, NY - j, k) = Real_E1 (i, j, k);
            Real_E1 (NX - i, j, k) = Real_E1 (i, NY - j, k);
            Real_E1 (NX - i, NY - j, k) = Real_E1 (i, j, k);	// igual
         }
   }
   // simetria
   for (k = 1; k < NZdiv2; k++) {
      for (j = 1; j <= (NYdiv2); j++) {
         Real_E1 (0, NY - j, k) = Real_E1 (0, j, k);
         Real_E1 (NXdiv2, NY - j, k) = Real_E1 (NXdiv2, j, k);
      }
      for (i = 1; i <= (NXdiv2); i++) {
         Real_E1 (NX - i, 0, k) = Real_E1 (i, 0, k);
         Real_E1 (NX - i, NYdiv2, k) = Real_E1 (i, NYdiv2, k);
      }
      for (i = 1; i <= (NXdiv2); i++) {
         for (j = 1; j <= (NYdiv2); j++) {
            Real_E1 (i, NY - j, k) = Real_E1 (i, j, k);
            Real_E1 (NX - i, j, k) = Real_E1 (i, NY - j, k);
            Real_E1 (NX - i, NY - j, k) = Real_E1 (i, j, k);	// igual
         }
      }
   }

   // simetria
   for (k = 1; k <= NZdiv2; k++) {
      Real_E1 (0, 0, NZ - k) = Real_E1 (0, 0, k);
      Real_E1 (0, NYdiv2, NZ - k) = Real_E1 (0, NYdiv2, k);

      Real_E1 (NXdiv2, 0, NZ - k) = Real_E1 (NXdiv2, 0, k);
      Real_E1 (NXdiv2, NYdiv2, NZ - k) = Real_E1 (NXdiv2, NYdiv2, k);

      for (j = 1; j <= NYdiv2; j++) {
         Real_E1 (0, NY - j, NZ - k) = Real_E1 (0, j, k);
         Real_E1 (0, j, NZ - k) = Real_E1 (0, NY - j, k);
         Real_E1 (NXdiv2, j, NZ - k) = Real_E1 (NXdiv2, NY - j, k);
         Real_E1 (NXdiv2, NY - j, NZ - k) = Real_E1 (NXdiv2, j, k);
      }

      for (i = 1; i <= NXdiv2; i++) {
         Real_E1 (i, 0, NZ - k) = Real_E1 (NX - i, 0, k);
         Real_E1 (NX - i, 0, NZ - k) = Real_E1 (i, 0, k);
         Real_E1 (i, NYdiv2, NZ - k) = Real_E1 (NX - i, NYdiv2, k);
         Real_E1 (NX - i, NYdiv2, NZ - k) = Real_E1 (i, NYdiv2, k);
      }

      for (i = 1; i <= (NXdiv2); i++) {
         for (j = 1; j <= (NYdiv2); j++) {
            Real_E1 (i, j, NZ - k) = Real_E1 (NX - i, NY - j, k);
            Real_E1 (NX - i, NY - j, NZ - k) = Real_E1 (i, j, k);
            Real_E1 (i, NY - j, NZ - k) = Real_E1 (NX - i, j, k);
            Real_E1 (NX - i, j, NZ - k) = Real_E1 (i, NY - j, k);
         }
      }
   }
}

/*

// Variaveis auxiliares
unsigned  long pos;		// novidade
unsigned  long posB;		// novidade

unsigned  long NX_I;		// =NX-i;

unsigned  long NY_J;		// =NY-j;
unsigned  long NYxNX;
unsigned  long NY_JxNX;		// =NYxNX-JxNX;
unsigned  long JxNX;		// =j*NX;

unsigned  long NZ_K;		// =NZ-k;
unsigned  long KxNYxNX;		// =k*NYxNX;
unsigned  long NZ_KxNYxNX;	// =(NZxNYxNX-KxNYxNX);
unsigned  long NZxNYxNX;	// =NZxNYxNX;

unsigned  long NYdiv2xNX;	// =(NYdiv2)*NX;
// atualização de variáveis auxiliares
// k
NZ_K=NZ-k;
KxNYxNX=k*NYxNX;
NZ_KxNYxNX=(NZxNYxNX-KxNYxNX);
// j
NY_J=NY-j;
JxNX=j*NX;
NY_JxNX=NYxNX-JxNX;
// i
NX_I=NX-i;
*/
// ----------------------------------------
// Calculo       |Y|=sqrt(Y^2)
// ----------------------------------------
// Objeto TFFT.  // Calculo_ModuloYt
// |Y|=sqrt(Y^2), tiro a raiz quadrada obtendo |Y|
void CReconstrucaoZhirong::RaizQuadradaDeY2 () {
   for (unsigned int pos = 0; pos < (NZ * NY * NX); pos++)
      Re_data[pos] = (Re_data[pos] < 0) ? 0 : sqrt (Re_data[pos]);
}

// ----------------------------------------
// Transforma a Populacao Y na Z --- Filtro nao Linear
// ----------------------------------------
void CReconstrucaoZhirong::filtro_nao_linear () {
   unsigned int pos = 0;
   for (pos = 0; pos < (NZ * NY * NX); pos++)
      Re_data[pos] = (Re_data[pos] <= yc) ? 1 : 0;
}

// ----------------------------------------
// funcao de salvamento da imagem reconstruida em disco
// ----------------------------------------
bool CReconstrucaoZhirong::SalvarImagemEmDisco (string arqimg) {
   stringstream tmp;
	if ( nomeArquivo == arqimg ) {
      tmp << nomeArquivo << "_GT-1_" << NX << "_fa-" << fatorAmplificacaoNumeroPontos << "_np-" << numeroPontosCz << "_sp-" << sizePixel << ".dbm";
      nomeArquivo = tmp.str();
      arqimg = nomeArquivo;
	}
	ofstream fimg (arqimg.c_str());	// abre arquivo disco
  	if (fimg == nullptr) {
      cerr << "\nNao conseguiu abrir arquivo de disco: " << arqimg <<endl;
      return false;
   }
   fimg << setw (0) << "D1\n" << NX << " " << NY << " " << NZ << "\n";
	fimg << setw (0) << "# fatorAmplificacao: " << fatorAmplificacaoNumeroPontos << "\n";
	fimg << setw (0) << "# sizePixel: " 	    << sizePixel << "\n";
	fimg << setw (0) << "# numeroPixelsBorda: " << 0 << "\n";

   for (k = 0; k < NZ; k++) {
      for (j = 0; j < (NY); j++) {
         for (i = 0; i < (NX); i++)	{// salva arquivo de saida
				fimg << (int) Real_E1 (i, j, k) << " ";	// imagem reconstruida 3D
         }
			fimg << "\n";
     	}
   }
  	fimg.close ();		// fecha ponteiro para arquivo disco
	return true;
}

// ----------------------------------------
// Calcula o Covariograma da Imagem
// ----------------------------------------
// Depois de calculada a imagem reconstruida calcula o covariograma, para verificar
// se o resultado é bom.
// Calculo da correlacao nas diferentes direcoes
void CReconstrucaoZhirong::Covariograma () {
   int i, j, k, desloc;
   double porosidadeCalculada = 0;
   double *covarX;

   covarX = new double [NX / 2];	// TVetor*covarX = new TVetor(NX/2);
   if (covarX == nullptr)
      return;

   for (desloc = 0; desloc < (NX / 2); desloc++)	// covarX->Cte(0);
      covarX[desloc] = 0;

   // TCaracterizacao*car=new TCaracterizacao(pm3D);
   for (k = 0; k < (NZ); k++)
      for (j = 0; j < (NY); j++)
         for (i = 0; i < (NX); i++)
            if (Real_E1 (i, j, k))
               porosidadeCalculada++;	// car->Porosidade();
   porosidadeCalculada /= NX * NY * NZ;

   // car->Correlacao();
   // car->correlacao->Write("correlacao.cor");
   // car->CorrelacaoNormalizada();
   // car->correlacao->Write("correlacao.cor");
   // for(desloc=0;desloc<(NX/2)&&(desloc*fatorAmplificacaoNumeroPontos) < numeroPontosCz;  desloc++)
   for (desloc = 0; desloc < (NX / 2); desloc++) {
      covarX[desloc] = 0;
      for (k = 0; k < NZ; k++)
         for (j = 0; j < NY; j++)
            for (i = 0; i < (NX - desloc); i++)	// conferir
               if (Real_E1 (i, j, k) && Real_E1 (i + desloc, j, k))	// novo
                  covarX[desloc]++;	// novo
      // antes usava   covarX[desloc]+=Real_E1(i,j,k)*Real_E1(i+desloc,j,k);
      covarX[desloc] /= ((NX - desloc) * NY * NZ);
   }


   // Função de salvamento em disco dos valores de correlação...
   string arquivoCovariograma = "correlacao.cor";
   ofstream fout (arquivoCovariograma.c_str());
   if (fout == nullptr) {
      // cout<<"\nNao posso criar arquivo"<<arquivoCovariograma<<",na funcao CReconstrucaoZhirong::Covariograma().\n";
      exit (1);
   }
   for (desloc = 0; desloc < (NX / 2); desloc++) {
      fout << "\n" << setw (10) << (desloc * fatorAmplificacaoNumeroPontos)	// deslocamento
           << " " << setw (10) << Cz[desloc * fatorAmplificacaoNumeroPontos]	// correlacao
           << " " << setw (10) << covarX[desloc]	// correlacao imagem
           << " " << setw (10) << (Cz[desloc * fatorAmplificacaoNumeroPontos] - porosidade * porosidade)
              / (porosidade - porosidade * porosidade)	//
           << " " << setw (10) << (covarX[desloc] -
                                   porosidadeCalculada * porosidadeCalculada) /
              (porosidadeCalculada - porosidadeCalculada * porosidadeCalculada);
   }
   fout.close ();
   delete [] covarX;
}

// -----------------------------------------------------------------------------
// Funcao Go
// -----------------------------------------------------------------------------
int CReconstrucaoZhirong::Go () {
   time_t ti, tf;

   cout << "\nLendo arquivo com valores de Cz do disco ...";
   ti = time (nullptr);
   LeituraArquivoCz ();
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (LeituraArquivoCz)" << (difftime (tf, ti));

   ti = time (nullptr);
   cout << "\nCriando objeto CENormal para calculo de yc ...";
   CENormal *normal = new CENormal ();	// Cria objeto CENormal que representa a curva normal
   cout << " ...objeto CENormal criado.";
   normal->limiteErro = 1e-10;
   yc = normal->GoInv (porosidade);	// retorna yc da eq 3.15
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (normal->GoInv(porosidade);)" << (difftime (tf, ti));

   cout << "\nCalculando yc=normal->GoInv(" << porosidade << ")=" << yc;
   delete normal; // objeto

   cout << "\nCalculando   Ry1D ...";
   ti = time (nullptr);
   Calculo_Ry1D ();
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (Calculo_Ry1D)" << (difftime (tf, ti));

   cout << "\nCalculando   Ry3D_de_1D ...";
   ti = time (nullptr);
   Calcula_Ry3D_de_1D ();
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (Calcula_Ry3D_de_1D)" << (difftime (tf, ti));

   // Cria objeto para calculo das transformadas de Fourier
   int dims[3];
   dims[0] = NX;
   dims[1] = NY;
   dims[2] = NZ;
   cout << "\nCriando objeto TFFT_Properties para calculo das transformadas ...";
   objetoFourier = new CFFTPropriedades (3, dims, -1);
   cout << " ...objeto TFFT_Properties criado.";

   // Abaixo calcula a transformada de fourier de Ry3D. Que é igual ao power spectrum.
   // passar parametros para objeto fourier
   cout << "\nCalculando   |Y.^2|=FFT(Ry3D) ...";
   ti = time (nullptr);
   if (objetoFourier->Go (Re_data, Im_data))
      return 1;
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (objetoFourier->Go)" <<
           (difftime (tf, ti));

   cout << "\nCalculando   |Y.|=sqrt Y.^2 ...";
   ti = time (nullptr);
   RaizQuadradaDeY2 ();
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (RaizQuadradaDeY2)" <<
           (difftime (tf, ti));

   // Abaixo determina a transformada de fourier de Y a partir de |Y|+angulo de fase.
   cout << "\nCalculando   Y. de |Y.|+angulo de fase ...";
   ti = time (nullptr);
   objetoFourier->FFTY_of_ModY (Re_data, Im_data);
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (FFTY_of_ModY)" << (difftime (tf, ti));

   // Tenho fft de Y, quero Y, faço fft-1
   cout << "\nCalculando   Y=FFT-1(Y.) ... ";
   ti = time (nullptr);
   objetoFourier->SetScaling (0);
   if (objetoFourier->Go_Inverse (Re_data, Im_data))
      return 1;			// se retorno=1 sai
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (objetoFourier->Go_Inverse)" << (difftime (tf, ti));

   // Calcula a media e covariancia
   cout << "\nConferindo a media e a covariancia... ";
   ti = time (nullptr);
   //CEstatisticaBasica *estatistica = new CEstatisticaBasica ();
   CEstatistica *estatistica = new CEstatistica ();
   estatistica->Covariancia (Re_data, NX, NY, NZ);	// calcula covariancia e a média
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (estatistica->Covariancia(Re_data,NX,NY,NZ);)" << (difftime (tf, ti));

   cout << " media=" << estatistica->Media();
   cout << " covariancia=" << estatistica->Covariancia();

   if (fabs (estatistica->Media() ) > 0.1 || fabs ( estatistica->Covariancia() ) > 1.2) {
      cout << "\nCUIDADO!\tValores de média e covariância ruins";
      cout << "\nChecar a função correlação de entrada e os parâmetros de entrada.\n";
   }

   // Normalizacao_de_Z()
   // Como a parte real de Y pode ter pequenos erros, nao e exatamente 0 e 1,
   // faco uma normalizacao para deixar mais correto.[opcional].
   cout << "\nCalculando normalizacao da parte real ...";
   ti = time (nullptr);
   unsigned int pos = 0;
   double sqrt_covariancia = sqrt (estatistica->Covariancia());
   for (pos = 0; pos < (NZ * NY * NX); pos++)
      Re_data[pos] = (Re_data[pos] - estatistica->Media()) / sqrt_covariancia;
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (normalizacao)" << (difftime (tf, ti));

   cout << "\nConferindo a media e a covariancia depois da normalização... ";
   estatistica->Covariancia (Re_data, NX, NY, NZ);	// já calcula a média
   cout << " media=" << estatistica->Media();
   cout << " covariancia=" << estatistica->Covariancia();
   delete estatistica; // objeto

   cout << "\nCalculando   filtro nao linear ... ";
   ti = time (nullptr);
   filtro_nao_linear ();					// Calcula o filtro nao linear Z real-> Z(0,1)
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (filtro_nao_linear)" << (difftime (tf, ti));

   cout << "\nSalvando imagem reconstruída em disco... ";
   ti = time (nullptr);
   SalvarImagemEmDisco ( nomeArquivo );
   tf = time (nullptr);
   cout << "\nTempo decorrido processo (SalvarImagemEmDisco)" << (difftime (tf, ti));

   cout << "\nFim da função CReconstrucaoZhirong::Go." << endl;
   return 1;
}
