#ifndef CGrafoConexaoSerial_M4_h
#define CGrafoConexaoSerial_M4_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CGrafoConexaoSerial_M4.h
@begin      Oct 20 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <vector>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CGrafoConexaoSerial_M3_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M3.h>
#endif

// ===============================================================================
// Documentacao Classe: CGrafoConexaoSerial_M4
// ===============================================================================
/**
* @brief Acrescenta o conceito de correção das condutâncias com base na distância
* do centro de massa dos objetos conectados.
 * A informacao dos centros de massa é armazenada na classe CGrafoConexaoSerial_M4 (this),
 * usando vetores cmx, cmy, cmz. 
 * Esta solucao foi adotada por permitir a destruicao dos vetores cmx e cmy apás o 
 * cálculo das condutâncias.
 * Nota: 
 * A classe CGrafoConexaoSerial_M5 faz o mesmo, mas elimina píxeis não conectados a ambos os planos.
 * A classe CGrafoConexaoSerial_M6_Tortuosidade faz o mesmo que o modelo 3, 
 * mas corrige condutâncias com base centro massa armazenado nos objetos do esqueleto!
 *
 * @author   André Duarte Bueno
 * @see	     Grafo
* @ingroup  HCGrafo
*/
class CGrafoConexaoSerial_M4:public CGrafoConexaoSerial_M3 {
// --------------------------------------------------------------Atributos
public:
     /// Vetor de float para armazenar a informação do cmx de cada sitio do grafo
     std::vector < float >cmx;
     /// Vetor de float para armazenar a informação do cmy de cada sitio do grafo
     std::vector < float >cmy;
     /// Vetor de float para armazenar a informação do cmz de cada sitio do grafo
     std::vector < float >cmz;

public:
// -------------------------------------------------------------Construtor
/// Construtor
     CGrafoConexaoSerial_M4 ( std::string _nomeArquivo ) :CGrafoConexaoSerial_M3 ( _nomeArquivo )  {
          tipoGrafo  =  ETipoGrafo::Grafo3DBy2D_M4;
     }

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CGrafoConexaoSerial_M4 () = default;

     /// Adiciona a saida dos centros de massa
     virtual void Write (std::ofstream out ) override;

     // Adiciona o calculo dos centros de massa
     // virtual void  CalcularCentroMassa();

// ----------------------------------------------------------------Métodos

protected:
     /**
      * @brief Calcula a condutancia conforme na classe anterior,
      * e depois corrige levando em conta o centro de massa dos objetos
      * */
     virtual void CalcularCondutancias ( long double _viscosidade,
                                         long double _dimensaoPixel,
                                         unsigned long int _fatorAmplificacao ) override;

     /// Depois de criar os objetos, determina e armazena seus centro de massa
     virtual void AdicionarObjetos
     ( CRotulador2DCm *rotulador, unsigned long int ultimoRotuloUtilizado,
       CContorno::ETipoContorno tipoContorno ) override;

     // A funcao void CGrafoConexaoSerial::EliminarObjetosRedundantes()
     // elimina os ramos mortos(grupos de objetos CSitio),
     // e reordena o vector de objetos.

     /// Vai reorganizar os vetores cmx e cmy,considerando os sitios que foram eliminados
     virtual void ReorganizarCmxCmy () override;

     // Função Go
     // virtual CGrafo* Go( TCMatriz3D<int> * _img3D,unsigned long int _tamanhoMascara = 1);

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGrafoConexaoSerial_M4& obj);
//       friend istream& operator>> (istream& is, CGrafoConexaoSerial_M4& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGrafoConexaoSerial_M4& obj);
// istream& operator>> (istream& is, CGrafoConexaoSerial_M4& obj);

#endif
