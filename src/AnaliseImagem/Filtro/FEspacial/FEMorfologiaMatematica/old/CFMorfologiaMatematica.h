// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFMorfologiaMatematica_h
#define CFMorfologiaMatematica_h

// inclue a biblioteca de filtros
#ifndef TFEspacial_h
#include <AnaliseImagem/Filtro/FEspacial/TFEspacial.h>
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
Nome deste arquivo:	CFMorfologiaMatematica.h
Nome da classe:      CFMorfologiaMatematica
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:    CFMorfologiaMatematica
============================================================================
Assunto:             Classe b�sica para os filtros da morfologia matem�tica
Superclasse:         TFiltro
Descri��o:           Tem a fun��o cria mascara, que cria uma m�scara,
							euclidiana (uma bola) que � utilizada como elemento
                     estruturante nas opera��es de filtragem.
                     Obs: para funcionamento do afinamento e espessamento
                     foram acrescentados dois flags e duas fun��es
                     CriaEEB1 e CriaEEB2, posteriormente
                     remodelar este funcionamento, que ficou um pouco confuso.

Acesso:           import
Cardinalidade:    1:1    ,um filtro uma mascara
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/
class CFMorfologiaMatematica:public TFEspacial
{
  // unsigned int RaioBola;      // vai ser armazenada no filtro
  bool EEHitMiss1, EEHitMiss2;	// flags de controle de cria��o da mascara
  // na fun��o cria mascara, se ativos, alteram a mascara
  // criada. O default � estarem desativados

public:
    CFMorfologiaMatematica (CMatriz2D * &matriz,
			    unsigned int _tamanhoMascara):TFEspacial (matriz,
								      _tamanhoMascara)
  {
    EEHitMiss1 = EEHitMiss2 = false;
  };
  // Abaixo a fun��o Go que � reescrita aqui, vazia, para inibir o
  // calculo da convolu��o realizado na classe TFEspacial
  // As opera��es da morfologia matem�tica s�o diferentes.
  // virtual CMatriz2D * Go( CMatriz2D*& matriz){return matriz;};// 
  virtual CMatriz2D *Go (CMatriz2D * &matriz, unsigned int _tamanhoMascara =
			 0)
  {
    return matriz;
  };
  //  que realiza a convolu��o da imagem com o filtro
  // a cria mascara recebe o tamanho da mascara
  virtual void CriaMascara (unsigned int _tamanhoMascara);	// Cria a mascara adequada

  // as opera��es recebem o raio da bola
  virtual CMatriz2D *Abertura (CMatriz2D * &matriz, unsigned int _RaioBola);	// 
  virtual CMatriz2D *Erosao (CMatriz2D * &matriz, unsigned int _RaioBola);	// 
  virtual CMatriz2D *Dilatacao (CMatriz2D * &matriz, unsigned int _RaioBola);	// 
  virtual CMatriz2D *Fechamento (CMatriz2D * &matriz, unsigned int _RaioBola);	// 

  // Corrigir arrumar, as fun��es abaixo
  virtual CMatriz2D *DeteccaoContorno (CMatriz2D * &matriz, unsigned int _RaioBola);	// 
  virtual CMatriz2D *HitMiss (CMatriz2D * &matriz, unsigned int _RaioBola);	// 
  virtual CMatriz2D *Afinamento (CMatriz2D * &matriz, unsigned int _RaioBola);
  virtual CMatriz2D *Espessamento (CMatriz2D * &matriz,
				   unsigned int _RaioBola);
  virtual CMatriz2D *Esqueleto (CMatriz2D * &matriz, unsigned int _RaioBola);	// 

protected:
  // 
  void CriaEEB1 (unsigned int _tamanhoMascara);	// Fun��es que criam mascaras do tipo b1
  void CriaEEB2 (unsigned int _tamanhoMascara);	// e b2, usadas no afinamento e espessamento

};
typedef CFMorfologiaMatematica TFMMClassica;

#endif // CFMorfologiaMatematica_h
