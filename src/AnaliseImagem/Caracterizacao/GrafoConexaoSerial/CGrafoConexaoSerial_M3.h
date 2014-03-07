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
  * @author 	André Duarte Bueno
  * @see			grafos
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
       * @brief Para cada sítio calcula a condutancia do sítio considerando
       * o raioHidraulico = area / perimetro.
       * A condutancia é calculada usando a equação de Poiselle...(Bear, pg...).
       * Aqui calcula a condutância dos sítios e dos links
      */
     virtual void CalcularCondutancias ( long double _viscosidade, long double _dimensaoPixel,
                                         unsigned long int _fatorAmplificacao ) override;

     /**
       * @brief Pesquisa o vetor dos links e elimina os links repetidos.
       * é definido como público para ser chamado depois de Go e depois do
       * calculo das condutâncias, visto que para eliminar os links repetidos,
       * acumula as condutâncias duplicadas.
       * Se for chamado antes do calculo das condutâncias, vai acumular os raios hidraulicos
      */
     virtual void EliminarCondutanciasRepetidas () override;

     virtual bool SetarMatrizAVetorB ( TCMatriz2D< int >*&A, CVetor*&B ) const override;

protected:
     // Métodos redefinidos
     /**
      * @brief Determina as conexões entre os objetos
       * além de estabelecer os links entre os objetos, calcula o raio Hidraulico
       * das ligações
      */
     virtual void DeterminarConeccoesObjetos ( unsigned long int maiorRotuloUtilizado ) override;

     /// Redefinida,  retorna um CObjetoRede_CC_Sitio ou derivado
     //virtual CObjetoRede *CriarObjetoGrafo ( CContorno::ETipoContorno tipoContorno ) override;

     // Função Go
     // virtual CGrafo* Go( TCMatriz3D<int> * _img3D,unsigned long int  _tamanhoMascara = 1);
     // virtual CGrafo* Go( std::string nomeArquivo,unsigned long int _tamanhoMascara = 0) ;

     // --------------------------------------------------------------------Get
     // --------------------------------------------------------------------Set
     // -----------------------------------------------------------------Friend
     //       friend ostream& operator<< (ostream& os, CGrafoConexaoSerial_M3& obj);
     //       friend istream& operator>> (istream& is, CGrafoConexaoSerial_M3& obj);
};

#endif
