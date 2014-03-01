// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEMMIDF3D_h
#define TCFEMMIDF3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
	 Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIDF3D.h
Nome da classe:      TCFEMMIDF3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao Classe:		TCFEMMIDF3D
==================================================================================
Nome Classe:         TCFEMMIDF3D
Assunto:             Matriz IDF de uma imagem
Superclasse:         TCFEMMIDF3D
Descrição:

Acesso:              import
Cardinalidade:
Concorrência:
Transformacoes:
Especificacoes da linguagem:
Persistencia:
Tamanho:
Abstrata/Concreta:   Concreta (mas na pratica é abstrata pois nao e criada)
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <AnaliseImagem/Matriz/TCMatriz3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica3D.h>

// ----------------------------------------------------------------------------
// Classe:       TCFEMMIDF3D
// ----------------------------------------------------------------------------
/**
 * @brief  Determina a matriz IDF - imagem de distância ao fundo, de uma imagem 3D.
 * Herdeira de CFEMorfologiaMatematica3D e CMatriz3D.
 */
template<typename T>
class TCFEMMIDF3D : public TCFEMorfologiaMatematica3D<T> , public TCMatriz3D<int>
{
		// Atributos
	protected:
		int minimo; 				///< valor minimo utilizado no calculo minimoIda e minimoVolta
		unsigned int raioMaximo;	///< raio máximo da idf
		unsigned int raioBola;		///< raio da bola atual,entre 0 e raioMaximo

		// Métodos
		// void IDFNosPlanosDeContorno(int& base);     // Calcula a idf nos planos de contorno
		// void IDFNosPlanosDeContornoIDA(int& base);  // |_
		// void IDFNosPlanosDeContornoVOLTA(int& base);// -|

		/// Calcula o valor mínimo
		inline void min (int t) {
			if (t < minimo)
				minimo = t;
		}

		/// Usada por erosao, dilatacao, abertura e fechamento
		inline void VerificaImagem (TCMatriz3D<T> * &matriz);	// usada por erosao,dilatacao,abertura e fechamento
		// para verificar se a imagem passada é a mesma, se diferente recalcula Go.

		// 2007 abaixo usada mas não definida?
		/// Para verificar se a imagem passada é a mesma, se diferente recalcula Go.
		void ExecutadaPorGo (TCMatriz3D<T> * &matriz);	// antiga InicializaIDF
		// Funcao executada exclusivamente por Go
		// 1-verifica se a idf tem as mesmas dimensoes de pm
		// 2-se forem iguais continua, se forem diferentes, desaloca e realoca a matriz de dados
		// 3-copia pm para idf, usando data2D[i][j]=pm->data2D[i][j]
		// agora já pode executar Go

	public:
		///  Precisa ser redefinida nas classes bases
		virtual unsigned int Mi () = 0;	// definida, nas filhas

		/// Construtor
		// ponteiro para matriz imagem,tamanho da mascara a ser criada,raioMáximo a ser utilizado
		TCFEMMIDF3D (TCMatriz3D<T> * &matriz, unsigned int _tamanhoMascara = 3, unsigned int _raioMax = 32000, int _indice=1, int _fundo=0 );

		/// Destrutor
		virtual ~ TCFEMMIDF3D () { }

		/// Retorna o raioMaximo
		unsigned int RaioMaximo () const {
			return raioMaximo;
		}
		/// Seta raioMaximo
		void RaioMaximo (unsigned int raioM) {
			raioMaximo = raioM;
		}
		/// Retorna o raioBola
		unsigned int RaioBola () const {
			return raioBola;
		}
		/// Seta o raioBola
		void RaioBola (unsigned int _raioBola) {
			raioBola = _raioBola;
		}

		/// Método de Abertura
		virtual TCMatriz3D<T> *Abertura (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);
		/// Método de Erosao
		virtual TCMatriz3D<T> *Erosao (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);
		/// Método de Dilatacao
		virtual TCMatriz3D<T> *Dilatacao (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);
		/// Método de Fechamento
		virtual TCMatriz3D<T> *Fechamento (TCMatriz3D<T> * &matriz, unsigned int _RaioBola);

		/// Corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
		virtual void CorrigeAbertura (TCMatriz3D<T> * &matriz, int &regiao) = 0;

	protected:
		/// Método recursivo que fechar (marca com 0) todos os pontos adjacentes (no plano) ao ponto passado. Método utilizado em CorrigeAbertura()
		void FecharAdjacencias ( int x, int y, int plano, TCMatriz3D<T> * &matriz);
		//  /// Metodo recursivo que retorna o número de planos (entre 0 e raioy), a partir do passado como parâmetro, cuja mascara passe pela região informada e atende as adjacências. Método utilizado em CorrigeAbertura()
		//  int  NumeroPlanosMascaraPassaRegiao ( int i, int j, int plano, int &raiox, int &raioy, TCMatriz3D<int> * &pcm, TCMatriz3D<int> * &matriz, int &regiao );

};

#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDF3D.cpp>

#endif
