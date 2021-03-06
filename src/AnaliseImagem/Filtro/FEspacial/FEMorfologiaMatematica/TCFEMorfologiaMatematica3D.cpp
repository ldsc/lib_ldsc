// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

// #define TFiltroDLL           // define como sendo uma dll
#ifndef TCFEMorfologiaMatematica3D_h
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica3D.h>
#endif
#include "AnaliseImagem/Geometria/Bola/BCDiscreta/CBCEuclidiana3D.h"
#include "AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd3453D.h"

/*
==================================================================================
Documentacao      CriaMascara
==================================================================================
Descricao:	operacao da morfologia matematica usando metodo do deslocamento
	Se a mascara ja existe deve ser eliminada
 Obs: Acrescentado controle para chamada das funções que criam os elementos
	estruturantes, b1 e b2 que sao utilizados nas funções de afinamento
e espessamento, depois repensar forma de uso
Programador:      Andre Duarte Bueno
*/
template<typename T>
void TCFEMorfologiaMatematica3D<T>::CriaMascara (int _tamanhoMascara)
{
	// Abaixo dependendo do flag chama função de criação dos elementos estruturantes
	// b1 e b2
	if (EEHitMiss1){
		CriaEEB1 (_tamanhoMascara);
		return;
	} else if (EEHitMiss2) {
		CriaEEB2 (_tamanhoMascara);
		return;
	}
	if (this->mask) {			// se existe uma mascara
		if (this->mask->NX () == _tamanhoMascara)	// e é do mesmo  tamanho
			return;			// sai
		delete this->mask;		// se  nao é do mesmo tamanho apaga
		this->mask = NULL;
	}				// e abaixo cria uma nova
	this->mask = new CBCEuclidiana3D (_tamanhoMascara);	// se nao existe a mascara, cria uma nova
	//this->mask = new CBCd3453D (_tamanhoMascara);	// se nao existe a mascara, cria uma nova
	// testar alocacao.. TOperacao::TestaAlocacao(this->mask,"ponteiro para mask, Funcao CriaMascara");
}

// obs: b1 e b2 poderiam ser lidos do disco
template<typename T>
void TCFEMorfologiaMatematica3D<T>::CriaEEB1 (unsigned int /*_tamanhoMascara*/)
{				// parametro nao usado
	/*delete this->mask;
this->mask=new CMCEuclidiana3D(3);
if(this->mask)
 {
 this->mask->Constante(0);
 this->mask->data3D[1][1][0]=1;
 this->mask->data3D[0][2][0]=1;
 this->mask->data3D[1][2][0]=1;
 this->mask->data3D[2][2][0]=1;
	 }
EEHitMiss1=EEHitMiss2=false;  */
}

// obs: b1 e b2 poderiam ser lidos do disco
template<typename T>
void TCFEMorfologiaMatematica3D<T>::CriaEEB2 (unsigned int /*_tamanhoMascara*/ )
{				// parametro nao usado
	/*delete this->mask;
this->mask=new CMCEuclidiana3D(3);
if(this->mask)
 {
 this->mask->Constante(0);
 this->mask->data3D[0][0][0]=1;
 this->mask->data3D[1][0][0]=1;
 this->mask->data3D[2][0][0]=1;
	 }
EEHitMiss1=EEHitMiss2=false; */
}

