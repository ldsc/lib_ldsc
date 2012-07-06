// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFMMIDFd5711_h)
#define CFMMIDFd5711_h


/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:		Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFd5711.h
Nome da classe:      CFMMIDFd5711
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao CLASSE:		CFMMIDFd5711
============================================================================
Nome Classe:         CFMMIDFd5711
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d5711
Superclasse:         CFMMIDFd5711
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

#include <TFiltro/TFEspacial/TFEMorfologiaMatematica/CFMMIDFdijk.h>

// ----------------------------------------------------------------------------
// Classe:       CFMMIDFd5711
// ----------------------------------------------------------------------------
class CFMMIDFd5711:public CFMMIDFdijk
{
public:			// mi mj mk rb
  CFMMIDFd5711 (CMatriz2D * &matriz):CFMMIDFdijk (matriz, 5, 7, 11, 2)
  {
  }

  virtual ~ CFMMIDFd5711 ()
  {
  }				// Destrutor

  // virtual  void CriaMascara(unsigned int _tamanhoMascara);// redefinida
  // virtual CMatriz2D * Go( CMatriz2D *& matriz,unsigned int _tamanhoMascara=0);

};
#endif // CFMMIDFd5711_h
