//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMRotacaoVerticaleDiferenca_h
#define CMRotacaoVerticaleDiferenca_h

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMRotacaoVerticaleDiferenca.h
Nome da classe:      CMRotacaoVerticaleDiferenca
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
#include  <AnaliseImagem/Filtro/Mascara/CMascara.h>
/**
 * @brief Mascara para o filtro de rotação vertical e diferença.
 * 
 * Representa uma mascara, é herdeira da matriz 2D, ou seja
 * é uma máscara bibimensional.
 * 
 */
class CMRotacaoVerticaleDiferenca : public CMascara
{
public:      
 	/// Construtor
	CMRotacaoVerticaleDiferenca (unsigned int tamanhoMascara)
    :CMascara (tamanhoMascara)
  {
    CMRotacaoVerticaleDiferenca::PreencheMascara ();
  }
		
  /// Destrutor
  ~CMRotacaoVerticaleDiferenca ()
  {
  }

protected:
	
	/// Preenche a mascara com valores adequados.
  virtual void PreencheMascara ();	
};

#endif //    CMRotacaoVerticaleDiferenca

