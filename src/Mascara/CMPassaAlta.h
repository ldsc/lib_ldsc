//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMPassaAlta_h
#define CMPassaAlta_h

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMPassaAlta.h
Nome da classe:      CMPassaAlta
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:    CMPassaAlta
==================================================================================
Assunto:        Mascara para filtro passa alta
Superclasse:    CMascara
Descrição:      mascara utilizada no filtro passa alta,
		a funcao preenche mascara preenche os valores da matriz
                     2D com os valores adequados.
Acesso:           import
Cardinalidade:    1:n    ,muitos vetores
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/

#include  <Mascara/CMascara.h>
/**
 * @brief Mascara para o filtro passa alta.
 * 
 * Representa uma mascara, é herdeira da matriz 2D, ou seja
 * é uma máscara bibimensional.
 * 
 */
class CMPassaAlta : public CMascara
{
public:
	/// Construtor
	CMPassaAlta (unsigned int tamanhoMascara)
    : CMascara (tamanhoMascara)
  {
    CMPassaAlta::PreencheMascara ();
  }

  /// Destrutor
  ~CMPassaAlta ()
  {
  }

protected:
	
	/// Preenche a mascara com os valores adequados
  virtual void PreencheMascara (); 

};
#endif //    CMPasssaAlta_h
