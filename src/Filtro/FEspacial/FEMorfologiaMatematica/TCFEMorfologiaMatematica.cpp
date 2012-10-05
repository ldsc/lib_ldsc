//  Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
//using namespace std;
#ifndef TCFEMorfologiaMatematica_h
#include "Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica.h"
#endif

#include "Geometria/Bola/BCDiscreta/CBCEuclidiana.h"
#include "Geometria/Bola/BCDiscreta/CBCd5711.h"

/*
	==================================================================================
	Documentacao      CriaMascara
	==================================================================================
	Descrição:
	operacao da morfologia matematica usando metodo do deslocamento
	Se a mascara ja existe deve ser eliminada
	Obs: Acrescentado controle para chamada das funções que criam os elementos
	estruturantes, b1 e b2 que são utilizados nas funções de afinamento e espessamento,
	depois repensar forma de uso
	Programador:      Andre Duarte Bueno
*/
template<typename T>
void TCFEMorfologiaMatematica<T>::CriaMascara ( int _tamanhoMascara ) {
	//  Abaixo dependendo do flag chama função de criação dos elementos estruturantes
	//  b1 e b2
	if ( EEHitMiss1 ) {
		CriaEEB1 ( _tamanhoMascara );
		return;
	} else if ( EEHitMiss2 ) {
		CriaEEB2 ( _tamanhoMascara );
		return;
	}

	if ( this->mask ) {		//  se existe uma mascara
		if ( this->mask->NX () == _tamanhoMascara )	//  e é do mesmo  tamanho
			return;			//  sai
		delete this->mask;		//  se  não é do mesmo tamanho apaga objeto mask
		this->mask = NULL;
	}				//  e abaixo cria uma nova
	// 2007 ->VERIFICAR abaixo (deve criar uma bola CBCEuclidianae não CBCEuclidiana
	//mask = new CBCEuclidiana ( _tamanhoMascara );	//  se não existe a mascara, cria uma nova
	//mudei para testar...
	this->mask = new CBCd5711 ( _tamanhoMascara );	//  se não existe a mascara, cria uma nova
}

//  obs: b1 e b2 poderiam ser lidos do disco
template<typename T>
void TCFEMorfologiaMatematica<T>::CriaEEB1 ( unsigned int /*_tamanhoMascara*/ ) {				//  parametro nao usado
	delete this->mask; 			//  apaga objeto mask
	this->mask = NULL;
	this->mask = new CBCEuclidiana ( 3 );
	if ( this->mask ) {
		this->mask->Constante ( 0 );
		this->mask->data2D[1][1] = 1;
		this->mask->data2D[0][2] = 1;
		this->mask->data2D[1][2] = 1;
		this->mask->data2D[2][2] = 1;
	}
	EEHitMiss1 = EEHitMiss2 = false;
}

//  obs: b1 e b2 poderiam ser lidos do disco
template<typename T>
void TCFEMorfologiaMatematica<T>::CriaEEB2 ( unsigned int /*_tamanhoMascara*/ ) {				//  parametro nao usado
	delete this->mask; 			//  apaga objeto mask
	this->mask = NULL;
	this->mask = new CBCEuclidiana ( 3 );
	if ( this->mask ) {
		this->mask->Constante ( 0 );
		this->mask->data2D[0][0] = 1;
		this->mask->data2D[1][0] = 1;
		this->mask->data2D[2][0] = 1;
	}
	EEHitMiss1 = EEHitMiss2 = false;
}

