//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMCDiscreta_h
#define CMCDiscreta_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMCDiscreta.h
Nome da classe:      CMCDiscreta
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
//  ----------------------------------------------------------------------------
//  Classe:       CMCDiscreta
//  ----------------------------------------------------------------------------
/**
 * @brief Implementa conceito de mascara de chanfro discreta.
 *
 * Implementa a classe CMCDiscreta a qual representa uma mascara de chanfro discreta.
 * Acrescenta o atributo raioBase.
 */
class CMCDiscreta	
{
		//  Atributos novos
	public:
		int raioBase;	///<  Raio utilizado pelas mascaras para definir o raio da bola.

	protected:

		//  Metodos
	public:
		///  Construtor, tamanho da mascara
		CMCDiscreta (int rb):raioBase (rb)
		{
		}
		///  Destrutor
		virtual ~ CMCDiscreta ()
		{
		}

	public:
		/// Seta o raio base
		void RaioBase (int rb)
		{
			raioBase = rb;
		}

		/// Retorna o raio
		int RaioBase () const
		{
			return raioBase;
		}

		//  virtual CalculaRaioBase();
};
#endif //  TMascMascDiscreta
