// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFMMDeslocamento_h
#define CFMMDeslocamento_h

// inclue a biblioteca de filtros
#ifndef CFMorfologiaMatematica_h
#include <TFiltro/TFEspacial/TFEMorfologiaMatematica/CFMorfologiaMatematica.h>
#endif
/*
----------------------------------------------------------------------------
PROJETO:	Bibliotecas LMPT
		Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMDeslocamento.h
Nome da classe:      CFMMDeslocamento
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:    CFMMDeslocamento
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
class CFMMDeslocamento:public CFMorfologiaMatematica
{

public:
  CFMMDeslocamento (CMatriz2D * &matriz,
		    unsigned int  _tamanhoMascara)
:CFMorfologiaMatematica (matriz,	     _tamanhoMascara)
  {
  }

  // virtual CMatriz2D * Go( );
  // virtual void CriaMascara(unsigned int _tamanhoMascara);     // Cria a mascara adequada

  // as opera��es recebem o raio da bola
  // virtual CMatriz2D*  Abertura(CMatriz2D*& matriz,unsigned int _RaioBola);    // 
  virtual CMatriz2D *Erosao (CMatriz2D * &matriz, unsigned int _RaioBola);	// 
  virtual CMatriz2D *Dilatacao (CMatriz2D * &matriz, unsigned int _RaioBola);	// 
  // virtual CMatriz2D*  Fechamento(CMatriz2D*& matriz,unsigned int _RaioBola);// 

  // virtual CMatriz2D*  DeteccaoContorno(CMatriz2D*& matriz,unsigned int _RaioBola);// 
  // virtual CMatriz2D*  HitMiss(CMatriz2D*& matriz,unsigned int _RaioBola);// 
  // virtual CMatriz2D*  Esqueleto(CMatriz2D*& matriz,unsigned int _RaioBola);// 

protected:

};
#endif // CFMMDeslocamento_h
