// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFERotacaoVerticaleDiferenca_h
#define CFERotacaoVerticaleDiferenca_h

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
Nome deste arquivo:	CFERotacaoVerticaleDiferenca.h
Nome da classe:      CFERotacaoVerticaleDiferenca
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro CFERotacaoVerticaleDiferenca.
 * Herdeiro de CFEspacial.
 */
class CFERotacaoVerticaleDiferenca : public CFEspacial
{
public:
		/// Construtor
  CFERotacaoVerticaleDiferenca (CMatriz2D * &matriz, unsigned int _tamanhoMascara)
  : CFEspacial (matriz, _tamanhoMascara)
  {
  }

  /// Cria a mascara adequada
  virtual void CriaMascara (unsigned int _tamanhoMascara);	
};
#endif // CFERotacaoVerticaleDiferenca_h
