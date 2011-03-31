//Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(TFMMIDF3D_h)
#define TFMMIDF3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TFMMIDF3D.h
Nome da classe:      TFMMIDF3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao CLASSE:		TFMMIDF3D
============================================================================
Nome Classe:         TFMMIDF3D
Assunto:             Matriz IDF de uma imagem
Superclasse:         TFMMIDF3D
Descrição:

Acesso:              import
Cardinalidade:
Concorrência:
Transformacoes:
Especificacoes da linguagem:
Persistencia:
Tamanho:
Abstrata/Concreta:   Concreta (mas na pratica é abstrata pois nao e criada)
Arquivo de documentacao auxiliar:
*/

//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------
//#define TMatrizAPP			//define a matriz como aplicativo
#include "TMatriz\TMatriz3D.h"
#include "TFiltro\TFEspacial3D\TFEMorfologiaMatematica3D\TFMorfologiaMatematica3D.h"

//----------------------------------------------------------------------------
//Classe:	TFMMIDF3D                  
//----------------------------------------------------------------------------
class _LIB_LMPT_CLASS TFMMIDF3D : public TFMorfologiaMatematica3D, public TMatriz3D
{
 protected:
  	int minimo;

  protected:
	 unsigned int raioMaximo; 		  //raio máximo da idf
	 unsigned int raioBola;		           //raio da bola atual,entre 0 e raioMaximo

    static bool atualizaIDF;		//Se verdadeira após cada processamento recalcula a idf

  protected:
	//void IDFNosPlanosDeContorno(int& base);     //Calcula a idf nos planos de contorno
	void IDFNosPlanosDeContornoIDA(int& base);  //|_
	void IDFNosPlanosDeContornoVOLTA(int& base);//-|

	inline void min(int& t);

	inline   void VerificaImagem(TMatriz3D*& matriz);//usada por erosao,dilatacao,abertura e fechamento
         							//para verificar se a imagem passada é a mesma, se diferente recalcula Go.
	inline	void ExecutadaPorGo(TMatriz3D*& matriz);//antiga InicializaIDF
                              //Funcao executada exclusivamente por Go
						//1-verifica se a idf tem as mesmas dimensoes de pm
						//2-se forem iguais continua, se forem diferentes, desaloca e realoca a matriz de dados
						//3-copia pm para idf, usando data2D[i][j]=pm->data2D[i][j]
						//agora já pode executar Go
  protected:
  public:
   virtual unsigned int Getmi()=0;		//definida, nas filhas

  public:
					   //ponteiro para matriz imagem,tamanho da mascara a ser criada,raioMáximo a ser utilizado
	 TFMMIDF3D(TMatriz3D *& matriz, unsigned int _tamanhoMascara=3, unsigned int _raioMax=32000);
	 virtual ~TFMMIDF3D(){};   //Destrutor

	 unsigned int GetraioMaximo()const 						{return raioMaximo;};
	 void			  SetraioMaximo(unsigned int raioM)		{raioMaximo=raioM;};
	 unsigned int GetraioBola()const 						{return raioBola;};
	 void			  SetraioBola(unsigned int _raioBola)	{raioBola=raioBola;};


	  virtual TMatriz3D*  Abertura	(TMatriz3D*& matriz,unsigned int _RaioBola);	//
	  virtual TMatriz3D*  Erosao		(TMatriz3D*& matriz,unsigned int _RaioBola);    //
	  virtual TMatriz3D*  Dilatacao	(TMatriz3D*& matriz,unsigned int _RaioBola);	//
	  virtual TMatriz3D*  Fechamento	(TMatriz3D*& matriz,unsigned int _RaioBola);//
};
#endif



