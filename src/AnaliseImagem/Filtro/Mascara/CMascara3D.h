//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMascara3D_h
#define CMascara3D_h

/*
-------------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
				Biblioteca para manipulacao de matrizes
-------------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMascara3D.h
Nome da classe:      CMascara3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

#include  <AnaliseImagem/Matriz/TCMatriz3D.h>


/**
 * @brief Representa uma mascara, é herdeira da matriz 3D, ou seja
 * é uma máscara tri-dimensional.
 *
 * Acrescenta funções de preenchimento da mascara, e calculo do peso.
 * O peso é a soma dos elementos da máscara
 *
 *   Considerações gerais:
 *
 *    Obs: O resultado da expressão (nx-1)/2 para diferentes n:
 *    Observe que arredonta 1/2=0.5=0. arredonda 0.5 para zero.
 *    n=    0               1       2       3       4       5       6
 *    (nx-1)/2= 0           0       0       1       1       2       2
 *    (nx-2)/2= -1          0       0       0       1       1       2
*/

class CMascara3D : public TCMatriz3D<int>
{
	protected:
		float peso;	///< Peso usado para que a mascara tenha Somatorio/peso=1

	protected:
		///   Calcula o peso da mascara
		float CalculaPeso ();

		///   Preenche a mascara com os valores adequados, deve ser redefinida nas classes filhas
		virtual void PreencheMascara () {  }

	public:
		/// Construtor, recebe a dimensao da mascara
		CMascara3D (unsigned int tamanhoMascara);

		/// Destrutor
		~CMascara3D ();

		/// Raio da bola inclusa direcao x
		inline unsigned int RaioX () const
		{
			return (nx - 1) / 2;
		}

		/// Raio da bola inclusa direcao y
		inline unsigned int RaioY () const
		{
			return (ny - 1) / 2;
		}

		/// Raio da bola inclusa direcao z
		inline unsigned int RaioZ () const
		{
			return (nz - 1) / 2;
		}

		/// retorna o peso da mascara
		inline float Peso () const
		{
			return peso;
		}

};
#endif //    CMascara3D_h
