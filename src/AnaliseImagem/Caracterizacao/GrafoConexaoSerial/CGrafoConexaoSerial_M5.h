#ifndef CGrafoConexaoSerial_M5_h
#define CGrafoConexaoSerial_M5_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CGrafoConexaoSerial_M5.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iostream>
#include <cstdlib>
#include <cstdio>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// #include <Base/_LIB_LDSC_CLASS.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M4.h>

/**
 * @brief Determina o grafo de imagens 3D, modelo 5.
 * Note que como é herdeiro do modelo 4, considera correçcão das distâncias (sem limite).
 * Elimina os sítios isolados. Sítios que estao no plano atual rp
 * e nao tem conexão com o plano anterior (ra)
 * e posterior (img3D[i][j][ plano + 1 ])
 * De forma que o calculo das distâncias reais entre os sítios
 * vai ser mais correto no que se refere ao escoamento.

 * Superclasse:    CGrafo -> CGrafoConexaoSerial -> CGrafoConexaoSerial_M2
 * -> CGrafoConexaoSerial_M5
 * @author 	André Duarte Bueno
 * @see			grafos
 * @ingroup  HCGrafo
*/
class CGrafoConexaoSerial_M5 : public CGrafoConexaoSerial_M4 {
// --------------------------------------------------------------Atributos
protected:
     // Objeto para saída (deveria ser comentado!).
     std::ofstream pixeisDeletados;

     /// Ponteiro para imagem 3D. @todo: justificar!
     TCMatriz3D<int> *img3D;

public:
// -------------------------------------------------------------Construtor
/// Construtor
     CGrafoConexaoSerial_M5 ( std::string _nomeArquivo ) : CGrafoConexaoSerial_M4 ( _nomeArquivo )  {
          tipoGrafo  =  ETipoGrafo::GrafoConexaoSerial_M5;
		  string arqDisco = std::string("PixeisDeletados_") + _nomeArquivo;
          pixeisDeletados.open ( arqDisco.c_str() );
          if ( pixeisDeletados.bad() )   {
               std::cerr << "\nNão conseguiu abrir arquivo de disco:" << arqDisco << std::endl;
               exit ( 0 );
          }
     }

// --------------------------------------------------------------Destrutor
     /// Destrutor
     virtual ~ CGrafoConexaoSerial_M5 ()  {
          pixeisDeletados.close ();
     }

// ----------------------------------------------------------------Métodos
     /// Processa a determinação do grafo.
     CRede *Go ( TCMatriz3D<int> *_img3D, unsigned long int funcao ) override;

protected:
     /// Adiciona objetos ao grafo
     virtual void AdicionarObjetos ( CRotulador2DCm *rotulador,
                                     unsigned long int ultimoRotuloUtilizado,
                                     ETipoObjetoGrafo tipoObjeto ) override;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGrafoConexaoSerial_M5& obj);
//       friend istream& operator>> (istream& is, CGrafoConexaoSerial_M5& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGrafoConexaoSerial_M5& obj);
// istream& operator>> (istream& is, CGrafoConexaoSerial_M5& obj);

#endif
