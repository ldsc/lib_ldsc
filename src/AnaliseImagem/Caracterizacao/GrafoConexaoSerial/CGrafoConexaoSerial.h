#ifndef CGrafoConexaoSerial_h
#define CGrafoConexaoSerial_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
			Laboratorio de Desenvolvimento de Software Cientifico 	 [LDSC].
@author     André Duarte Bueno
@file       CGrafoConexaoSerial.h
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

#ifndef CRedeContorno_h
#include <EstruturaDados/CRedeContorno.h>
#endif

#ifndef CRotulador2DCm_h
#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/CRotulador2DCm.h>
#endif

#ifndef TCMatriz3D_h
#include <AnaliseImagem/Matriz/TCMatriz3D.h>
#endif

#ifndef TCMatriz2D_H
#include <AnaliseImagem/Matriz/TCMatriz2D.h>
#endif

#ifndef CVetor_h
#include <AnaliseImagem/Matriz/CVetor.h>
#endif

// Lista de classes da hierarquia de grafos
class CGrafoConexaoSerial_M1;
class CGrafoConexaoSerial_M2;
class CGrafoConexaoSerial_M3;
class CGrafoConexaoSerial_M4;
class CGrafoConexaoSerial_M5;
class CGrafoConexaoSerial_M6;
// // Lista de apelidos
// using CGrafoConexaoSerial_M1_CondutânciaPorPixel = CGrafoConexaoSerial_M1;
// using CGrafoConexaoSerial_M2_CondutânciaPelaAreaMedia = CGrafoConexaoSerial_M2;
// using CGrafoConexaoSerial_M3_CondutânciaPelaAreaInterseccao = CGrafoConexaoSerial_M3;
// using CGrafoConexaoSerial_M4_CondutânciaPelaAreaInterseccaoCorrigidaPelaDistancia = CGrafoConexaoSerial_M4;
// using CGrafoConexaoSerial_M6_CondutânciaPelaAreaInterseccaoCorrigidaPelaDistanciaCalculaTortuosidade = CGrafoConexaoSerial_M6;
// using CGrafoConexaoSerial_M6_Tortuosidade = CGrafoConexaoSerial_M6;

