#ifndef CGrafoConexaoSerial_M6_Tortuosidade_h
#define CGrafoConexaoSerial_M6_Tortuosidade_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CGrafoConexaoSerial_M6_Tortuosidade.h
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

#ifndef CGrafoConexaoSerial_M3_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M3.h>
#endif

// ===============================================================================
// Documentacao Classe: CGrafoConexaoSerial_M6_Tortuosidade
// ===============================================================================
/**
 * @brief Determina o grafo, modelo 6. Herdeiro de CGrafoConexaoSerial_M3, acrescenta
 * correção da condutividade em função da distância entre os centros de massa dos objetos.
 * Nesta classe o centro de massa é armazenado nos objetos do grafo e não em vetores cmx,cmy.
 * Também acrescenta o cálculo da tortuosidade.
 * 
 * @author	André Duarte Bueno
 * @see		Grafos
 * @ingroup  HCGrafo
*/
class CGrafoConexaoSerial_M6_Tortuosidade : public CGrafoConexaoSerial_M3, public CTortuosidade {
// --------------------------------------------------------------Atributos
private:
     /// Usado no cálculo da tortuosidade, representa o número total de coneções entre os objetos
     long int numeroDerivacoesUsadasCalculoTortuosidade = 0;

// -------------------------------------------------------------Construtor
public:
     /// Construtor
     CGrafoConexaoSerial_M6_Tortuosidade ( std::string _nomeArquivo ) :CGrafoConexaoSerial_M3 ( _nomeArquivo ),
          numeroDerivacoesUsadasCalculoTortuosidade ( 0 )  {
          tipoGrafo  =  ETipoGrafo::GrafoConexaoSerial_M6;
     }

// --------------------------------------------------------------Destrutor
     /// Destrutor
     virtual ~ CGrafoConexaoSerial_M6_Tortuosidade () = default;

// ----------------------------------------------------------------Métodos
     /**
      * @brief Redefinida, na classe  CGrafoConexaoSerial_M3 calcula as condutâncias
      * dos sítios sem considerar o deslocamento dos centros de massa
      * aqui considera as posições dos centros de massa
     */
     virtual void CalcularCondutancias ( long double _viscosidade,
                                         long double _dimensaoPixel,
                                         unsigned long int _fatorAmplificacao ) override;

protected:
     /// Redefinida, para  o tipoContorno=centro retorna um CObjetoEsqueleto
     virtual CObjetoRede *CriarObjetoGrafo ( CContorno::ETipoContorno tipoContorno ) override;

//Verificar porque precisa	 redefinicao abaixo!!!
/**
* @brief Função usada para criar os objetos, todos do tipo CObjetoRede_Tipo
*que não tem métodos virtuais, sendo + rápido!
* Recebe o tipo de objeto a ser criado.
*/
virtual CObjetoRede_Tipo* CriarObjetoGrafo ( ETipoObjetoGrafo tipoObjeto ) {
	CObjetoRede_Tipo* data = new CObjetoRede_Tipo ( tipoObjeto );
	assert ( data );
	return data;
};

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
//       friend ostream& operator<< (ostream& os, CGrafoConexaoSerial_M6_Tortuosidade& obj);
//       friend istream& operator>> (istream& is, CGrafoConexaoSerial_M6_Tortuosidade& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGrafoConexaoSerial_M6_Tortuosidade& obj);
// istream& operator>> (istream& is, CGrafoConexaoSerial_M6_Tortuosidade& obj);
#endif
