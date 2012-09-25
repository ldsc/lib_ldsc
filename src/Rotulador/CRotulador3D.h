#ifndef CRotulador3D_h
#define CRotulador3D_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CRotulador3D...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     Andre Duarte Bueno
  @file       CRotulador3D.h
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by Andre Duarte Bueno
  @email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CRotulador_h
#include <Rotulador/CRotulador.h>
#endif

#ifndef TCMatriz3D_h
#include <Matriz/TCMatriz3D.h>
#endif

/**
 * @brief	Representa um objeto rotulador 3D.
 *   Herdeira da CRotulador e da CMatriz3D
 * @author 	Andre Duarte Bueno	
 *  @see	Rotulagem
*/
class CRotulador3D : public TCMatriz3D<int>, public CRotulador
{

   // --------------------------------------------------------------Atributos
public:
   /// Se ativo usa rotulagem 3D, se inativo usa 3D a partir de 2D
   bool direto;

   /// Ponteiro para a imagem recebida no construtor ou em Go
	 TCMatriz3D<int> *pm;

   // -------------------------------------------------------------Construtor
   /// Construtor, recebe ponteiro para imagem 3D usado para setar nx,ny,nz
	 CRotulador3D (TCMatriz3D<int> * _pm, int _indice=1, int _fundo=0)
			: TCMatriz3D<int> (_pm->NX (), _pm->NY (), _pm->NZ ()) // Aloca matriz de dados
      , CRotulador ( _indice, _fundo), direto (true), pm (_pm)  {    // Seta atributos
      path = _pm-> path;
   }

   /// Recebe o nome da matriz de disco, abre arquivo de disco e seta matriz
   CRotulador3D (std::string fileName)
			:TCMatriz3D<int> (fileName), CRotulador (), direto (true),pm (NULL) {
   }

   /// Recebe a informação das dimensoes da imagem
   CRotulador3D (unsigned int nx, unsigned int ny, unsigned int nz)
			:TCMatriz3D<int> (nx, ny, nz), CRotulador (), direto (true), pm (NULL) {
   }

   // --------------------------------------------------------------Destrutor
   /// Destrutor
   virtual ~ CRotulador3D () {
   }

   // ----------------------------------------------------------------Metodos
protected:

   // Verifica se a imagem recebida tem as mesmas dimensoes do rotulador
   // se diferente, realoca o rotulador
	 virtual bool PreparaImagem (TCMatriz3D<int> * matriz);

   /// 1a passagem, identifica pixeis válidos
   virtual void IdentificaObjetos ();

   /// 2a passagem, rotula a imagem
   virtual void RotulaImagem ( /*int _rotuloInicial=0 */ );

   // virtual void IdentificaObjetos_3D();  // Executa 1a passagem diretamente em 3D
   // virtual void IdentificaObjetos_3D_Usando2D();// Executa 1a passagem utilizando rotulagem 2D
   // Metodos antigos, podem ser descartados
   // virtual void FuncaoRotulaImagemAntiga()
   //{cout<<"\n FuncaoRotulaImagemAntiga Inativa para objeto 3D";};
   // virtual       void FuncaoRotulaImagemSequencialAntiga()
   //{cout<<"\nFuncaoRotulaImagemSequencialAntiga Inativa para objeto 3D";};

public:
   /// Define o flag direto
   void SetDireto (bool flag)
   {
      direto = flag;
   }

   // Metodos herdados virtuais
   /// Determina a area dos objetos
   virtual void CalculaAreaObjetos ();

   /// Determina o perimetro dos objetos
   virtual void CalculaPerimetroObjetos ();

   // Metodos utilizados na rotulagem, redefinidos aqui
   /// Executa toda a sequencia de rotulagem
	 virtual bool Go (TCMatriz3D<int> * matriz /*, int rotuloInicial=0 */ );


   // --------------------------------------------------------------------Get
   // unsigned int Rotulo (i,j,k)
   //{return data3D[i][j][k]       + rotuloInicial;};
   // --------------------------------------------------------------------Set
   // -----------------------------------------------------------------Friend
   //       friend ostream& operator<< (ostream& os, CRotulador3D& obj);
   //       friend istream& operator>> (istream& is, CRotulador3D& obj);
};

// -----------------------------------------------------------------Friend
// Declaracao de Funcoes Friend
// ostream& operator<< (ostream& os, CRotulador3D& obj);
// istream& operator>> (istream& is, CRotulador3D& obj);

#endif
