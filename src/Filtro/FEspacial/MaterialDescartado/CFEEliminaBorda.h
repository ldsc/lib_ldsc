// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEEliminaBorda_h
#define CFEEliminaBorda_h

// inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <Filtro/FEspacial/CFEspacial.h>
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
Nome deste arquivo:	CFEEliminaBorda.h
Nome da classe:      CFEEliminaBorda
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
============================================================================
Documentacao CLASSE:    CFEEliminaBorda
============================================================================
Assunto:             Filtros espaciais (atuam sobre o espaço geométrico da imagem).
Superclasse:         CFiltro-> CFEspacial
Descrição:           Elimina a borda de uma imagem, substituindo pontos da borda, por pontos
							definidos pela funcao SetvalorBorda.
                     O valor default para a borda é zero (0).
                     Ex:
                     CFEEliminaBorda(valorBorda).Go(imagem,espessuraBorda);

Acesso:           import
Cardinalidade:    1:1    ,de uma maneira geral usa-se um filtro para atuar sobre uma imagem
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/

class CFEEliminaBorda : public CFEspacial
{
  int valorBorda;

public:
  CFEEliminaBorda (int vB = 0, CMatriz2D * matriz = 0, unsigned int _tamanhoMascara = 0)
    : CFEspacial (matriz, _tamanhoMascara),    valorBorda    (vB)
  {
  }

  ~CFEEliminaBorda ()
  {
  }

  // virtual void CriaMascara(unsigned int _tamanhoMascara){};
  virtual CMatriz2D *Go (CMatriz2D * imagem, int espessura);

  void ValorBorda (int v)
  {
    valorBorda = v;
  }

  int ValorBorda () const
  {
    return valorBorda;
  }
};
#endif // CFEEliminaBorda_h
