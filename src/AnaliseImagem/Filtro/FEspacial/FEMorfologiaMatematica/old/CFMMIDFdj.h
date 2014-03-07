// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFMMIDFdj_h)
#define CFMMIDFdj_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFdj.h
Nome da classe:      CFMMIDFdj
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:		CFMMIDFdj
============================================================================
Nome Classe:         CFMMIDFdj
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d8
Superclasse:         CFMMIDFdj
Descri��o:

Acesso:              import
Cardinalidade:
Concorr�ncia:
Transformacoes:
Especificacoes da linguagem:
Persistencia:
Tamanho:
Abstrata/Concreta:   Concreta 
Arquivo de documentacao auxiliar:
*/
// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <AnaliseImagem/Filtro/FEspacial/TFEMorfologiaMatematica/TFMMIDF.h>
#include <AnaliseImagem/Filtro/Mascara/TMCDiscreta/TMCdi.h>

// ----------------------------------------------------------------------------
// Classe:       CFMMIDFdj
// ----------------------------------------------------------------------------
class CFMMIDFdj:public TFMMIDF, public TMCdi
{
  // unsigned int mj, rb;                // parametros passados para criacao da bola
protected:
  virtual unsigned int Mi ()
  {
    return mi;
  };			// redefinida, herdada de TFMMIDF

public:
CFMMIDFdj (CMatriz2D * &matriz, unsigned int _mj, unsigned int _rb):TFMMIDF (matriz), TMCdi (_mj,   _rb)
  {
  }

  // ponteiro para matriz imagem
  // mi r rb da bola
  virtual ~ CFMMIDFdj ()
  {
  }				// Destrutor

  virtual void CriaMascara (unsigned int _tamanhoMascara);	// redefinida
  virtual CMatriz2D *Go (CMatriz2D * &matriz, unsigned int _tamanhoMascara =
			 0);
};
#endif // CFMMIDFdj_h
