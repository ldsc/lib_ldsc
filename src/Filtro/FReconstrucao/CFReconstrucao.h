#ifndef CFReconstrucao_h
#define CFReconstrucao_h
/*
----------------------------------------------------------------------------
PROJETO:	LIB_LDSC
		Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   Andre D.Bueno, Leandro Puerari, ...
Copyright @1997:    Todos os direitos reservados.
Nome deste arquivo: CFReconstrucao.h
Nome da classe:     CFReconstrucao
----------------------------------------------------------------------------
*/
#include <Matriz/CMatriz3D.h>
#include <Matriz/TCMatriz2D.h>

/**
 * @brief A classe CFReconstrucao cria uma imagem 3D a partir da rotação dos planos de uma imagem 2D
 * 
*/

using namespace std;
/// Enumeração referente aos eixos X e Y de uma imagem.
enum Eeixo { E_EIXO_X, E_EIXO_Y
};

class CFReconstrucao
{
public:
  /// Construtor Default
  CFReconstrucao ( ) { }

  /// Destrutor
  virtual ~ CFReconstrucao () { }

  /// Executa a reconstrução e retorna a imagem 3D reconstruída.
  CMatriz3D * Go ( TCMatriz2D< int > * matriz2D, Eeixo _eixo=E_EIXO_X );
  
  /// Executa a reconstrução e retorna a imagem 3D reconstruída.
  CMatriz3D * Go ( string arquivo, Eeixo _eixo=E_EIXO_X );
    
  //void teste(string arquivo);
};

#endif
