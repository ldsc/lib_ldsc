#ifndef CGrafo_3Dby2D_M5_h
#define CGrafo_3Dby2D_M5_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CGrafo_3Dby2D_M5.h
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafo_3Dby2D_M4.h>

/**
 * @brief Determina o grafo de imagens 3D, modelo 5.
 * Note que como é herdeiro do modelo 4, considera correçcão das distâncias (sem limite).
 * Elimina os sítios isolados. Sítios que estao no plano atual rp
 * e nao tem conexão com o plano anterior (ra)
 * e posterior (img3D[i][j][ plano + 1 ])
 * De forma que o calculo das distâncias reais entre os sítios
 * vai ser mais correto no que se refere ao escoamento.

 * Superclasse:    CGrafo -> CGrafo_3Dby2D -> CGrafo_3Dby2D_M2
 * -> CGrafo_3Dby2D_M5
 * @author 	André Duarte Bueno
 * @see			grafos
 * @ingroup  HCGrafo
*/
class CGrafo_3Dby2D_M5 : public CGrafo_3Dby2D_M4 {
// --------------------------------------------------------------Atributos
protected:
     // Objeto para saída (deveria ser comentado!).
     std::ofstream pixeisDeletados;

     /// Ponteiro para imagem 3D. @todo: justificar!
     TCMatriz3D<int> *img3D;

public:
// -------------------------------------------------------------Construtor
     /// Construtor
     CGrafo_3Dby2D_M5 ( char *_nomeArquivo ) :CGrafo_3Dby2D_M4 ( _nomeArquivo )  {
          tipoGrafo  =  ETipoGrafo::grafo3DBy2D_M5;

          char arqDisco[256];
          sprintf ( arqDisco, "PixeisDeletados_%s", _nomeArquivo );
          pixeisDeletados.open ( arqDisco );
          if ( pixeisDeletados.bad() )   {
               std::cerr << "\nNão conseguiu abrir arquivo de disco:" << arqDisco << std::endl;
               exit ( 0 );
          }
     }

// --------------------------------------------------------------Destrutor
     /// Destrutor
     virtual ~ CGrafo_3Dby2D_M5 ()  {
          pixeisDeletados.close ();
     }

// ----------------------------------------------------------------Métodos
     /// Processa a determinação do grafo.
     CGrafo *Go ( TCMatriz3D<int> *_img3D, unsigned long int funcao ) override;

protected:
     /// Adiciona objetos ao grafo
     virtual void AdicionarObjetos ( CRotulador2DCm *rotulador,
                                     unsigned long int ultimoRotuloUtilizado,
                                     CContorno::ETipoContorno tipoContornoObjeto ) override;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGrafo_3Dby2D_M5& obj);
//       friend istream& operator>> (istream& is, CGrafo_3Dby2D_M5& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGrafo_3Dby2D_M5& obj);
// istream& operator>> (istream& is, CGrafo_3Dby2D_M5& obj);

#endif