// ===============================================================================
// Documentacao da  @class CGrafoConexaoSerial
// ===============================================================================
/**
 * @brief Herdeira de CRede, gera a estrutura de sítios baseado na avaliação de cada plano
 * da imagem tridimensional (planos bidimensionais).
 * Cria os atributo: nomeArquivo,  vector objeto, rotuloPrimeiroObjetoPlano1, rotuloUltimoObjetoPlanoN_1;
 * Cria os atributos: ra (rotulador plano anterior), rp(rotulador plano atual), plano(z em análise),
 * img2D,  tipoObjeto(a ser criado), maiorRotuloUtilizado(maiorRotuloUtilizado_AnteriorPrimeiroRotuloDe_ra).
 * 
 * Go()->Determina o grafo varrendo a imagem.
 * AdicionarObjetos->Adiciona os objetos do plano atual, usando rotulador.
 * DeterminarConexoesObjetos->Estabelece as conexões (nesta classe os links são paralelos, para cada píxel uma conexão)
 * EliminarRamosMortos->Elimina os ramos mortos. Num ramo morto o fluxo é nulo.
 *
 * Nota: Observe que da forma como esta, sempre usa sítios (e nunca CObjetoRede_Ligacao):
 * plano z=0        -> CObjetoRede_Sitio_EST ; 
 * plano z < nz-1   -> CObjetoRede_Sitio (CENTER); 
 * plano z == nz-1; -> CObjetoRede_Sitio_WEST 
 *
 * Adiciona os métodos:
 * Go()->retorna objeto do grafo (de objetos CObjetoRede_Sitio ).
 * CalcularCondutâncias()->converte raioHidraulico em condutancia (somente CObjetoRede_Sitio ??).
 *
 * Lista dos herdeiros:
 * -CGrafoConexaoSerial_M1	Modelo_1 : Para cada pixel uma ligação;
 * Condutância do pixel.
 *
 * -CGrafoConexaoSerial_M2	Modelo_2 : Para cada intersecção entre objetos uma ligação;
 * Condutância média entre os dois objetos.
 *
 * -CGrafoConexaoSerial_M3	Modelo_3 : Para cada intersecção entre objetos uma ligação;
 * Condutância calculada sobre a area da intersecção (usa plano rotulagem adicional rotint).
 *
 * -CGrafoConexaoSerial_M4	Modelo_4 : Para cada intersecção entre objetos uma ligação;
 * Condutância calculada sobre a area da intersecção (usa plano rotulagem adicional rotint).
 * Adicionalmente calcula as distâncias entre os objetos, para correção das condutâncias.
 *
 * -CGrafoConexaoSerial_M5	Modelo_5 : O mesmo que M4, mas para melhorar o cálculo do fator de correção
 * das distâncias; elimina píxeis que não tem intersecção com planos anterior e posterior.
 * Abaixo píxeis marcados com # são eliminados
 * ----*************------****************------***************
 * ---#*************#-------**************##------**** ----****## <-- # eliminados
 * ----*************------****************------***************
 * O M5 não foi testado (ou foi pouco testado).
 * 
 * -CGrafoConexaoSerial_M6 : O mesmo que o M4, mas no M6 o centro de massa é armazenado
 * nos objetos; adiciona cálculo tortuosidade;
 *
 * CGraThining3D (classe não implementada): Gera a estrutura de sítios e ligações,
 * usando o esqueleto 3D de Ma.  Código desenvolvido pelo Liang Zhirong.
 * PS: Ainda não implementada usando POO-ANSIC++
 * PS: Existente no Imago usando COI-LIB

 *  -Uso:
 * A primeira utilização desta classe, e que deu origem a ela, é a determinação do
 * grafo de "sítios" representativos da imagem 3D, a partir de imagens bidimensionais.
 * Note que é uma rede de sítios, mas o cálculo da condutância usa equação de poiselle,
 * pois a mesma representa mais corretamente a condutância entre "anéis" 
 * entre planos paralelos.
 *
 * A saída pode ser obtida com a função Write, que salva em disco a estrutura de sítios
 * (sua propriedade, lista conexões e lista condutâncias).
 * O arquivo de saída inclui tipoDoGrafo, número objetos e depois dados dos objetos.
 *
 * Também é possível usar diretamente o grafo obtido para determinar propriedades de interesse,
 * sem a necessidade de salvar os dados em disco.
 * Este mecanismo é utilizado por CPermeabilidadeGrafo.

 * @author	André Duarte Bueno
 * @see		Grafo
 * @ingroup  HCGrafo
*/
class CGrafoConexaoSerial : public CRedeContorno {
   // --------------------------------------------------------------Atributos
private:
   /// Indica versão da função que elimina os ramos mortos;
   /// = 0;  não elimina ramos mortos; =1 usa versão 1 (lenta); =2 usa versão 2 que é mais rápida.
   int eliminaRamosMortos { 2 } ;
//    int eliminaConexoesRepetidas { 1 } ;	///< Elimina conexões repetidas

protected:
	/// Flag que informa se é para deletar conexões em paralelo e somar suas condutâncias;
	/// =0 pois modelo original não tinha isso; para ativar use DeletaConexoesParalelo(1);
	bool deletaConexoesParalelo {false};

   // Dimensoes da imagem tridimensional @todo: eliminar nx ny nz, usar apenas dentro de Go()
   int nx { 0 } ; ///< Dimensão nx da imagem tridimensional
   int ny { 0 } ; ///< Dimensão ny da imagem tridimensional
   int nz { 0 } ; ///< Dimensão nz da imagem tridimensional

   /**
     * @brief Informa o plano que esta sendo avaliado.
     * É passado para o objeto, fazendo-se objeto->x=plano,
     * desta forma a previsão inicial do valor de x poderá considerar o plano.
   */
   unsigned long int plano { 0 };

