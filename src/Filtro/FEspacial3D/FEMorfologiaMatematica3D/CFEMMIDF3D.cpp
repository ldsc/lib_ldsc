// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDF3D.cpp
Nome da classe:      CFEMMIDF3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa as funções da classe CFEMMIDF3D.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMIDF3D.h"// Classe base
#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMIDFd3453D.h" // Utilizado pelo método CorrigeAbertura
#include "Geometria/Bola/BCDiscreta3D/CBCDiscreta3D.h"	// Cria objeto

/*
==================================================================================
Atributos estaticos da classe
==================================================================================
*/
// Se verdadeira após cada processamento recalcula a idf
bool  CFEMMIDF3D::atualizaIDF = 0;	// por default, não recalcula a idf apos dilatacao ou fechamento

/*
==================================================================================
Documentacao construtor
==================================================================================
Executa o construtor da classe base, o CMatriz3D(_pm)
que realiza a copia dos dados.
Depois copia o endereco da imagem no atributo pm.
Assim pm aponta para a matriz recebida e data3D para a matriz IDF
*/
// Construtor sobrecarregado, recebe também o tamanho do raio máximo
CFEMMIDF3D::CFEMMIDF3D (CMatriz3D * &matriz, unsigned int _tamanhoMascara, unsigned int _raioMax) :
	CFEMorfologiaMatematica3D (matriz, _tamanhoMascara),
    	CMatriz3D (matriz->NX (), matriz->NY (), matriz->NZ ()),
    	raioBola (_tamanhoMascara),
    	raioMaximo (_raioMax),
    	indiceAtivo (1),
    	indiceInativo (0) 
{
	path = matriz->Path();
}

/*
==================================================================================
Função    ExecutadaPorGo
==================================================================================
Função usada exclusivamente por Go:

1-Quando o usuário quer calcular a idf, chama a funcao Go (pm).
	No caso da imagem pm passada ter dimemsões diferentes da pm passada para o construtor, devo
	desalocar a matriz idf, definir os novos valores de nx e ny e realocar a idf.
	Se as dimensões da matriz passada e minhas forem iguais posso continuar.

2-Copia valores da matriz passada para a matriz idf, usando data2D[i][j]=pm->data2D[i][j]

	Esta função foi incluída porque o usuário programador pode chamar Go(pm)
	sendo pm diferente daquela passada par o construtor.	Ou seja cria-se uma segurança a mais.

	Outro fato que justifica esta função é que antes, criava objeto idf, executava Go, e depois
	podia executar a erosao, mas sempre sobre a idf da imagem passada para o construtor
	da classe. Agora posso passar uma nova matriz para a função Go, diferente da passada para o construtor.

	O terceiro motivo é que no caso de se realizar uma sequencia de erosoes com a mesma
	mascara, eu precisava criar um objeto idf, calcular go e calcular a erosao,
	criar nova idf (sobre pm erodida), executar go e então realizar a erosao...
	Agora posso recalcular Go a qualquer momento, sem recriar o objeto idf.
	Exemplo:
			cria idf      		pfmm=new TMMIDF(pm,tamanhoMascara);
			executa go    		pfmm->Go(pm,tamanhoMascara);
			executa erosao      pfmm->Erosao(pm,raioBola);       // erosao sobre imagem original
			executa go    		pfmm->Go(pm,tamanhoMascara);
			executa erosao      pfmm->Erosao(pm,raioBola);	// erosao sobre imagem já erodida

Assim, toda funcao Go herdeira da CFEMMIDF deve chamar esta funcao:
	 ExecutadaPorGo(matriz);

*/
// Funcao chamada exclusivamente por Go, ou seja depois de executada a idf vai ser recalculada.
void CFEMMIDF3D::ExecutadaPorGo (CMatriz3D * &matriz)	// ,unsigned int _tamanhoMascara)
{
  pm = matriz;			// armazena endereço matriz
  // verifica se a matriz tem as mesmas dimensoes da idf(this)
  if (this->nx != matriz->NX () || this->ny != matriz->NY () || this->nz != matriz->NZ ())
    {
      Desaloca ();		// desaloca a matriz existende e depois
      this->nx = matriz->NX ();	// define novo tamanho
      this->ny = matriz->NY ();
      this->nz = matriz->NZ ();
      Aloca ();			// aloca de acordo com novo tamanho
    }
  // Agora tenho de armazenar valores de pm na idf

  // substituir por this=matriz;
  register int mi = Mi ();
  for (int i = 0; i < nx; i++)
    for (int j = 0; j < ny; j++)
      for (int k = 0; k < nz; k++)
	   if (matriz->data3D[i][j][k] != 0)	// como a imagem recebida por ser uma outra idf
	     this->data3D[i][j][k] = mi;	// define this com 0 e 1
  // // // // // // // // // // // // // 
	   else			// AQUI AQUI AQUI AQUI: trocar 1 por mi
	     this->data3D[i][j][k] = 0;
  // this->data2D[i][j]=matriz->data2D[i][j];
// Write("ExecutadaPorGo.txt");
}

/*
==================================================================================
Função    VerificaImagem
==================================================================================
Usada pelas funções Erosao,Dilatacao, fechamento e abertura para verificar se é a mesma imagem.
Se for outra imagem recalcula Go.
Tambem armazena endereço da imagem em pm.
*/
void CFEMMIDF3D::VerificaImagem (CMatriz3D * &matriz)
{ // se for a mesma imagem e tiver o mesmo tamanho sai
  if (pm == matriz && nx == matriz->NX () && ny == matriz->NY () && nz == matriz->NZ ())
  	return;		// sai
  else			// senão chama Go, que redefine o tamanho da imagem
    	Go (matriz);	// e calcula valores idf
}				


