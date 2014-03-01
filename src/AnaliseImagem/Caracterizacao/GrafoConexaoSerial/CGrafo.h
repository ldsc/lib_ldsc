#ifndef CGrafo_h
#define CGrafo_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CGrafo.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <vector>
#include <cassert> /// @todo: usar static_assert de C++11!

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------

#include <Base/_LIB_LDSC_CLASS.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>
#include <AnaliseImagem/Matriz/TCMatriz3D.h>
#include <AnaliseImagem/Matriz/TCMatriz2D.h>
#include <AnaliseImagem/Matriz/CVetor.h>
#include <MetNum/Contorno/CContorno.h>


// ===============================================================================
// Documentacao CLASSE: CGrafo
// ===============================================================================
/**
 * @brief Um CGrafo é uma representação para uma estrutura de dados.
 * Um CGrafo é composto de uma lista de objetos do tipo CObjetoGrafo.
 * A forma como os objetos se relacionam é definida, normalmente, pelo próprio CObjetoGrafo.
 * Assim, existe uma hierarquia de grafos cujo pai é CGrafo e 
 * uma hierarquia de objetos de grafo cujo pai e CObjetoGrafo.
 * 
 * Exemplo:
 * Determinação do grafo de uma imagem 3D:
 * A primeira utilização desta classe, e que deu origem a ela, é a determinação do 
 * grafo de "sítios" representativos da imagem 3D, a partir de imagens bidimensionais.
 * A classe que implementa, de fato, a determinação do grafo 3D a partir de seções 2D é a
 * CGra3Dby2D (que tem diversas filhas).
 * Para obter o Grafo dos "sítios", basta passar uma imagem_3D TCMatriz3D<int> * para a função Go; 
 * A função Go trabalha sobre os planos bidimensionais, para realizar a classificação dos sítios 
 * e estabelecer as conexoes entre eles.
 * 
 * Uso:
 * A saída pode ser obtida com a função Write, 
 * que salva em disco a estrutura de sítios determinada e os links entre os sítios.
 * Também é possível usar diretamente o grafo obtido para determinar propriedades de interesse, 
 * sem a necessidade de salvar os dados em disco.
 * Este mecanismo é utilizado por CPermeabilidadeGrafo.
 * 
 * Nota:
 * Na prática o Grafo armazena objetos que tem uma propriedade, sendo uma espécie de rede.
 * Note ainda que se o objeto tiver a informação do centro de massa COSitioCM então temos
 * uma espécie de esqueleto, ou seja, objetos com propriedades conectados e 
 * com informação das coordenadas/posição.
 * 
 * @todo: Reformular a hierarquia de objetos do grafo e a hierarquia de grafos de forma
 * a representar mais corretamente os conceitos teóricos.  
 *  
 * RESUMO DAS CLASSES
 * CGrafo
 * ------------------------------------
 * Objetivo específico:	Dada uma imagem 3D, gerar uma estrutura de sítios e ligações equivalente.
 * 
 * Cria os atributo: nomeArquivo, vector objeto, firstObjectOfSolver, lastObjectOfSolver;
 * CriarObjetoGrafo->retorna objeto do grafo (CSitio)
 * CalcularCondutancias->converte raioHidraulico em condutancia (somente CSitio).
 *
 * CGraThining3D (classe não implementada)
 * ------------------------------------
 * Gera a estrutura de sítios e ligações, usando o esqueleto 3D de Ma.
 * Código desenvolvido pelo Liang Zhirong.
 * PS: Ainda não implementada usando POO-ANSIC++
 * PS: Existente no Imago usando COI-LIB
 *
 * CGrafoContorno
 * ------------------------------------
 * Adiciona vetor de contornos (usados para identificar condições de contorno).
 * De um modo geral vai ter dois objetos contornos,
 * um para face esquerda outro para face direita.
 *
 * CGra3Dby2D
 * ------------------------------------
 * Gera a estrutura de "sítios" baseado na avaliação de cada plano 
 * bidimensional da imagem tridimensional. A classe CGra3Dby2D tem herdeiras.
 * 
 * Acrescenta os atributos: ra (rotulador imagem anterior), rp, img2D, 
 * plano, tipoContornoObjeto, maiorRotuloUtilizado.
 * Go()->Determina o grafo varrendo a imagem.
 * AdicionarObjetos->Adiciona os objetos do plano atual, usando rotulador.
 * DeterminarConeccoesObjetos->Estabelece as coneccoes (aqui os links são repetidos para cada coneccao entre pixeis)
 * EliminarConeccoesObjetos->Elimina os ramos mortos. Num ramo morto o fluxo é nulo.
 *
 * CGra3Dby2_M1 (cada píxel uma conexão)
 * ------------------------------------
 * Na função Go :
 * Os planos i e i+1 são rotulados, gerando-se a seguir os sítios.
 * Depois estabelece-se as conecções entre os planos consecutivos.
 * Observe que duas regiões que se interceptam vão gerar um número
 * grande de conecções (com conecções redundantes).
 * A desvantagem do modelo 1 é gerar um número muito grande de conecções, uma para cada pixel conexo.
 * A vantagem é que o mesmo não usa um plano auxiliar.
 * Todo: eliminar número de conexões redundantes calculando condutância efetiva.
 * Todo: criar classe herdeira de M1, chamada M8, deve calcular a idf em 3D,
 * e usar mesmo procedimento de M1, mas agora considerando como "peso" o valor da idf.
 *
 * CGra3Dby2_M2 (condutância pela condutância média dos objetos conectados)
 * ------------------------------------
 * Na função Go :
 * Os planos i e i+1 são rotulados, gerando-se a seguir os sítios.
 * Depois gera-se um objeto de rotulagem com o plano intermediário
 * (da conexão dos planos i com i+1) e um vetor de link's válidos.
 * Verifica os objetos no plano i e no plano i+1, se estiverem conexos 
 * estabelece a conexão nos dois sentidos. Para evitar a repetição da 
 * mesma conecção, seta o vetor de links para aquele rótulo como sendo inválido.
 * Desta forma as conexões são corretamente estabelecidas, e elimina-se
 * as conecções redundantes.
 * Note que CGra3Dby2_M2 assume uma condutancia media.
 * 
 * Neste modelo, a informação da propriedade de ligação (o raio hidraulico,
 * ou resistência/condutância) é definida e armazenada pela propriedade do "sítio"
 * e não da ligação.
 * Acrescenta o atributo rotint (rotulador intermediário).
 * 
 * Go()->Cria rotint(rotulador intermediário), chama CGra3Dby2D::Go, deleta rotint
 * DeterminarConeccoesObjetos()->Estabelece as coneccoes, usa rotint para
 * eliminar repeticoes dos links.
 * 
 * CGra3Dby2_M3 (condutância pela área intersecção)
 * ------------------------------------
 * Neste modelo adiciona-se uma lista de condutancias, associando a cada
 * coneccao uma condutancia proporcional a area de intersecao entre os objetos.
 * Enquanto no CGra3Dby2_M2 assume-se uma condutancia media, 
 * aqui a condutancia é função do raio hidraulico da intersecção dos objetos.
 * 
 * DeterminarConeccoesObjetos()->Conecta os objetos em planos distintos,
 * determina os raioshidraulicos dos "sítios",
 * determina e armazena os raioHidraulicos das ligações(novo).
 * CriarObjetoGrafo()->retorna COGSitio_CC.
 * CalcularCondutancias()->determina as condutâncias das ligações.
 * 
 * CGra3Dby2_M4 (corrige condutâncias considerando distâncias centros massa)
 * ------------------------------------
 * Neste modelo, herdeiro de M3, adiciona-se uma lista com os centros de massa cmx e cmy.
 * Na função DeterminaConeccoes(), armazena-se os centros de massas de cada objeto.
 * As informações dos centros de massa são utilizadas na função CalcularCondutancias()
 * para correção das condutancias levando em conta as distâncias.
 * Os centros de massa tambem podem ser usados para gerar visualizações do grafo, 
 * para cálculo da tortuosidade, e distâncias entre objetos.
 * 
 * ->CalcularCondutancias() = Corrige as condutancias considerando os centros de massa.
 * ->AdicionarObjetos() = Chama AdicionarObjetos da classe base e depois marca os centros de massa.
 * ->ReorganizarCmxCmy() = Depois de eliminarRamosMortos, precisa reorganizar os centros de massa.
 * Nota: Pensar em mover atributos centro massa para objetos(verificar modelo 6).
 *
 * CGra3Dby2_M6 (informação dos centros de massa armazenadas nos objetos)
 * ------------------------------------
 * Faz exatamente a mesma coisa que o modelo 4.
 * A diferença é que não cria na classe grafo os vetores cmx e cmy.
 * As informações dos centros de massa são armazenadas nos próprios sítios;
 * assim criou-se os objetos COGSitio_CC_CM (sitio com link para resistencia e centro de massa).
 * 
 * ->CriarObjetoGrafo() = Retorna COGSitio_CC_CM.
 * ->AdicionarObjetos() = Reescreve totalmente a Adcionar objetos, armazena as informações 
 * do COGSitio_CC_CM o que inclue os centros de massa.
 * ->CalcularCondutancias()= Corrige as condutancias considerando os centros de massa.
 * Nota: conferir códigos e se a saída é a mesma do Modelo 4.
 * @author André Duarte Bueno
 * @see    grafos
*/
class  CGrafo
{
  // --------------------------------------------------------------Atributos
  private:
  /// Nome do arquivo de disco (nome do arquivo do grafo).
  std::string nomeArquivo{""};

