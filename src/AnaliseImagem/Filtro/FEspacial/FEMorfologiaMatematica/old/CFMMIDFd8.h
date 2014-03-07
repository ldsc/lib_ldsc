// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

#if !defined(TIDFd8_h)
#define CFMMIDFd8_h


/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFd8.h
Nome da classe:      CFMMIDFd8
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:		CFMMIDFd8
============================================================================
Nome Classe:         CFMMIDFd8
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d8
Superclasse:         CFMMIDFd8
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
#include <AnaliseImagem/Filtro/FEspacial/TFEMorfologiaMatematica/CFMCFMMIDFdj.h>

// ----------------------------------------------------------------------------
// Classe:       CFMMIDFd8
// ----------------------------------------------------------------------------
class CFMMIDFd8:public CFMMIDFdj
{
public:			// mj,rb
  CFMMIDFd8 (CMatriz2D * &matriz):CFMMIDFdj (matriz, 1, 1)
  {
  }

  virtual ~ CFMMIDFd8 ()
  {
  }				// Destrutor

//   virtual  void CriaMascara(unsigned int _tamanhoMascara);              // redefinida
//       virtual CMatriz2D * Go( CMatriz2D *& matriz,unsigned int _tamanhoMascara=0);
};
#endif // CFMMIDFd8_h
