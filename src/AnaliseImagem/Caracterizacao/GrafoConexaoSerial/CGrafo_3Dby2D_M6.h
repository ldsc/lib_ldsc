#ifndef CGrafo_3Dby2D_M6_h
#define CGrafo_3Dby2D_M6_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CGrafo_3Dby2D_M6.h
@begin      Oct 20 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CTortuosidade_h
#include <AnaliseImagem/Simulacao/Tortuosidade/CTortuosidade.h>
#endif

#ifndef CGrafo_3Dby2D_M3_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafo_3Dby2D_M3.h>
#endif

// ===============================================================================
// Documentacao Classe: CGrafo_3Dby2D_M6
// ===============================================================================
/**
 * @brief Determina o grafo, modelo 6.
 * Superclasse: CGrafo -> CGrafo_3Dby2D -> CGra3Dby2_M2 -> CGra3Dby2_M3 -> CGrafo_3Dby2D_M6
 * @author	André Duarte Bueno
 * @see		Grafos
 * @ingroup  HCGrafo
*/
class CGrafo_3Dby2D_M6 : public CGrafo_3Dby2D_M3, public CTortuosidade {
// --------------------------------------------------------------Atributos
private:
     /// Usado no cálculo da tortuosidade, representa o número total de coneções entre os objetos
     long int numeroDerivacoesUsadasCalculoTortuosidade = 0;

// -------------------------------------------------------------Construtor
public:
     /// Construtor
     CGrafo_3Dby2D_M6 ( std::string _nomeArquivo ) :CGrafo_3Dby2D_M3 ( _nomeArquivo ),
          numeroDerivacoesUsadasCalculoTortuosidade ( 0 )  {
          tipoGrafo  =  ETipoGrafo::grafo3DBy2D_M6;
     }

// --------------------------------------------------------------Destrutor
     /// Destrutor
     virtual ~ CGrafo_3Dby2D_M6 () = default;

// ----------------------------------------------------------------Métodos
     /**
      * @brief Redefinida, na classe  CGra3Dby2_M3 calcula as condutâncias
      * dos sítios sem considerar o deslocamento dos centros de massa
      * aqui considera as posições dos centros de massa
     */
     virtual void CalcularCondutancias ( long double _viscosidade,
                                         long double _dimensaoPixel,
                                         unsigned long int _fatorAmplificacao ) override;


protected:
     /// Redefinida, para  o tipoContorno=centro retorna um CObjetoRede_Sitio_CC_CM
     virtual CObjetoRede *CriarObjetoGrafo ( CContorno::ETipoContorno tipoContorno ) override;

     /**
      * @brief Função que cria os objetos e os adiciona a lista de objetos.
      * Recebe como parâmetros um objeto rotulador (com os objetos a serem adicionados)
      * o valor do maior rotulo utilizado o tipo de objeto a ser criado
     */
     virtual void AdicionarObjetos ( CRotulador2DCm *rotulador,unsigned long int rotuloAtual,
                                     CContorno::ETipoContorno tipo ) override;


// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGrafo_3Dby2D_M6& obj);
//       friend istream& operator>> (istream& is, CGrafo_3Dby2D_M6& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGrafo_3Dby2D_M6& obj);
// istream& operator>> (istream& is, CGrafo_3Dby2D_M6& obj);
#endif