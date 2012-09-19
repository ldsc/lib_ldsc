#ifndef CCamara2D_h
#define CCamara2D_h

/*
  ----------------------------------------------------------------------------
  PROJETO:	LIB_LDSC
  Bibliotecas de Objetos
  ----------------------------------------------------------------------------

  Desenvolvido por: 	Laboratorio de Desenvolvimento de Software Cientifico.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
			Liang Zirong, Paulo C. Philippi,Marcos Damiani,...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CCamara2D.h
  Nome da classe:     CCamara2D
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  ----------------------------------------------------------------------------
*/

#ifndef CCamara_h
#include <ConEqu/CCamara.h>
#endif

#ifndef TMatriz2D_h
#include <Matriz/TMatriz2D.h>
#endif

/**
 * @brief Representa uma camara (bidimensional) de um porosimetro a mercúrio.
 * 
 * As dimensões adotadas para a camara são aquelas do algorítimo de Magnani.
 * Esta classe é herdeira da classe TMatriz2D< int >, ou seja, uma camara é uma imagem
 * (uma matriz de dados) e é herdeira de CCamara.
 * 
 * Enquanto CCamara é uma classe abstrata, CCamara2D e CCamara3D são concretas.
 * 
 * Existem 3 construtores:
 * um vazio;
 * um que recebe uma imagem;
 * e um que recebe o nome do arquivo de disco que contém os dados da imagem.
 * 
 * Exemplo de uso:
 * Opção 1:
 * Cria camara, pode receber o raio maximo como parametro, o default é 10.
 * CCamara2D* camara = new CCamara2D();		
 * 
 * Define dimensões das paredes da camara
 * se nao chamada utiliza parametros default definidos pela função DimensoesPadroes()
 * camara->setAtributo();
 * 
 * Calcula as dimensoes da camara e cria a imagem da camara
 * camara->CriaCamara( img );
 *
 * Opção 2:
 * Cria camara utilizando tamanhos default para as paredes
 * CCamara2D * camara = new CCamara2D( img );		
 * 
 * Opção 3:
 * Cria camara utilizando tamanhos default para as paredes a imagem é carregada do arquivo de disco
 * CCamara2D * camara = new CCamara2D( fileName );	
*/
class CCamara2D : public CCamara, public TMatriz2D< int >
{
  //  Métodos
  public:

  /// Construtor Default, recebe o raio máximo da camara.
  CCamara2D( int raioMaximo = 10 )  : CCamara ( raioMaximo ){ }
  
  // Construtor sobrecarregado, cria camara assumindo dimensões padrões.
  //  CCamara2D (TMatriz2D< int >*& img,int raioMaximo=10):CCamara(raioMaximo)
  //  {
  //   CCamara2D::CriaCamara(img);
  //  }
  //  Construtor sobrecarregado, cria camara assumindo dimensões padrões e lendo a imagem do disco
  //  CCamara2D (std::string fileName,int raioMaximo=10):CCamara(raioMaximo)
  //  {
  //   TMatriz2D< int > imagem(fileName);
  //   CCamara2D::CriaCamara(&imagem);
  //  }
  
  ///  Destrutor
  virtual ~ CCamara2D(){ }
  
  //  CCamara2D operator= (const CCamara2D& aCCamara2D)
  //  { if (this == &aCCamara2D) return *this;  };
  //  Operator== operador igualdade
  //  int operator== (const CCamara2D& aCCamara2D)
  //  { return (  (TMatriz2D< int >2D::operator== (aCCamara2D))   );  };
  
  ///  Diálogo para entrada de dados das dimensões da camara
  virtual void setDimensoesParedesCamara(){ }
  
  ///  Cria a camara a partir das dimensões da imagem e das paredes 
  virtual TMatriz2D< int > * CriaCamara ( TMatriz2D< int > * &img );	//  ,int raioMaximo=10);
  
  //  virtual TMatriz2D< int >* CriaCamara(const TMatriz2D< int >*& const img);
  // Preenchimento das paredes da camara com o valor dado pelo indice = FUNDO ( PRETO = 0 )
  // que efetivamente pintam os pixel's das "paredes" na camara
  
  /// Preenche camara superior com valor dado pelo indice FUNDO
  virtual void DefineCamaraSuperior ( /*int indice */ );
  
  /// Preenche camara inferior com valor dado pelo indice FUNDO
  virtual void DefineCamaraInferior ( /*int indice */ );
  
  /// Cria a membrana superior, com valor dado pelo indice FUNDO
  virtual void DefineMembranaSuperior ( /*int indice */ );
  
  /// Cria membrana  inferior, com valor dado pelo indice FUNDO
  virtual void DefineMembranaInferior ( /*int indice */ );
  
  /// Preenche parede esquerda com valor dado pelo indice FUNDO
  virtual void DefineParedeEsquerda ( /*int indice */ );
  
  /// Preenche parede direita com valor dado pelo indice FUNDO
  virtual void DefineParedeDireita ( /*int indice */ );
  
  //  Métodos NOVOS
  /// Copia os pixeis da imagem para a camara, pode ser chamada para alterar a imagem no centro da camara,
  /// desde que tenham as mesmas dimensoes.
  /// Se a imagem tiver outras dimensoes e necessario redefinir a camara chamando CriaCamara novamente
  virtual void DefineImagem (TMatriz2D< int > * &img);
};

/*
  ???????????????????????????????????????????
  Incluir em todas as classes as declarações:
  ???????????????????????????????????????????
  virtual bool DialogInput (void*); //  função herdada para criacao e passagem de parametros para o objeto
  virtual bool DialogOutput(void*);  //  função herdada para criacao e passagem de parametros para o objeto

  Uso:
  objeto->DialogInput();
  objeto->DialogOutput();

  Obs: Como não serão usadas, o compilador não solicita a implementação das mesmas.
  Se for usar a COI-LIB usar dialogo da COI-LIB.
*/

#endif
