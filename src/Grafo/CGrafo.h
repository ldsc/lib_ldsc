#ifndef CGrafo_h
#define CGrafo_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CGrafo...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     André Duarte Bueno
  @file       CGrafo.h
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by André Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <vector>
#include <cassert>

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif	

#ifndef CObjetoGrafo_h
#include <Grafo/CObjetoGrafo.h>
#endif	

#ifndef CMatriz3D_h
#include <Matriz/CMatriz3D.h>
#endif	

#ifndef TCMatriz2D_H
#include <Matriz/TCMatriz2D.h>
#endif	

#ifndef CVetor_h
#include <Matriz/CVetor.h>
#endif	

#ifndef CContorno_h
#include <Contorno/CContorno.h>
#endif	

// ===============================================================================
// Documentacao CLASSE: CGrafo
// ===============================================================================
/** 
 * @brief Um CGrafo é uma representação para uma estrutura de dados.
 * Um CGrafo é composto de uma lista de objetos do tipo CObjetoGrafo.
 * A forma como os objetos se relacionam é definida  normalmente pelo próprio CObjetoGrafo.
 * Assim, existe uma hierarquia de grafos cujo pai é CGrafo e uma hierarquia de objetos de grafo cujo pai e CObjetoGrafo.
 * 
 * Exemplo:
 * Determinação do grafo de uma imagem 3D:
 * A primeira utilização desta classe, e que deu origem a éla é a determinação do 
 * grafo de sítios representativos da imagem 3D,a partir de imagens bidimensionais. 
 * A classe que implementa de fato a determinação do grafo 3D a partir de seções 2D é a CGra3Dby2D (que tem diversas filhas).
 * Para obter o Grafo  dos sítios, basta passar uma imagem_3D CMatriz3D* para a função Go, que trabalha sobre os planos 
 * bidimensionais, para realizar a classificação dos sítios e estabelecer as conexoes entre eles.
 * 
 * Uso:
 * A saída pode ser obtida com a função Write, que salva em disco a estrutura de sítios determinada e os links entre os sítios.
 * 
 * Pode-se usar diretamente o grafo obtido para determinar propriedades de interesse, sem a necessidade de se salvar os dados em disco. 
 * Este mecanismo é utilizado por CPermeabilidadeGrafo.   
 * 
 * RESUMO DAS CLASSES
 * CGrafo
 * ------
 * Objetivo específico:	Dada uma imagem 3D, gerar uma estrutura de sítios e ligações equivalente.
 * 
 * Cria fileName e vector objeto.
 * firstObjectOfSolver, lastObjectOfSolver;
 * 
 * --
 * GetObjetoGrafo->retorna objeto do grafo (CSitio)
 * CalculoCondutancias->converte raioHidraulico em condutancia (somente CSitio)
 * CGraThining3D (não implementado)
 * ---------------------------------
 * Gera a estrutura de sítios e ligações, usando o esqueleto 3D de Ma.
 * Código desenvolvido pelo LIANG.
 * 
 * PS: Ainda não implementada usando POO-ANSIC++
 * PS: Existente no Imago usando COI-LIB
 *
 * CGrafoContorno
 * -- ------------------
 * Adiciona vetor de contornos.
 * CGra3Dby2D
 * ------------------
 * Gera a estrutura de sítios e ligações baseado na avaliação de cada plano 
 * bidimensional da imagem tridimensional. Vai ter mais de uma herdeira.
 * 
 * Acrescenta: ra,rp,img2D,plano, tipoContornoObjeto, maiorRotuloUtilizado.
 * --
 * Go->Determina o grafo varrendo a imagem
 * AdicionarObjetos->Adiciona os objetos do plano atual, usando rotulador
 * DeterminarConeccoesObjetos->Estabelece as coneccoes links repetidos
 * EliminarConeccoesObjetos->Elimina os ramos mortos
 * 
 * CGra3Dby2_M1
 * -----------------------
 * Na função Go :
 * Os planos i e i+1 são rotulados, gerando-se a seguir os sítios.
 * Depois estabelece-se as conecções entre os planos consecutivos.
 * Observe que duas regiões que se interceptam vão gerar um número
 * grande de conecções (com conecções redundantes).
 *
 * CGra3Dby2_M2
 * ------------------------------
 * Na função Go :
 * Os planos i e i+1 são rotulados, gerando-se a seguir os sítios.
 * Depois gera-se um objeto de rotulagem com o plano intermediário
 * (da conexão dos planos i com i+1) e um vetor de link's válidos.
 * Verifica os objetos no plano i e no plano i+1 se estiverem conexos 
 * estabelece a conexão nos dois sentidos. Para evitar a repetição da 
 * mesma conecção, seta o vetor de links para aquele rótulo como sendo inválido.
 * Desta forma as conexões são corretamente estabelecidas, e elimina-se
 * as conecções redundantes.
 * 
 * Neste modelo, a informação da propriedade de ligação (o raio hidraulico,
 * ou resistência) é definida e armazenada pelo raioHidraulico do sítio 
 * e não da ligação.
 * 
 * Acrescenta rotint
 * --
 * Go->Cria rotint chama CGra3Dby2D::Go, deleta rotint		
 * DeterminarConeccoesObjetos->Estabelece as coneccoes usa rotint para
 * eliminar repeticoes dos links
 * 
 * CGra3Dby2_M3
 * --------------------------
 * Neste modelo adiciona-se uma lista de condutancias, associando a cada
 * coneccao uma condutancia proporcional a area de intersecao entre os objetos.
 * Enquanto no CGra3Dby2_M2 assume-se uma condutancia media, aqui
 * a condutancia é função da área de intersecção dos objetos.
 * --
 * DeterminarConeccoesObjetos->Conecta os objetos em planos distintos,
 * determina os raioshidraulicos dos sítios
 * Determina e armazena os raioHidraulicos das ligações(novo).
 * GetObjetoGrafo->retorna COGSitioLR
 * CalculoCondutancias->determina as condutâncias das ligações
 * 
 * CGra3Dby2_M4
 * --------------------------
 * Neste modelo, herdeiro de Adv3, adiciona-se uma lista com os centros de massa cmx e cmy.
 * Na função DeterminaConeccoes, armazena-se os centros de massas de cada objeto.
 * As informações dos centros de massa são utilizadas na função CalcularCondutancias
 * para correção das condutancias levando em conta as distâncias.
 * 
 * ->CalculoCondutancias  = Corrige as condutancias considerando os centros de massa
 * ->AdicionarObjetos     = Chama Adcionar objetos da classe base e depois marca os centros de massa
 * ->ReorganizarCmxCmy    = Depois de eliminarRamosMortos, precisa reorganizar os centros de massa
 * 
 * CGra3Dby2_M6
 * --------------------------
 * Faz exatamente a mesma coisa que o modelo 4.
 * A diferença é que não cria na classe grafo os vetores cmx e cmy.
 * As informações dos centros de massa são armazenadas nos próprios sítios,
 * assim criou-se os objetos COGSitioLRCM (sitio com link para resistencia e centro de massa).
 * 
 * ->GetObjetoGrafo		= Retorna COGSitioLRCm
 * ->AdicionarObjetos   	= Reescreve totalmente a Adcionar objetos, armazena as informações do COGSitioLRCM o que inclue os centros de massa.
 * ->CalculoCondutancias	= Corrige as condutancias considerando os centros de massa
 *
 * @author 	André Duarte Bueno	
 * @see		grafos
*/
class  CGrafo
{
  // --------------------------------------------------------------Atributos
  private:	
  /// Nome do arquivo de disco.
  std::string fileName;

