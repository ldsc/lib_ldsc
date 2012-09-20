#ifndef  CCamara3D_h
#define  CCamara3D_h

/*.
  ----------------------------------------------------------------------------
  PROJETO:		LIB_LDSC
  Bibliotecas de Objetos
  ----------------------------------------------------------------------------

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi,
  Marcos Damiani,...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CCamara3D.h
  Nome da classe:      CCamara3D
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  ----------------------------------------------------------------------------
*/

#ifndef CCamara_h
#include <ConEqu/CCamara.h>
#endif

#ifndef TCMatriz3D_h
#include <Matriz/TCMatriz3D.h>
#endif


/**
 * @brief Representa uma camara (tridimensional) de um porosimetro a mercúrio.
 * 
 * As dimensões adotadas para a camara são aquelas do algorítimo de Magnani.
 * 
 * Esta classe é herdeira da classe CCamara e da clasee CMatriz3D, ou seja, 
 * uma camara é uma imagem (uma matriz de dados).
 * A CCamara é uma classe abstrata, a CCamara2D e CCamara3D são concretas.
 * 
 * Existem 3 construtores, um vazio, um que recebe uma imagem
 * e um que recebe o nome do arquivo de disco que contém os dados da imagem.
 * 
 * Exemplo de uso:
 * // Opção 1:
 * CCamara3D* camara = new CCamara3D();		
 * // Define dimensões das paredes da camara
 * camara->setAtributo();			
 * // Cria camara,  se Nao chamada utiliza parametros default
 * // definidos pela função dimensoesPadroes()
 * // Calcula as dimensoes da camara e cria a imagem da camara
 * camara->criaCamara( img );	
 * 
 * // Opção 2
 * // Cria camara utilizando tamanhos default para as paredes
 * CCamara3D *camara=new CCamara3D(img);		
 *
 * // Opção 3
 * // Cria camara utilizando tamanhos default para as paredes
 * // a imagem é carregada do arquivo de disco
 * CCamara3D * camara = new CCamara3D ( fileName ) ;	
*/
class CCamara3D : public CCamara, public TCMatriz3D<int>
{
  // Métodos
 public:

  /// Construtor Default, recebe o raio maximo.
  /// O raio máximo pode ser calculado usando a classe IDF.	 
  CCamara3D (int raioMaximo = 10) : CCamara (raioMaximo)
    {
    }

    // Construtor sobrecarregado, recebe imagem e raio máximom a seguir cria camara assumindo dimensoes padroes.
    // CCamara3D (TCMatriz3D<int> *& img,int& raioMaximo=10):CCamara(raioMaximo)
    //  {
    //  CCamara3D::CriaCamara(img);
    //  }

    // 1-Construtor sobrecarregado, cria camara assumindo dados padrões e lendo a imagem do disco
    // CCamara3D (std::string fileName,int& raioMaximo=10):CCamara(raioMaximo)
    //  {
    //   CMatriz3D imagem(fileName);
    //   CCamara3D::CriaCamara(&imagem);
    //  }

    /// Destructor
    virtual ~ CCamara3D ()
      {
      }

    // CCamara3D operator= (const CCamara3D& aCCamara3D)
    //  { if (this == &aCCamara3D) return *this;  };
      
    // Operator== operador igualdade
    // int operator== (const CCamara3D& aCCamara3D)
    // { return (  (CMatriz3D::operator== (aCCamara3D))   );  };

    /// Diálogo para entrada de dados das dimensões da camara
    virtual void setDimensoesParedesCamara ()
    {
    }

    /// Função que cria a camara a partir das dimensões das paredes e da imagem
		virtual TCMatriz3D<int> *CriaCamara (TCMatriz3D<int> * &img);	// ,int raioMaximo=10);

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

    // Métodos NOVOS
    // 7-Copia os pixels da imagem para a camara, pode ser chamada para alterar a imagem no centro da camara
    // se as dimensões da imagem forem diferentes das da imagem anterior, 
    // então voce deve chamar CriaCamara novamente.
		virtual void DefineImagem (TCMatriz3D<int> * &img);
};

#endif
