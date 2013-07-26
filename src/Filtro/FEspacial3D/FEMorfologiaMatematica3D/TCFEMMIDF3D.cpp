// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
	Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi, Leandro Puerari, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIDF3D.cpp
Nome da classe:      TCFEMMIDF3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa as funções da classe TCFEMMIDF3D.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#ifndef TCFEMMIDF3D_h
#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDF3D.h"// Classe base
#endif
#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h" // Utilizado pelo método CorrigeAbertura
#include "Geometria/Bola/BCDiscreta3D/CBCDiscreta3D.h" // Cria objeto

/*
==================================================================================
Atributos estaticos da classe
==================================================================================
*/
// Se verdadeira após cada processamento recalcula a idf
template<typename T>
bool  TCFEMMIDF3D<T>::atualizaIDF = 0;	// por default, não recalcula a idf apos dilatacao ou fechamento

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
template<typename T>
TCFEMMIDF3D<T>::TCFEMMIDF3D (TCMatriz3D<T> * &matriz, unsigned int _tamanhoMascara, unsigned int _raioMax, int _indice, int _fundo )
	:TCFEMorfologiaMatematica3D<T> (matriz, _tamanhoMascara, _indice, _fundo ), TCMatriz3D<int> (matriz->NX (), matriz->NY (), matriz->NZ ()),
		raioMaximo (_raioMax) {
	raioBola = _tamanhoMascara;
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

2-Copia valores da matriz passada para a matriz idf, usando data2D[i][j]=this->pm->data2D[i][j]

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
	 executa erosao   pfmm->Erosao(pm,raioBola);       // erosao sobre imagem original
	 executa go    		pfmm->Go(pm,tamanhoMascara);
	 executa erosao   pfmm->Erosao(pm,raioBola);	// erosao sobre imagem já erodida

Assim, toda funcao Go herdeira da TCFEMMIDF deve chamar esta funcao:
	ExecutadaPorGo(matriz);

*/
// Funcao chamada exclusivamente por Go, ou seja depois de executada a idf vai ser recalculada.
template<typename T>
void TCFEMMIDF3D<T>::ExecutadaPorGo (TCMatriz3D<T> *&matriz) {	// ,unsigned int _tamanhoMascara) {
	this->pm = matriz;			// armazena endereço matriz
	// verifica se a matriz tem as mesmas dimensoes da idf(this)
	if (this->nx != matriz->NX () || this->ny != matriz->NY () || this->nz != matriz->NZ ()) {
		Desaloca ();		// desaloca a matriz existende e depois
		this->nx = matriz->NX ();	// define novo tamanho
		this->ny = matriz->NY ();
		this->nz = matriz->NZ ();
		Aloca ();			// aloca de acordo com novo tamanho
	}
	// Agora tenho de armazenar valores de pm na idf
	// substituir por this=matriz;
	register int mi = Mi ();
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < nx; i++) {
		for (j = 0; j < ny; j++) {
			for (k = 0; k < nz; k++) {
				if (matriz->data3D[i][j][k] != this->FUNDO) {	// como a imagem recebida por ser uma outra idf
					this->data3D[i][j][k] = mi;
				} else {
					this->data3D[i][j][k] = 0; // O fundo da IDF é sempre 0
				}
			}
		}
	}
	//Write("ExecutadaPorGo.dgm");
}

/*
==================================================================================
Função    VerificaImagem
==================================================================================
Usada pelas funções Erosao,Dilatacao, fechamento e abertura para verificar se é a mesma imagem.
Se for outra imagem recalcula Go.
Tambem armazena endereço da imagem em pm.
*/
template<typename T>
void TCFEMMIDF3D<T>::VerificaImagem (TCMatriz3D<T> *&matriz) {
	// se for a mesma imagem e tiver o mesmo tamanho sai
	if (this->pm == matriz && nx == matriz->NX () && ny == matriz->NY () && nz == matriz->NZ ()) {
		return;		// sai
	} else {			// senão chama Go, que redefine o tamanho da imagem
		this->Go (matriz);	// e calcula valores idf
	}
}				