/*

==================================================================================
Documentacao      Erosao
==================================================================================
Descricao:
Operacao da morfologia matematica usando metodo classico, deslocamento do EE.
Realiza operacao classica de erosao, percorre a imagem e compara com o EE (a mascara)
Se a multiplicacao da mascara pela imagem der algum valor zero, deve erodir

Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz3D<T> * TCFEMorfologiaMatematica3D<T>::Erosao (TCMatriz3D<T> * & matriz,  unsigned int _RaioBola ) {
	this->pm = matriz;
	this->tamanhoMascara = (2 * _RaioBola) + 1;
	if ( this->tamanhoMascara > this->pm->NX() || this->tamanhoMascara > this->pm->NY() || this->tamanhoMascara > this->pm->NZ() ) {
		cerr << "Erro em TCFEMorfologiaMatematica::Erosao: O Mascara é maior que a imagem" << endl;
		return NULL;
	}
	CriaMascara(this->tamanhoMascara);            // Se o tamanho da mascara foi alterado vai cria nova mascara
	unsigned int i, j, k, l, m, n;

	TCMatriz3D<T> rImg ( *this->pm );    					 // Cria uma copia da imagem
	unsigned int raioMascaraX= this->mask->RaioX();     // variaveis auxiliares
	unsigned int raioMascaraY= this->mask->RaioY();
	unsigned int raioMascaraZ= this->mask->RaioZ();
	unsigned int maskNX = this->mask->NX();
	unsigned int maskNY = this->mask->NY();
	unsigned int maskNZ = this->mask->NZ();
	unsigned int pmNX = this->pm->NX();
	unsigned int pmNY = this->pm->NY();
	unsigned int pmNZ = this->pm->NZ();

	//  Procede a operação de erosão desconsiderando a borda
	//  Normal, centro da imagem
	for (  i = raioMascaraX; i <  pmNX - raioMascaraX; i++ )
		for (  j = raioMascaraY; j < pmNY - raioMascaraY; j++ )
			for (  k = raioMascaraZ; k < pmNZ - raioMascaraZ; k++ ) {
				if(rImg.data3D[i][j][k] != FUNDO) // se o ponto é poro (diferente de FUNDO), verifica se é para erodir
					for( l = 0; l < maskNX; l++)
						for( m = 0; m < maskNY; m++)
							for( n = 0; n < maskNZ; n++)
								// Se um ponto da mascara estiver ativo
								// e na imagem inativo, erodir o ponto central
								if( this->mask->data3D[l][m][n] > 0 && rImg.data3D[i+l-raioMascaraX][j+m-raioMascaraY][k+n-raioMascaraZ] == FUNDO) {
									this->pm->data3D[i][j][k] = FUNDO;
									goto PulaForMask1;
								}
PulaForMask1:{}
			}

	// Falta implementar a erosão nas bordas e nos cantos.
	return this->pm;
}
/*
// Modificações:
 // 2-Procede a operação de erosao separando a imagem em partes internas e externas.
 // a)Procede a operação de erosao no centro da imagem, desconsiderando a borda
 // 			...........
 // 			...*****...
 // 			...*****...
 // 			...*****...
 // 			...........
 // b)Procede a operação de erosao nas regiões da borda (superior, inferior, esquerda e direita)
 // 			   Superior
 // 			  ...*****...
 // 			  ***     ***
 // Esquerda***     *** Direita
 // 			  ***     ***
 // 			  ...*****...
 // 				 Inferior
 // c)Procede a operação de erosao nos cantos da imagem
 // 	Canto Superior esquerdo		Canto superior direito
 // 							 ***.....***
 // 			 				 ...     ...
 // 			 				 ...     ...
 // 			 				 ...     ...
 // 			 				 ***.....***
 // Canto inferior esquerdo		canto inferior direito

 // Valores dos intervalos a serem percorridos
 // Normal, centro da imagem

 (x da imagem)	raioMascaraX; <-------->		this->pm->NX()-raioMascaraX
 (y da imagem)  raioMascaraY  <-------->		this->pm->NY()-raioMascaraY
 (x da mascara)	0				  <-------->		this->mask->NX()
 (y da mascara)	0				  <-------->		this->mask->NY()
*/

/*
==================================================================================
Documentacao      Dilatacao
==================================================================================
Descricao:
operacao da morfologia matematica usando metodo classico, com  deslocamento do EE
dilatacao classica, percorre a imagem com o EE e compara.
		 // Modificações:

		 // Pensar em trocar a multiplicação por uma combinação do tipo

		 // (this->mask->data3D[k][l] & rImg.data3D[i+k-raioMascaraX][j+l-raioMascaraY])  >0)
		 // o operador & retorna 1 se ambos os lados forem != de 0.

Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz3D<T> * TCFEMorfologiaMatematica3D<T>::Dilatacao (TCMatriz3D<T> * &matriz, unsigned int _RaioBola) {
	this->pm = matriz;
	this->tamanhoMascara = ( 2 * _RaioBola ) + 1;
	if ( this->tamanhoMascara > this->pm->NX() || this->tamanhoMascara > this->pm->NY() || this->tamanhoMascara > this->pm->NZ() ) {
		cerr << "Erro em TCFEMorfologiaMatematica::Erosao: O Mascara é maior que a imagem" << endl;
		return NULL;
	}
	CriaMascara(this->tamanhoMascara);              	 // criada na classe CFEMorfologicos
	unsigned int i, j, k, l, m, n;                            // variaveis auxiliares

	TCMatriz3D<T> rImg( *this->pm ); 						 // copia a imagem
	unsigned int raioMascaraX= this->mask->RaioX();     // variaveis auxiliares
	unsigned int raioMascaraY= this->mask->RaioY();
	unsigned int raioMascaraZ= this->mask->RaioZ();
	unsigned int maskNX = this->mask->NX();
	unsigned int maskNY = this->mask->NY();
	unsigned int maskNZ = this->mask->NZ();
	unsigned int pmNX = this->pm->NX();
	unsigned int pmNY = this->pm->NY();
	unsigned int pmNZ = this->pm->NZ();

	for (  i = raioMascaraX; i <  pmNX - raioMascaraX; i++ )
		for (  j = raioMascaraY; j < pmNY - raioMascaraY; j++ )
			for (  k = raioMascaraZ; k < pmNZ - raioMascaraZ; k++ ) {
				if(rImg.data3D[i][j][k] == FUNDO) // se o ponto é poro (diferente de FUNDO), verifica se é para erodir
					for( l = 0; l < maskNX; l++)
						for( m = 0; m < maskNY; m++)
							for( n = 0; n < maskNZ; n++)
								// se houver a interseccao de um ponto da bola com a imagem, o ponto i,j,k da imagem é dilatado
								if( this->mask->data3D[l][m][n] > 0 && rImg.data3D[i+l-raioMascaraX][j+m-raioMascaraY][k+n-raioMascaraZ] != FUNDO) {
									this->pm->data3D[i][j][k] = INDICE;
									goto PulaForMask1;
								}
PulaForMask1:{}
			}

	// Falta implementar a dilatação nas bordas e nos cantos.
	return this->pm;
}

/*
==================================================================================
Documentacao      Fechamento
==================================================================================
Descricao:
operacao da morfologia matematica usando metodo classico do deslocamento do EE.
O fechamento é a operacao de dilatacao seguida de erosao
Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz3D<T> * TCFEMorfologiaMatematica3D<T>::Fechamento (TCMatriz3D<T> * &matriz, unsigned int _RaioBola) {
	//  dilatacao
	if ( Dilatacao( matriz, _RaioBola ) == NULL )
		return NULL;
	//  erosao
	if ( Erosao ( matriz, _RaioBola ) == NULL )
		return NULL;
	// dilatou e erodiu
	return matriz;
}

/*

==================================================================================
Documentacao      Abertura
==================================================================================
Descricao:
Operacao da morfologia matematica usando metodo classoco, do deslocamento do EE
A abertura é a erosao seguida da dilatacao
A operacao de erosao, cria uma copia de pm, cria a mascara e processa a erosao, devolvendo a pm erodida
A operacao de dilatacao, cria uma copia de pm, cria a mascara e processa a dilatacao, devolvendo a pm dilatada
Pode-se  incluir diretamente aqui o processamento da erosao seguida da dilatacao o que deixa o processo mais rapido.
Obs: observe que o objeto TFAbertura nao cria nem usa uma mascara, sao os objetos erosao e dilatacao que criam e usam uma mascara.
Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz3D<T> * TCFEMorfologiaMatematica3D<T>::Abertura (TCMatriz3D<T> * &matriz, unsigned int _RaioBola) {
	//  erosao
	if ( Erosao ( matriz, _RaioBola ) == NULL )
		return NULL;
	//  dilatacao
	if ( Dilatacao ( matriz, _RaioBola ) == NULL )
		return NULL;
	// erodiu e dilatou
	return matriz;
}

template<typename T>
TCMatriz3D<T> * TCFEMorfologiaMatematica3D<T>::DeteccaoContorno (TCMatriz3D<T> * &matriz, unsigned int /*_RaioBola*/ ) {
	return matriz;
}