  protected:
  /// Enumeração para os diferentes tipos de grafo.
  // Note que desta forma tem de incluir aqui informações das classes derivadas,
  // sempre que criar classe herdeira tem de colocar aqui a enumeração correspondente.
  enum class ETipoGrafo : unsigned char {
      grafo, grafoContorno, grafo3DBy2D, 
	  grafo3DBy2D_M1, grafo3DBy2D_M2, grafo3DBy2D_M3, grafo3DBy2D_M4, grafo3DBy2D_M5, grafo3DBy2D_M6 
  };
	  
  /// Identifica o tipo de grafo, deve ser definido no construtor.
  ETipoGrafo tipoGrafo { ETipoGrafo::grafo } ;

  // O primeiro e último plano tem propriedades fixas(pressão constante), sendo assim, 
  // não precisam ser calculados.
  // O objetivo de se criar os atributos abaixo é eliminar a chamada do calculo das propriedades
  // nos objetos destes planos.
  /// @todo: Pensar em criar um vector<int> indicePrimeiroObjetoPlano;
  /// sendo indicePrimeiroObjetoPlano[i] o rótulo do primeiro objeto do plano.
  // firstObjectOfSolver=indicePrimeiroObjetoPlano[1];
  // lastObjectOfSolver=indicePrimeiroObjetoPlano[size() - 1];