/*
==================================================================================
Função    Erosao.
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
template<typename T>
TCMatriz3D<T> * TCFEMMIDF3D<T>::Erosao (TCMatriz3D<T> * &matriz, unsigned int _RaioBola) {
	// -->Padrao
	VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
	this->tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
	// Deve calcular o tamanhoMascara antes de criar a mascara
	this->CriaMascara (this->tamanhoMascara);	// Cria a mascara adequada, do tamanho de tamanhoMascara
	CBCDiscreta3D *maskd = dynamic_cast < CBCDiscreta3D * >(this->mask);	// Cria ponteiro para mascara com acesso a GetRaioBolaTangente
	// Processamento da erosao em si
	int raioBolaInclusa = maskd->RaioBolaInclusa ();
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (k = 0; k < nz; k++) {
		for (j = 0; j < ny; j++) {
			for (i = 0; i < nx; i++) {
				// se o ponto da idf for maior que a bola tangente, faz ponto=1
				if (data3D[i][j][k] > raioBolaInclusa) {
					this->pm->data3D[i][j][k] = this->INDICE; //indiceAtivo;	// seta ponto ativo
				} else {
					this->pm->data3D[i][j][k] = this->FUNDO; //indiceInativo;	// seta ponto inativo
				}
			}
		}
	}
	return this->pm; // pm é a matriz erodida
}

/*
==================================================================================
Função    Dilatacao
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
template<typename T>
TCMatriz3D<T> * TCFEMMIDF3D<T>::Dilatacao (TCMatriz3D<T> * &matriz, unsigned int _RaioBola) {
	// -->Padrao
	VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
	this->tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
	// Deve calcular o tamanhoMascara antes de criar a mascara
	this->CriaMascara ( this->tamanhoMascara );	// Cria a mascara adequada,do tamanho de tamanhoMascara

	CBCDiscreta3D *maskd = dynamic_cast < CBCDiscreta3D * >(this->mask);	// Cria ponteiro para mascara com acesso a GetraioBolaTangente

	// Processamento da Dilatacao em si
	// pm->Constante(0);        // zera a matriz imagem
	int mi = Mi ();		//
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < nx; i++)	// percorre toda a idf e pinta pontos na imagem
		for (j = 0; j < ny; j++)
			for (k = 0; k < nz; k++)
				if (data3D[i][j][k] >= mi)
					this->pm->data3D[i][j][k] = this->INDICE; //indiceAtivo;	// pm->data3D[ii][jj][kk]=1;
				else
					this->pm->data3D[i][j][k] = this->FUNDO; //indiceInativo;	// pm->data3D[ii][jj][kk]=0;

	// Otimizacao Mascara (bola)
	int raio = maskd->RaioX ();
	//   int raioBolaTangente=maskd->GetraioBolaTangente();
	//   int raioBolaInclusa=maskd->RaioBolaInclusa();
	int rmx;			// raio mais x, raio+x
	int rmy;			//
	int rmz;			//
	// Variáveis para SIMETRIA Bola
	int posxe, posxd;		// x esquerda e x direita
	int posys, posyn;		// y sul e y norte
	int poszb, poszf;		// z back e z front
	int xx, yy, zz;		// posicoes xx e yy da bola
	// falta verificar o contorno
	for (k = raio; k < nz - raio; k++) {
		for (j = raio; j < ny - raio; j++) {
			for (i = raio; i < nx - raio; i++) {
				if (data3D[i][j][k] == mi)	{// usar simetria
					// PINTA A BOLA OTIMIZADO: CONSIDERA SIMETRIA
					for (zz = 0; zz <= raio; zz++) {	// percorre a mascara
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
								if (maskd->data3D[rmx][rmy][rmz] != 0) {
									this->pm->data3D[posxe][posyn][poszf] = this->INDICE; //indiceAtivo;
									this->pm->data3D[posxe][posys][poszf] = this->INDICE; //indiceAtivo;
									this->pm->data3D[posxd][posyn][poszf] = this->INDICE; //indiceAtivo;
									this->pm->data3D[posxd][posys][poszf] = this->INDICE; //indiceAtivo;
									this->pm->data3D[posxe][posyn][poszb] = this->INDICE; //indiceAtivo;
									this->pm->data3D[posxe][posys][poszb] = this->INDICE; //indiceAtivo;
									this->pm->data3D[posxd][posyn][poszb] = this->INDICE; //indiceAtivo;
									this->pm->data3D[posxd][posys][poszb] = this->INDICE; //indiceAtivo;
								}
							}
						}
					}
					/* // PINTA A BOLA NAO OTIMIZADO: NAO CONSIDERA SIMETRIA
					for (zz=-raio; zz<=raio; zz++) { // percorre a mascara
						kmz= k + zz;
						rmz= raio + zz;
						for (yy=-raio; yy<=raio; yy++) {
							jmy= j + yy;
							rmy= raio + yy;
							// usar simetria
							for (xx=-raio; xx <=raio;xx++) {
								// imx=i+x;  // rmx=raio+x;
								if(maskd->data3D[raio+xx][rmy] [rmz]!=0) {
									this->pm->data3D[i+xx][jmy][kmz]=this->INDICE;;  // pinta na imagem
								}
							}
						}
					}*/
				}
			}
		}
	}
	// verifica atualização idf
	if ( atualizaIDF ) // verifica o flag de atualizacao da idf após dilatação
		this->Go ( this->pm );		 // se ativo recalcula a idf
	return this->pm;		    // pm é a matriz Dilatacao
}

