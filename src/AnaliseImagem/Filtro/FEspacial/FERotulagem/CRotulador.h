#ifndef CRotulador_h
#define CRotulador_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CRotulador...
  ===============================================================================
  Desenvolvido por:
  Laboratorio de Desenvolvimento de Software Cientifico
  [LDSC].
  @author     Andre Duarte Bueno
  @file       CRotulador.h
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by Andre Duarte Bueno
  @email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef CVetor_h
#include <AnaliseImagem/Matriz/CVetor.h>
#endif

// ===============================================================================
// Documentacao Classe: CRotulador
// ===============================================================================
/**
 * @brief Classe base usada para rotular imagens, implementada efetivamente nas classes CRotulador2D e CRotulador3D.
 *
 * Representa um objeto rotulador.
 * Realiza as tarefas de rotulagem.
 * Rotula a imagem, permite calcular a area, perimetro e raio hidraulico dos objetos pertencentes a imagem.
 *
 * Vai ter como herdeiros objetos rotuladores efetivos para imagens 2D e 3D.
 *
 * Pode ter como herdeiro tambem, objetos de verificacao da conectividade entre os diversos objetos rotulados.
 *
 * Os rotulos vão de rotuloInicial -> até -> rotuloFinal.
 *
 * Durante o processo de rotulagem, o rotuloAtual e rotuloMaximoUtilizado podem ser maiores que o rotuloFinal.
 *
 * Descreve-se a seguir as diversas versões deste código:
 *
 * Versao1:
 * Funcional com a funcao FuncaoRotulaImagemAntiga é o codigo do Hoshen-Kopelmam implementado pelo Fabio, e levemente modificado pelo Andre
 *
 * Versao2:
 * Funcional com a funcao FuncaoRotulaImagemSequencialAntiga.
 * Funcao nova que deixa a imagem rotulada com rotulos sesquênciais.
 * Ou seja, elimina-se os furos na sequência, exigia uma terceira passagem pela imagem.
 *
 * Versao3:
 * Funcional. Nesta versao o procedimento de calculo foi todo reformulado pelo Andre, com a inclusão do vetor de conversao.
 * Usava a Go_OLD, que era lenta.
 *
 * Versao4:
 * Funcional. Usa a funcao Go, ~30% mais rápido que o algoritimo do Hoshen e Kopelman
 * Ou seja: 30% mais rápido, com somente duas passagens e imagem sequêncial.
 *
 * Versao5:
 * Funcional. Em determinado momento foi adicionada a possibilidade de se adicionar o rotulo inicial, a partir do qual a imagem seria rotulada.
 * Na primeira versao que admitia o rotulo inicial, a funcao IdentificaObjetos recebia um int rotuloInicial, e todo o calculo era modificado,
 * entretanto, o calculo da area e perimetro dava um pau (bug), pois os vetores eram definidos para (Ex: 50 objetos), e acessava (ex: rotuloInicial=77, de 77->77+50).
 * A solucao adotada foi voltar atrás, deixar a imagem com rotulos de 0->rotuloMaximo, e criar funções Get para acessar os vetores area, perimetro, e raioHidraulico.
 * Adicionados os atributos: rotuloInicial e rotuloFinal.
 *
 * Versao6:
 * Funcional. Eliminado o vetor  raioHidraulico, agora usa a função GetraioHidraulico(int rotulo), sempre calcula o raioHidraulico, e retorna um double.
 * Também foi adicionada a funcao GetRotulo(i,j) que retorna data2D[i][j]+rotuloInicial???????
 *
 * Versao 7:
 * Eliminada a possibilidade de se passar o rutuloInicial, criava uma série de dificuldades que condicionavam o calculo do vetorArea e do vetorPerimetro
 * a fazerem um conjunto de testes que prejudicavam a performance.
 * Reformulado o calculo da funcao perimetro objetos, agora nao compara mais com 0, e sim com a variavel FUNDO.
 *
 * Reformulado o calculo do perimetro, antes considerava o centro da imagem e as bordas
 * (esquerda,direita,superior e inferior) agora inclue também os 4 pontos das 4 bordas da imagem.
 *
 * PS: Existe um BUG declarado(lógico), que é o seguinte. Se toda a imagem for preta, vai ficar com rotulo 1, mas vai declarar a existencia de dois objetos o 0 e o 1.

 * @short
   @author 	Andre Duarte Bueno
   @version	7
   @see		Veja em CMatriz as classes bases do CRotulador2D e CRotulador3D.
*/
class CRotulador
{
  // --------------------------------------------------------------Atributos
 protected:
	/// Representa a cor de indice (poro) da imagem original, normalmente=1
  int INDICE; // Tirei o unsigned por causa dos warnings, verificar efeito
	/// Representa a cor de fundo da imagem original, normalmente=0
  int FUNDO; // Tirei o unsigned por causa dos warnings, verificar efeito

