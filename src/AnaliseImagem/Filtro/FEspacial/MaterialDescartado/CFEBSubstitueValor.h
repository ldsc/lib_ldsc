// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEBSubstitueValor_h
#define CFEBSubstitueValor_h

// inclue a biblioteca de filtros
#ifndef CFEBorda_h
#include <AnaliseImagem/Filtro/FEspacial/CFEBorda.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LMPT
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:  	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEBSubstitueValor.h
Nome da classe:      CFEBSubstitueValor
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
============================================================================
Documentacao Classe:    CFEBSubstitueValor
============================================================================
Assunto:             Filtros espaciais (atuam sobre o espa�o geom�trico da imagem).
Superclasse:         CFiltro-> CFEspacial
Descri��o:           Substitue os pontos da borda com valor i por j.
                     Ex:
		     Exemplo de uso: CFEBSubstitueValor(valorAntigo,novoValor).Go(imagem);

Acesso:           import
Cardinalidade:    1:1    ,de uma maneira geral usa-se um filtro para atuar sobre uma imagem
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/

class CFEBSubstitueValor : public CFEBorda
{
  int valorAntigo;

public:
  CFEBSubstitueValor (int _valorAntigo, int vB = 0, CMatriz2D * matriz = 0, unsigned int _tamanhoMascara = 0):CFEBorda (vB, matriz, _tamanhoMascara),
    valorAntigo
    (_valorAntigo)
  {
  }

  virtual ~ CFEBSubstitueValor ()
  {
  }

  virtual CMatriz2D *Go (CMatriz2D * &imagem, unsigned int espessura = 1);

};
#endif // CFEBSubstitueValor