/*
==================================================================================
Função    Fechamento
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
template<typename T>
TCMatriz3D<T> * TCFEMMIDF3D<T>::Fechamento (TCMatriz3D<T> * &matriz, unsigned int _RaioBola) {
	bool atualizaIDF_old = atualizaIDF;	// armazena valor de atualizaIDF
	atualizaIDF = 1;		// ativa, para que a Dilatacao recalcule  a idf
	Dilatacao (matriz, _RaioBola);	// processa a dilatação, e depois Go
	atualizaIDF = atualizaIDF_old;	// atualizaIDF volta ao estado anterior
	Erosao (matriz, _RaioBola);	// Processa a erosão, considerando imagem idf atualizada
	return matriz;
}

/*
==================================================================================
Função    Abertura
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
template<typename T>
TCMatriz3D<T> * TCFEMMIDF3D<T>::Abertura (TCMatriz3D<T> * &matriz, unsigned int _RaioBola) {
	VerificaImagem (matriz); // verifica se é a mesma imagem (se diferente recalcula Go) Go não está fazendo nada!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	this->tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
	// Deve calcular o tamanhoMascara antes de criar a mascara
	this->CriaMascara (this->tamanhoMascara);	// Cria a mascara adequada,do tamanho de tamanhoMascara

	CBCDiscreta3D *maskd = dynamic_cast < CBCDiscreta3D * >(this->mask);	// Cria ponteiro para mascara com acesso a GetRaioBolaTangente

	// Processamento da abertura em si
	// imagemModificada=0;
	this->pm->Constante (this->FUNDO);	// zera a matriz imagem

	// Otimizacao Mascara (bola)
	int raio = maskd->RaioX ();
	int raioBolaTangente = maskd->RaioBolaTangente ();
	int raioBolaInclusa = maskd->RaioBolaInclusa ();
	int rmx;			// raio mais x, raio+x
	int rmy;			//
	int rmz;			//

	// Variáveis para SIMETRIA Bola
	int posxe, posxd;		// x esquerda e x direita
	int posys, posyn;		// y sul e y norte
	int poszb, poszf;		// z back e z front
	int xx, yy, zz;		// posicoes xx e yy da bola

	// falta verificar o contorno
	for (int k = raio; k < nz - raio; k++) {
		for (int j = raio; j < ny - raio; j++) {
			for (int i = raio; i < nx - raio; i++) {
				if (data3D[i][j][k] > raioBolaTangente) {	// se for maior que a bola tangente vai permanecer
					this->pm->data3D[i][j][k] = this->INDICE;
					// imagemModificada=1;
				} else if (data3D[i][j][k] > raioBolaInclusa) { // se for maior que a inclusa e menor ou igual a tangente pintar a bola
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
								if (maskd->data3D[rmx][rmy][rmz] != 0) {
									this->pm->data3D[posxe][posyn][poszf] = this->INDICE;
									this->pm->data3D[posxe][posys][poszf] = this->INDICE;
									this->pm->data3D[posxd][posyn][poszf] = this->INDICE;
									this->pm->data3D[posxd][posys][poszf] = this->INDICE;
									this->pm->data3D[posxe][posyn][poszb] = this->INDICE;
									this->pm->data3D[posxe][posys][poszb] = this->INDICE;
									this->pm->data3D[posxd][posyn][poszb] = this->INDICE;
									this->pm->data3D[posxd][posys][poszb] = this->INDICE;
								}
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
												if(maskd->data3D[raio+xx][rmy] [rmz]!=0) {
													 this->pm->data3D[i+xx]   [jmy] [kmz]=this->INDICE;  // pinta na imagem
												}
										 }
									}
							 }*/
				}
			}
		}
	}
	return this->pm;			// pm é a matriz abertura
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
								this->pm->data3D[posxe][posyn][poszf]=
										this->pm->data3D[posxe][posys][poszf]=
								this->pm->data3D[posxe][posyn][poszb]=
										this->pm->data3D[posxe][posys][poszb]=

										this->pm->data3D[posxd][posyn][poszf]=
										this->pm->data3D[posxd][posys][poszf]=
										this->pm->data3D[posxd][posyn][poszb]=
										this->pm->data3D[posxd][posys][poszb]=indiceAtivo;
							}
				 }
