#ifndef CGrafoConexaoSerial_M3_h
#define CGrafoConexaoSerial_M3_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
			Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CGrafoConexaoSerial_M3.h
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

#ifndef CGrafoConexaoSerial_M2_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M2.h>
#endif

/*
  ===============================================================================
  Documentacao Classe: CGrafoConexaoSerial_M3
  ===============================================================================
*/
/**
  * @brief Determina o grafo de imagens 3D, modelo 3.
  * Os modelos M1 e M2 não setam a condutância da conexão, apenas dos próprios objetos.
  * O M3 necessita que os objetos tenham um vetor de condutâncias.
  * Vai setar as condutâncias entre objetos nos planos i e i+1 
  * considerando o raio hidrâulico do plano intermediário.
  * @author André Duarte Bueno
  * @see	grafos
  * Superclasse: CGrafo -> CGrafoConexaoSerial -> CGrafoConexaoSerial_M2 -> CGrafoConexaoSerial_M3
  * @ingroup  HCGrafo
  */
class CGrafoConexaoSerial_M3 : public CGrafoConexaoSerial_M2 {
     // --------------------------------------------------------------Atributos
     // Função chamada por Go e redefinida aqui
     // calcula as condutâncias das ligações entre dois sítios
     // virtual void CalcularPropriedadesConeccoes() ;
public:

     // -------------------------------------------------------------Construtor
     /// Construtor
     CGrafoConexaoSerial_M3 ( std::string _nomeArquivo ) :CGrafoConexaoSerial_M2 ( _nomeArquivo ) {
          tipoGrafo  =  ETipoGrafo::GrafoConexaoSerial_M3;
     }

     // --------------------------------------------------------------Destrutor
     /// Destrutor
     virtual ~ CGrafoConexaoSerial_M3 ()  = default;

     // ----------------------------------------------------------------Métodos
     /**
       * @brief Como no modelo 2, converte a propriedade do objeto (raio hidrâulico) em condutância.
	   * O diferencial é que também calcula a condutância da conexão entre dois objetos considerando
	   * o raio hidraulico da intersecção dos dois objetos no plano intermediário.
       * objeto[i]->propriedade = f( objeto[i]->propriedade = raio hidraulico)
       * objeto[i]->condutancia[j] = f(  raio hidraulico intersecção entre objetos i e j )
       * A condutancia é calculada usando a equação de Poiselle...(Bear, pg...; veja Tese Bueno ou Liang).
      */
     virtual void CalcularCondutancias ( long double _viscosidade, long double _dimensaoPixel,
                                         unsigned long int _fatorAmplificacao ) override;

// /// Seta matriz A e B de solver externo.
// 	 virtual bool SetarMatrizAVetorB ( TCMatriz2D< int >*&A, CVetor*&B ) const override;

protected:
     // Métodos redefinidos
     /**
      * @brief Determina as conexões entre os objetos
       * além de estabelecer os links entre os objetos, calcula o raio Hidraulico
       * das ligações.
      */
     virtual void DeterminarConexoesObjetos ( unsigned long int maiorRotuloUtilizado ) override;

     // --------------------------------------------------------------------Get
     // --------------------------------------------------------------------Set
     // -----------------------------------------------------------------Friend
     //       friend ostream& operator<< (ostream& os, CGrafoConexaoSerial_M3& obj);
     //       friend istream& operator>> (istream& is, CGrafoConexaoSerial_M3& obj);
};

#endif