  /// Vetor de conversao (identifica rotulos validos e invalidos)
  CVetor *vConversao;

  // PS: nao adianta ser unsigned long se o vetor é do tipo int
  // ou seja deve ser do mesmo tipo do vetor vConversao
  /** @brief É o numero do maior rotulo utilizado, no final da funcao identifica objetos.
     Usado para limitar a pesquisa dos rotulos validos
     Quando usar vetores alocados dinamicamente pode ser eliminado*/
  int rotuloMaximoUtilizado;

  /// flag ativo se a imagem ja foi rotulada
  bool rotulado;

  // INTERVALOS DOS ROTULOS (Incluido versao 5, eliminado na 7)
  // problemas com a determinação da area, perimetro... (fica muito complicado)
  // a melhor solucao é criar rotulo inicial, e só permitir acesso as funcoes
  // internas com GetCMX(rotulo)...assim substitue por cmx(rotulo+rotuloinicial)
  // Valor do rotulo inicial
  //        int rotuloInicial;
  // Valor do rotuloAtual
  //        int rotuloAtual;
  // Valor do rotulo final
  //        int rotuloFinal;

  /** @brief Calculado no final da funcao PesquisaRotulosValidosEOrdena.
     É o numero de objetos identificados na imagem .Inclue o objeto 0.*/
  int numeroObjetos;

  /// Vetor com a area dos objetos (inicia com objeto 0)
  CVetor *areaObjetos;

  // vector< int*>  areaObjetos;
  /// Vetor com o perimetro dos objetos (inicia com objeto 0)
  CVetor *perimetroObjetos;

  // Abaixo eliminado na versao 6
  // Vetor com o raio hidraulico dos objetos
  // CVetor* raioHidraulicoObjetos;
 public:
  // -------------------------------------------------------------Construtor
  /// Construtor
  CRotulador ( int _indice=1, int _fundo=0);

  // --------------------------------------------------------------Destrutor
  /// Destrutor
  virtual ~ CRotulador ();

  // ---------------------------------------------------------------Metodos
 protected:
  // Funcoes utilizadas na rotulagem usando Go()
  /** @brief Passo 1: 1ê passagem (pela imagem toda),
     Deve: Alocar o vetor vConversao (e nao deleta-lo)
     definir o valor de  rotuloMaximoUtilizado
     identificar todos os pixeis validos e invalidos na imagem*/
  virtual void IdentificaObjetos () = 0;

  /** @brief Passo 2: Substitue rotulos validos por validos ordenados no vetor vConversao,
     e calcula  o numeroObjetos*/
  void PesquisaRotulosValidosEOrdena ();

  /** @brief Passo 3: Elimina rotulos negativos (indiretos) no vetor de conversao
     so deixa rotulos validos (diretos)
     poderia fazer esta verificacao de três para frente*/
  void UniformizaTabelaRotulos ();

  /** @brief Passo 4: 2a passagem (pela imagem toda), define rotulos finais*/
  virtual void RotulaImagem () = 0;

