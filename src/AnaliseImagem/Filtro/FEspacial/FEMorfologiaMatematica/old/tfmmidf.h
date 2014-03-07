// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFMMIDF_h)
#define CFMMIDF_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
				Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:		Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDF.h
Nome da classe:      CFMMIDF
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao Classe:		CFMMIDF
============================================================================
Nome Classe:         CFMMIDF
Assunto:             Matriz IDF de uma imagem
Superclasse:         CFMMIDF
Descri��o:

Acesso:              import
Cardinalidade:
Concorr�ncia:
Transformacoes:
Especificacoes da linguagem:
Persistencia:
Tamanho:
Abstrata/Concreta:   Concreta (mas na pratica � abstrata pois nao e criada)
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------
// #define CMatrizAPP                                                            // define a matriz como aplicativo
#ifndef CMatriz2D_h
#include <AnaliseImagem/Matriz/TCMatriz2D.h>


#endif

#ifndef CFMorfologiaMatematica_h
#include <AnaliseImagem/Filtro/FEspacial/TFEMorfologiaMatematica/CFMorfologiaMatematica.h>
#endif

#ifndef TMascara_h
#include <AnaliseImagem/Filtro/Mascara/TMascara.h>
#endif

// ----------------------------------------------------------------------------
// Classe:       CFMMIDF
// ----------------------------------------------------------------------------
class CFMMIDF:public CFMorfologiaMatematica, public CMatriz2D
{
// Atributos
protected:
  int minimo;			// valor minimo utilizado calculo minimoIda e minimoVolta

  unsigned int raioMaximo;	// raio m�ximo da idf
  unsigned int raioBola;	// raio da bola atual,entre 0 e raioMaximo

  static bool atualizaIDF;	// Se verdadeira ap�s cada processamento recalcula a idf

  int indiceAtivo;
  int indiceInativo;
// M�todos
protected:
  // void IDFNosPlanosDeContorno(int& base);     // Calcula a idf nos planos de contorno

  void DilatacaoNosContornos ();	// usada pela abertura, para pintar pontos nos contornos
  // void IDFNosPlanosDeContornoIDA(int& base);    // Calcula a idf nos planos de contorno
  // void IDFNosPlanosDeContornoVOLTA(int& base);  // Calcula a idf nos planos de contorno

  inline void min (const int &t);

  inline void VerificaImagem (CMatriz2D * &matriz);	// usada por erosao,dilatacao,abertura e fechamento
  // para verificar se a imagem passada � a mesma, se diferente recalcula Go.
  inline void ExecutadaPorGo (CMatriz2D * &matriz);	// antiga InicializaIDF
  // Funcao executada exclusivamente por Go
  // 1-verifica se a idf tem as mesmas dimensoes de pm
  // 2-se forem iguais continua, se forem diferentes, desaloca e realoca a matriz de dados
  // 3-copia pm para idf, usando data2D[i][j]=pm->data2D[i][j]
  // agora j� pode executar Go


public:
    virtual unsigned int Mi () = 0;	//  Precisa ser redefinida nas classes bases
  // deve retornar o mi.
  // ponteiro para matriz imagem, tamanho da mascara a ser criada,raioM�ximo a ser utilizado
    CFMMIDF (CMatriz2D * &matriz, unsigned int _tamanhoMascara =
	     3, unsigned int _raioMax = 32000);
    virtual ~ CFMMIDF ()
  {
  };				// Destrutor

  unsigned int GetraioMaximo () const
  {
    return raioMaximo;
  };
  void SetraioMaximo (unsigned int raioM)
  {
    raioMaximo = raioM;
  };
  unsigned int GetraioBola () const
  {
    return raioBola;
  };
  void SetraioBola (unsigned int _raioBola)
  {
    raioBola = raioBola;
  };


  virtual CMatriz2D *Abertura (CMatriz2D * &matriz, unsigned int _RaioBola);
  virtual CMatriz2D *Erosao (CMatriz2D * &matriz, unsigned int _RaioBola);
  virtual CMatriz2D *Dilatacao (CMatriz2D * &matriz, unsigned int _RaioBola);
  virtual CMatriz2D *Fechamento (CMatriz2D * &matriz, unsigned int _RaioBola);

  virtual CMatriz2D *Esqueleto (CMatriz2D * &matriz, unsigned int _RaioBola =
				0);

  virtual void SetIndiceAtivo (int &indice)
  {
    indiceAtivo = indice;
  };
  virtual void SetIndiceInativo (int &indice)
  {
    indiceInativo = indice;
  };
};
#endif
