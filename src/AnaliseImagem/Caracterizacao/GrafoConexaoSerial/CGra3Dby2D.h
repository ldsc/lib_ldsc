#ifndef CGra3Dby2D_h
#define CGra3Dby2D_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
			Laboratorio de Desenvolvimento de Software Cientifico 	 [LDSC].
@author     André Duarte Bueno
@file       CGra3Dby2D.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CGrafoContorno_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoContorno.h>
#endif

#ifndef CRotulador2DCm_h
#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/CRotulador2DCm.h>
#endif

// Lista de classes da hierarquia de grafos
class CGra3Dby2_M1;
class CGra3Dby2_M2;
class CGra3Dby2_M3;
class CGra3Dby2_M4;
class CGra3Dby2_M5;
class CGra3Dby2_M6;
// Lista de apelidos
using CGra3Dby2_M1_CondutanciaPorPixel = CGra3Dby2_M1;
using CGra3Dby2_M2_CondutanciaPelaAreaMedia = CGra3Dby2_M2;
using CGra3Dby2_M3_CondutanciaPelaAreaInterseccao = CGra3Dby2_M3;
using CGra3Dby2_M4_CondutanciaPelaAreaInterseccaoCorrigidaPelaDistancia = CGra3Dby2_M4;
using CGra3Dby2_M6_CondutanciaPelaAreaInterseccaoCorrigidaPelaDistanciaCalculaTortuosidade = CGra3Dby2_M6;
using CGra3Dby2_M6_Tortuosidade = CGra3Dby2_M6;

// ===============================================================================
// Documentacao da  @class CGra3Dby2D
// ===============================================================================
/**
 * @brief Determina o grafo a partir de imagens tridimensionais.
 *
 * Gera o grafo a partir de imagens tridimensionais.
 * Gera a estrutura de sítios baseado na avaliação
 * de cada plano da imagem tridimensional (planos bidimensionais).
 * Vai ter mais de uma herdeira.
 * Observe que da forma como esta, sempre usa sítios (CSitio e herdeiros)
 * e nunca COGLigacao ou COGComponenteGrafo.
 *
 * Lista dos herdeiros:
 * -CGra3Dby2_M1	Modelo_1
 * Para cada pixel uma ligação
 * Condutancia do pixel
 *
 * -CGra3Dby2_M2	Modelo_2
 * Para cada objeto uma ligação
 * Condutancia média entre os dois objetos
 *
 * -CGra3Dby2_M3	Modelo_3
 * Para cada objeto uma ligação
 * Condutancia calculada sobre a area da intersecção
 *
 * -CGra3Dby2_M4	Modelo_4
 * Para cada objeto uma ligação.
 * Condutancia calculada sobre a área da intersecção
 * Adicionalmente calcula as distâncias entre
 * os objetos, para correção das condutâncias.
 *
 * -CGra3Dby2_M5	Modelo_5
 * -# sub ítem do modelo 5
 * -# sub ítem do modelo 5
 *
 * -CGra3Dby2_M6
 * A condutância é corrigida levando em conta as distâncias dos centros de massa,
 * que aqui são armazenados no próprio sítio.
 * Calcula tortuosidade.
 *
 * @author	André Duarte Bueno
 * @see		Grafo
 * @ingroup  HCGrafo
*/
class CGra3Dby2D : public CGrafoContorno {
     // --------------------------------------------------------------Atributos
private:
     /// Indica versão da função que elimina os ramos mortos, a 2 é mais rápida.
     int eliminaRamosMortos = 2 ;
     int eliminaConeccoesRepetidas = 1 ;	///< Elimina conecções repetidas

protected:
     // O primeiro e último plano tem propriedades fixas(pressão constante), sendo assim,
     // não precisam ser calculados.
     // O objetivo de se criar os atributos abaixo é eliminar a chamada do calculo das propriedades
     // nos objetos destes planos.
     // firstObjectOf Solver=rotuloPrimeiroObjetoPlano1;
     // lastObjectOf Solver=rotuloUltimoObjetoPlanoN_1
     /// Rótulo do primeiro objeto do plano z=1 (logo após o plano z=0)
     /// @todo: Pensar em criar um vector<int> indicePrimeiroObjetoPlano;
     /// sendo indicePrimeiroObjetoPlano[i] o rótulo do primeiro objeto do plano.
     unsigned int rotuloPrimeiroObjetoPlano1 {0};

