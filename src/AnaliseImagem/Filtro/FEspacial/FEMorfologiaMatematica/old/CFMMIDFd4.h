// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFMMIDFd4_h)
#define CFMMIDFd4_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFd4.h
Nome da classe:      CFMMIDFd4
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:		CFMMIDFd4
============================================================================
Nome Classe:         CFMMIDFd4
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d4
Superclasse:         CFMMIDFd4
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
#include <AnaliseImagem/Filtro/FEspacial/TFEMorfologiaMatematica/CFMMIDFdi.h>


// ----------------------------------------------------------------------------
// Classe:       CFMMIDFd4
// ----------------------------------------------------------------------------
class CFMMIDFd4:public CFMMIDFdi
{
public:			// mi,rb
  CFMMIDFd4 (CMatriz2D * &matriz):CFMMIDFdi (matriz, 1, 1)
  {
  }
  virtual ~ CFMMIDFd4 ()
  {
  }				// Destrutor

public:
//    virtual  void CriaMascara(unsigned int _tamanhoMascara);    // redefinida
//       virtual CMatriz2D * Go( CMatriz2D *& matriz,unsigned int _tamanhoMascara=0);

};
#endif // CFMMIDFd4_h
