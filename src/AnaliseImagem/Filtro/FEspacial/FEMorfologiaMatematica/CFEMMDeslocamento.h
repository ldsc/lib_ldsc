//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMDeslocamento_h
#define CFEMMDeslocamento_h

//  inclue a biblioteca de filtros
#ifndef CFEMorfologiaMatematica_h
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/CFEMorfologiaMatematica.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:	Bibliotecas LDSC
		Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMDeslocamento.h
Nome da classe:      CFEMMDeslocamento
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Classe basica para os filtros da morfologia matematica, 
 * baseada no método do deslocamento.
 * Tem a função cria mascara, que cria uma máscara,
 * euclidiana (uma bola) que é utilizada como elemento
 * estruturante nas operações de filtragem.
*/
class CFEMMDeslocamento:public CFEMorfologiaMatematica
{

public:
	/// Construtor
  CFEMMDeslocamento (CMatriz2D * &matriz,
		     unsigned int
		     _tamanhoMascara):CFEMorfologiaMatematica (matriz,
							       _tamanhoMascara)
  {
  }

  /// Processa erosão  
  virtual CMatriz2D *Erosao (CMatriz2D * &matriz, unsigned int _RaioBola);	
   
  /// Processa dilatação
  virtual CMatriz2D *Dilatacao (CMatriz2D * &matriz, unsigned int _RaioBola);	  
  //  virtual CMatriz2D*  Fechamento(CMatriz2D*& matriz,unsigned int _RaioBola);
  //  virtual CMatriz2D*  DeteccaoContorno(CMatriz2D*& matriz,unsigned int _RaioBola);
  //  virtual CMatriz2D*  HitMiss(CMatriz2D*& matriz,unsigned int _RaioBola); 
  //  virtual CMatriz2D*  Esqueleto(CMatriz2D*& matriz,unsigned int _RaioBola);
};
#endif //  CFEMMDeslocamento_h
