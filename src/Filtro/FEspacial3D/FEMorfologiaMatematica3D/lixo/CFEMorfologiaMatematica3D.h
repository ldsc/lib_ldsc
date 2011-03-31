// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
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
Assunto:             Classe básica para os filtros da morfologia matemática
Superclasse:         TFiltro
Descrição:           Tem a função cria mascara, que cria uma máscara,
							euclidiana (uma bola) que é utilizada como elemento
                     estruturante nas operações de filtragem.
                     Obs: para funcionamento do afinamento e espessamento
                     foram acrescentados dois flags e duas funções
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
  bool EEHitMiss1, EEHitMiss2;	// flags de controle de criação da mascara
  // na função cria mascara, se ativos, alteram a mascara
  // criada. O default é estarem desativados

public:
    CFEMorfologiaMatematica3D (TMatriz3D * &matriz,
			      unsigned int
			      _tamanhoMascara):TFEspacial3D (matriz,
							     _tamanhoMascara),
    EEHitMiss1 (false), EEHitMiss2 (false)
  {
  };
  // Abaixo a função Go que é reescrita aqui, vazia, para inibir o
  // calculo da convolução realizado na classe TFEspacial
  // As operações da morfologia matemática são diferentes.
  virtual TMatriz3D *Go (TMatriz3D * &matriz, unsigned int _tamanhoMascara =
			 0)
  {
    return matriz;
  };

  virtual void CriaMascara (unsigned int _tamanhoMascara);	// Cria a mascara adequada

  // as operações recebem o raio da bola
  virtual TMatriz3D *Abertura (TMatriz3D * &matriz, unsigned int _RaioBola);
  virtual TMatriz3D *Erosao (TMatriz3D * &matriz, unsigned int _RaioBola);
  virtual TMatriz3D *Dilatacao (TMatriz3D * &matriz, unsigned int _RaioBola);
  virtual TMatriz3D *Fechamento (TMatriz3D * &matriz, unsigned int _RaioBola);

  // Corrigir arrumar, as funções abaixo
  virtual TMatriz3D *DeteccaoContorno (TMatriz3D * &matriz,
				       unsigned int _RaioBola);
  virtual TMatriz3D *HitMiss (TMatriz3D * &matriz, unsigned int _RaioBola);
  virtual TMatriz3D *Afinamento (TMatriz3D * &matriz, unsigned int _RaioBola);
  virtual TMatriz3D *Espessamento (TMatriz3D * &matriz,
				   unsigned int _RaioBola);
  virtual TMatriz3D *Esqueleto (TMatriz3D * &matriz, unsigned int _RaioBola);

protected:

  void CriaEEB1 (unsigned int _tamanhoMascara);	// Funções que criam mascaras do tipo b1
  void CriaEEB2 (unsigned int _tamanhoMascara);	// e b2, usadas no afinamento e espessamento

};
typedef CFEMorfologiaMatematica3D CFEMMClassica3D;
#endif // CFEMorfologiaMatematica_h