/*
	==================================================================================
	Documentacao      Erosao
	==================================================================================
	Descrição:
	Operacao da morfologia matematica usando metodo classico, deslocamento do EE.
	Realiza operacao classica de erosao, percorre a imagem e compara com o EE (a mascara)
	Se a multiplicacao da mascara pela imagem der algum valor zero, deve erodir
	Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * TCFEMorfologiaMatematica<T>::Erosao ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola ) {
	this->pm = matriz;
	this->tamanhoMascara = ( 2 * _RaioBola ) + 1;
	if ( this->tamanhoMascara > this->pm->NX() || this->tamanhoMascara > this->pm->NY() ) {
		cerr << "Erro em TCFEMorfologiaMatematica::Erosao: O Mascara é maior que a imagem" << endl;
		return NULL;
	}

	CriaMascara ( this->tamanhoMascara );	//  Se o tamanho da mascara foi alterado vai criar nova mascara
	TCMatriz2D<T> rImg ( *this->pm );		//  Cria uma copia da imagem

	unsigned int i, j, k, l;	//  variaveis auxiliares
	unsigned int raioMascaraX = this->mask->RaioX ();
	unsigned int raioMascaraY = this->mask->RaioY ();
	unsigned int maskNX = this->mask->NX();
	unsigned int maskNY = this->mask->NY();

	unsigned int pmNX = this->pm->NX();
	unsigned int pmNY = this->pm->NY();

	/*
			// Implementada para usar simetria na mascara, mas aparentemente não está mais rápida. fazer testes usando time...
			 unsigned int incK = 0;
			unsigned int incL = 0;
		 // Procede a operação de erosão desconsiderando a borda
		 for (i = raioMascaraX; i < pmNX - raioMascaraX; i++) {
			for (j = raioMascaraY; j < pmNY - raioMascaraY; j++) {
			 if (rImg.data2D[i][j] != FUNDO)	{ //  se o ponto é poro >0, verifica se é para erodir
				for (k = 0; k < raioMascaraX+1; k++) { //Usando simetria para percorrer somente 1/4 da mascara.
												for (l = 0; l < raioMascaraY+1; l++) {//  Se um ponto da mascara estiver ativo e na imagem inativo, erodir o ponto central.
					incK = maskNX-1-k;
					incL = maskNY-1-l;
					if (this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == FUNDO)	{
					 this->pm->data2D[i][j] = FUNDO;
					 goto PulaForMask1;
					} else if (this->mask->data2D[incK][l] > 0 && rImg.data2D[i + incK - raioMascaraX][j + l - raioMascaraY] == FUNDO) {
					 this->pm->data2D[i][j] = FUNDO;
					 goto PulaForMask1;
					} else if (this->mask->data2D[k][incL] > 0 && rImg.data2D[i + k - raioMascaraX][j + incL - raioMascaraY] == FUNDO) {
					 this->pm->data2D[i][j] = FUNDO;
					 goto PulaForMask1;
					} else if (this->mask->data2D[incK][incL] > 0 && rImg.data2D[i + incK - raioMascaraX][j + incL - raioMascaraY] == FUNDO) {
					 this->pm->data2D[i][j] = FUNDO;
					 goto PulaForMask1;
					}
				 }
				}
			 }
			 PulaForMask1: NULL;
			}
		 }
		*/

	//  Procede a operação de erosão desconsiderando a borda
	//  Normal, centro da imagem
	for ( i = raioMascaraX; i < pmNX - raioMascaraX; i++ )
		for ( j = raioMascaraY; j < pmNY - raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] != FUNDO )	//  se o ponto é poro (diferente de FUNDO), verifica se é para erodir
				for ( k = 0; k < maskNX; k++ )
					for ( l = 0; l < maskNY; l++ )
						//  Se um ponto da mascara estiver ativo
						//  e na imagem inativo, erodir o ponto central
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == FUNDO ) {	//  <1
							this->pm->data2D[i][j] = FUNDO;
							goto PulaForMask1;
						}
PulaForMask1:{}
		}

	//  Percorre a Borda superior
	for ( i = raioMascaraX; i < pmNX - raioMascaraX; i++ )
		for ( j = 0; j < raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] != FUNDO )
				for ( k = 0; k < maskNX; k++ )
					for ( l = raioMascaraY-j; l < maskNY; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == FUNDO ) {
							this->pm->data2D[i][j] = FUNDO;
							goto PulaForMask2;
						}