/*
==================================================================================
Função    Erosao.
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
CMatriz3D * CFEMMIDF3D::Erosao (CMatriz3D * &matriz, unsigned int _RaioBola)	// 
{
  // -->Padrao
  VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
  tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
  // Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara (tamanhoMascara);	// Cria a mascara adequada, do tamanho de tamanhoMascara

  CBCDiscreta3D *maskd = dynamic_cast < CBCDiscreta3D * >(mask);	// Cria ponteiro para mascara com acesso a GetraioBolaTangente

  // Processamento da erosao em si
  int raioBolaInclusa = maskd->RaioBolaInclusa ();
  for (int k = 0; k < nz; k++)
    for (int j = 0; j < ny; j++)
      for (int i = 0; i < nx; i++)
	{
	  // se o ponto da idf for maior que a bola tangente, faz ponto=1
	  if (data3D[i][j][k] > raioBolaInclusa)
	    pm->data3D[i][j][k] = indiceAtivo;	// seta ponto ativo
	  else			//    ,indiceAtivo(1)   ,indiceInativo(0)

	    pm->data3D[i][j][k] = indiceInativo;	// seta ponto inativo
	}
  return pm;			// pm é a matriz erodida
}

/*
==================================================================================
Função    Abertura
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
CMatriz3D * CFEMMIDF3D::Abertura (CMatriz3D * &matriz, unsigned int _RaioBola) {
	VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go) Go não está fazendo nada!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  	tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
  	// Deve calcular o tamanhoMascara antes de criar a mascara
  	CriaMascara (tamanhoMascara);	// Cria a mascara adequada,do tamanho de tamanhoMascara

  	CBCDiscreta3D *maskd = dynamic_cast < CBCDiscreta3D * >(mask);	// Cria ponteiro para mascara com acesso a GetRaioBolaTangente

  	// Processamento da abertura em si
  	// imagemModificada=0;
  	pm->Constante (indiceInativo);	// zera a matriz imagem

  	// Otimizacao Mascara (bola)
  	int raio = maskd->RaioX ();
  	int raioBolaTangente = maskd->RaioBolaTangente ();
  	int raioBolaInclusa = maskd->RaioBolaInclusa ();
  	int imx;			// i mais x, i+x
  	int rmx;			// raio mais x, raio+x
  	int jmy;			// 
  	int rmy;			// 
  	int kmz;			// 
  	int rmz;			// 
  	// Variáveis para SIMETRIA Bola
  	int posxe, posxd;		// x esquerda e x direita
  	int posys, posyn;		// y sul e y norte
  	int poszb, poszf;		// z back e z front
  	int xx, yy, zz;		// posicoes xx e yy da bola
  	// falta verificar o contorno
  	int ***pmdata3D = pm->GetData3D ();
  	for (int k = raio; k < nz - raio; k++)
    		for (int j = raio; j < ny - raio; j++)
      		for (int i = raio; i < nx - raio; i++) {
	   			if (data3D[i][j][k] > raioBolaTangente) {	// se for maior que a bola tangente vai permanecer
	   	 			pmdata3D[i][j][k] = indiceAtivo;
	      			// imagemModificada=1;
	   			} else if (data3D[i][j][k] > raioBolaInclusa) // se for maior que a inclusa e menor ou igual a tangente pintar a bola
	   				// PINTA A BOLA OTIMIZADO: CONSIDERA SIMETRIA
	   				for (zz = 0; zz <= raio; zz++)	{ // percorre a mascara
						poszb = k - zz;
						poszf = k + zz;
						rmz = raio + zz;
						for (yy = 0; yy <= raio; yy++) {
							posyn = j + yy;
							posys = j - yy;
							rmy = raio + yy;
							for (xx = 0; xx <= raio; xx++) {
								posxe = i - xx;	// por ser simétrica
								posxd = i + xx;
								rmx = raio + xx;
								if (maskd->data3D[rmx][rmy][rmz] != 0)
									pmdata3D[posxe][posyn][poszf] =
									pmdata3D[posxe][posys][poszf] =
									pmdata3D[posxd][posyn][poszf] =
									pmdata3D[posxd][posys][poszf] =
									pmdata3D[posxe][posyn][poszb] =
									pmdata3D[posxe][posys][poszb] =
									pmdata3D[posxd][posyn][poszb] =
									pmdata3D[posxd][posys][poszb] = indiceAtivo;
							}
						}
					}
					/*// PINTA A BOLA NAO OTIMIZADO: NAO CONSIDERA SIMETRIA
		   			for (zz=-raio; zz<=raio; zz++) {          // percorre a mascara
						kmz= k + zz;
         					rmz= raio + zz;
						for (yy=-raio; yy<=raio; yy++) {
             					jmy= j + yy;
             					rmy= raio + yy;                          // usar simetria
				 			for (xx=-raio; xx <=raio;xx++) {
               					// imx=i+x;  // rmx=raio+x;
             						if(maskd->data3D[raio+xx][rmy] [rmz]!=0)
						   			pm->data3D[i+xx]   [jmy] [kmz]=indiceAtivo;  // pinta na imagem
             					}
           				}
         				}*/
			}
  	return pm;			// pm é a matriz abertura
}

