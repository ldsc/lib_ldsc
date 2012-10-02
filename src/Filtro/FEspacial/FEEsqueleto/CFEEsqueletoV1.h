#ifndef CFEEsqueletoV1_h
#define CFEEsqueletoV1_h

/*
----------------------------------------------------------------------------
Bibliotecas e Defines
----------------------------------------------------------------------------
*/
#ifndef CFEEsqueleto_h
#include <Filtro/FEspacial/FEEsqueleto/CFEEsqueleto.h>
#endif

#ifndef CImagem_h
#include <Matriz/CImagem.h>
#endif

#ifndef TCFEMMIDF_h
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDF.h>
#endif

#ifndef TCFEMMIDFd34_h
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.h>
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
Nome deste arquivo:	CFEEsqueletoV1.h
Nome da classe:      CFEEsqueletoV1
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief
 * Classe desenvolvida para teste de alguns tipos de algorítimos de esqueletização,
 * modelo 1.
*/
class CFEEsqueletoV1 : public CFEEsqueleto
{
// Atributos
protected:
  
	TCFEMMIDFd34<int> * idf;	///< Ponteiro para imagem idf d34

// Métodos
public:			
	/// Construtor
CFEEsqueletoV1 (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara = 0)
	:CFEEsqueleto (matriz, _tamanhoMascara),    idf    (0)
  {
  }
  
/// Destrutor
   ~CFEEsqueletoV1 ()
  {
    if (idf)
      delete idf; 				// deleta objeto idf
  }		
  
/// Processa determinação do esqueleto.
  virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * &matriz,
			 unsigned int _tamanhoMascara = 0);

};
#endif //  CFEEsqueletoV1_h