PulaForMask2:{}
		}

	//  Percorre a Borda inferior
	for ( i = raioMascaraX; i < pmNX - raioMascaraX; i++ )
		for ( j = pmNY-1; j >= pmNY-raioMascaraY; j-- ) {
			if ( rImg.data2D[i][j] != FUNDO )
				for ( k = 0; k < maskNX; k++ )
					for ( l = 0; l < maskNY-raioMascaraY + pmNY-j-1; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == FUNDO ) {
							this->pm->data2D[i][j] = FUNDO;
							goto PulaForMask3;
						}
PulaForMask3:{}
		}

	//  Percorre a Borda esquerda
	for ( i = 0; i < raioMascaraX; i++ )
		for ( j = raioMascaraY; j < pmNY - raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] != FUNDO )
				for ( k = raioMascaraX-i; k < maskNX; k++ )
					for ( l = 0; l < maskNY; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == FUNDO ) {
							this->pm->data2D[i][j] = FUNDO;
							goto PulaForMask4;
						}
PulaForMask4:{}
		}

	//  Percorre a Borda direita
	for ( i = pmNX-1; i >= pmNX-raioMascaraX; i-- )
		for ( j = raioMascaraY; j < pmNY-raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] != FUNDO )
				for ( k = 0; k < maskNX-raioMascaraX+pmNX-i-1; k++ )
					for ( l = 0; l < maskNY; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == FUNDO ) {
							this->pm->data2D[i][j] = FUNDO;
							goto PulaForMask5;
						}
PulaForMask5:{}
		}

	//  Percorre o Canto superior  esquerdo
	for ( i = 0; i <= raioMascaraX; i++ )
		for ( j = 0; j <= raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] != FUNDO )
				for ( k = raioMascaraX-i; k < maskNX; k++ )
					for ( l = raioMascaraY-j; l < maskNY; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == FUNDO ) {
							this->pm->data2D[i][j] = FUNDO;
							goto PulaForMask6;
						}
PulaForMask6:{}
		}

	//  Percorre o Canto superior direito
	//for (i = pmNX - raioMascaraX; i < pmNX; i++)
	for ( i = pmNX-1; i >= pmNX-raioMascaraX; i-- )
		for ( j = 0; j <= raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] != FUNDO )
				for ( k = 0; k < maskNX-raioMascaraX+pmNX-i-1; k++ )
					for ( l = raioMascaraY-j; l < maskNY; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == FUNDO ) {
							this->pm->data2D[i][j] = FUNDO;
							goto PulaForMask7;
						}
PulaForMask7:{}
		}

	//  Percorre o Canto  inferior esquerdo
	for ( i = 0; i <= raioMascaraX; i++ )
		for ( j = pmNY-1; j >= pmNY-raioMascaraY; j-- ) {
			if ( rImg.data2D[i][j] != FUNDO )
				for ( k = raioMascaraX-i; k < maskNX; k++ )
					for ( l = 0; l < maskNY-raioMascaraY + pmNY-j-1; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == FUNDO ) {
							this->pm->data2D[i][j] = FUNDO;
							goto PulaForMask8;
						}
PulaForMask8:{}
		}

	//  Percorre o Canto inferior direito
	for ( i = pmNX-1; i >= pmNX-raioMascaraX; i-- )
		for ( j = pmNY-1; j >= pmNY-raioMascaraY; j-- ) {
			if ( rImg.data2D[i][j] != FUNDO )
				for ( k = 0; k < maskNX-raioMascaraX+pmNX-i-1; k++ )
					for ( l = 0; l < maskNY-raioMascaraY + pmNY-j-1; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] == FUNDO ) {
							this->pm->data2D[i][j] = FUNDO;
							goto PulaForMask9;
						}
PulaForMask9:{}
		}

	return this->pm;
}

//  Modificações:
//  2-Procede a operação de erosão separando a imagem em partes internas e externas.
//  a)Procede a operação de erosão no centro da imagem, desconsiderando a borda
//                        ...........
//                        ...*****...
//                        ...*****...
//                        ...*****...
//                        ...........
//  b)Procede a operação de erosão nas regiões da borda (superior, inferior, esquerda e direita)
//                          Superior
//                         ...*****...
//                         ***     ***
//                 Esquerda***     *** Direita
//                         ***     ***
//                         ...*****...
//                          Inferior
//  c)Procede a operação de erosão nos cantos da imagem
//Canto Superior esquerdo   Canto superior direito
//                  ***.....***
//                  ...     ...
//                  ...     ...
//                  ...     ...
//                  ***.....***
//Canto inferior esquerdo   canto inferior direito
//  Valores dos intervalos a serem percorridos
//  Normal, centro da imagem
/*
(x da imagem)  raioMascaraX; <-------->         this->pm->NX()-raioMascaraX
(y da imagem)  raioMascaraY  <-------->         this->pm->NY()-raioMascaraY
(x da mascara)  0            <-------->         this->mask->NX()
(y da mascara)  0            <-------->         this->mask->NY()
*/

