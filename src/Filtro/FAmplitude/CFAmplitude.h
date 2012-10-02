// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFAmplitude_h
#define CFAmplitude_h

#ifndef TCFiltro_h
#include <Filtro/TCFiltro.h>
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
Nome deste arquivo:	CFAmplitude.h
Nome da classe:      CFAmplitude
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief     Filtro de amplitude (atuam sobre o espaço de cor).
 * 
 * A superclasse desta hierarquia é a classe CFiltro
 * que representa um filtro abstrato.
 * Um filtro é um objeto que recebe como parâmetro uma imagem
 * e através de conceitos próprios atua sobre esta imagem retornando
 * uma imagem alterada, ou seja um imagem filtrada.
 * imagem-> filtro-> imagem.
 * Assim um filtro recebe uma imagem e retorna uma imagem.
 *
 * 
 * Os filtros podem ser divididos basicamente em dois tipos:
 * Um atua sobre o espaço de cor da imagem (são os filtros de amplitude)
 * Outro atua sobre o espaço geométrico da imagem (são os filtros espaciais)
 * Cardinalidade:    1:1    ,de uma maneira geral usa-se um filtro para atuar sobre uma imagem
*/
template<typename T>
class CFAmplitude : public TCFiltro<T>
{
public:
/// Construtor
	CFAmplitude (TCMatriz2D< T > * &matriz)
		: TCFiltro<T> (matriz) {
  }


  /// Destrutor
	~CFAmplitude () {
  }
  
};

#endif //  CFAmplitude_h