/*
==================================================================================
Documentacao      HitMiss
==================================================================================
Descricao:
 realiza operação Hit-Mis ou HOM.
 Faz uma copia da imagem, depois processa erosao sobre a matriz original com
 elemento estruturante b1, depois acha o complemento da matriz
	 e realiza erosao sobre copia com EE b2,
 depois realiza a intersecção das duas matrizes

Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz3D<T> * TCFEMorfologiaMatematica3D<T>::HitMiss (TCMatriz3D<T> * &matriz, unsigned int /*_RaioBola*/ ) {
	/*TCMatriz3D<int> * matriz2=new CMatriz3D(matriz);	// Cria copia da matriz
	matriz2->Inverter();                   // e transforma na complementar
	EEHitMiss1=true;                       // ativa criação da mascara para bi
	TCFEMorfologiaMatematica3D::Erosao(matriz,_RaioBola);// Erosao sobre a matriz com Bi
	EEHitMiss2=true;                       // ativa criação da mascara para be
	TCFEMorfologiaMatematica3D::Erosao(matriz2,_RaioBola);// Erosao sobre a matriz2 com be
	matriz->Intersecao(matriz2);				// intersecção entre matriz e matriz2
	delete matriz2;
	*/
	return matriz;
}

template<typename T>
TCMatriz3D<T> * TCFEMorfologiaMatematica3D<T>::Afinamento (TCMatriz3D<T> * &matriz, unsigned int /*_RaioBola*/) {
	/*TCMatriz3D<int> * matriz2=new CMatriz3D(matriz);       // Cria copia da matriz
	HitMiss(matriz2,  _RaioBola);                       // Realiza operação HitMiss
	matriz2->Inverter();                               // o mesmo que matriz2->Complementar();
	matriz->Intersecao(matriz2);                    // intersecção entre matriz e matriz2
	delete matriz2;
	*/
	return matriz;
}

template<typename T>
TCMatriz3D<T> * TCFEMorfologiaMatematica3D<T>::Espessamento (TCMatriz3D<T> * &matriz, unsigned int /*_RaioBola*/ ) {
	/*TCMatriz3D<int> * matriz2=new CMatriz3D(matriz); // Cria copia da matriz
	HitMiss(matriz2,  _RaioBola);    // Realiza operação HitMiss
	matriz->Uniao(matriz2);          // união entre matriz e matriz2
	delete matriz2;
	*/
	return matriz;
}

// Implementar
template<typename T>
TCMatriz3D<T> * TCFEMorfologiaMatematica3D<T>::Esqueleto (TCMatriz3D<T> * &matriz, unsigned int /*_RaioBola*/ ) {
	return matriz;
}
