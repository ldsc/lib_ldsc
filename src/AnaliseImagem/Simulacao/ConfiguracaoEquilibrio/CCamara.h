#ifndef CCamara_h
#define CCamara_h

/*.
  ----------------------------------------------------------------------------
  PROJETO:		LIB_LDSC
  Bibliotecas de Objetos
  ----------------------------------------------------------------------------

  Desenvolvido por:	LDSC - Laboratorio de Desenvolvimento de Software Cientifico
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
  Liang Zirong, Paulo C. Philippi,  Marcos Damiani,...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CCamara.h
  Nome da classe:      CCamara
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  ----------------------------------------------------------------------------
*/

#ifndef __IOSTREAM_H
#include <iostream>
#endif

/**
 * @brief  Representa uma camara de um porosimetro a mercúrio onde "experimentos" podem ser realizados.
 * 
 * Esta classe é genérica, mas as herdeiras serão imagens reais.
 * A CCamara é uma classe abstrata, a CCamara2D e CCamara3D são classes concretas.
 * 
 * Nesta classe são definidos os atributos da camara, e as funções de construção da camara.
 * As dimensões adotadas para a camara são aquelas do algoritmo de Magnani.
 *
 * Os métodos aqui implementados são os métodos do tipo SetAtributo que são utilizadas
 * para definir os valores das paredes da camara.
 * A função defineCamara chama as funções individuais de definição da camara
 * (função de definição das paredes, das membranas e das camaras inferior e superior).
 * 
 * Tem ainda a função dimensões padrões que calcula as dimensões da camara
 * tendo como base o raioMaximo do processo. Ou seja as dimensões da camara costumam
 * variar de acordo com o processo a ser estudado.
 * 
 * As funções "define" devem ser chamadas pelo usuário após CriaCamara
 * para definição das diversas partes da camara com indice diferente do default=0.
 * Assim, as diferentes partes que compôem a camara podem ter indices diferentes.
 * O que pode ser futuramente util.
 * 
 * Uso:		  
 * Normalmente o usuário cria um objeto CCamara, depois define os atributos
 * da camara usando SetAtributo, e, finalmente, cria a camara usando CriaCamara.
 * Se a medida que os processos forem realizados a camara for alterada, pode-se
 * redefinir as paredes da camara usando as funções set.
 
 * Um processo de erosao/dilatacao pode corroer as paredes da camara, sendo necessário
 * a chamada das funcoes set para reconstruir as paredes da camara.
 * 
 * Acesso:
 * Cardinalidade:
 * Abstrata/Concreta:	  Abstrata
 * Arquivo de documentacao auxiliar:
  ----------------------------------------------------------------------------
 */
class CCamara
{
  //  --------------------------------------------------------------Atributos
 protected:
  //  Estes atributos se referem as espessuras da camara, 
	 // e são utilizados para o desenho da mesma
  
   /// Espessura da camara superior
  unsigned int camaraSuperior;

  /// Espessura da membrana superior
  unsigned int membranaSuperior;

  /// Espessura da parede esquerda
  unsigned int paredeEsquerda;

  /// Espessura da parede direita
  unsigned int paredeDireita;

  /// Espessura da membrana inferior
  unsigned int membranaInferior;

  /// Espessura da camara inferior
  unsigned int camaraInferior;

  ///  Dimensões da imagem do meio poroso
  // As dimensoes da camara serao maiores,  pois consideram as dimensoes das  paredes
  unsigned int nxImg, nyImg, nzImg;

  //  Ponteiro para imagem recebida?
  //  CImagem2D* imagem;                                   

  ///  Índice de cor com que as paredes serão pintadas ( default = 0 )
  ///  também usado para pintar as membranas.
  int indiceParedes; //  corParedes

  ///  Usado para pintura camara inferior
  int indiceCamaraInferior; // corCamaraInferior

  ///  Usado para pintura da camara superior
  //  A pintura das membranas usa o indice das paredes para a parte da membrana
  //  e o indice das camaras para pintura dos vazios da membrana.
  int indiceCamaraSuperior; // corCamaraSuperior

  ///  Métodos
  public:
	 
  ///  Construtor default, recebe o raio máximo
  CCamara (int &raioMaximo)
  {
  	// Inicializa os atributos
  	CCamara::DimensoesPadroes ( raioMaximo );
  }

  //  1-Construtor sobrecarregado, cria camara assumindo dimensoes padroes
  //  CCamara (/*TImagem2D* img,*/int raioMaximo=10)
  //        {dimensoesPadroes(raioMaximo);criaCamara(img);}
  //  1-Construtor sobrecarregado, cria camara assumindo dados padrões e lendo a imagem do disco
  //  CCamara (std::string nomeArquivo,int raioMaximo=10)
  //        {dimensoesPadroes(raioMaximo);criaCamara(nomeArquivo);}
  //  Construtor de copia
  //  CCamara (const CCamara& aCCamara );
  
  ///  Destructor
  virtual ~ CCamara ()
  {
  }

  //  CCamara operator= (const CCamara& aCCamara)
  //                        { if (this == &aCCamara) return *this;  };
  //  Operator== operador igualdade
  //  int operator== (const CCamara& aCCamara)
  //  { return (  (TImagem2D::operator== (aCCamara))   );  };

  /// Sobrecarga ostreans (saida dos atributos  da camara)
  friend std::ostream & operator<< (std::ostream & os, const CCamara & camara);

  ///  Sobrecarga istreans (entrada dos atributos  da camara)
  friend std::istream & operator>> (std::istream & is, CCamara & camara);