/*
==================================================================================
Documentacao      Dilatacao
==================================================================================
Descrição:
operacao da morfologia matematica usando metodo classico, com  deslocamento do EE
dilatacao classica, percorre a imagem com o EE e compara.
//  Modificações:
//  Pensar em trocar a multiplicação por uma combinação do tipo
//  (this->mask->data2D[k][l] & rImg.data2D[i+k-raioMascaraX][j+l-raioMascaraY])  >0)
//  o operador & retorna 1 se ambos os lados forem != de 0.
Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * TCFEMorfologiaMatematica<T>::Dilatacao ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola ) {
	this->pm = matriz;
	this->tamanhoMascara = ( 2 * _RaioBola ) + 1;
	if ( this->tamanhoMascara > this->pm->NX() || this->tamanhoMascara > this->pm->NY() ) {
		cerr << "Erro em TCFEMorfologiaMatematica::Erosao: O Mascara é maior que a imagem" << endl;
		return NULL;
	}
	CriaMascara ( this->tamanhoMascara );	//  criada na classe TFMorfologicos
	unsigned int i, j, k, l;	//  variaveis auxiliares

	TCMatriz2D<T> rImg ( *this->pm );		//  copia a imagem

	unsigned int raioMascaraX = this->mask->RaioX ();	//  variavel auxiliar para acelerar processo
	unsigned int raioMascaraY = this->mask->RaioY ();
	unsigned int maskNX = this->mask->NX();
	unsigned int maskNY = this->mask->NY();

	unsigned int pmNX = this->pm->NX();
	unsigned int pmNY = this->pm->NY();

	//  percorre a matriz imagem, exceto a borda
	for ( i = raioMascaraX; i < ( pmNX - raioMascaraX ); i++ )
		for ( j = raioMascaraY; j < ( pmNY - raioMascaraY ); j++ ) {
			if ( rImg.data2D[i][j] == FUNDO )	//  se o ponto for fundo da imagem, pode ser dilatado
				for ( k = 0; k < maskNX; k++ )
					for ( l = 0; l < maskNY; l++ )
						//  se houver a interseccao de um ponto da bola com a imagem, o ponto i,j da imagem é dilatado
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] != FUNDO ) {
							this->pm->data2D[i][j] = INDICE;
							goto PulaForMask1;
						}
PulaForMask1:{}
		}

	//  Percorre a Borda superior
	//  o y da imagem vai de 0 até raioMascaraY
	//  o y da mascara começa de  raioMascaraY vai até  this->mask->NY()
	//  ou seja, somente a borda superior
	for ( i = raioMascaraX; i < pmNX - raioMascaraX; i++ )
		for ( j = 0; j < raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] == FUNDO )	//  se o ponto for solido(branco), pode ser dilatado
				for ( k = 0; k < maskNX; k++ )
					for ( l = raioMascaraY-j; l < maskNY; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] != FUNDO ) {
							this->pm->data2D[i][j] = INDICE;
							goto PulaForMask2;
						}
PulaForMask2:{}
		}

	//  Percorre a Borda inferior
	for ( i = raioMascaraX; i < pmNX - raioMascaraX; i++ )
		for ( j = pmNY-1; j >= pmNY-raioMascaraY; j-- ) {
			if ( rImg.data2D[i][j] == FUNDO )	//  se o ponto for solido(branco), pode ser dilatado
				for ( k = 0; k < maskNX; k++ )
					for ( l = 0; l < maskNY-raioMascaraY+pmNY-j-1; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] != FUNDO ) {
							this->pm->data2D[i][j] = INDICE;
							goto PulaForMask3;
						}
PulaForMask3:{}
		}

	//  Percorre a Borda esquerda
	for ( i = 0; i < raioMascaraX; i++ )
		for ( j = raioMascaraY; j < pmNY - raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] == FUNDO )	//  se o ponto for solido(branco), pode ser dilatado
				for ( k = raioMascaraX-i; k < maskNX; k++ )
					for ( l = 0; l < maskNY; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] != FUNDO ) {
							this->pm->data2D[i][j] = INDICE;
							goto PulaForMask4;
						}
PulaForMask4:{}
		}

	//  Percorre a Borda direita
	for ( i = pmNX-1; i >= pmNX-raioMascaraX; i-- )
		for ( j = raioMascaraY; j < pmNY - raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] == FUNDO )	//  se o ponto for solido(branco), pode ser dilatado
				for ( k = 0; k < maskNX-raioMascaraX+pmNX-i-1; k++ )
					for ( l = 0; l < maskNY; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] != FUNDO ) {
							this->pm->data2D[i][j] = INDICE;
							goto PulaForMask5;
						}
PulaForMask5:{}
		}

	//  Percorre o Canto superior esquerdo
	for ( i = 0; i <= raioMascaraX; i++ )
		for ( j = 0; j <= raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] == FUNDO )	//  se o ponto for solido(branco), pode ser dilatado
				for ( k = raioMascaraX-i; k < maskNX; k++ )
					for ( l = raioMascaraY-j; l < maskNY; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] != FUNDO ) {
							this->pm->data2D[i][j] = INDICE;
							goto PulaForMask6;
						}
PulaForMask6:{}
		}
	//  Percorre o Canto superior direito
	//for (i = pmNX - raioMascaraX; i < pmNX; i++)
	for ( i = pmNX-1; i >= pmNX-raioMascaraX; i-- )
		for ( j = 0; j <= raioMascaraY; j++ ) {
			if ( rImg.data2D[i][j] == FUNDO )	//  se o ponto for solido(branco), pode ser dilatado
				for ( k = 0; k < maskNX-raioMascaraX+pmNX-i-1; k++ )
					for ( l = raioMascaraY-j; l < maskNY; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] != FUNDO ) {
							this->pm->data2D[i][j] = INDICE;
							goto PulaForMask7;
						}
PulaForMask7:{}
		}

	//  Percorre o Canto  inferior esquerdo
	for ( i = 0; i <= raioMascaraX; i++ )
		for ( j = pmNY-1; j >= pmNY-raioMascaraY; j-- ) {
			if ( rImg.data2D[i][j] == FUNDO )	//  se o ponto for solido(branco), pode ser dilatado
				for ( k = raioMascaraX-i; k < maskNX; k++ )
					for ( l = 0; l < maskNY-raioMascaraY + pmNY-j-1; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] != FUNDO ) {
							this->pm->data2D[i][j] = INDICE;
							goto PulaForMask8;
						}
PulaForMask8:{}
		}

	//  Percorre o Canto inferior direito
	for ( i = pmNX-1; i >= pmNX-raioMascaraX; i-- )
		for ( j = pmNY-1; j >= pmNY-raioMascaraY; j-- ) {
			if ( rImg.data2D[i][j] == FUNDO )	//  se o ponto for solido(branco), pode ser dilatado
				for ( k = 0; k < maskNX-raioMascaraX+pmNX-i-1; k++ )
					for ( l = 0; l < maskNY-raioMascaraY + pmNY-j-1; l++ )
						if ( this->mask->data2D[k][l] > 0 && rImg.data2D[i + k - raioMascaraX][j + l - raioMascaraY] != FUNDO ) {
							this->pm->data2D[i][j] = INDICE;
							goto PulaForMask9;
						}
PulaForMask9:{}
		}

	return this->pm;
}

/*
	==================================================================================
	Documentacao      Fechamento
	==================================================================================
	Descrição:
	operacao da morfologia matematica usando metodo classico do deslocamento do EE.
	O fechamento é a operacao de dilatacao seguida de erosao
	Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * TCFEMorfologiaMatematica<T>::Fechamento ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola ) {
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
	Descrição:
	Operacao da morfologia matematica usando metodo classoco, do deslocamento do EE
	A abertura é a erosao seguida da dilatacao
	A operacao de erosao, cria uma copia de pm, cria a mascara e
	processa a erosao, devolvendo a pm erodida
	A operacao de dilatacao, cria uma copia de pm, cria a mascara
	e processa a dilatacao, devolvendo a pm dilatada
	Pode-se  incluir diretamente aqui o processamento da erosao seguida da dilatacao
	o que deixa o processo mais rapido.
	Obs: observe que o objeto CFAbertura não cria nem usa uma mascara, são os objetos
	erosao e dilatacao que criam e usam uma mascara.
	Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * TCFEMorfologiaMatematica<T>::Abertura ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola ) {
	//  erosao
	if ( Erosao ( matriz, _RaioBola ) == NULL )
		return NULL;
	//  dilatacao
	if ( Dilatacao ( matriz, _RaioBola ) == NULL )
		return NULL;
	// erodiu e dilatou
	return matriz;
}

// Não implementada
template<typename T>
TCMatriz2D<T> * TCFEMorfologiaMatematica<T>::DeteccaoContorno ( TCMatriz2D<T> * &matriz, unsigned int /*_RaioBola*/ ) {
	return matriz;
}

