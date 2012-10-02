// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEBorda3D_h
#define CFEBorda3D_h

// inclue a biblioteca de filtros
#ifndef TCFEspacial3D_h
#include <Filtro/FEspacial3D/TCFEspacial3D.h>
#endif


/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEBorda3D.h
Nome da classe:      CFEBorda3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:    CFEBorda3D
==================================================================================
Assunto:             Filtros espaciais (atuam sobre o espaço geométrico da imagem).
Superclasse:         TFiltro-> CFEspacial<int>
Descrição:           Elimina a borda de uma imagem, substituindo pontos da borda por valorBorda
                     O valor default para a borda é zero (0).
                     Ex:
                     CFEBorda3D(valorBorda).Go(imagem,espessuraBorda);

Acesso:           import
Cardinalidade:    1:1    ,de uma maneira geral usa-se um filtro para atuar sobre uma imagem
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/

class CFEBorda3D		// : public CFEspacial3D
{
protected:

  int valorBorda;		// valor dos pontos da borda
//  int espessura;                // espessura da borda

public:

    CFEBorda3D (int vB
		/*,TCMatriz3D<int> *& matriz, unsigned int _tamanhoMascara=0 */ )
  {
  }

  // :  CFEspacial3D(matriz, _tamanhoMascara ) ,valorBorda(vB)     {};
  ~CFEBorda3D ()
  {
  }

  virtual TCMatriz3D<int> *Go (TCMatriz3D<int> * &imagem, unsigned int espessura = 1);

  void ValorBorda (int v)
  {
    valorBorda = v;
  }

  int ValorBorda () const
  {
    return valorBorda;
  }
//  void Setespessura(int v)             {espessura=v;};

//  int Getespessura()const                      {return espessura;}

};
#endif // CFEBorda3D_h
