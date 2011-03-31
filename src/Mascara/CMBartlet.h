//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMBartlet_h
#define CMBartlet_h

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMBartlet.h
Nome da classe:      CMBartlet
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

#include  <Mascara/CMascara.h>

/**
 * @brief Mascara para o filtro de Bartlet.
 * 
 * Representa uma mascara, é herdeira da matriz 2D, ou seja
 * é uma máscara bibimensional.
 * 
*/
class CMBartlet : public CMascara
{
public:	
	/// Construtor
  CMBartlet (unsigned int tamanhoMascara):CMascara (tamanhoMascara)
  {
    CMBartlet::PreencheMascara ();
  }

  /// Destrutor
  ~CMBartlet ()
  {
  }

protected:
	///   Redefinida, preenche os valores da mascara
  virtual void PreencheMascara ();	
};
#endif //    CMBartlet_h