   // O primeiro e último plano tem propriedades fixas(pressão constante), sendo assim,
   // não precisam ser calculados.
   // O objetivo de se criar os atributos abaixo é eliminar a chamada do cálculo das propriedades
   // nos objetos destes planos.
   // firstObjectOf Solver=rotuloPrimeiroObjetoPlano1;
   // lastObjectOf Solver=rotuloUltimoObjetoPlanoN_1
   /// Rótulo do primeiro objeto do plano z=1 (logo após o plano z=0)
   /// @todo: Pensar em criar um vector<int> indicePrimeiroObjetoPlano;
   /// sendo indicePrimeiroObjetoPlano[i] o rótulo do primeiro objeto do plano.
   unsigned int rotuloPrimeiroObjetoPlano1 { 0 };

   /// Rótulo do último objeto do plano z=n-1 (imediatamente antes do plano z=n)
   unsigned int rotuloUltimoObjetoPlanoN_1 { 0 };

   /**
     * @brief Valor do maior rotulo já utilizado.
     * Em cada plano, os rótulos iniciam em 0 (o fundo), 1 o primeiro objeto,...n
     * A variável maior RotuloUtilizado vai acumulando o número de objetos do grafo.
   */
   unsigned long int maiorRotuloUtilizado { 0 };

   /// Rotulador bidimensional para imagem anterior
   CRotulador2DCm* ra { nullptr };

   /// Rotulador bidimensional para imagem posterior
   CRotulador2DCm* rp { nullptr };

   /// Imagem usada internamente para copiar planos ra, rp e plano intermediário
   TCMatriz2D< int >* img2D { nullptr };

// Por default o objeto esta no centro do grafo e não em seus contornos
//  	CContorno::ETipoContorno tipoContornoObjeto = CContorno::ETipoContorno::CENTER;

   /*Usado para definir o maior rótulo do plano anterior a ra.
     Ex: Se ra é o plano 12, rp é o plano 13,
     maiorRotuloUtilizado =  soma de todos os rótulos dos planos 0->11.*/

public:
   // -------------------------------------------------------------Construtor
   /// Construtor, recebe um nome de arquivo;
   /// Só constroi explicitamente, chamando CGrafoConexaoSerial{string};
   explicit CGrafoConexaoSerial ( const std::string& _nomeArquivo )
      : CRedeContorno ( _nomeArquivo ) {
      tipoGrafo  =  ETipoGrafo::GrafoConexaoSerial ;
   }

   // --------------------------------------------------------------Destrutor
   /// Destrutor, deleta objetos internos
   virtual ~ CGrafoConexaoSerial() {
      if ( ra )
         delete ra; 			// deleta objeto

      if ( rp )
         delete rp; 			// deleta objeto

      if ( img2D )
         delete img2D; 		// deleta objeto

   }

   // ----------------------------------------------------------------Métodos
   /// Salva grafo em disco (redefinida).
   void Write() const {
      CBaseGrafoRedeEsqueleto::Write();
   }

   /// Salva grafo em disco.
   virtual void Write ( std::ostream& out ) const override {
      CRede:: Write ( out );
   }

   /**
   * @brief 	Função Go, realiza a determinação da rede.
   * Recebe imagem 3D e _tamanhoMascara.
   *
   * Neste modelo, a informação da propriedade de ligação (o raio hidraulico,
   * ou resistência/condutância) é definida e armazenada pela propriedade do "sítio"
   * e não da ligação.
   */
   virtual CRede* Go ( TCMatriz3D<int>* _img3D, unsigned long int _tamanhoMascara = 1 ) /*override*/;

   /**
     * @brief 	Função Go, realiza a determinação da rede.
    * Recebe nome arquivo com a imagem 3D e _tamanhoMascara.
   */
   virtual CRede* Go ( std::string nomeArquivo, unsigned long int _tamanhoMascara = 0 ) /*override*/;

   /**
     * @brief Elimina as conexões em paralelo e acumula as condutâncias.
   */
   virtual void EliminarConexoesParalelo_SomarCondutancias();

   /**
    * @brief  Para o conjunto de objetos da rede, transforma uma propriedade em outra (ex: raio Hidraulico em condutancia).
    * Tem mais de uma herdeira. Ou seja a conversão raioHidraulico->condutancia  é feita de diferentes formas.
   * Note que CalcularCondutâncias muda de acordo com o problema em questão;
   * aqui, o problema é a condutância de objetos relacionados a imagens,
   * então recebe parâmetros da imagem e do fluido.
   * Em outras hierarquias, outros tipos de grafos, CalcularCondutâncias receberá
   * outros parâmetros.
   */
   virtual void CalcularCondutancias ( long double _viscosidade, long double _dimensaoPixel,
                                       unsigned long int _fatorAmplificacao )  = 0;