/*
	==================================================================================
	Documentacao      HitMiss
	==================================================================================
	Descrição:
	realiza operação Hit-Mis ou HOM.
	Faz uma copia da imagem, depois processa erosão sobre a matriz original com
	elemento estruturante b1, depois acha o complemento da matriz
	e realiza erosao sobre copia com EE b2,
	depois realiza a intersecção das duas matrizes
	Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * TCFEMorfologiaMatematica<T>::HitMiss ( TCMatriz2D<T> * &matriz, unsigned int /*_RaioBola*/ ) {
	/*TCMatriz2D< int >* matriz2=new TCMatriz2D< int >(matriz);       //  Cria copia da matriz
	matriz2->Inverter();                   //  e transforma na complementar
	EEHitMiss1=true;                       //  ativa criação da mascara para bi
	TCFEMorfologiaMatematica::Erosao(matriz,_RaioBola);//  Erosão sobre a matriz com Bi
	EEHitMiss2=true;                       //  ativa criação da mascara para be
	TCFEMorfologiaMatematica::Erosao(matriz2,_RaioBola);//  Erosão sobre a matriz2 com be
	matriz->Intersecao(matriz2);                              //  intersecção entre matriz e matriz2
	delete matriz2; // apaga objeto imagem
	*/
	return matriz;
}