  //  Função que redimensiona a camara no caso de imagem ter dimensões diferentes da anterior
  //               void redimensionaCamara(TImagem2D* img);
  //  usar com cuidado, todos os ponteiros para a camara
  //  precisam ser corrigidos
  //  2-Diálogo para entrada de dados das dimensões da camara
  ///  inputDialog(void*) observe que é indefinido, resescrever nas classes gráficas
  virtual void InputDialog ()
  {
  }

  ///  Define os valores padrões das dimensões da camara em função 
  ///  do raioMaximo, é chamada pela função CriaCamara
  void DimensoesPadroes (int &raioMaximo)
  {
    //  raioMaximo = 10;
    //  Dimensoes
    camaraSuperior = 2 * raioMaximo + 1;
    membranaSuperior = 3;
    paredeEsquerda = raioMaximo + 1;
    paredeDireita = raioMaximo + 1;
    membranaInferior = 3;
    camaraInferior = 2 * raioMaximo + 1;	//  membrana+2;

    nxImg = nyImg = nzImg = 0;

    //  Indices de "cor"
    indiceParedes = 0;				//  cor das paredes

    indiceCamaraInferior = 22;
    indiceCamaraSuperior = 33;

  }

  //  Funcoes de definicao das propriedades do objeto
  /// Seta espessura da camara superior
  void CamaraSuperior (int espessura)
  {
    camaraSuperior = espessura;
  }

  /// Seta espessura da camara inferior
  void CamaraInferior (int espessura)
  {
    camaraInferior = espessura;
  }

  /// Seta espessura da parede esquerda
  void ParedeEsquerda (int espessura)
  {
    paredeEsquerda = espessura;
  }

  /// Seta espessura da parede direita
  void ParedeDireita (int espessura)
  {
    paredeDireita = espessura;
  }
  
  /// Seta espessura da membrana inferior
  void MembranaInferior (int espessura)
  {
    membranaInferior = espessura;
  }
  
  /// Seta espessura da membrana superior  
  void MembranaSuperior (int espessura)
  {
    membranaSuperior = espessura;
  }

  /// Seta indice das paredes
  void IndiceParedes (int indice)
  {
    indiceParedes = indice;
  }

  /// Seta indice da camara inferior
  void IndiceCamaraInferior (int indice)
  {
    indiceCamaraInferior = indice;
  }

  /// Seta indice da camara superior
  void IndiceCamaraSuperior (int indice)
  {
    indiceCamaraSuperior = indice;
  }
  
  /// Retorna indice das paredes
  int IndiceParedes () const
  {
    return indiceParedes;
  }

  /// Retorna indice da camara inferior 
  int IndiceCamaraInferior () const
  {
    return indiceCamaraInferior;
  }

  /// Retorna indice da camara superior
  int IndiceCamaraSuperior () const
  {
    return indiceCamaraSuperior;
  }

  //  5-Função que cria a camara a partir das dimensões das paredes e da imagem
  //  virtual void* criaCamara(void* img){};
  //  virtual TCMatriz2D< int >* criaCamara(TImagem2D* img){}; //  definidas nas classes herdeiras
  //  virtual TCMatriz3D<int> * criaCamara(TImagem3D* img){}; //  chamadas diretamente pelo usuário

  ///   Preenchimento das paredes da camara com o valor dado pelo indice FUNDO (PRETO=0)
  void DefineCamara ( /*int indice */ );			//  Executa todas as funções abaixo
  
  /// Define a Camara Superior.
  virtual void DefineCamaraSuperior ( /*int indice */ ) = 0;	//  que efetivamente pintam os pixel's

  /// Define a Camara Inferior
  virtual void DefineCamaraInferior ( /*int indice */ ) = 0;	//  das "paredes" na camara

  /// Define a Membrana Superior
  virtual void DefineMembranaSuperior ( /*int indice */ ) = 0;	//  Estas funções são definidas 

  /// Define a Membrana Inferior
  virtual void DefineMembranaInferior ( /*int indice */ ) = 0;	//  nas classes herdeiras

  /// Define a Parede Esquerda
  virtual void DefineParedeEsquerda ( /*int indice */ ) = 0;

  /// Define a Parede Direita
  virtual void DefineParedeDireita ( /*int indice */ ) = 0;

  //  7-Copia os pixels da imagem para a camara, pode ser chamada para alterar a imagem no centro da camara
  //  se as dimensões da imagem forem diferentes das da imagem anterior, vai redimensinar a camara
  //   virtual void defineImagem (TImagem2D* img);

  /// Retorna deslocamento NX (a paredeEsquerda)
  int DeslocamentoNX () const
  {
    return paredeEsquerda;
  }

  /// Retorna o deslocamento NY (camaraInferior + membranaInferior)
  int DeslocamentoNY () const
  {
    return camaraInferior + membranaInferior;
  }

  /// Retorna o deslocamento NZ (paredeEsquerda)
  int DeslocamentoNZ () const
  {
    return paredeEsquerda;
  }

  /// Retorna espessura da camara superior
  int CamaraSuperior ()
  {
    return camaraSuperior;
  }

  /// Retorna espessura da camara inferior
  int CamaraInferior ()
  {
    return camaraInferior;
  }

  /// Retorna espessura da membrana inferior
  int MembranaInferior ()
  {
    return membranaInferior;
  }
  
  /// Retorna espessura da membrana superior  
  int MembranaSuperior ()
  {
    return membranaSuperior;
  }

  /// Retorna  nxImg
  unsigned int NxImg () const
  {
    return nxImg;
  }

  /// Retorna NyImg
  unsigned int NyImg () const
  {
    return nyImg;
  }

  /// Retorna NzImg
  unsigned int NzImg () const
  {
    return nzImg;
  }
};

#endif
