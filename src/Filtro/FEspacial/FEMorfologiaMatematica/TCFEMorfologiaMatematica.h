//  itens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEMorfologiaMatematica_h
#define TCFEMorfologiaMatematica_h

//  inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <Filtro/FEspacial/TCFEspacial.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:	Bibliotecas LDSC
		Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, Cunha Neto J.A.B., Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMorfologiaMatematica.h
Nome da classe:     TCFEMorfologiaMatematica
*/

/**
 * @brief Classe básica para os filtros da morfologia matematica.
 *
 * Tem a função que  cria uma máscara, euclidiana (uma bola) que é utilizada como elemento
 * estruturante nas operacoes de filtragem.
 *
 * Obs: para funcionamento do afinamento e espessamento
 * foram acrescentados dois flags e duas funcoes CriaEEB1 e CriaEEB2,
 * posteriormente remodelar este funcionamento, que ficou um pouco confuso.
*/
template<typename T>
class TCFEMorfologiaMatematica : public TCFEspacial<T>
{
		bool EEHitMiss1; 	///<  flags de controle de criacao da mascara
		bool EEHitMiss2;	///<  flags de controle de criacao da mascara
	public:
		int INDICE;
		int FUNDO;

	public:
		/// Construtor
		TCFEMorfologiaMatematica ( TCMatriz2D< T > * &matriz, unsigned int _tamanhoMascara, int _indice=1, int _fundo=0 )
			: TCFEspacial<T> ( matriz, _tamanhoMascara ), EEHitMiss1 ( false ), EEHitMiss2 ( false ), INDICE ( _indice ), FUNDO ( _fundo ) { }

		/**
				 *  Abaixo a funcao Go que é reescrita aqui, vazia, para inibir o
				 * calculo da convolucao realizado na classe CFEspacial<int>
				 * As operacaes da morfologia matematica sao diferentes.
				 * (Verificar necessidade de herança/).
				 * @param matriz
				 * @param _tamanhoMascara
				 * @return
				 */
		virtual TCMatriz2D<T> * Go ( TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara = 0 ) {
			_tamanhoMascara = 0; // evitar warning
			return matriz;
		}

		///  Cria mascara recebe o tamanho da mascara. Cria a mascara adequada.
		virtual void CriaMascara ( int _tamanhoMascara );

		/// Processa  Abertura, as operacoes recebem o raio da bola
		virtual TCMatriz2D<T> *Abertura ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola );

		/// Processa  Erosao, as operacoes recebem o raio da bola
		virtual TCMatriz2D<T> *Erosao ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola );

		/// Processa  Dilatacao, as operacoes recebem o raio da bola
		virtual TCMatriz2D<T> *Dilatacao ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola );

		/// Processa  Fechamento, as operacoes recebem o raio da bola
		virtual TCMatriz2D<T> *Fechamento ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola );

		//  Corrigir arrumar, as funcoes abaixo
		/// Processa  DeteccaoContorno, as operacoes recebem o raio da bola
		virtual TCMatriz2D<T> *DeteccaoContorno ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola );

		/// Processa  HitMiss, as operacoes recebem o raio da bola
		virtual TCMatriz2D<T> *HitMiss ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola );

		/// Processa  Afinamento, as operacoes recebem o raio da bola
		virtual TCMatriz2D<T> *Afinamento ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola );

		/// Processa  Espessamento, as operacoes recebem o raio da bola
		virtual TCMatriz2D<T> *Espessamento ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola );

		/// Processa  Esqueleto, as operacoes recebem o raio da bola
		virtual TCMatriz2D<T> *Esqueleto ( TCMatriz2D<T> * &matriz, unsigned int _RaioBola );

	protected:
		/// Cria mascara do tipo b1, usadas no afinamento e espessamento
		void CriaEEB1 ( unsigned int _tamanhoMascara );

		/// Cria mascara do tipo b2, usadas no afinamento e espessamento
		void CriaEEB2 ( unsigned int _tamanhoMascara );
};

//typedef TCFEMorfologiaMatematica CFEMMClassica;
#include "Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica.cpp"
#endif //  TCFEMorfologiaMatematica_h