template<typename T>
TCMatriz2D<T> * TCFEMorfologiaMatematica<T>::Afinamento ( TCMatriz2D<T> * &matriz, unsigned int /*_RaioBola*/ ) {
	/*TCMatriz2D< int >* matriz2=new TCMatriz2D< int >(matriz);      //  Cria copia da matriz
	HitMiss(matriz2,  _RaioBola);                 //  Realiza operação HitMiss
	matriz2->Inverter();                         //  o mesmo que matriz2->Complementar();
	matriz->Intersecao(matriz2);               //  intersecção entre matriz e matriz2
	delete matriz2; // apaga objeto imagem
	*/
	return matriz;
}

template<typename T>
TCMatriz2D<T> * TCFEMorfologiaMatematica<T>::Espessamento ( TCMatriz2D<T> * &matriz, unsigned int /*_RaioBola*/ ) {
	/*TCMatriz2D< int >* matriz2=new TCMatriz2D< int >(matriz);       //  Cria copia da matriz
	HitMiss(matriz2,  _RaioBola);                //  Realiza operação HitMiss
	matriz->Uniao(matriz2);                     //  união entre matriz e matriz2
	delete matriz2; // apaga objeto imagem
	*/
	return matriz;
}

// Implementar
template<typename T>
TCMatriz2D<T> * TCFEMorfologiaMatematica<T>::Esqueleto ( TCMatriz2D<T> * &matriz, unsigned int /*_RaioBola*/ ) {
	return matriz;
}