  protected:
  // Rotulo do primeiro e ultimo objetos utilizados pelo solver
  // O uso dos atributos abaixo tem objetivos de desempenho (apenas)
  // Criam uma sobrecarga no entendimento da solução do sistema.
  // Pensar em criar, aqui ou na classe derivada, um vetor com o índice
  // do primeiro objeto do plano.
  // vector<int> indicePrimeiroObjetoPlano;
  // firstObjectOfSolver=indicePrimeiroObjetoPlano[1];
  // lastObjectOfSolver=indicePrimeiroObjetoPlano[size() - 1];
	
  /// Rótulo do primeiro objeto do plano z=1 (logo após o plano z=0)
  unsigned int firstObjectOfSolver;
	
  /// Rótulo do último objeto do plano z=n (imediatamente antes do plano z=n)
  unsigned int lastObjectOfSolver;
 	
  public:
  /// Usa-se objeto[i] para obter ponteiro para o objeto i do grafo
  std::vector<CObjetoGrafo*> objeto;

  // -------------------------------------------------------------Construtor
  /// Contróe o objeto, recebe um nome de arquivo
  CGrafo(std::string _fileName) : fileName ( _fileName) { }

  // --------------------------------------------------------------Destrutor
  /// Destróe o objeto, destróe objetos do grafo
  virtual ~CGrafo()
  {
	// O grafo é o proprietário dos sítios e deve eliminá-los
	// Deleta cada sítio criado
	for (unsigned long int i = 0; i < objeto.size(); i++)
	  delete objeto[i];
  }
  // ----------------------------------------------------------------Métodos
  protected:
  /**
   * @brief Função usada para criar os objetos do grafo. 
   * Recebe um CContorno::ETipoContorno que informa o tipo de objeto a ser criado.
   * Retorna um objeto herdeiro de CObjetoGrafo, de acordo com o ETipoContorno
  */
  virtual CObjetoGrafo* GetObjetoGrafo(CContorno::ETipoContorno tipoContorno);

