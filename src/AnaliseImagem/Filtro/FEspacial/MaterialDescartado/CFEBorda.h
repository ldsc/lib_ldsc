// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEBorda_h
#define CFEBorda_h

// inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <AnaliseImagem/Filtro/FEspacial/CFEspacial.h>
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
Nome deste arquivo:	CFEBorda.h
Nome da classe:      CFEBorda
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
============================================================================
Documentacao Classe:    CFEBorda
============================================================================
Assunto:             Filtros espaciais (atuam sobre o espa�o geom�trico da imagem).
Superclasse:         CFiltro-> CFEspacial
Descri��o:           Elimina a borda de uma imagem, substituindo pontos da borda por valorBorda
                     O valor default para a borda � zero (0).
                     Ex:
                     CFEBorda(valorBorda).Go(imagem,espessuraBorda);

Acesso:           import
Cardinalidade:    1:1    ,de uma maneira geral usa-se um filtro para atuar sobre uma imagem
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/
class CFEBorda : public CFEspacial
{
protected:
  int valorBorda;		// valor dos pontos da borda
//  int espessura;                   // espessura da borda

public:
  CFEBorda (int vB = 0, CMatriz2D * matriz = 0, unsigned int _tamanhoMascara = 0)
    :CFEspacial (matriz, _tamanhoMascara),    valorBorda (vB)
  {
  }

  ~CFEBorda ()
  {
  }

  virtual CMatriz2D *Go (CMatriz2D * &imagem, unsigned int espessura = 1);

  void ValorBorda (int v)
  {
    valorBorda = v;
  }

  int ValorBorda () const
  {
    return valorBorda;
  }
//  void Setespessura(int v)             {espessura=v;};
//  int Getespessura()const                      {return espessura;}
};
#endif // CFEBorda_h