/*
// Considerando simetria
   unsigned int posxe,posxd;                          // x esquerda e x direita
   unsigned int posys,posyn;	                   	// y sul e y norte
   unsigned int poszf,poszb;                   		// z front , e z back
		   for (int z=0; z<=raio; z++)      	// percorre a mascara
         {
         rmz= raio + z;					// posicao na mascara
         poszf= k + z;					// posicao k da imagem mais z da mascara
         poszb= k - z;
			for (int y=0; y<=raio; y++)
             {
             rmy= raio + y;                           // posicao na mascara
		   	 posys= j - y;			// posicao j da imagem e yy da mascara
	   		 posyn= j + y;
				 for (int x=0; x <=raio;x++)
             	{
               rmx=raio+x;				   // posicao na mascara
				   posxe= i - x;         // posicao i da imagem mais x da mascara
				   posxd= i + x;
             	if(maskd->data2D[rmx][rmy] [rmz]!=0)
		              pm->data3D[posxe][posyn][poszf]=
                    pm->data3D[posxe][posys][poszf]=
      		        pm->data3D[posxe][posyn][poszb]=
                    pm->data3D[posxe][posys][poszb]=

                    pm->data3D[posxd][posyn][poszf]=
                    pm->data3D[posxd][posys][poszf]=
                    pm->data3D[posxd][posyn][poszb]=
                    pm->data3D[posxd][posys][poszb]=indiceAtivo;
             	}
         }
*/

/*================================================================================
Função    CorrigeAbertura
==================================================================================
Este método corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.*/
void CFEMMIDF3D::CorrigeAbertura (CMatriz3D * &matriz, int &regiao) 
{
	// Método - 2
	CMatriz3D *ptr_mask = static_cast<CMatriz3D*>(mask);
	char fileName[64];
	int max;
	// calcula idf d345 da mascara
	CFEMMIDFd3453D *idfMask = NULL;
	idfMask = new CFEMMIDFd3453D(ptr_mask);
	idfMask->Go(ptr_mask);
	
/*	//grava em disco a IDF da mascara.
	static int contMasK = 1;
	sprintf (fileName, "idfMascara%d.dgm", contMasK++);
	idfMask->WriteFormat( WRITEFORM_DI_X_Y_Z_GRAY_ASCII );
  	max = idfMask->MaiorValor();
  	idfMask->NumCores( (max > 1)? max : max+2 );
	idfMask->Path(matriz->Path());
	idfMask->Write(fileName);
*/
	
	// calcula idf d345 da imagem abertura.
	CFEMMIDFd3453D *idfAbertura = NULL;
	idfAbertura = new CFEMMIDFd3453D(matriz);
	idfAbertura->Go(matriz);

/*	//grava em disco a IDF da imagem abertura.
	static int contAbertura = 1;
	sprintf (fileName, "idfAbertura%d.dgm", contAbertura++);
	idfAbertura->WriteFormat( WRITEFORM_DI_X_Y_Z_GRAY_ASCII );
  	max = idfAbertura->MaiorValor();
  	idfAbertura->NumCores( (max > 1)? max : max+2 );
	idfAbertura->Path(matriz->Path());
	idfAbertura->Write(fileName);
*/
	
	// Pega plano central da mascara
	CMatriz2D * pcm = NULL;
 	pcm = idfMask->LePlano(mask->RaioZ(), EIXO_Z);
	
/*	//grava em disco o plano da mascara.
	static int contPlanoMask = 1;
	sprintf (fileName, "planoMascara%d.pgm", contPlanoMask++);
	pcm->WriteFormat( WRITEFORM_PI_X_Y_GRAY_ASCII );
  	max = pcm->MaiorValor();
  	pcm->NumCores( (max > 1)? max : max+2 );
	pcm->Path(matriz->Path());
	pcm->Write(fileName);
*/
	/////////////////////////////////////////////////////////
	// matriz 	-> é a imagem abertura atual.		  	//
	// this 		-> é a IDF da imagem original.	  	//
	// idfAbertura	-> é a IDF da imagem abertura atual. 	//
	// mask 		-> é a máscara atual.			  	//
	// idfMask	-> é a IDF da mascara atual.		  	//
	// pcm 		-> é o plano central de idfMask.		//
	/////////////////////////////////////////////////////////
	pair<int,int> maiorMenor = pcm->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
	int centro = maiorMenor.first;
	int borda  = maiorMenor.second;
	int raiox = mask->RaioX();
	int raioy = mask->RaioY();
	int raioz = mask->RaioZ();
	int cont, temp;
	for (int j = raioy+2; j < ny-raioy-2; j++) {
		for (int k = raioz+2; k < nz-raioz-2; k++) {	
			for (int i = raiox+2; i < nx-raiox-2; i++) { // Percorre a imagem idf
				if ( matriz->data3D[i][j][k] == regiao ) { // Se o pixel analizado faz parte da região abertura
					if ( idfAbertura->data3D[i][j][k] >= centro ) { // Se o ponto da IDF da região abertura for >= ao centro da idfMask
						// Verificacoes para saber se a mascara, centrada no ponto analizado, passa pela regiao abertura e tem suas bordas próximas ao fundo da imagem.
						if ( ( ( idfAbertura->data3D[i-raiox][j][k] == borda ) && ( idfAbertura->data3D[i][j][k-raioz] == borda ) && ( idfAbertura->data3D[i+raiox][j][k] < centro ) && ( idfAbertura->data3D[i+raiox-1][j][k] < centro ) && ( idfAbertura->data3D[i][j][k+raioz] < centro ) && ( idfAbertura->data3D[i][j][k+raioz-1] < centro ) ) 
						  || ( ( idfAbertura->data3D[i-raiox][j][k] == borda ) && ( idfAbertura->data3D[i][j][k-raioz] == borda ) && ( idfAbertura->data3D[i+raiox+1][j][k] == borda ) && ( idfAbertura->data3D[i][j][k+raioz+1] == borda ) )
						  || ( ( idfAbertura->data3D[i-raiox-1][j][k] == borda ) && ( idfAbertura->data3D[i][j][k-raioz-1] == borda ) && ( idfAbertura->data3D[i+raiox+1][j][k] == borda ) && ( idfAbertura->data3D[i][j][k+raioz+1] == borda ) && ( idfAbertura->data3D[i+raiox+2][j][k] == 0 ) && ( idfAbertura->data3D[i][j][k+raioz+2] == 0 ) )
						  || ( ( idfAbertura->data3D[i-raiox-2][j][k] == borda ) && ( idfAbertura->data3D[i][j][k-raioz-2] == borda ) && ( idfAbertura->data3D[i+raiox+2][j][k] < centro ) && ( idfAbertura->data3D[i][j][k+raioz+2] < centro ) && ( idfAbertura->data3D[i+raiox+2][j][k] != 0 ) && ( idfAbertura->data3D[i][j][k+raioz+2] != 0 ) ) ) {
							cont = 0;
							temp = centro-1;
							while ( cont <= raioy ) { // verificas os planos seguintes, na altura do ponto ora analizado, para verificar até onde a mascara passa.
								if (matriz->data3D[i][j+cont+1][k] != regiao){ // o ponto não pertence a região abertura
									cont = 0;
									break;
								}
								if ( idfAbertura->data3D[i][j+cont+1][k] <= temp ){ // a mascara nao passa pola regiao
									temp = idfAbertura->data3D[i][j+cont+1][k];
									cont++;
								} else { //sai do loop
									break;
								}
							}
							if (cont > 0) {
								int inc = cont; //(cont / 2) + 3; //cerr << inc << endl;
								if ( matriz->data3D[i][j+inc][k] == regiao )
									FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i-1][j+inc][k] == regiao )
									FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i+1][j+inc][k] == regiao )
									FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i][j+inc][k-1] == regiao )
									FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i][j+inc][k+1] == regiao )
									FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i-1][j+inc][k-1] == regiao )
									FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i+1][j+inc][k+1] == regiao )
									FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i-1][j+inc][k+1] == regiao )
									FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i+1][j+inc][k-1] == regiao )
									FecharAdjacencias ( i, k, j+inc, matriz  );
							}
						}
					} else if ( (idfAbertura->data3D[i][j][k] == 3) && (idfAbertura->data3D[i-1][j][k] == 0) && (idfAbertura->data3D[i+1][j][k] == 0) 
						&& 	(idfAbertura->data3D[i][j][k-1] == 0) && (idfAbertura->data3D[i][j][k+1] == 0) && (matriz->data3D[i][j-1][k] == regiao) 
						&& 	(matriz->data3D[i][j+1][k] == regiao) ) { //fecha pixeis isolados...
						matriz->data3D[i][j][k] = 0;
					}
				}
			}
		}
    	}
  	delete idfMask;
  	delete idfAbertura;
	delete pcm;

