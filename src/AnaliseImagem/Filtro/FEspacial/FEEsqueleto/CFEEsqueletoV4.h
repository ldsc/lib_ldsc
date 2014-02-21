#ifndef CFEEsqueletoV4_h
#define CFEEsqueletoV4_h

/*
----------------------------------------------------------------------------
Bibliotecas e Defines
----------------------------------------------------------------------------
*/
#ifndef CFEEsqueleto_h
#include <AnaliseImagem/Filtro/FEspacial/FEEsqueleto/CFEEsqueleto.h>
#endif

#ifndef CImagem_h
#include <AnaliseImagem/Matriz/CImagem.h>
#endif

#ifndef TCFEMMIDF_h
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDF.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
		       	Biblioteca para manipulacao de imagens
----------------------------------------------------------------------------

Desenvolvido por:      	Laboratorio de Desenvolvimento de Software Cientifico 
		  	dos Materiais.
Programadores:         	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
		       	Liang Zirong,
		      	Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEEsqueletoV4.h
Nome da classe:      CFEEsqueletoV4
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief
 * Classe desenvolvida para teste de alguns tipos de algorítimos de esqueletização,
 * modelo 4.
 */
class CFEEsqueletoV4:public CFEEsqueleto
{

// Atributos
protected:
	TCFEMMIDF<int> * idf;		///< Ponteiro para imagem idf d

// Métodos
public:			
	/// Construtor
CFEEsqueletoV4 (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara = 0)
	:CFEEsqueleto (matriz, _tamanhoMascara),    idf    (0)
  {
  }

/// Destrutor
   ~CFEEsqueletoV4 ()
  {
    if (idf)
	    delete idf;				// deleta objeto idf
  }				

  /// Processa determinação do esqueleto.
  virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * &matriz,
			 unsigned int _tamanhoMascara = 0);

};
#endif //  CFEEsqueletoV4_h