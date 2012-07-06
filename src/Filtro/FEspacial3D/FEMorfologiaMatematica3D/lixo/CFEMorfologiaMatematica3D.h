// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMorfologiaMatematica3D_h
#define CFEMorfologiaMatematica3D_h

// inclue a biblioteca de filtros
#ifndef CTFEspacial3D_h
#include <Filtro/FEspacial3D/CFEspacial3D.h>
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
Nome deste arquivo:	CFEMorfologiaMatematica3D.h
Nome da classe:      CFEMorfologiaMatematica3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao CLASSE:    CFEMorfologiaMatematica3D
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
class CFEMorfologiaMatematica3D:public TFEspacial3D
{
  // unsigned int RaioBola;      // vai ser armazenada no filtro
  bool EEHitMiss1, EEHitMiss2;	// flags de controle de cria��o da mascara
  // na fun��o cria mascara, se ativos, alteram a mascara
  // criada. O default � estarem desativados

public:
    CFEMorfologiaMatematica3D (TMatriz3D * &matriz,
			      unsigned int
			      _tamanhoMascara):TFEspacial3D (matriz,
							     _tamanhoMascara),
    EEHitMiss1 (false), EEHitMiss2 (false)
  {
  };
  // Abaixo a fun��o Go que � reescrita aqui, vazia, para inibir o
  // calculo da convolu��o realizado na classe TFEspacial
  // As opera��es da morfologia matem�tica s�o diferentes.
  virtual TMatriz3D *Go (TMatriz3D * &matriz, unsigned int _tamanhoMascara =
			 0)
  {
    return matriz;
  };

  virtual void CriaMascara (unsigned int _tamanhoMascara);	// Cria a mascara adequada

  // as opera��es recebem o raio da bola
  virtual TMatriz3D *Abertura (TMatriz3D * &matriz, unsigned int _RaioBola);
  virtual TMatriz3D *Erosao (TMatriz3D * &matriz, unsigned int _RaioBola);
  virtual TMatriz3D *Dilatacao (TMatriz3D * &matriz, unsigned int _RaioBola);
  virtual TMatriz3D *Fechamento (TMatriz3D * &matriz, unsigned int _RaioBola);

  // Corrigir arrumar, as fun��es abaixo
  virtual TMatriz3D *DeteccaoContorno (TMatriz3D * &matriz,
				       unsigned int _RaioBola);
  virtual TMatriz3D *HitMiss (TMatriz3D * &matriz, unsigned int _RaioBola);
  virtual TMatriz3D *Afinamento (TMatriz3D * &matriz, unsigned int _RaioBola);
  virtual TMatriz3D *Espessamento (TMatriz3D * &matriz,
				   unsigned int _RaioBola);
  virtual TMatriz3D *Esqueleto (TMatriz3D * &matriz, unsigned int _RaioBola);

protected:

  void CriaEEB1 (unsigned int _tamanhoMascara);	// Fun��es que criam mascaras do tipo b1
  void CriaEEB2 (unsigned int _tamanhoMascara);	// e b2, usadas no afinamento e espessamento

};
typedef CFEMorfologiaMatematica3D CFEMMClassica3D;
#endif // CFEMorfologiaMatematica_h
