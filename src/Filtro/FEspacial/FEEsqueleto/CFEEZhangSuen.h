#ifndef CFEEZhangSuen_h
#define CFEEZhangSuen_h

/*
----------------------------------------------------------------------------
Bibliotecas e Defines
----------------------------------------------------------------------------
*/

// inclue a biblioteca de filtros
#ifndef CFEEsqueleto_h
#include <Filtro/FEspacial/FEEsqueleto/CFEEsqueleto.h>
#endif

// inclue a biblioteca de filtros
#ifndef CImagem_h
#include <Matriz/CImagem.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
		       	Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:      	Laboratorio de Desenvolvimento de Software Cientifico 
		     	dos Materiais.
Programadores:         	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
		      	Liang Zirong,
		 	Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEEZhangSuen.h
Nome da classe:      CFEEZhangSuen
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief
 * Classe para determinação do esqueleto
 * usando algoritimo Zhang & Suen's.
 * Thinnning algorithm, Stentiford's pre-processing Holt's post-processing
 * de esqueletização.
 */

class CFEEZhangSuen : public CFEEsqueleto
{

// Atributos
protected:
  int t00;  ///< Atributo auxiliar
  int t01;  ///< Atributo auxiliar
  int  t11; ///< Atributo auxiliar
  int t01s; ///< Atributo auxiliar

// Métodos
public:			
	/// Construtor
  CFEEZhangSuen (CMatriz2D * &matriz, unsigned int _tamanhoMascara = 0):CFEEsqueleto (matriz,
		_tamanhoMascara)
  {
  }

  /// Destrutor
   ~CFEEZhangSuen ()
  {
  }				

  /// Processa determinação do esqueleto.
  virtual CMatriz2D *Go (CMatriz2D * &matriz,
			 unsigned int _tamanhoMascara = 0);

protected:

	/// Método auxiliar thnz
  void thnz (CMatriz2D * im);

	/// Método auxiliar nays8
  int nays8 (CMatriz2D * im, int r, int c);

  	/// Método auxiliar Connectivity
  int Connectivity (CMatriz2D * im, int r, int c);

  	/// Método auxiliar Delete
  void Delete (CMatriz2D * im, CMatriz2D * tmp);

  	/// Método auxiliar check
  void check (int v1, int v2, int v3);

  	/// Método auxiliar edge
  int edge (CMatriz2D * im, int r, int c);

  	/// Método auxiliar stair
  void stair (CMatriz2D * im, CMatriz2D * tmp, int dir);

  	/// Método auxiliar Yokoi
  int Yokoi (CMatriz2D * im, int r, int c);

  	/// Método auxiliar pre_smooth
  void pre_smooth (CMatriz2D * im);

  	/// Método auxiliar match_du
  void match_du (CMatriz2D * im, int r, int c, int k);

  	/// Método auxiliar aae
  void aae (CMatriz2D * image);

  	/// Método auxiliar snays
  int snays (CMatriz2D * im, int r, int c);
};
#endif //  CFEEZhangSuen_h
