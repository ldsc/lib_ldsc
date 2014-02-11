//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMGaussiano_h
#define CMGaussiano_h

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
Nome deste arquivo:	CMGaussiano.h
Nome da classe:      CMGaussiano
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

#include  <AnaliseImagem/Filtro/Mascara/CMascara.h>
/**
 * @brief Mascara para o filtro gaussiano.
 * 
 * Representa uma mascara, é herdeira da matriz 2D, ou seja
 * é uma máscara bibimensional.
 * 
 */
class CMGaussiano : public CMascara
{
public:		
 	/// Construtor
	CMGaussiano (unsigned int tamanhoMascara):CMascara (tamanhoMascara)
  {
    CMGaussiano::PreencheMascara ();
  }

  /// Destrutor
  ~CMGaussiano ()
  {
  }

protected:
	///   Redefinida, preenche os valores da mascara
  virtual void PreencheMascara ();	
};

#endif //    CMGaussiano_h