/*	// Método - 1
	CMatriz3D *ptr_mask = static_cast<CMatriz3D*>(mask);
	char fileName[64];
	int max;
	// calcula idf d345 da mascara
	CFEMMIDFd3453D *idfMask = NULL;
	idfMask = new CFEMMIDFd3453D(ptr_mask);
	idfMask->Go(ptr_mask);
	//grava em disco a IDF da mascara.
	static int contMasK = 1;
	sprintf (fileName, "idfMascara%d.dgm", contMasK++);
	idfMask->WriteFormat( WRITEFORM_DI_X_Y_Z_GRAY_ASCII );
  	max = idfMask->MaiorValor();
  	idfMask->NumCores( (max > 1)? max : max+2 );
	idfMask->Path(matriz->Path());
	idfMask->Write(fileName);
/*	// PROVAVELMENTE NÃO SERÁ USADO. SE FOR USAR IDF DA ABERTURA, TERÁ DE CALCULAR A IDF DE CADA PLANO DA IMAGEM ABERTURA E NÃO A IDF DA IMAGEM TODA (3D)
	// calcula idf d345 da imagem abertura.
	CFEMMIDFd3453D *idfAbertura = NULL;
	idfAbertura = new CFEMMIDFd3453D(matriz);
	idfAbertura->Go(matriz);
	//grava em disco a IDF da imagem abertura.
	static int contAbertura = 1;
	sprintf (fileName, "idfAbertura%d.dgm", contAbertura++);
	idfAbertura->WriteFormat( WRITEFORM_DI_X_Y_Z_GRAY_ASCII );
  	max = idfAbertura->MaiorValor();
  	idfAbertura->NumCores( (max > 1)? max : max+2 );
	idfAbertura->Path(matriz->Path());
	idfAbertura->Write(fileName);
		
	// Pega plano central da mascara
	CMatriz2D * pcm = NULL;
// 	pcm = idfMask->LePlano(mask->RaioZ(), EIXO_Z);
	pcm = ptr_mask->LePlano(mask->RaioZ(), EIXO_Z);
	//grava em disco o plano da mascara.
/*	static int contPlanoMask = 1;
	sprintf (fileName, "planoMascara%d.pgm", contPlanoMask++);
	pcm->WriteFormat( WRITEFORM_PI_X_Y_GRAY_ASCII );
  	max = pcm->MaiorValor();
  	pcm->NumCores( (max > 1)? max : max+2 );
	pcm->Path(matriz->Path());
	pcm->Write(fileName);
	/////////////////////////////////////////////////////////
	// matriz 	-> é a imagem abertura atual.		  	//
	// this 		-> é a IDF da imagem original.	  	//
	// idfAbertura	-> é a IDF da imagem abertura atual. 	//
	// mask 		-> é a máscara atual.			  	//
	// idfMask	-> é a IDF da mascara atual.		  	//
	// pcm 		-> é o plano central de mask.			//
	/////////////////////////////////////////////////////////

	//pair<int,int> maiorMenor = pcm->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
	//int centro = maiorMenor.first;
	//int borda  = maiorMenor.second;
	int raiox = mask->RaioX();
	int raioy = mask->RaioY();
	int raioz = mask->RaioZ();
	bool passa; // Se verdadeiro quer dizer que a mascara passa na região abertura
	for (int j = raioy+2; j < ny-raioy-2; j++) {
		for (int k = raioz+2; k < nz-raioz-2; k++) {	// Percorre a imagem idf
			for (int i = raiox+2; i < nx-raiox-2; i++) {
				if ( matriz->data3D[i][j][k] == regiao ) { // Se o pixel analizado faz parte da região abertura
					passa = true;
					for (int x = 0; x <= 2*raiox; x++) { // percorre a mascara
						for (int y = 0; y <= 2*raioy; y++) {
							if ( (pcm->data2D[x][y] != 0) && (matriz->data3D[i-raiox+x][j][k-raioy+y] != regiao) ) {	
							// Se o ponto da mascara faz parte da regiao mascara e o ponto correspondente da imagem abertura não pertence a regiao abertura.
								passa = false;
								break;
							}
						}
						if (!passa) break; // Não passa, vai para o próximo ponto da imagem.
					}
					if (passa) { // A mascara passa pela regiao abertura. verificar se os pontos adjacentes a mascara, ou dois ponto adjacentes e dois pontos adjancentes + 1 não fazem parte da região abertura.
						if ( ( (matriz->data3D[i-raiox-1][j][k] != regiao) && (matriz->data3D[i+raiox+1][j][k] != regiao) && (matriz->data3D[i][j][k-raioy-1] != regiao) && (matriz->data3D[i][j][k+raioy+1] != regiao) )
						  || ( (matriz->data3D[i-raiox-1][j][k] != regiao) && (matriz->data3D[i+raiox+2][j][k] != regiao) && (matriz->data3D[i+raiox+1][j][k] == regiao) && (matriz->data3D[i][j][k-raioy-1] != regiao) && (matriz->data3D[i][j][k+raioy+1] == regiao) && (matriz->data3D[i][j][k+raioy+2] != regiao) )
						  || ( (matriz->data3D[i+raiox+1][j][k] != regiao) && (matriz->data3D[i-raiox-2][j][k] != regiao) && (matriz->data3D[i-raiox-1][j][k] == regiao) && (matriz->data3D[i][j][k-raioy-1] != regiao) && (matriz->data3D[i][j][k+raioy+1] == regiao) && (matriz->data3D[i][j][k+raioy+2] != regiao) )
						  || ( (matriz->data3D[i-raiox-1][j][k] != regiao) && (matriz->data3D[i+raiox+2][j][k] != regiao) && (matriz->data3D[i+raiox+1][j][k] == regiao) && (matriz->data3D[i][j][k+raioy+1] != regiao) && (matriz->data3D[i][j][k-raioy-1] == regiao) && (matriz->data3D[i][j][k-raioy-2] != regiao) )
						  || ( (matriz->data3D[i+raiox+1][j][k] != regiao) && (matriz->data3D[i-raiox-2][j][k] != regiao) && (matriz->data3D[i-raiox-1][j][k] == regiao) && (matriz->data3D[i][j][k+raioy+1] != regiao) && (matriz->data3D[i][j][k-raioy-1] == regiao) && (matriz->data3D[i][j][k-raioy-2] != regiao) ) ) {
							// Os pontos adjacentes a borda da mascara não fazem parte da regiao abertura. verificar a mesma regiao no plano seguinte.
							passa = true;
							for (int x = 0; x <= 2*raiox; x++) { // percorre a mascara
								for (int y = 0; y <= 2*raioy; y++) {
									if ( (pcm->data2D[x][y] != 0) && (matriz->data3D[i-raiox+x][j+1][k-raioy+y] != regiao) ) {	
									// Se o ponto da mascara faz parte da regiao mascara e o ponto correspondente da imagem abertura (no plano seguinte) não pertence a regiao abertura.
										passa = false;
										break;
									}
								}
								if ( ! passa ) break; // Não passa, vai para o próximo ponto da imagem.
							}
							if ( ! passa ) { // A mascara NÃO passa pela regiao abertura (do plano seguinte). Fechar toda a regiao do plano j+raioy
								if ( matriz->data3D[i][j+raioy][k] == regiao )
									FecharAdjacencias ( i, k, j+raioy, matriz  );
								else if ( matriz->data3D[i-1][j+raioy][k] == regiao )
									FecharAdjacencias ( i, k, j+raioy, matriz  );
								else if ( matriz->data3D[i+1][j+raioy][k] == regiao )
									FecharAdjacencias ( i, k, j+raioy, matriz  );
								else if ( matriz->data3D[i][j-raioy][k-1] == regiao )
									FecharAdjacencias ( i, k, j+raioy, matriz  );
								else if ( matriz->data3D[i][j-raioy][k+1] == regiao )
									FecharAdjacencias ( i, k, j+raioy, matriz  );
								else if ( matriz->data3D[i-1][j-raioy][k-1] == regiao )
									FecharAdjacencias ( i, k, j+raioy, matriz  );
								else if ( matriz->data3D[i+1][j-raioy][k+1] == regiao )
									FecharAdjacencias ( i, k, j+raioy, matriz  );
								else if ( matriz->data3D[i-1][j-raioy][k+1] == regiao )
									FecharAdjacencias ( i, k, j+raioy, matriz  );
								else if ( matriz->data3D[i+1][j-raioy][k-1] == regiao )
									FecharAdjacencias ( i, k, j+raioy, matriz  );
							}
						}
					}
				}
			}
		}
    	}
//  	delete idfMask;
//  	delete idfAbertura;
	delete pcm;*/
}

