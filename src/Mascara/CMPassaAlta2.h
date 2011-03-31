//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMPassaAlta2_h
#define CMPassaAlta2_h

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMPassaAlta2.h
Nome da classe:      CMPassaAlta2
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Mascara para o filtro Passa Alta 2.
 * 
 * Representa uma mascara, é herdeira da matriz 2D, ou seja
 * é uma máscara bibimensional.
 * 
 */
#include  <Mascara/CMascara.h>

class CMPassaAlta2 : public CMascara
{
public:
 	/// Construtor
	CMPassaAlta2 (unsigned int tamanhoMascara)
    :CMascara (tamanhoMascara)
  {
    CMPassaAlta2::PreencheMascara ();
  }

  /// Destrutor
  ~CMPassaAlta2 ()
  {
  }

protected:
	///   Preenche a mascara com os valores adequados
  virtual void PreencheMascara ();	
};
#endif //    CMPasssaAlta_h