  // void                   VerificaContorno(int &contorno,   int   &rotuloAtual);
  // void                  VerificaContorno ( TContorno::eTipo contorno, int  rotuloAtual);
  /** @brief Funcao que verifica os contornos*/
  void VerificaContorno (int contorno, int rotuloAtual);

  // Abaixo modificado na versao 5, nao recebe mais o rotulo inicial
  // Antiga: virtual void IdentificaObjetos(int rotuloInicial=1)=0;

  // Abaixo eliminado na versao 4
  // void                  OrdenaTabelaRotulos(); // ordena rotulos (usada em Go_Old)

 public:
  // Metodos de rotulagem antigos (devem ser eliminados do objeto)
  // Rotula desordenadamente (2 passagens)
  // virtual      void FuncaoRotulaImagemAntiga(){};
  // Ordena rotulos em imagem rotulada (+2passagens)
  // virtual      void FuncaoRotulaImagemSequencialAntiga(){};
  // virtual  void Go_Old();                                      // Rotula com metodo novo mas + lento

  // MÉTODOS DE CALCULO DOS ATRIBUTOS
  /// Determina a area dos objetos.
  virtual void CalculaAreaObjetos () = 0;

  /// Determina o perimetro dos objetos.
  virtual void CalculaPerimetroObjetos () = 0;

  /// Determina o raioHidraulico (area e perimetro) dos objetos.
  void CalculaRaioHidraulicoObjetos ();

  // O rotulo inicial é passado na funcao Go e nao pode ser modificado
  // void ReRotulaImagem( int _rotuloInicial) { };

  // --------------------------------------------------------------------Get
  // int GetrotuloInicial()const {return rotuloInicial;};
  // int GetrotuloAtual()const {return rotuloAtual;};
  // int GetrotuloFinal()   const {return rotuloFinal;};
  // int GetnumeroObjetos() const {return numeroObjetos;};

	/// O rotulo inicial é sempre 0.
  int RotuloInicial () const {
		return 0;
  }

	/// O ultimo rotulo corresponde ao numero de objetos -1.
  int RotuloFinal () const {
		return (numeroObjetos-1);
  }

  /// Retorna numero de objetos.
  int NumeroObjetos () const {
    return numeroObjetos;
  }

  /// Retorna area do objeto.
  int AreaObjetos (int k) const;

  /// Retorna permetro do objeto.
  int PerimetroObjetos (int k) const;

  /// Retorna o raio hidraulico do objeto.
  double RaioHidraulicoObjetos (int k) const;

  /// Retorna o vetor area dos objetos.
  CVetor *PtrAreaObjetos () const {
    return areaObjetos;
  }

  /// Retorna o vetor perimetro dos objetos
  CVetor *PtrPerimetroObjetos () const {
    return perimetroObjetos;
  }

  /// Seta o valor de FUNDO da imagem.
  inline void SetFundo ( int fundo ) {
    FUNDO = fundo;
  }

  /// Pega o valor de FUNDO da imagem.
  inline int GetFundo ( ) const {
    return FUNDO;
  }

  /// Seta o valor de INDICE da imagem.
  inline void SetIndice ( int indice ) {
    INDICE = indice;
  }

  /// Pega o valor de FUNDO da imagem.
  inline int GetIndice ( ) const {
    return INDICE;
  }

// --------------------------------------------------------------------Set
  // void SetrotuloInicial(int _rotuloInicial)
  // { rotuloInicial = _rotuloInicial;};

  // -----------------------------------------------------------------Friend
  //       friend ostream& operator<< (ostream& os, CRotulador* obj);
  //       friend istream& operator>> (istream& is, CRotulador* obj);
};

// -----------------------------------------------------------------Friend
// Declaracao de Funcoes Friend
// ostream& operator<< (ostream& os, CRotulador* obj);
// istream& operator>> (istream& is, CRotulador* obj);
#endif
