// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFMMIDFdijk_h)
#define CFMMIDFdijk_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFdijk.h
Nome da classe:      CFMMIDFdijk
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao CLASSE:		CFMMIDFdijk
============================================================================
Nome Classe:         CFMMIDFdijk
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d5711
Superclasse:         CFMMIDFdijk
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

#include <TFiltro/TFEspacial/TFEMorfologiaMatematica/TFMMIDF.h>
#include <TMascara/TMCDiscreta/TMCdijk.h>

// ----------------------------------------------------------------------------
// Classe:       CFMMIDFdijk
// ----------------------------------------------------------------------------
class CFMMIDFdijk:public TFMMIDF, public TMCdijk
{
protected:
  virtual unsigned int Mi ()
  {
    return mi;
  }		// redefinida, herdada de TFMMIDF

protected:
  virtual void CriaMascara (unsigned int _tamanhoMascara);	// redefinida
public:
  CFMMIDFdijk (CMatriz2D * &matriz, unsigned int _mi, unsigned int _mj,
	       unsigned int _mk, unsigned int _rb):TFMMIDF (matriz),
    TMCdijk (_mi, _mj, _mk, _rb)
  {
  }

  virtual ~ CFMMIDFdijk ()
  {
  }	// Destrutor

  virtual CMatriz2D *Go (CMatriz2D * &matriz, unsigned int _tamanhoMascara = 0);	// redefinida

};
#endif // CFMMIDFdijk_h
