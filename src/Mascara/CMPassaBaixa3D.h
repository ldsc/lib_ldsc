//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CMPassaBaixa3D_h
#define CMPassaBaixa3D_h

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
Nome deste arquivo:	CMPassaBaixa3D.h
Nome da classe:      CMPassaBaixa3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

#include  <Mascara/CMascara3D.h>
/**
 * @brief Mascara para o filtro passa baixa 3D.
 * Representa uma mascara, é herdeira da matriz 3D, ou seja
 * é uma máscara tridimensional.
 * 
 */
class CMPassaBaixa3D : public CMascara3D
{
public:
 	/// Construtor
	CMPassaBaixa3D (unsigned int tamanhoMascara) : CMascara3D (tamanhoMascara)
  {
    CMPassaBaixa3D::PreencheMascara ();
  }

  /// Destrutor
  ~CMPassaBaixa3D ()
  {
  }

protected:
	
	/// Preenche a mascara com valores adequados.
  virtual void PreencheMascara ();

};

#endif //    CMPassaBaixa3D_h