  /// Rótulo do primeiro objeto do plano z=1 (logo após o plano z=0)
	/// @todo: renomear firstObjectOfSolver -> primeiroObjetoPlano$1
  unsigned int firstObjectOfSolver {0};

  /// Rótulo do último objeto do plano z=n-1 (imediatamente antes do plano z=n)
	/// @todo: renomear lastObjectOfSolver -> ultimoObjetoPlano$n_1
  unsigned int lastObjectOfSolver {0};
  
  public:
  /// Usa-se objeto[i] para obter ponteiro para o objeto i do grafo
  /// @todo: trocar por unique_ptr shared_ptr?
  std::vector<CObjetoGrafo*> objeto ;

  // -------------------------------------------------------------Construtor
  /// Constroi o grafo, recebe um nome de arquivo de disco.
  CGrafo(std::string _nomeArquivo) : nomeArquivo ( _nomeArquivo) { }

  // --------------------------------------------------------------Destrutor
  /// Destroi o objeto, elimina os objetos do grafo.
  virtual ~CGrafo()  {
  // O grafo é o proprietário dos sítios e deve eliminá-los.
  for ( auto elemento :  objeto )
    delete elemento;
  }
  
  // ----------------------------------------------------------------Métodos
  protected:
  /**
   * @brief Função usada para criar os objetos do grafo.
   * Recebe um CContorno::ETipoContorno que informa o tipo de objeto a ser criado.
   * Retorna um objeto herdeiro de CObjetoGrafo, de acordo com o ETipoContorno.
   * Note que como temos diferentes modelos de rede e diferentes tipos de objetos,
   * CriarObjetoGrafo é reescrita nas classes derivadas, de forma a criar os objetos
   * de acordo com o modelo.
   * 
   * @todo: esta sendo sobrescrita nas classes herdeiras,
   * para juntar tudo numa função única, precisa criar ETipoObjetoGrafo
   * e então juntar tudo numa função  estática única (ver livro padrões projeto).
   * Note que terá de ser movida para dentro hierarquia de objetos do grafo.
   * NomePadrão: CObjetoGrafo::CriarObjeto -> criar objeto da hierarquia grafo.
  */
  virtual CObjetoGrafo* CriarObjetoGrafo(CContorno::ETipoContorno tipoContorno);

