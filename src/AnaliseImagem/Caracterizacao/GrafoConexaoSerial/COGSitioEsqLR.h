#ifndef COGSitioEsqLR_h
#define COGSitioEsqLR_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: COGSitioEsqLR...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       COGSitioEsqLR.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitioLR.h>

// ===============================================================================
// Documentacao CLASSE: COGSitioEsqLR
// ===============================================================================
/** 
 * @brief Representa um sítio conectado a face esquerda do grafo e que 
 * armazena a informação da conexão.
 * 
 * Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitioLR->COGSitioEsqLR
 * Assume valor de contorno = 0.
 * O 	valor de contorno é usado no calculo dos fluxo da malha como
 * um todo.Ou seja, um algoritimo externo percorre toda
 * a malha, e se o contorno for igual ao solicitado
 * calcula alguma propriedade neste contorno.
 * @author 	André Duarte Bueno	
 * @see:        CGrafo
 */
class COGSitioEsqLR:public COGSitioLR
{
// --------------------------------------------------------------Atributos

public:
// -------------------------------------------------------------Construtor
/// Destrutor
	COGSitioEsqLR ()
  {
  }

// --------------------------------------------------------------Destrutor
/// Construtor
  virtual ~ COGSitioEsqLR ()
  {
  }

// ----------------------------------------------------------------Métodos
// --------------------------------------------------------------Atributos

public:
   /// Retorna o tipo 
  virtual CContorno::ETipoContorno Contorno ()const
  {
    return CContorno::WEST;
  }

   /**
   * @brief Função herdade da classe CParametroSolver usada para calcular 
    * o valor de x retorna a pressão na fronteira direita
    * Oberve que nao altera o valor de x.
   */
  virtual long double Go (long double d = 0)
  {
    return x;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend

//       friend ostream& operator<< (ostream& os, COGSitioEsqLR& obj);
//       friend istream& operator>> (istream& is, COGSitioEsqLR& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGSitioEsqLR& obj);
// istream& operator>> (istream& is, COGSitioEsqLR& obj);

#endif
