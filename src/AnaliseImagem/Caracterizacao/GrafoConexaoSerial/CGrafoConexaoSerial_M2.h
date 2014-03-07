#ifndef CGrafoConexaoSerial_M2_h
#define CGrafoConexaoSerial_M2_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CGrafoConexaoSerial_M2.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// #ifndef _LIB_LDSC_CLASS_h
// #include <Base/_LIB_LDSC_CLASS.h>
// #endif
#ifndef CMath_h
#include <Base/CMath.h>
#endif

#ifndef CGrafoConexaoSerial_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial.h>
#endif

/**
 * @brief 	Determinação do grafo de imagens 3D usando modelo 2.
 * Para cada objeto uma ligação
 * Condutancia média entre os dois objetos
 *
 * @author 	André Duarte Bueno
 * @see			grafos
 * @ingroup  HCGrafo
*/
class CGrafoConexaoSerial_M2 : public CGrafoConexaoSerial {
// --------------------------------------------------------------Atributos
protected:
     /// Rotulador para imagem intermediária
     CRotulador2DCm *rotInt;

public:
// -------------------------------------------------------------Construtor
     /// Construtor
     CGrafoConexaoSerial_M2 ( std::string _nomeArquivo ) : CGrafoConexaoSerial ( _nomeArquivo ),  rotInt ( nullptr ) {
          tipoGrafo  =  ETipoGrafo::grafo3DBy2D_M2 ;
     }

// --------------------------------------------------------------Destrutor
     /// Destrutor
     virtual ~ CGrafoConexaoSerial_M2 () = default;

// ----------------------------------------------------------------Métodos
     /**
      * @brief Transforma uma propriedade raio Hidraulico em condutancia.
      * Tem mais de uma herdeira.
     */
     virtual void CalcularCondutancias ( long double _viscosidade, long double _dimensaoPixel,
                                         unsigned long int _fatorAmplificacao ) override;

     /**
     * @brief  Determina o grafo usando imagem 3D.
     * Aqui, apenas cria o rotulador intermediário e chama Go da classe base
     */
     virtual CGrafo *Go ( TCMatriz3D<int> *_img3D, unsigned long int _tamanhoMascara =   1 ) override;

     /**
      * @brief  Determina o grafo lendo a imagem do disco, plano a plano.
      * Aqui, apenas cria o rotulador intermediário e chama Go da classe base
     */
     virtual CGrafo *Go ( std::string nomeArquivo, unsigned long int funcao ) override;

protected:
     /**
      * @brief  conecta os objetos considerando um plano intermediário,
      * que é criado para eliminar a repetição dos links.
     */
     virtual void DeterminarConeccoesObjetos ( unsigned long int maiorRotuloUtilizado ) override;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGrafoConexaoSerial_M2& obj);
//       friend istream& operator>> (istream& is, CGrafoConexaoSerial_M2& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGrafoConexaoSerial_M2& obj);
// istream& operator>> (istream& is, CGrafoConexaoSerial_M2& obj);

#endif


/*  O calculo do fluxo de massa considera a diferença de pressão entre
  os dois objetos e a condutancia média, ou seja.
  Fluxo = (c1+c2)*DP / 2
    */