*/

// /*================================================================================
// Função    NumeroPlanosMascaraPassaRegiao
// ==================================================================================
// Metodo recursivo que retorna o número de planos (entre 0 e raioy), a partir do passado como parâmetro, cuja mascara passe pela região informada e atende as adjacências.
// Método utilizado em CorrigeAbertura() */
// int TCFEMMIDF3D::NumeroPlanosMascaraPassaRegiao ( int i, int j, int plano, int &raiox, int &raioy, TCMatriz3D<int> * &pcm, TCMatriz3D<int> * &matriz, int &regiao ){ // na primeira chamada, cont == raioy
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
// int TCFEMMIDF3D::NumeroPlanosMascaraNaoPassaRegiao ( int i, int j, int plano, int &raiox, int &raioy, TCMatriz3D<int> * &pcm, TCMatriz3D<int> * &matriz, int &regiao ){ // na primeira chamada, cont == raioy
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
template<typename T>
void TCFEMMIDF3D<T>::FecharAdjacencias ( int x, int y, int plano, TCMatriz3D<T> * &matriz) {
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
// IDFNosPlanosDeContorno(int& base);
void TCFEMMIDF3D::IDFNosPlanosDeContorno(int& base)
{
 // -------------------------
 // Percorrer todos os planos, se ativo, fazer=3 (menor distancia)
 // Desta forma posso percorrer o centro da imagem direto,sem me preocupar com acesso a pontos inexistentes.
 // Pensar em : this->pm->setPlanoX(corte=0,valor=3,condicaoPixel==1);
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
void TCFEMMIDF3D::IDFNosPlanosDeContornoIDA(int& base)
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
void TCFEMMIDF3D::IDFNosPlanosDeContornoVOLTA(int& base)
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
