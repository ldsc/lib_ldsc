// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFMMIDFdij_h)
#define CFMMIDFdij_h


/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFdij.h
Nome da classe:      CFMMIDFdij
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:		CFMMIDFdij
============================================================================
Nome Classe:         CFMMIDFdij
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         CFMMIDFdij
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
#include <AnaliseImagem/Filtro/Mascara/TMCDiscreta/TMCdij.h>

// ----------------------------------------------------------------------------
// Classe:       CFMMIDFdij
// ----------------------------------------------------------------------------
class CFMMIDFdij:public TFMMIDF, public TMCdij
{
protected:
  virtual unsigned int Mi ()
  {
    return mi;
  }				// redefinida, herdada de TFMMIDF

  virtual void CriaMascara (unsigned int _tamanhoMascara);	// redefinida

public:
  CFMMIDFdij (CMatriz2D * &matriz, unsigned int _mi, unsigned int _mj,
	      unsigned int _rb):TFMMIDF (matriz), TMCdij (_mi, _mj, _rb)
  {
  }

  virtual ~ CFMMIDFdij ()
  {
  }				// Destrutor

  virtual CMatriz2D *Go (CMatriz2D * &matriz, unsigned int _tamanhoMascara = 0);
};
#endif // CFMMIDFdij_h
