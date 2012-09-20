// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFdijk3D_h
#define CFEMMIDFdijk3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
   Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFdijk3D.h
Nome da classe:      CFEMMIDFdijk3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
==================================================================================
Documentacao CLASSE:		CFEMMIDFdijk3D
==================================================================================
Nome Classe:         CFEMMIDFdijk3D
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         CFEMMIDFdijk3D
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

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMIDF3D.h>
#include <Mascara/MCDiscreta/CMCdijk.h>

// ----------------------------------------------------------------------------
// Classe:       CFEMMIDFdijk3D
// ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro dijk.
 */
class CFEMMIDFdijk3D : public CFEMMIDF3D, public CMCdijk
{
protected:

   virtual void CriaMascara (unsigned int _tamanhoMascara);	// redefinida

protected:
   virtual unsigned int Mi () {
      return mi; // redefinida, herdada de CFEMMIDF3D
   }

public:
   /// Construtor:
   /// Recebe ponteiro para matriz imagem, métrica da mascara a ser criada, raio máximo a ser usado, identificação de índice e fundo
   CFEMMIDFdijk3D (TCMatriz3D<int> * &matriz, unsigned int _mi, unsigned int _mj, unsigned int _mk, unsigned int _rb, int _indice=1, int _fundo=0)
      :CFEMMIDF3D (matriz, 3, 32000, _indice, _fundo), CMCdijk (_mi, _mj, _mk, _rb)
   {
   }

   /// Destrutor
   virtual ~ CFEMMIDFdijk3D ()
   {
   }

   virtual TCMatriz3D<int> *Go (TCMatriz3D<int> * &matriz, unsigned int _tamanhoMascara = 0);
	// redefinida

};
#endif // CFEMMIDFd345_h