// /*================================================================================
// Função    NumeroPlanosMascaraPassaRegiao
// ==================================================================================
// Metodo recursivo que retorna o número de planos (entre 0 e raioy), a partir do passado como parâmetro, cuja mascara passe pela região informada e atende as adjacências.
// Método utilizado em CorrigeAbertura() */
// int CFEMMIDF3D::NumeroPlanosMascaraPassaRegiao ( int i, int j, int plano, int &raiox, int &raioy, CMatriz3D * &pcm, CMatriz3D * &matriz, int &regiao ){ // na primeira chamada, cont == raioy
// 	static int cont = 0; // número de planos analizados
// 	cont++;
// 	if (cont >= raioy) return cont;
// 	for (int x = 0; x <= 2*raiox; x++) { // percorre a mascara
// 		for (int y = 0; y <= 2*raioy; y++) {
// 			if ( (pcm->data2D[x][y] != 0) && (matriz->data3D[i-raiox+x][j][k-raioy+y] != regiao) ) {	
// 			// Se o ponto da mascara faz parte da regiao mascara e o ponto correspondente da imagem abertura não pertence a regiao abertura.
// 				return cont-1;
// 			}
// 		}
// 	}
// 	if ( ( (matriz->data3D[i-raiox-1][j][k] != regiao) && (matriz->data3D[i+raiox+1][j][k] != regiao) && (matriz->data3D[i][j][k-raioy-1] != regiao) && (matriz->data3D[i][j][k+raioy+1] != regiao) )
// 		|| ( (matriz->data3D[i-raiox-1][j][k] != regiao) && (matriz->data3D[i+raiox+2][j][k] != regiao) && (matriz->data3D[i+raiox+1][j][k] == regiao) && (matriz->data3D[i][j][k-raioy-1] != regiao) && (matriz->data3D[i][j][k+raioy+1] == regiao) && (matriz->data3D[i][j][k+raioy+2] != regiao) )
// 		|| ( (matriz->data3D[i+raiox+1][j][k] != regiao) && (matriz->data3D[i-raiox-2][j][k] != regiao) && (matriz->data3D[i-raiox-1][j][k] == regiao) && (matriz->data3D[i][j][k-raioy-1] != regiao) && (matriz->data3D[i][j][k+raioy+1] == regiao) && (matriz->data3D[i][j][k+raioy+2] != regiao) )
// 		|| ( (matriz->data3D[i-raiox-1][j][k] != regiao) && (matriz->data3D[i+raiox+2][j][k] != regiao) && (matriz->data3D[i+raiox+1][j][k] == regiao) && (matriz->data3D[i][j][k+raioy+1] != regiao) && (matriz->data3D[i][j][k-raioy-1] == regiao) && (matriz->data3D[i][j][k-raioy-2] != regiao) )
// 		|| ( (matriz->data3D[i+raiox+1][j][k] != regiao) && (matriz->data3D[i-raiox-2][j][k] != regiao) && (matriz->data3D[i-raiox-1][j][k] == regiao) && (matriz->data3D[i][j][k+raioy+1] != regiao) && (matriz->data3D[i][j][k-raioy-1] == regiao) && (matriz->data3D[i][j][k-raioy-2] != regiao) ) ) {
// 		// Os pontos adjacentes a borda da mascara não fazem parte da regiao abertura. verificar a mesma regiao no plano seguinte.
// 		return NumeroPlanosMascaraPassaRegiao ( i, j, plano-1, raiox, raioy, matriz, pcm );
// 	}
// 	return cont;
// }
// 
// /*================================================================================
// Função    NumeroPlanosMascaraNaoPassaRegiao
// ==================================================================================
// Metodo recursivo que retorna o número de planos (entre 0 e raioy), a partir do passado como parâmetro, cuja mascara não passe pela região informada.
// Método utilizado em CorrigeAbertura() */
// int CFEMMIDF3D::NumeroPlanosMascaraNaoPassaRegiao ( int i, int j, int plano, int &raiox, int &raioy, CMatriz3D * &pcm, CMatriz3D * &matriz, int &regiao ){ // na primeira chamada, cont == raioy
// 	static int contador = 0; // número de planos analizados
// 	contador++;
// 	if (contador >= raioy) return contador;
// 	for (int x = 0; x <= 2*raiox; x++) { // percorre a mascara
// 		for (int y = 0; y <= 2*raioy; y++) {
// 			if ( (pcm->data2D[x][y] != 0) && (matriz->data3D[i-raiox+x][j][k-raioy+y] != regiao) ) {	
// 			// Se o ponto da mascara faz parte da regiao mascara e o ponto correspondente da imagem abertura não pertence a regiao abertura.
// 				return NumeroPlanosMascaraNaoPassaRegiao ( i, j, plano-1, raiox, raioy, matriz, pcm );
// 			}
// 		}
// 	}
// 	return contador;
// }

