//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMCdijk_h
#define CMCdijk_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMCdijk.h
Nome da classe:      CMCdijk
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCdij.h>

//  ----------------------------------------------------------------------------
//  Classe:       CMCdijk
//  ----------------------------------------------------------------------------
/**
 * @brief Mascara de chanfro dijk.
 * Representa uma mascara de chanfro.
 * Herdeira de CMCdij -> CMCDi -> CMCDiscreta
 */
class CMCdijk : public CMCdij
{
	protected:
		int mk; ///< Atributo mk

	public:
		/// Seta mk
		void Mk (int kk)
		{
			mk = kk;
		}

		/// Obtêm mk
		int Mk () const
		{
			return mk;
		}

	public:

		/// Construtor
		CMCdijk (int ii, int jj, int kk, int rb)
			: CMCdij (ii, jj, rb), mk (kk)
		{
		}
		///  Destrutor
		virtual ~ CMCdijk ()
		{
		}

};
#endif // CMCdijk_h
