#ifndef CGra3Dby2D_M3_h
#define CGra3Dby2D_M3_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CGra3Dby2D_M3...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     André Duarte Bueno
  @file       CGra3Dby2D_M3.h
  @begin      Oct 20 2000
  @copyright  (C) 2000 by André Duarte Bueno
  @email      andre@lmpt.ufsc.br
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

#ifndef CGra3Dby2D_M2_h
#include <Grafo/CGra3Dby2D_M2.h>
#endif

/*
  ===============================================================================
  Documentacao CLASSE: CGra3Dby2D_M3
  ===============================================================================
*/
 /**
  * @brief Determina o grafo de imagens 3D, modelo 3.
  * @author 	André Duarte Bueno		
  * @see			grafos
  * Superclasse: CGrafo -> CGra3Dby2D -> CGra3Dby2_M2 -> CGra3Dby2D_M3
  */
class CGra3Dby2D_M3 : public CGra3Dby2D_M2
{
  // --------------------------------------------------------------Atributos
  // Função chamada por Go e redefinida aqui
  // calcula as condutancias das ligações entre dois sítios
  // virtual void CalcularPropriedadesConeccoes() ;
 public:

  // -------------------------------------------------------------/** Construtor  */
/// Construtor
	 CGra3Dby2D_M3 (std::string _fileName):CGra3Dby2D_M2 (_fileName)
    {
    }

    // --------------------------------------------------------------/** Destrutor */        
/// Destrutor
    virtual ~ CGra3Dby2D_M3 ()
      {
      }

    // ----------------------------------------------------------------Métodos
    /**
     * @brief Para cada sítio calcula a condutancia do sítio considerando
     * o raioHidraulico = area / perimetro.
     * A condutancia é calculada usando a equação de Poiselle...(Bear, pg...).
     * Aqui calcula a condutância dos sítios e dos links
    */
    virtual void CalculoCondutancias (long double _viscosidade,
				      long double _sizePixel,
				      unsigned long int _fatorAmplificacao);

    /**
     * @brief Pesquisa o vetor dos links e elimina os links repetidos.
     * é definido como público para ser chamado depois de Go e depois do 
     * calculo das condutâncias, visto que para eliminar os links repetidos,
     * acumula as condutâncias duplicadas.
     * Se for chamado antes do calculo das condutâncias, vai acumular os raios hidraulicos
    */
    virtual void EliminarCondutanciasRepetidas ();	// TESTE

    virtual bool SetMatrizAVetorB (CMatriz2D * &A, CVetor * &B) const;	// TESTAR

 protected:
    // Métodos redefinidos
    /**
    * @brief Determina as conecções entre os objetos
     * além de estabelecer os links entre os objetos, calcula o raio Hidraulico
     * das ligações
    */
    virtual void DeterminarConeccoesObjetos (unsigned long int
					     maiorRotuloUtilizado);

    /// Redefinida,  retorna um COGSitioLR ou derivado
    virtual CObjetoGrafo *GetObjetoGrafo (CContorno::ETipoContorno tipoContorno);

    // Função Go
    // virtual CGrafo* Go( CMatriz3D * _img3D,unsigned long int  _tamanhoMascara = 1);
    // virtual CGrafo* Go( std::string fileName,unsigned long int _tamanhoMascara = 0) ;

    // --------------------------------------------------------------------Get
    // --------------------------------------------------------------------Set
    // -----------------------------------------------------------------Friend
    //       friend ostream& operator<< (ostream& os, CGra3Dby2D_M3& obj);
    //       friend istream& operator>> (istream& is, CGra3Dby2D_M3& obj);
};


// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGra3Dby2D_M3& obj);
// istream& operator>> (istream& is, CGra3Dby2D_M3& obj);

#endif
