#ifndef CFEspacial_h
#define CFEspacial_h

// inclue a biblioteca de filtros
#ifndef CFiltro_h
#include <Filtro/CFiltro.h>
#endif

// inclue a biblioteca de mascaras
#ifndef CMascara_h
#include <Mascara/CMascara.h>
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
Documentacao CLASSE:    CFEspacial
============================================================================
Assunto:             Classe básica para os filtros da morfologia matemática
Superclasse:         CFiltro
Descrição:           Representa a superclasse para os filtros que atuam sobre
							o espaço discreto da imagem.
                     Ou seja atua sobre o espaço geométrico da imagem,
                     enquanto os filtros de amplitude atuam sobre o espaço de cor.
                     Tem uma variável inteira que define o tamanho da mascara a ser utilizada
                     e um ponteiro para um objeto da hierarquia das mascaras.
                     Como a classe base CFiltro tem um ponteiro para uma matriz imagem, eu tenho
                     acesso a imagem através de pm, e a mascara através de mask.
                     A função cria mascara é utilizada para criação da mascara
                     e a função Go executa o filtro em sí.
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

// Métodos
public:			// construtor
    CFEspacial (CMatriz2D * &matriz, unsigned int _tamanhoMascara)	// ponteiro para imagem, e
  : CFiltro (matriz), tamanhoMascara (_tamanhoMascara)	// tamanho da mascara
  {
    mask = NULL;
  }

   ~CFEspacial ()
  {
    if (mask)
      delete mask;
  }				// destrutor

  virtual unsigned int TamanhoMascara () const
  {
    return tamanhoMascara;
  }

  virtual void TamanhoMascara (unsigned int _tamanhoMascara)
  {
    tamanhoMascara = _tamanhoMascara;
  }

  virtual CMatriz2D *Go (CMatriz2D * &matriz, unsigned int _tamanhoMascara =0);

    // virtual CMatriz2D * Go( CMatriz2D *& matriz); // realiza o processamento da filtragem

protected:
  virtual void CriaMascara (unsigned int _tamanhoMascara);	// Cria a mascara adequada

};
#endif // CFEspacial_h
