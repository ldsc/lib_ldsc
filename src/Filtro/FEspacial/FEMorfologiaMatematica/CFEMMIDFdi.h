//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined(CFEMMIDFdi_h)
#define CFEMMIDFdi_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
  Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFdi.h
Nome da classe:      CFEMMIDFdi
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------

#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDF.h>
#include <Mascara/MCDiscreta/CMCdi.h>

//  ----------------------------------------------------------------------------
//  Classe:       CFEMMIDFdi
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro di.
 */
class CFEMMIDFdi : public CFEMMIDF, public CMCdi
{
   //  unsigned int mi, rb;   //  parametros passados para criacao da bola

protected:
	
	///  Redefinida, herdada de CFEMMIDF
   virtual unsigned int Mi ()
   {
      return mi;
   }

public:
   /// Construtor.
   CFEMMIDFdi (TMatriz2D< int > * &matriz, unsigned int _mi, unsigned int _rb, int _indice=1, int _fundo=0)
      :CFEMMIDF (matriz, _rb, 32000, _indice, _fundo), CMCdi (_mi, _rb)
   {
   }

   ///  Destrutor.
   virtual ~ CFEMMIDFdi ()
   {
   }

   /// Cria a mascara.
   virtual void CriaMascara (unsigned int _tamanhoMascara);

   /// Realiza processamento da imagem.
   virtual TMatriz2D< int > *Go (TMatriz2D< int > * &matriz, unsigned int _tamanhoMascara =	 0);
};
#endif //  CFEMMIDFdi_h