     /// Rótulo do último objeto do plano z=n-1 (imediatamente antes do plano z=n)
     unsigned int rotuloUltimoObjetoPlanoN_1 {0};

     /// Dimensoes da imagem tridimensional
     int nx = 0 ; ///< Dimensão nx
     int ny = 0 ; ///< Dimensão ny
     int nz = 0 ; ///< Dimensão nz

     /// Rotulador bidimensional para imagem anterior
     CRotulador2DCm *ra = nullptr;

     /// Rotulador bidimensional para imagem posterior
     CRotulador2DCm *rp = nullptr;

     /// Imagem usada internamente para copiar planos ra, rp e plano intermediário
     TCMatriz2D< int > *img2D = nullptr;

     /**
       * @brief Informa o plano que esta sendo avaliado.
       * É passado para o objeto, fazendo-se objeto->x=plano,
       * desta forma a previsão inicial do valor de x poderá considerar o plano.
     */
     unsigned long int plano = 0;

// Por default o objeto esta no centro do grafo e não em seus contornos
//  	CContorno::ETipoContorno tipoContornoObjeto = CContorno::ETipoContorno::CENTER;

     /*Usado para definir o maior rótulo do plano anterior a ra.
       Ex: Se ra é o plano 12, rp é o plano 13,
       maiorRotuloUtilizado =  soma de todos os rótulos dos planos 0->11.*/

     /**
       * @brief Valor do maior rotulo já utilizado.
       * Em cada plano, os rótulos iniciam em 0 (o fundo), 1 o primeiro objeto,...n
       * A variável maior RotuloUtilizado vai acumulando o número de objetos do grafo.
     */
     unsigned long int maiorRotuloUtilizado = 0;

public:
     // -------------------------------------------------------------Construtor
     /// Construtor, recebe um nome de arquivo; só se for chamado CGra3Dby2D{string};
     explicit CGra3Dby2D ( std::string _nomeArquivo )
          : CGrafoContorno ( _nomeArquivo ) {
          tipoGrafo  =  ETipoGrafo::grafo3DBy2D ;
     }

     // --------------------------------------------------------------Destrutor
     /// Destrutor, deleta objetos internos
     virtual ~ CGra3Dby2D () {
          if ( ra ) {
               delete ra; 			// deleta objeto
          }
          if ( rp ) {
               delete rp; 			// deleta objeto
          }

          if ( img2D ) {
               delete img2D; 		// deleta objeto
          }
     }

     // ----------------------------------------------------------------Métodos
     /**
       * @brief 	Função Go, realiza a determinação de todo o grafo.
     */
     virtual CGrafo *Go ( TCMatriz3D<int> *_img3D, unsigned long int _tamanhoMascara = 1 ) override;

     /**
       * @brief Função Go, realiza a determinação de todo o grafo.
     */
     virtual CGrafo *Go ( std::string nomeArquivo, unsigned long int _tamanhoMascara = 0 ) override;

     /**
       * @brief VAZIA: implementada no modelo 3, elimina os links repetidos.
     */
     virtual void EliminarCondutanciasRepetidas ()	{	}

     /**
      * @brief  Transforma uma propriedade em outra (ex: raio Hidraulico em condutancia).
      * Tem mais de uma herdeira. Ou seja a conversão raioHidraulico->condutancia
      * é feita de diferentes formas.
      * @todo: verificar possibilidade de receber estes parametros no construtor da classe?
     */
     virtual void CalcularCondutancias ( long double _viscosidade, long double _dimensaoPixel,
                                         unsigned long int _fatorAmplificacao ) {};

     /// Seta a matriz A e o vetor B, a serem utilizados por um solver externo (ex: gmres)
     virtual bool SetarMatrizAVetorB ( TCMatriz2D< int > *&A, CVetor *&B ) const;

