#ifndef CFEInversao3D_h
#define CFEInversao3D_h
/*
----------------------------------------------------------------------------
PROJETO:	LIB_LDSC - Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Leandro Puerari,...
Copyright @1997:    Todos os direitos reservados.
Nome deste arquivo: CFEInversao3D.h
Nome da classe:     CFEInversao3D
----------------------------------------------------------------------------
*/

#include <Matriz/TCImagem3D.h>
#include <Matriz/TCMatriz3D.h>


/**
 * @brief Inverte os valores de imagens binárias tridimensionais.
 * 
 * A classe CFEInversao3D processa a inversão através da função Go.
 * 
 * Go() irá retornar um ponteiro para uma nova Imagem 3D com os valores invertidos.
 * Se a imagem passada no construtor não for uma imagem binária, Go retorará NULL.
 * 
*/

using namespace std;

class CFEInversao3D
{
//Construtor / Destrutor
public:
  	/// Construtor
  	CFEInversao3D ( ) { }

  	/// Destrutor
  	virtual ~ CFEInversao3D () { }

// Métodos
  	/// Recebe TCImagem3D, executa a inversão e retornar ponteiro para a imagem 3D invertida.
		TCImagem3D<int> * Go ( TCImagem3D<int> * imagem );
  	
  	/// Recebe CMatriz3D, executa a inversão e retornar ponteiro para a imagem 3D invertida.
  	TCMatriz3D<int> * Go ( TCMatriz3D<int> * imagem );
};

#endif
