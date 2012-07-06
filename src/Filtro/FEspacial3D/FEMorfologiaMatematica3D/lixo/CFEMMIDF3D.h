// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFEMMIDF3D_h)
#define CFEMMIDF3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDF3D.h
Nome da classe:      CFEMMIDF3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
============================================================================
Documentacao CLASSE:		CFEMMIDF3D
============================================================================
Nome Classe:         CFEMMIDF3D
Assunto:             Matriz IDF de uma imagem
Superclasse:         CFEMMIDF3D
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

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <Matriz/CMatriz3D.h>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMorfologiaMatematica3D.h>

// ----------------------------------------------------------------------------
// Classe:       CFEMMIDF3D                  
// ----------------------------------------------------------------------------
class CFEMMIDF3D : public CFEMorfologiaMatematica3D, public TMatriz3D
{
protected:
  int minimo;

protected:
  unsigned int raioMaximo;	// raio máximo da idf
  unsigned int raioBola;	// raio da bola atual,entre 0 e raioMaximo

  static bool atualizaIDF;	// Se verdadeira após cada processamento recalcula a idf

  int indiceAtivo;
  int indiceInativo;

protected:
  // void IDFNosPlanosDeContorno(int& base);     // Calcula a idf nos planos de contorno
  // void IDFNosPlanosDeContornoIDA(int& base);  // |_
  // void IDFNosPlanosDeContornoVOLTA(int& base);// -|

    inline void min (int &t);

  inline void VerificaImagem (TMatriz3D * &matriz);	// usada por erosao,dilatacao,abertura e fechamento
  // para verificar se a imagem passada é a mesma, se diferente recalcula Go.
  inline void ExecutadaPorGo (TMatriz3D * &matriz);	// antiga InicializaIDF
  // Funcao executada exclusivamente por Go
  // 1-verifica se a idf tem as mesmas dimensoes de pm
  // 2-se forem iguais continua, se forem diferentes, desaloca e realoca a matriz de dados
  // 3-copia pm para idf, usando data2D[i][j]=pm->data2D[i][j]
  // agora já pode executar Go

public:
    virtual unsigned int Mi () = 0;	// definida, nas filhas

public:
  // ponteiro para matriz imagem,tamanho da mascara a ser criada,raioMáximo a ser utilizado
    CFEMMIDF3D (TMatriz3D * &matriz, unsigned int _tamanhoMascara =
	       3, unsigned int _raioMax = 32000);
    virtual ~ CFEMMIDF3D ()
  {
  }				// Destrutor

  unsigned int RaioMaximo () const
  {
    return raioMaximo;
  }

  void RaioMaximo (unsigned int raioM)
  {
    raioMaximo = raioM;
  }

  unsigned int RaioBola () const
  {
    return raioBola;
  }

  void RaioBola (unsigned int _raioBola)
  {
    raioBola = raioBola;
  }


  virtual TMatriz3D *Abertura (TMatriz3D * &matriz, unsigned int _RaioBola);	// 

  virtual TMatriz3D *Erosao (TMatriz3D * &matriz, unsigned int _RaioBola);	// 

  virtual TMatriz3D *Dilatacao (TMatriz3D * &matriz, unsigned int _RaioBola);	// 

  virtual TMatriz3D *Fechamento (TMatriz3D * &matriz, unsigned int _RaioBola);	// 

  virtual void IndiceAtivo (int indice)
  {
    indiceAtivo = indice;
  }

  virtual void IndiceInativo (int indice)
  {
    indiceInativo = indice;
  }

};
#endif