   /// Seta a matriz A e o vetor B, a serem utilizados por um solver externo (ex: gmres)
   virtual bool SetarMatrizAVetorB ( TCMatriz2D< int >*& A, CVetor*& B ) const;

   /**
    * @brief No caso de queda de energia, foi projetado um sistema de reconstrução do grafo();
    * A primeira etapa é o recálculo de todo o grafo a partir da imagem (é rápido);
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
     * o valor do maior rotulo utilizado, e o tipo de objeto a ser criado.
     */
   virtual void AdicionarObjetos ( CRotulador2DCm* rotulador,
                                   unsigned long int ultimoRotuloUtilizado, ETipoObjetoGrafo tipoObjeto );
//    virtual void AdicionarObjetos ( CRotulador2DCm* rotulador, unsigned long int rotuloAtual,
//                                    ETipoGrafo tipoObjeto );

   /// Função que conecta objetos em planos adjacentes.
   virtual void DeterminarConexoesObjetos ( unsigned long int maiorRotuloUtilizado ) = 0;

   /// Função Usada para calcular a condutancia das ligações
   // virtual void CalcularPropriedadesConeccoes() {};

   /// Função que elimina ramos mortos (com 0 ou 1 conexão); não contam para fluxo.
   void EliminarRamosMortos();

   /**
     * @brief Reorganiza os links para cmx e cmy.
     * Funcao nova, como alguns ramos estão sendo deletados eu chamo esta função para reorganizar
     * os centros de massa respeitando o novo estado do grafo.
     * Necessaria para o modelo 4, por isto é definida aqui como sendo vazia.
   */
   virtual void ReorganizarCmxCmy()	{	}

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
     * efetivamente por EliminarRamosMortos_0_ou_1_conexao_v2
   */
   virtual bool MarcarParaDelecaoObjeto ( int i );

   // virtual bool DeletarObjeto(CObjetoRede* sitio) ;

private:
   /// Elimina objetos redundantes, versão 1
   void EliminarRamosMortos_0_ou_1_conexao_v1();

   /// Elimina objetos redundantes, versão 2
   void EliminarRamosMortos_0_ou_1_conexao_v2();

   // --------------------------------------------------------------------Get
public:
   /// Retorna nx.
   int Nx()    {
      return nx;
   }

   /// Retorna ny.
   int Ny()    {
      return ny;
   }

   /// Retorna nz.
   int Nz()    {
      return nz;
   }

   /// Retorna o número que informa o método que irá eliminar os ramos mortos.
   int EliminaRamosMortos() {
      return eliminaRamosMortos;
   }

//    /// Elimina conexões repetidas
//    int EliminaConexoesRepetidas() {
//       return eliminaConexoesRepetidas;
//    }

	/// Retorna deletaConexoesParalelo, =0 default, não deleta conexões em paralelo (+ lento)
	bool DeletaConexoesParalelo() { return deletaConexoesParalelo; };

   // --------------------------------------------------------------------Set
   /// Seta elimina ramos mortos
   void EliminaRamosMortos ( int _e ) {
      eliminaRamosMortos = _e;
   }

//    /// Seta conexões repetidas
//    void EliminaConexoesRepetidas ( int _r ) {
//       eliminaConexoesRepetidas = _r;
//    }
	/// @todo, testar ativação deste flag e resultados para modelo 1.
	/// Seta deletaConexoesParalelo;  =1 indica para deletar conexões em paralelo (+ rápido)
	void DeletaConexoesParalelo(bool b) { deletaConexoesParalelo = b; };

   // -----------------------------------------------------------------Friend
   //       friend ostream& operator<< (ostream& os, CGrafoConexaoSerial& obj);
   //       friend istream& operator>> (istream& is, CGrafoConexaoSerial& obj); //  Public attributes

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGrafoConexaoSerial& obj);
// istream& operator>> (istream& is, CGrafoConexaoSerial& obj);
#endif
