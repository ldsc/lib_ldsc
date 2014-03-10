#ifndef CGrafoConexaoSerial_M2_h
#define CGrafoConexaoSerial_M2_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
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
 * @brief 	Determinação do grafo de conexão serial de imagens 3D usando modelo 2.
 * Aqui a condutância entre dois sítios é dada pela condutância média entre os dois objetos.
 * Adiciona rotulador intermediário.
 *
 * @author 	André Duarte Bueno
 * @see		grafos
 * @ingroup HCGrafo
*/
class CGrafoConexaoSerial_M2 : public CGrafoConexaoSerial {
// --------------------------------------------------------------Atributos
protected:
   /// Rotulador para imagem intermediária
   CRotulador2DCm* rotInt { nullptr };

public:
// -------------------------------------------------------------Construtor
   /// Construtor
   CGrafoConexaoSerial_M2 ( std::string _nomeArquivo ) : CGrafoConexaoSerial ( _nomeArquivo )   {
      tipoGrafo  =  ETipoGrafo::GrafoConexaoSerial_M2 ;
   }

// --------------------------------------------------------------Destrutor
   /// Destrutor
   virtual ~ CGrafoConexaoSerial_M2 () = default;

// ----------------------------------------------------------------Métodos
   /**
    * @brief Transforma uma propriedade raio Hidraulico em condutancia.
    * Tem mais de uma herdeira. Aqui o calculo da condutância entre os objetos
    * considera uma condutancia média.
    * cm = (c1 + c2)/2;
    */
   virtual void CalcularCondutancias ( long double _viscosidade, long double _dimensaoPixel,
                                       unsigned long int _fatorAmplificacao ) override;

   /**
   * @brief  Determina o grafo usando imagem 3D.
   * A mudança em relação a classe GrafoConexaoSerial, é que cria um rotulador intermediário
   * e a seguir chama CGrafoConexaoSerial::Go(), da classe base.
   * Os planos i e i+1 são rotulados, gerando-se a seguir os sítios.
   * Depois gera-se um objeto de rotulagem com o plano intermediário
   * (da conexão dos planos i com i+1) e um vetor de conexões válidas.
   * Verifica os objetos no plano i e no plano i+1, se estiverem conexos
   * estabelece a conexão nos dois sentidos. Para evitar a repetição da
   * mesma conexão, seta o vetor de conexões para aquele rótulo como sendo inválido.
   * Desta forma as conexões são corretamente estabelecidas, e elimina-se
   * as conexões redundantes. Go chama deleta rotint para economizar memória.
   */
   virtual CRede* Go ( TCMatriz3D<int>* _img3D, unsigned long int _tamanhoMascara =   1 ) override;

   /**
    * @brief  Determina o grafo lendo a imagem do disco, plano a plano.
    * Aqui, apenas cria o rotulador intermediário e chama Go da classe base
   */
   virtual CRede* Go ( std::string nomeArquivo, unsigned long int funcao ) override;

protected:
   /**
    * @brief  conecta os objetos considerando um plano intermediário,
    * que é criado para eliminar a repetição das conexões.
   */
   virtual void DeterminarConexoesObjetos ( unsigned long int maiorRotuloUtilizado ) override;

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
