// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEEmboss_h
#define CFEEmboss_h

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
Nome deste arquivo:	CFEEmboss.h
Nome da classe:      CFEEmboss
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
============================================================================
Documentacao Classe:    CFEEmboss
============================================================================
Assunto:             Filtros espaciais (atuam sobre o espa�o geom�trico da imagem).
Superclasse:         CFiltro-> CFEspacial
Descri��o:           Cria e utiliza uma m�scara para atuar sobre a imagem.
Acesso:           import
Cardinalidade:    1:1    ,de uma maneira geral usa-se um filtro para atuar sobre uma imagem
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/
class CFEEmboss : public CFEspacial
{
public:
  // construtor
  CFEEmboss (CMatriz2D * &matriz, unsigned int _tamanhoMascara)	
  :CFEspacial (matriz, _tamanhoMascara)
  {
  }

  // destrutor     
  ~CFEEmboss ()
  {
  }

  // Cria a mascara adequada
  virtual void CriaMascara (unsigned int _tamanhoMascara);	
};
#endif // CFEEmboss_h
