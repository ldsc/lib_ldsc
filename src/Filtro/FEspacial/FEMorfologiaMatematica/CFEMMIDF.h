// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDF_h
#define CFEMMIDF_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp - Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------
Desenvolvido por:      	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:         	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDF.h
Nome da classe:      CFEMMIDF
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------
// #define CMatrizAPP         // define a matriz como aplicativo
#ifndef TCMatriz2D_H
#include <Matriz/TCMatriz2D.h>
#endif

#ifndef CFEMorfologiaMatematica_h
#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMorfologiaMatematica.h>
#endif

#ifndef CMascara_h
#include <Mascara/CMascara.h>
#endif

// ----------------------------------------------------------------------------
// Classe:       CFEMMIDF
// ----------------------------------------------------------------------------
/**
 * @brief  Determina a matriz IDF - imagem de distância ao fundo, de uma imagem.
 * Herdeira de CFEMorfologiaMatematica e TCMatriz2D< int >.
 */
class CFEMMIDF : public CFEMorfologiaMatematica, public TCMatriz2D< int > {
   // Atributos
protected:
   int minimo;					/// valor minimo utilizado no calculo minimoIda e minimoVolta
   unsigned int raioMaximo;	/// raio máximo da idf
   unsigned int raioBola;		/// raio da bola atual,entre 0 e raioMaximo
   static bool atualizaIDF;	/// Se verdadeira após cada processamento recalcula a idf
   int indiceAtivo; 			/// indice ativo
   int indiceInativo; 			/// indice inativo

   // Métodos
protected:
   // void IDFNosPlanosDeContorno(int& base);     // Calcula a idf nos planos de contorno
   /// Usada pela abertura, para pintar pontos nos contornos
   void DilatacaoNosContornos ();
   // void IDFNosPlanosDeContornoIDA(int& base);    // Calcula a idf nos planos de contorno
   // void IDFNosPlanosDeContornoVOLTA(int& base);  // Calcula a idf nos planos de contorno
   /// Calcula o valor mínimo
   inline void min ( const int &t ) {
      if ( t < minimo )
         minimo = t;
   }

   /// Usada por erosao, dilatacao, abertura e fechamento
   inline void VerificaImagem ( TCMatriz2D< int > * &matriz );

   /// Verifica se a imagem passada é a mesma, se diferente recalcula Go.
   // antiga InicializaIDF
   void ExecutadaPorGo ( TCMatriz2D< int > * &matriz );

   // Funcao executada exclusivamente por Go
   // 1-verifica se a idf tem as mesmas dimensoes de pm
   // 2-se forem iguais continua, se forem diferentes, desaloca e realoca a matriz de dados
   // 3-copia pm para idf, usando data2D[i][j]=pm->data2D[i][j]
   // agora já pode executar Go

   /// Método chamado por Go das classes herdeiras para inverter a imagem caso o FUNDO != 0
   void InverterSeNecessario ();

   //
public:
   // linha abaixo incluida em 2007 por causa de aviso de erro
   // virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * &matriz,unsigned int _tamanhoMascara = 0)=0;

   ///  Retorna mi. Precisa ser redefinida nas classes bases
   virtual unsigned int Mi () = 0;


   /// Construtor
   // ponteiro para matriz imagem, tamanho da mascara a ser criada,raioMáximo a ser utilizado, valores de indice e fundo na imagem
   CFEMMIDF ( TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara = 3, unsigned int _raioMax = 32000, int _indice=1, int _fundo=0 );

   /// Destrutor
   virtual ~ CFEMMIDF ( ) { }

   /// Retorna o raioMaximo
   unsigned int RaioMaximo () const {
      return raioMaximo;
   }
   /// Seta raioMaximo
   void RaioMaximo ( unsigned int raioM ) {
      raioMaximo = raioM;
   }

   /// Obtêm raioBola
   unsigned int RaioBola () const {
      return raioBola;
   }

   /// Seta raioBola
   void RaioBola ( unsigned int _raioBola ) {
      raioBola = _raioBola;
   }

   /// Seta indiceAtivo
   virtual void IndiceAtivo ( int indice ) {
      indiceAtivo = indice;
   }

   /// Seta indiceInativo
   virtual void IndiceInativo ( int indice ) {
      indiceInativo = indice;
   }

   /// Método de Abertura
   virtual TCMatriz2D< int > *Abertura ( TCMatriz2D< int > * &matriz, unsigned int _RaioBola );
   /// Método de Erosao
   virtual TCMatriz2D< int > *Erosao ( TCMatriz2D< int > * &matriz, unsigned int _RaioBola );
   /// Método de Dilatacao
   virtual TCMatriz2D< int > *Dilatacao ( TCMatriz2D< int > * &matriz, unsigned int _RaioBola );
   /// Método de Fechamento
   virtual TCMatriz2D< int > *Fechamento ( TCMatriz2D< int > * &matriz, unsigned int _RaioBola );
   /// Método de Esqueleto
   virtual TCMatriz2D< int > *Esqueleto ( TCMatriz2D< int > * &matriz, unsigned int _RaioBola = 0 );

   ///  Corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
   void CorrigeAbertura ( TCMatriz2D< int > * &matriz, int &regiao );
};
#endif