     /**
      * @brief No caso de queda de energia, foi projetado um sistema de reconstrução do grafo();
      * A primeira etapa é o recalculo de todo o grafo a partir da imagem (é rápido);
      * Em seguida, faz a leitura de um arquivo de disco que armazena as propriedades x (pressões)
      * de cada objeto do grafo. Esta função lê os valores de x de cada objeto do grafo.
      */
     bool LerVetorPropriedades_x();

     /// Salva propriedades dos objetos em disco (permite uso LerVetorPropriedades_x()).
     bool SalvarVetorPropriedades_x();


protected:

     // FUNÇÕES AUXILIARES
     /**
     * @brief Função que cria os objetos e os adiciona a lista de objetos.
       * Recebe como parâmetros um objeto rotulador (com os objetos a serem adicionados),
       * o valor do maior rotulo utilizado, e o tipo de objeto a ser criado
       */
     virtual void AdicionarObjetos ( CRotulador2DCm *rotulador, unsigned long int rotuloAtual,
                                     CContorno::ETipoContorno tipo );

     /// Função que conecta objetos em planos adjacentes.
     virtual void DeterminarConeccoesObjetos ( unsigned long int maiorRotuloUtilizado ) = 0;

     /// Função Usada para calcular a condutancia das ligações
     // virtual void CalcularPropriedadesConeccoes() {};

     /// Função que elimina sítios redundantes (com 0 links)
     void EliminarObjetosRedundantes ();

     /**
       * @brief Reorganiza os links para cmx e cmy.
       * Funcao nova, como alguns ramos estão sendo deletados eu chamo esta função para reorganizar
       * os centros de massa respeitando o novo estado do grafo.
       * Necessaria para o modelo 4, por isto é definida aqui como sendo vazia.
     */
     virtual void ReorganizarCmxCmy ()	{	}

     /*
       * @brief Função que calcula o centro de massa dos objetos.
       * Vai ser redefinida nas herdeiras M4 e M5
       * Não implementada, vai deletar os centros de massa ao deletar os objetos.
     */
     // virtual void  CalcularCentroMassa(){};

     /* Função que deleta um objeto do grafo(não usada) */
     // virtual bool DeletarObjeto(int ri) {return 0;};

     /**
       * @brief Função que marca um objeto do grafo para deleção é deletado
       * efeticamente por EliminarObjetosRedundantes_2
     */
     virtual bool MarcarParaDelecaoObjeto ( int i );

     // virtual bool DeletarObjeto(CObjetoGrafo* sitio) ;

private:
     /// Elimina objetos redundantes, versão 1
     void EliminarObjetosRedundantes_1 ();

     /// Elimina objetos redundantes, versão 2
     void EliminarObjetosRedundantes_2 ();

     // --------------------------------------------------------------------Get
public:
     /// Retorna nx.
     int Nx ()    {
          return nx;
     }

     /// Retorna ny.
     int Ny ()    {
          return ny;
     }

     /// Retorna nz.
     int Nz ()    {
          return nz;
     }

     /// Retorna o número que informa o método que irá eliminar os ramos mortos.
     int EliminarRamosMortos() {
          return eliminaRamosMortos;
     }

     /// Elimina conecções repetidas
     int EliminarConeccoesRepetidas() {
          return eliminaConeccoesRepetidas;
     }

     // --------------------------------------------------------------------Set
     /// Seta elimina ramos mortos
     void EliminarRamosMortos ( int _e ) {
          eliminaRamosMortos = _e;
     }

     /// Seta conecções repetidas
     void EliminarConeccoesRepetidas ( int _r ) {
          eliminaConeccoesRepetidas = _r;
     }

     // -----------------------------------------------------------------Friend
     //       friend ostream& operator<< (ostream& os, CGra3Dby2D& obj);
     //       friend istream& operator>> (istream& is, CGra3Dby2D& obj); //  Public attributes
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGra3Dby2D& obj);
// istream& operator>> (istream& is, CGra3Dby2D& obj);
#endif