/*================================================================================
Função    CorrigeAbertura
==================================================================================
Método recursivo que fechar (marca com 0) todos os pontos adjacentes (no plano) ao ponto passado. Método utilizado em CorrigeAbertura() */
void CFEMMIDF3D::FecharAdjacencias ( int x, int y, int plano, CMatriz3D * &matriz) 
{
	if ( matriz->data3D[x][plano][y] != 0 ) {
		matriz->data3D[x][plano][y] = 0;
		if ( x < matriz->NX()-1 )
			FecharAdjacencias (x+1, y, plano, matriz);
		if ( x > 1 )
			FecharAdjacencias (x-1, y, plano, matriz);
		if ( y < matriz->NZ()-1 )
			FecharAdjacencias (x, y+1, plano, matriz);
		if ( y > 1 )
			FecharAdjacencias (x, y-1, plano, matriz);
	}
}
/*
==================================================================================
Função    Dilatacao
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
CMatriz3D * CFEMMIDF3D::Dilatacao (CMatriz3D * &matriz, unsigned int _RaioBola)
{
  // -->Padrao
  VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
  tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
  // Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara (tamanhoMascara);	// Cria a mascara adequada,do tamanho de tamanhoMascara

  CBCDiscreta3D *maskd = dynamic_cast < CBCDiscreta3D * >(mask);	// Cria ponteiro para mascara com acesso a GetraioBolaTangente

  // Processamento da Dilatacao em si
  // pm->Constante(0);        // zera a matriz imagem
  int mi = Mi ();		// 
  int ***pmdata3D = pm->GetData3D ();
  for (int jj = 0; jj < ny; jj++)	// percorre toda a idf e
    for (int ii = 0; ii < nx; ii++)	// pinta pontos na imagem
      for (int kk = 0; kk < nz; kk++)	// pinta pontos na imagem
	if (data3D[ii][jj][kk] >= mi)
	  pmdata3D[ii][jj][kk] = indiceAtivo;	// pm->data3D[ii][jj][kk]=1;
	else
	  pmdata3D[ii][jj][kk] = indiceInativo;	// pm->data3D[ii][jj][kk]=0;

  // Otimizacao Mascara (bola)
  int raio = maskd->RaioX ();
//   int raioBolaTangente=maskd->GetraioBolaTangente();
//   int raioBolaInclusa=maskd->RaioBolaInclusa();
  int imx;			// i mais x, i+x
  int rmx;			// raio mais x, raio+x
  int jmy;			// 
  int rmy;			// 
  int kmz;			// 
  int rmz;			// 
  // Variáveis para SIMETRIA Bola
  int posxe, posxd;		// x esquerda e x direita
  int posys, posyn;		// y sul e y norte
  int poszb, poszf;		// z back e z front
  int xx, yy, zz;		// posicoes xx e yy da bola
  // falta verificar o contorno
  for (int k = raio; k < nz - raio; k++)
    for (int j = raio; j < ny - raio; j++)
      for (int i = raio; i < nx - raio; i++)
	{
	  if (data3D[i][j][k] == mi)	// usar simetria

/*
// // // // // // // // // // // // // // // // // // // 
// PINTA A BOLA NAO OTIMIZADO: NAO CONSIDERA SIMETRIA
		   for (zz=-raio; zz<=raio; zz++)           // percorre a mascara
         {
         kmz= k + zz;
         rmz= raio + zz;
			for (yy=-raio; yy<=raio; yy++)
           {
             jmy= j + yy;
             rmy= raio + yy;                          // usar simetria
				 for (xx=-raio; xx <=raio;xx++)
             	{
               // imx=i+x;  // rmx=raio+x;
             	if(maskd->data3D[raio+xx][rmy] [rmz]!=0)
						   pm->data3D[i+xx]   [jmy] [kmz]=indiceAtivo;  // pinta na imagem
             	}
           }
         }
// // // // // // // // // // // // // // // // // // // 
*/

