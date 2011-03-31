//Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(TFMMIDFd3453D_h)
#define TFMMIDFd3453D_h


/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
				Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:		Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TFMMIDFd3453D.h
Nome da classe:      TFMMIDFd3453D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
============================================================================
Documentacao CLASSE:		TFMMIDFd3453D
============================================================================
Nome Classe:         TFMMIDFd3453D
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         TFMMIDFd3453D
Descrição:

Acesso:              import
Cardinalidade:
Concorrência:
Transformacoes:
Especificacoes da linguagem:
Persistencia:
Tamanho:
Abstrata/Concreta:   Concreta 
Arquivo de documentacao auxiliar:
*/

//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------

#include "TFiltro\TFEspacial3D\TFEMorfologiaMatematica3D\TFMMIDF3D.h"
#include "TMascara\TMCDiscreta\TMCdijk.h"

//----------------------------------------------------------------------------
//Classe:	TFMMIDFd3453D
//----------------------------------------------------------------------------
class  _LIB_LMPT_CLASS TFMMIDFd3453D : public TFMMIDF3D//, public TMCdijk
{
  protected:
    virtual  void CriaMascara(unsigned int _tamanhoMascara);				//redefinida
  public:
                                               	//Construtor:
                                                //ponteiro para matriz imagem, tamanho da mascara a ser criada,raio máximo a ser usado
	 TFMMIDFd3453D(TMatriz3D *& matriz);
    virtual ~TFMMIDFd3453D(){};                	//Destrutor
    virtual TMatriz3D * Go( TMatriz3D *& matriz,unsigned int _tamanhoMascara=0);//redefinida

  public:
};
#endif    //TFMMIDFd345_h


