// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEMorfologiaMatematica3D_h
#define TCFEMorfologiaMatematica3D_h

// inclue a biblioteca de filtros
#ifndef TCFEspacial3D_h
#include <AnaliseImagem/Filtro/FEspacial/TCFEspacial3D.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
	 Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
	 Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMorfologiaMatematica3D.h
Nome da classe:      TCFEMorfologiaMatematica3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
==================================================================================
Documentacao CLASSE:    TCFEMorfologiaMatematica3D
==================================================================================
Assunto:             Classe básica para os filtros da morfologia matemática
Superclasse:         TFiltro
Descrição:           Tem a função cria mascara, que cria uma máscara,
			 euclidiana (uma bola) que é utilizada como elemento
										 estruturante nas operações de filtragem.
										 Obs: para funcionamento do afinamento e espessamento
										 foram acrescentados dois flags e duas funções
										 CriaEEB1 e CriaEEB2, posteriormente
										 remodelar este funcionamento, que ficou um pouco confuso.

Acesso:           import
Cardinalidade:    1:1    ,um filtro uma mascara
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/
/**
 * @brief Classe básica para os filtros 3D da morfologia matematica.
 *
 * Tem a função que  cria uma máscara,
 * euclidiana (uma bola) que é utilizada como elemento
 * estruturante nas operacoes de filtragem.
 *
 * Obs: para funcionamento do afinamento e espessamento
 * foram acrescentados dois flags e duas funcoes
 * CriaEEB1 e CriaEEB2, posteriormente
 * remodelar este funcionamento, que ficou um pouco confuso.
*/
template<typename T>
class TCFEMorfologiaMatematica3D : public TCFEspacial3D<T>
{
		// unsigned int RaioBola;      // vai ser armazenada no filtro
		bool EEHitMiss1;	/// Flag de controle de criação da mascara (Na função cria mascara, se ativos, alteram a mascara criada. O default é estarem desativados)
		bool EEHitMiss2;	/// Flag de controle de criação da mascara

	public:
		int INDICE;
		int FUNDO;

	public:
		///Construtor
		TCFEMorfologiaMatematica3D (TCMatriz3D<T> * &matriz, unsigned int _tamanhoMascara, int _indice=1, int _fundo=0 )
			: TCFEspacial3D<T> (matriz,  _tamanhoMascara), EEHitMiss1 (false), EEHitMiss2 (false), INDICE ( _indice ), FUNDO ( _fundo ) {
		}

		///Destrutor
		virtual ~TCFEMorfologiaMatematica3D ( ) {
		}
		// Abaixo a função Go que é reescrita aqui, vazia, para inibir o
		// calculo da convolução realizado na classe CFEspacial<int>
		// As operações da morfologia matemática são diferentes.
		/**
		* Abaixo a funcao Go que é reescrita aqui, vazia, para inibir o
		* calculo da convolucao realizado na classe CFEspacial<int>
		* As operacaes da morfologia matematica sao diferentes.
		* @param matriz
		* @param _tamanhoMascara
		* @return
 */
		virtual TCMatriz3D<T> *Go (TCMatriz3D<T> * &matriz, unsigned int _tamanhoMascara = 0) {
			_tamanhoMascara = _tamanhoMascara; // evitar warning
			return matriz;
		}

		///  Cria mascara recebe o tamanho da mascara. Cria a mascara adequada.
		virtual void CriaMascara (int _tamanhoMascara);	// Cria a mascara adequada

		/// Processa  Abertura, as operacoes recebem o raio da bola
		virtual TCMatriz3D<T> *Abertura (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);
		/// Processa  Erosao, as operacoes recebem o raio da bola
		virtual TCMatriz3D<T> *Erosao (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);
		/// Processa  Dilatacao, as operacoes recebem o raio da bola
		virtual TCMatriz3D<T> *Dilatacao (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);
		/// Processa  Fechamento, as operacoes recebem o raio da bola
		virtual TCMatriz3D<T> *Fechamento (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);

		// Corrigir arrumar, as funções abaixo
		/// Processa  DeteccaoContorno, as operacoes recebem o raio da bola
		virtual TCMatriz3D<T> *DeteccaoContorno (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);

		/// Processa  HitMiss, as operacoes recebem o raio da bola
		virtual TCMatriz3D<T> *HitMiss (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);

		/// Processa  Afinamento, as operacoes recebem o raio da bola
		virtual TCMatriz3D<T> *Afinamento (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);

		/// Processa  Espessamento, as operacoes recebem o raio da bola
		virtual TCMatriz3D<T> *Espessamento (TCMatriz3D<T> * &matriz,  unsigned int _RaioBola);

		/// Processa  Esqueleto, as operacoes recebem o raio da bola
		virtual TCMatriz3D<T> *Esqueleto (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);

	protected:
		///  criam mascara do tipo b1, usadas no afinamento e espessamento
		void CriaEEB1 (unsigned int _tamanhoMascara);	// Funções que criam mascaras do tipo b1

		/// Cria mascara do tipo b2, usadas no afinamento e espessamento
		void CriaEEB2 (unsigned int _tamanhoMascara);	// e b2, usadas no afinamento e espessamento

};

#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica3D.cpp>

#endif // TCFEMorfologiaMatematica3D_h
