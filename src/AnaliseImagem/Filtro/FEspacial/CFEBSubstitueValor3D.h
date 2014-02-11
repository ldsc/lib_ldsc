// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEBSubstitueValor3D_h
#define CFEBSubstitueValor3D_h

// inclue a biblioteca de filtros
#ifndef CFEBorda3D_h
#include <AnaliseImagem/Filtro/FEspacial/CFEBorda3D.h>
#endif


/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEBSubstitueValor3D.h
Nome da classe:      CFEBSubstitueValor3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:    CFEBSubstitueValor3D
==================================================================================
Assunto:             Filtros espaciais (atuam sobre o espaço geométrico da imagem).
Superclasse:         TFiltro-> CFEspacial<int>
Descrição:           Substitue os pontos da borda com valor i por j.
                     Ex:Exemplo de uso: CFEBSubstitueValor3D(valorAntigo,valorNovoBorda).Go(imagem);

Acesso:           import
Cardinalidade:    1:1    ,de uma maneira geral usa-se um filtro para atuar sobre uma imagem
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/

class CFEBSubstitueValor3D : public CFEBorda3D
{
  int valorAntigo;

public:
    CFEBSubstitueValor3D (int _valorAntigo,
			  int vB
			  /*,TCMatriz3D<int> *& matriz, unsigned int _tamanhoMascara=0 */
			  )
  : CFEBorda3D (vB /*,matriz, _tamanhoMascara */ ), valorAntigo (_valorAntigo)
  {
  }

  virtual ~ CFEBSubstitueValor3D ()
  {
  }

  virtual TCMatriz3D<int> *Go (TCMatriz3D<int> * &imagem, unsigned int espessura = 1);

};
#endif // CFEBSubstitueValor3D
