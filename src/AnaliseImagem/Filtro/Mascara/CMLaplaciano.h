//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMLaplaciano_h
#define CMLaplaciano_h

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMLaplaciano.h
Nome da classe:      CMLaplaciano
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

#include  <AnaliseImagem/Filtro/Mascara/CMascara.h>
/**
 * @brief Mascara para o filtro laplaciano .
 * Representa uma mascara, é herdeira da matriz 2D, ou seja
 * é uma máscara bibimensional.
 * 
 */
class CMLaplaciano : public CMascara
{
public:			
	/// Construtor
	CMLaplaciano (unsigned int tamanhoMascara):CMascara (tamanhoMascara)
  {
    CMLaplaciano::PreencheMascara ();
  }

  /// Destrutor
  ~CMLaplaciano ()
  {
  }

protected:
	///   Redefinida, preenche os valores da mascara
  virtual void PreencheMascara ();	
};

#endif //    CMLaplaciano_h
