// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMDeslocamento_h
#define CFEMMDeslocamento_h

// inclue a biblioteca de filtros
#ifndef CFEMorfologiaMatematica_h
#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMorfologiaMatematica.h>
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
Nome deste arquivo:	CFEMMDeslocamento.h
Nome da classe:      CFEMMDeslocamento
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
============================================================================
Documentacao CLASSE:    CFEMMDeslocamento
============================================================================
Assunto:             Classe b�sica para os filtros da morfologia matem�tica
Superclasse:         TFiltro
Descri��o:           Tem a fun��o cria mascara, que cria uma m�scara,
		euclidiana (uma bola) que � utilizada como elemento
		estruturante nas opera��es de filtragem.

Acesso:           import
Cardinalidade:    1:1    ,um filtro uma mascara
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/

class CFEMMDeslocamento : public CFEMorfologiaMatematica
{

public:
  CFEMMDeslocamento (TMatriz2D * &matriz,
		    unsigned int
		    _tamanhoMascara):CFEMorfologiaMatematica (matriz,
							     _tamanhoMascara)
  {
  };
  // virtual TMatriz2D * Go( );
  // virtual void CriaMascara(unsigned int _tamanhoMascara);     // Cria a mascara adequada

  // as opera��es recebem o raio da bola

  virtual TMatriz2D *Abertura (TMatriz2D * &matriz, unsigned int _RaioBola);	// 

  virtual TMatriz2D *Erosao (TMatriz2D * &matriz, unsigned int _RaioBola);	// 

  virtual TMatriz2D *Dilatacao (TMatriz2D * &matriz, unsigned int _RaioBola);	// 

  virtual TMatriz2D *Fechamento (TMatriz2D * &matriz, unsigned int _RaioBola);	// 

  virtual TMatriz2D *DeteccaoContorno (TMatriz2D * &matriz, unsigned int _RaioBola);	// 

  virtual TMatriz2D *HitMiss (TMatriz2D * &matriz, unsigned int _RaioBola);	// 

  virtual TMatriz2D *Esqueleto (TMatriz2D * &matriz, unsigned int _RaioBola);	// 


};
#endif // CFEMMDeslocamento_h
