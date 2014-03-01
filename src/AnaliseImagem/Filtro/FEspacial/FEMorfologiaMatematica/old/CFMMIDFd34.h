// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFMMIDFd34_h)
#define CFMMIDFd34_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFd34.h
Nome da classe:      CFMMIDFd34
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:		CFMMIDFd34
============================================================================
Nome Classe:         CFMMIDFd34
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         CFMMIDFd34
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
#include <TFiltro/TFEspacial/TFEMorfologiaMatematica/CFMMIDFdij.h>

// ----------------------------------------------------------------------------
// Classe:       CFMMIDFd34
// ----------------------------------------------------------------------------
class CFMMIDFd34:public CFMMIDFdij
{
public:			// mi mj rb
  CFMMIDFd34 (CMatriz2D * &matriz):CFMMIDFdij (matriz, 3, 4, 1)
  {
  }

  virtual ~ CFMMIDFd34 ()
  {
  }				// Destrutor


//   virtual  void CriaMascara(unsigned int _tamanhoMascara);                            // redefinida
//   virtual CMatriz2D * Go( CMatriz2D *& matriz,unsigned int _tamanhoMascara=0);
};
#endif // CFMMIDFd34_h