// // // // // // // // // // // // // // // // // // // 
// PINTA A BOLA OTIMIZADO: CONSIDERA SIMETRIA
	    for (zz = 0; zz <= raio; zz++)	// percorre a mascara
	      {
		poszb = k - zz;
		poszf = k + zz;
		rmz = raio + zz;
		for (yy = 0; yy <= raio; yy++)
		  {
		    posyn = j + yy;
		    posys = j - yy;
		    rmy = raio + yy;
		    for (xx = 0; xx <= raio; xx++)
		      {
			posxe = i - xx;	// por ser simétrica
			posxd = i + xx;
			rmx = raio + xx;
			if (maskd->data3D[rmx][rmy][rmz] != 0)
			  pmdata3D[posxe][posyn][poszf] =
			    pmdata3D[posxe][posys][poszf] =
			    pmdata3D[posxd][posyn][poszf] =
			    pmdata3D[posxd][posys][poszf] =
			    pmdata3D[posxe][posyn][poszb] =
			    pmdata3D[posxe][posys][poszb] =
			    pmdata3D[posxd][posyn][poszb] =
			    pmdata3D[posxd][posys][poszb] = indiceAtivo;
		      }
		  }
	      }
// // // // // // // // // // // // // // // // // // // 

	}
  // verifica atualização idf
  if (atualizaIDF)		// verifica o flag de atualizacao da idf após dilatação
    Go (pm);			// se ativo recalcula a idf

  return pm;			// pm é a matriz Dilatacao
}

