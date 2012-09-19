// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEDeteccaoContornoMasc_h
#define CFEDeteccaoContornoMasc_h

// inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <Filtro/FEspacial/CFEspacial.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong,
		      	Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEDeteccaoContornoMasc.h
Nome da classe:      CFEDeteccaoContornoMasc
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro de detecção de contorno, CFEDeteccaoContornoMasc.
 * Herdeiro de CFEspacial.
 */

class CFEDeteccaoContornoMasc : public CFEspacial
{
public:
		/// Construtor
  CFEDeteccaoContornoMasc (TCMatriz2D< int > * &matriz,
			   unsigned int _tamanhoMascara)
    : CFEspacial (matriz,  _tamanhoMascara)
  {
  }

  /// Cria a mascara adequada
  virtual void CriaMascara (unsigned int _tamanhoMascara);	
};
#endif // CFEDeteccaoContornoMasc_h
