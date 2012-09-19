#ifndef CFEInversao_h
#define CFEInversao_h
/*
----------------------------------------------------------------------------
PROJETO:	LIB_LDSC - Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Leandro Puerari,...
Copyright @1997:    Todos os direitos reservados.
Nome deste arquivo: CFEInversao.h
Nome da classe:     CFEInversao
----------------------------------------------------------------------------
*/

#include <Matriz/CImagem.h>
#include <Matriz/TCMatriz2D.h>


/**
 * @brief Inverte os valores de imagens binárias bidimensionais.
 * 
 * A classe CFEInversao processa a inversão através da função Go.
 * 
 * Go irá retornar um ponteiro para uma nova Imagem com os valores invertidos.
 * Se a imagem passada no construtor não for uma imagem binária, Go retorará NULL.
 * 
*/

using namespace std;

class CFEInversao
{
//Construtor / Destrutor
public:
  	/// Construtor
  	CFEInversao ( ) { }

  	/// Destrutor
  	virtual ~ CFEInversao () { }

// Métodos
  	/// Recebe CImagem, executa a inversão e retornar ponteiro para a imagem 3D invertida.
  	CImagem * Go ( CImagem * imagem );
  	
  	/// Recebe TCMatriz2D< int >, executa a inversão e retornar ponteiro para a imagem 3D invertida.
  	TCMatriz2D< int > * Go ( TCMatriz2D< int > * imagem );
};

#endif
