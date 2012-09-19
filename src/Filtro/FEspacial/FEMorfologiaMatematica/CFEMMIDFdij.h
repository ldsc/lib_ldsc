//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFdij_h
#define CFEMMIDFdij_h


/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
  Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFdij.h
Nome da classe:      CFEMMIDFdij
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDF.h>
#include <Mascara/MCDiscreta/CMCdij.h>

//  ----------------------------------------------------------------------------
//  Classe:       CFEMMIDFdij
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro dij.
 */
class CFEMMIDFdij : public CFEMMIDF, public CMCdij
{
protected:

   virtual void CriaMascara (unsigned int _tamanhoMascara);

public:
   ///  Retorna mi (Redefinida, herdada de CFEMMIDF).
   virtual unsigned int Mi () {
      return mi;
   }

   /// Construtor
   CFEMMIDFdij (TCMatriz2D< int > * &matriz, unsigned int _mi, unsigned int _mj, unsigned int _rb, int _indice=1, int _fundo=0)
      :CFEMMIDF (matriz, 3, 32000, _indice, _fundo), CMCdij (_mi, _mj, _rb)
   {
   }

   ///  Destrutor
   virtual ~ CFEMMIDFdij ()
   {
   }

   /// Processa determinação da idf.
   virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara = 0);
};
#endif //  CFEMMIDFdij_h
