#ifndef CDistribuicaoIDF_h
#define CDistribuicaoIDF_h

#ifndef CMatriz2D_h
#include <AnaliseImagem/Matriz/CMatriz2D.h>
#endif

#include <fstream>
#include <string>
#include <vector>


/* ----------------------------------------------------------------------------
 * PROJETO:	LIB_LDSC
 * ----------------------------------------------------------------------------
 * Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico
 * Programadores: Leandro Puerari, Andre Duarte Bueno.
 * Copyright @2011:    Todos os direitos reservados.
 * Nome deste arquivo: CDistribuicao.h
 * Nome da classe:     CDistribuicao
 * Arquivos de documentacao do projeto em: path\doc\
 * ----------------------------------------------------------------------------
 */
/**
 * @brief Declara a classe CDistribuicao, a qual permite determinar,
 * através da Imagem de Distância ao Fundo (IDF),
 * a distribuição de tamanho dos objetos (poros) em uma imagem.
*/

using namespace std;

class CDistribuicaoIDF {
    vector <int> raios;
    //int tamVectorRaios;
public:
    /// Construtor (recebe CMatriz2D)
    CDistribuicaoIDF ( CMatriz2D * _pm ) : pm ( _pm ) {
        //tamVectorRaios = 100;
        //raios.reserve(tamVectorRaios);
    }

    /// Destrutor
    ~ CDistribuicaoIDF () { }

// Métodos
    bool Go ( string fname, int indice=1, int fundo=0 );

//Atributos
private:
    CMatriz2D * pm;

};

#endif