/*
==================================================================================
Função    Fechamento
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
CMatriz3D * CFEMMIDF3D::Fechamento (CMatriz3D * &matriz, unsigned int _RaioBola)
{
  bool atualizaIDF_old = atualizaIDF;	// armazena valor de atualizaIDF
  atualizaIDF = 1;		// ativa, para que a Dilatacao recalcule  a idf
  Dilatacao (matriz, _RaioBola);	// processa a dilatação, e depois Go
  atualizaIDF = atualizaIDF_old;	// atualizaIDF volta ao estado anterior

  Erosao (matriz, _RaioBola);	// Processa a erosão, considerando imagem idf atualizada
  return matriz;
}

/*
// IDFNosPlanosDeContorno(int& base);
void CFEMMIDF3D::IDFNosPlanosDeContorno(int& base)
{
 // -------------------------
 // Percorrer todos os planos, se ativo, fazer=3 (menor distancia)
 // Desta forma posso percorrer o centro da imagem direto,sem me preocupar com acesso a pontos inexistentes.
 // Pensar em : pm->setPlanoX(corte=0,valor=3,condicaoPixel==1);
 int pos;										// posicao intermediaria (temporaria)
 int x,y,z;										// Indices para percorrer a matriz
    // ----------IDA
	 for (y=0; y < ny ;y++)					// plano leste x=0
	  	for (z=0; z < nz ;z++)
			 if (data3D[0][y][z]!=0)
				data3D[0][y][z]=base;
	 for (x=0; x < nx ;x++)           	// plano sul y=0
		for (z=0; z < nz ;z++)
			 if (data3D[x][0][z]!=0)
				data3D[x][0][z]=base;
	 for (y=0; y < ny ;y++)					// plano back z=0
		for (x=0; x < nx ;x++)
			 if (data3D[x][y][0]!=0)
				data3D[x][y][0]=base;

    // ----------VOLTA
	 pos=nx-1;
	 for (y=ny-1; y >0 ;y--)				// plano oeste x=nx-1
	  	for (z=nz-1; z >0 ;z--)
			 if (data3D[pos][y][z]!=0)
				data3D[pos][y][z]=base;
	 pos=ny-1;
	 for (x=nx-1; x >0 ;x--)           	// plano norte y=ny-1
	  	for (z=nz-1; z >0 ;z--)
			 if (data3D[x][pos][z]!=0)
				data3D[x][pos][z]=base;
	 pos=nz-1;
	 for (y=ny-1; y >0 ;y--)				// plano front z=nz-1
      for (x=nx-1; x >0 ;x--)
			 if (data3D[x][y][pos]!=0)
				data3D[x][y][pos]=base;
}        */
//       void IDFNosPlanosDeContornoIda(int& base);     // Calcula a idf nos planos de contorno
//       void IDFNosPlanosDeContornoVolta(int& base);     // Calcula a idf nos planos de contorno
/*
void CFEMMIDF3D::IDFNosPlanosDeContornoIDA(int& base)
{
 int x,y,z;										// Indices para percorrer a matriz
    // ----------IDA
	 for (z=0; z < nz ;z++)            // plano leste x=0
		 for (y=0; y < ny ;y++)
			 if (data3D[0][y][z]!=0)
				data3D[0][y][z]=base;
	 for (z=0; z < nz ;z++)            // plano sul y=0
		 for (x=0; x < nx ;x++)
			 if (data3D[x][0][z]!=0)
				data3D[x][0][z]=base;
	 for (y=0; y < ny ;y++)				  // plano back z=0
		for (x=0; x < nx ;x++)
			 if (data3D[x][y][0]!=0)
				data3D[x][y][0]=base;
}
void CFEMMIDF3D::IDFNosPlanosDeContornoVOLTA(int& base)
{
 int x,y,z;									  // Indices para percorrer a matriz
 int pos;									  // posicao intermediaria (temporaria)
    // ----------VOLTA
	 pos=nx-1;
	  	for (z=nz-1; z >0 ;z--)         // plano oeste x=nx-1
		 for (y=ny-1; y >0 ;y--)
			 if (data3D[pos][y][z]!=0)
				data3D[pos][y][z]=base;
	 pos=ny-1;
	  	for (z=nz-1; z >0 ;z--)         // plano norte y=ny-1
		 for (x=nx-1; x >0 ;x--)
			 if (data3D[x][pos][z]!=0)
				data3D[x][pos][z]=base;
	 pos=nz-1;
	 for (y=ny-1; y >0 ;y--)			  // plano front z=nz-1
      for (x=nx-1; x >0 ;x--)
			 if (data3D[x][y][pos]!=0)
				data3D[x][y][pos]=base;
}
*/