    // ///Deleta um objeto do grafo
    // Deleta consideranto a posição no vetor.
    // virtual bool DeletarObjeto(int ri) = 0;
    // Deleta consideranto o endereço do objeto.
    // virtual bool DeletarObjeto(CObjetoGrafo* sitio) = 0;

  public:
    /// Seta a matriz A e o vetor B, a serem utilizados por um solver externo
    virtual bool SetarMatrizAVetorB (TCMatriz2D< int >* &A, CVetor*& B) const;
    // 	bool SetarMatrizAVetorB (TCMatriz2D< int >* &A, CVetor*& B)const ;	// GMRES

    /**
     * @brief Movida de CPermeabilidadeGrafo para cá.
     * Transforma uma propriedade raio Hidraulico em condutancia.
     * Tem mais de uma herdeira.
     * Todo: Mover de volta para calculoPermeabilidade?
    */
    virtual void CalcularCondutancias(long double _viscosidade, long double _sizePixel, 
				     unsigned long int _fatorAmplificacao);
    /**
     * @brief Função que recebe uma imagem 3D e gera a lista de objetos e seus relacionamentos.
    */
    virtual CGrafo* Go( TCMatriz3D<int> * _img3D ,unsigned long int _tamanhoMascara = 0) = 0;

    /** 
     * @brief Função que recebe o nome do arquivo de uma imagem 3D, carrega imagem do disco
     * e gera a lista de objetos e seus relacionamentos.
     * Nas classes derivadas a função abaixo deve ser reescrita, o objetivo e eliminar a leitura 
     * de toda a imagem tridimensional, e ir realizando a determinacao do grafo com a leitura dos planos.
    */
    virtual CGrafo* Go( std::string nomeArquivo, unsigned long int _tamanhoMascara = 0) {
      TCMatriz3D<int> * img3D = new TCMatriz3D<int> (nomeArquivo);
      assert(img3D);
//       img3D->Constante(0); // bug?? Verificar??
      return Go(img3D, _tamanhoMascara);
    }

    /**
     * @brief Salva o grafo e seus objetos em disco.
     * Salva a informação do número de objetos e os dados de cada objeto em disco
     * chamando Write de cada objeto.    */
    virtual void Write(std::string nomeArquivo);

    /**
     * @brief No caso de queda de energia, foi projetado um sistema de reconstrução do grafo(); 
     * A primeira etapa é o recalculo de todo o grafo a partir da imagem (é rápido); 
     * Em seguida, faz a leitura de um arquivo de disco que armazena as propriedades x (pressões)
     * de cada objeto do grafo. Esta função lê os valores de x de cada objeto do grafo. */
    bool LerVetorPropriedades_x();

    /// Salva propriedades dos objetos em disco (permite uso LerVetorPropriedades_x()).
    bool SalvarVetorPropriedades_x();

    // --------------------------------------------------------------------Get
    /// Retorna o nome do arquivo de disco
    std::string FileName() const { return nomeArquivo; }

    /// Retorna o tipo de grafo
    ETipoGrafo TipoGrafo( ) { return tipoGrafo; }

    /// Retorna o tipo do grafo como uma string, útil para gerar nomes arquivos saída.
    std::string TipoGrafoString( )
	{ switch( tipoGrafo )
	  {
		case ETipoGrafo::grafo          : return {"grafo"};
		case ETipoGrafo::grafoContorno  : return {"grafoContorno"};
		case ETipoGrafo::grafo3DBy2D    : return {"grafo3DBy2D"};
		case ETipoGrafo::grafo3DBy2D_M1 : return {"grafo3DBy2D_M1"};
		case ETipoGrafo::grafo3DBy2D_M2 : return {"grafo3DBy2D_M2"};
		case ETipoGrafo::grafo3DBy2D_M3 : return {"grafo3DBy2D_M3"};
		case ETipoGrafo::grafo3DBy2D_M4 : return {"grafo3DBy2D_M4"}; 
		case ETipoGrafo::grafo3DBy2D_M5 : return {"grafo3DBy2D_M5"};
		case ETipoGrafo::grafo3DBy2D_M6 : return {"grafo3DBy2D_M6"};
	  }
	}

    // --------------------------------------------------------------------Set
    // -----------------------------------------------------------------Friend
    /// Escreve em "os" os dados do objeto grafo e seus agregados
    friend std::ostream& operator<< (std::ostream& os, const CGrafo& obj);

    // friend istream& operator>> (istream& is, CGrafo& obj);
};

std::ostream& operator<< (std::ostream& os, const CGrafo& obj);
// istream& operator>> (istream& is, CGrafo& obj);
#endif
