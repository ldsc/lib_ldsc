
// inclue a biblioteca de filtros
#ifndef CFiltro_h
#include <AnaliseImagem/Filtro/CFiltro.h>
#endif

// inclue a biblioteca de mascaras
#ifndef CMascara_h
#include <AnaliseImagem/Filtro/Mascara/CMascara.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LMPT
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEspacial.h
Nome da classe:      CFEspacial
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
============================================================================
Documentacao Classe:    CFEspacial
============================================================================
Assunto:             Classe b�sica para os filtros da morfologia matem�tica
Superclasse:         CFiltro
Descri��o:           Representa a superclasse para os filtros que atuam sobre
							o espa�o discreto da imagem.
                     Ou seja atua sobre o espa�o geom�trico da imagem,
                     enquanto os filtros de amplitude atuam sobre o espa�o de cor.
                     Tem uma vari�vel inteira que define o tamanho da mascara a ser utilizada
                     e um ponteiro para um objeto da hierarquia das mascaras.
                     Como a classe base CFiltro tem um ponteiro para uma matriz imagem, eu tenho
                     acesso a imagem atrav�s de pm, e a mascara atrav�s de mask.
                     A fun��o cria mascara � utilizada para cria��o da mascara
                     e a fun��o Go executa o filtro em s�.
Acesso:           import
Cardinalidade:    1:1    ,um filtro uma mascara
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/
class CFEspacial : public CFiltro
{
// Atributos
protected:
  unsigned int tamanhoMascara;

public:
    CMascara * mask;		// ponteiro para mascara

// M�todos
public:			// construtor
    CFEspacial (CMatriz2D * &matriz, unsigned int _tamanhoMascara)	// ponteiro para imagem, e
  : CFiltro (matriz), tamanhoMascara (_tamanhoMascara)	// tamanho da mascara
  {
    mask = NULL;
  }
				// destrutor
  ~CFEspacial ()
  {
    delete mask;
  }
  virtual unsigned int GetTamanhoMascara ()
  {
    return tamanhoMascara;
  }

  virtual void SetTamanhoMascara (unsigned int tMask)
  {
    tamanhoMascara = tMask;
  }

  virtual CMatriz2D *Go ();	// Chama CriaMascara e processa o filtro

protected:
  virtual void CriaMascara (unsigned int _tamanhoMascara);	// Cria a mascara adequada

};
#endif //  CFEspacial_h
