//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMDeteccaoContorno_h
#define CMDeteccaoContorno_h

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, 
Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMDeteccaoContorno.h
Nome da classe:      CMDeteccaoContorno
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/


#include  <AnaliseImagem/Filtro/Mascara/CMascara.h>
/**
 * @brief Mascara para o filtro de Deteccao de Contorno.
 * 
 * Representa uma mascara, é herdeira da matriz 2D, ou seja
 * é uma máscara bibimensional.
 * 
 */
class CMDeteccaoContorno : public CMascara
{
public:
	/// Construtor
	CMDeteccaoContorno (unsigned int tamanhoMascara)
    :CMascara (tamanhoMascara)
  {
    CMDeteccaoContorno::PreencheMascara ();
  }

  /// Destrutor
  ~CMDeteccaoContorno ()
  {
  }

protected:
	///   Preenche a mascara com os valores adequados
  virtual void PreencheMascara ();	
};
#endif //    CMPasssaAlta_h