    // ///Deleta um objeto do grafo
    // virtual bool DeletarObjeto(int ri) = 0;
    // virtual bool DeletarObjeto(CObjetoGrafo* sitio) = 0;
  
  public:
    /// Seta a matriz A e o vetor B, a serem utilizados por um solver externo
    virtual bool SetMatrizAVetorB (TCMatriz2D< int >* &A, CVetor*& B)const ;
    // 	bool SetMatrizAVetorB (TCMatriz2D< int >* &A, CVetor*& B)const ;	// GMRES
	
    /// Retorna o nome do arquivo de disco
    const std::string FileName() const {return fileName;}
	
    /**
     * @brief Movida de CPermeabilidadeGrafo para cá.
     * Transforma uma propriedade raio Hidraulico em condutancia.
     * Tem mais de uma herdeira.
    */
    virtual void CalculoCondutancias(long double _viscosidade, long double _sizePixel, unsigned long int fatorAmplificacao);
	
    /**
     * @brief Função que recebe uma imagem 3D e gera a lista 
     * de objetos e seus relacionamentos
    */
    virtual CGrafo* Go( CMatriz3D * _img3D ,unsigned long int _tamanhoMascara = 0) = 0;

    /** 
     * @brief Função que recebe o nome do arquivo de uma  
     * imagem 3D e gera a lista de objetos e seus relacionamentos.
     * Nas classes derivadas a função abaixo deve ser reescrita, o objetivo
     * e eliminar a leitura de toda a imagem tridimensional.
     * e ir realizando a determinacao do grafo com a leitura dos planos.
    */
    virtual CGrafo* Go( std::string fileName,
         unsigned long int _tamanhoMascara = 0) {
      CMatriz3D * img3D = new CMatriz3D (fileName);
      assert(img3D);
      img3D->Constante(0);
      return Go(img3D, _tamanhoMascara);
    }

    /**
     * @brief Salva o grafo e seus objetos em disco.
     * Salva a informação do número de objetos e os dados de cada objeto em disco
     * chamando Write de cada objeto.
    */
    virtual void Write(std::string fileName);

    /**
     * @brief No caso de queda de energia, foi projetado um sistema de 
     * reconstrução do grafo e leitura de um arquivo de disco 
     * que armazena as propriedades x de cada objeto do grafo.
     * Esta função lê os valores de x de cada objeto do grafo.
    */
    bool LeVetorPropriedades();

    /// Salva propriedades em disco
    bool SalvaVetorPropriedades();

    // --------------------------------------------------------------------Get
    // --------------------------------------------------------------------Set
    // -----------------------------------------------------------------Friend
    /// Escreve em os os dados do objeto grafo e seus agregados
    friend std::ostream& operator<< (std::ostream& os, const CGrafo& obj);

    // friend istream& operator>> (istream& is, CGrafo& obj);
};

std::ostream& operator<< (std::ostream& os, const CGrafo& obj);
// istream& operator>> (istream& is, CGrafo& obj);
#endif
