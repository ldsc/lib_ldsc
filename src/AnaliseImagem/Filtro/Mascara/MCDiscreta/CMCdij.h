//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CMCdij_h
#define CMCdij_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
	Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico 
	dos Materiais.
Programadores:   Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMCdij.h
Nome da classe:      CMCdij
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCdi.h>

//  ----------------------------------------------------------------------------
//  Classe:       CMCdij
//  ----------------------------------------------------------------------------
/**
 * @brief Mascara  de chanfro dij.
 * É derivada da classe base CMCdi --> CMCDiscreta.
 * Fundamentada na métrica d34.
 * Representa uma mascara de chanfro.
 */
class CMCdij : public CMCdi
{
	protected:
		int mj; ///< Atributo mj

	public:
		/// Seta mj
		void Mj (int _mj) {
			mj = _mj;
		}

		/// Obtêm mj
		int Mj () const {
			return mj;
		}

	public:
		///  Construtor
		CMCdij (int _mi, int _mj, int rb)
			: CMCdi (_mi, rb), mj (_mj) {
		}

		///  Destrutor
		virtual ~ CMCdij () {
		}
};

#endif //   CMCdij
