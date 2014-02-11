//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMPassaBaixa_h
#define CMPassaBaixa_h

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
   Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
   Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMPassaBaixa.h
Nome da classe:      CMPassaBaixa
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Mascara para o filtro  passa baixa.
 * 
 * Representa uma mascara, é herdeira da matriz 2D, ou seja
 * é uma máscara bibimensional.
 * 
 */
#include  <AnaliseImagem/Filtro/Mascara/CMascara.h>

class CMPassaBaixa : public CMascara
{
public:
	/// Construtor
   CMPassaBaixa (unsigned int tamanhoMascara):CMascara (tamanhoMascara) {
      CMPassaBaixa::PreencheMascara ();
   }

   /// Destrutor
   ~CMPassaBaixa () { }

protected:
	/// Preenche a mascara com valores adequados.
   virtual void PreencheMascara ();

};
#endif // CMPassaBaixa_h
