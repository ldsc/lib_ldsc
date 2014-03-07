// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFMMIDFdi_h)
#define CFMMIDFdi_h


/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFdi.h
Nome da classe:      CFMMIDFdi
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:		CFMMIDFdi
============================================================================
Nome Classe:         CFMMIDFdi
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d4
Superclasse:         CFMMIDFdi
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
// Classe:       CFMMIDFdi
// ----------------------------------------------------------------------------
class CFMMIDFdi:public TFMMIDF, public TMCdi
{
  // unsigned int mi, rb;                                                                        // parametros passados para criacao da bola

protected:
  virtual unsigned int Mi ()
  {
    return mi;
  }			// redefinida, herdada de TFMMIDF

public:
CFMMIDFdi (CMatriz2D * &matriz, unsigned int _mi, unsigned int _rb):TFMMIDF (matriz), TMCdi (_mi, _rb)
  {
  }

  // ponteiro para matriz imagem mi e rb da bola
  virtual ~ CFMMIDFdi ()
  {
  }				// Destrutor

  virtual void CriaMascara (unsigned int _tamanhoMascara);	// redefinida
  virtual CMatriz2D *Go (CMatriz2D * &matriz, unsigned int _tamanhoMascara = 0);
};
#endif // CFMMIDFdi_h
