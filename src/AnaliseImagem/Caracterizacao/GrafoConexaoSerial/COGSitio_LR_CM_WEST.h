#ifndef COGSitioEsqLRCM_h
#define COGSitioEsqLRCM_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: COGSitioEsqLRCM...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico 
            [LDSC].
@author     André Duarte Bueno
@file       COGSitioEsqLRCM.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitioLRCM.h>

// ===============================================================================
// Documentacao CLASSE: COGSitioEsqLRCM
// ===============================================================================
/** 
 * @brief Representa um sítio conectado a face esquerda do grafo e que 
 * armazena a informação da conexão.
 * 
 * Assume valor de contorno = CContorno::WEST.
 * O valor de contorno é usado no calculo dos fluxo da malha como
 * um todo. Ou seja, um algoritimo externo percorre todo
 * o grafo, e se o contorno for igual ao solicitado
 * calcula alguma propriedade neste contorno.
 * @see:        Grafo
 * Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitioLR->COGSitioEsqLR
 * @author 	André Duarte Bueno
*/
class COGSitioEsqLRCM : public COGSitioLRCM
{
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
/// Construtor
	COGSitioEsqLRCM ()
  {
  }

// --------------------------------------------------------------Destrutor
/// Destrutor
    virtual ~ COGSitioEsqLRCM ()
  {
  }

// ----------------------------------------------------------------Métodos

public:
   /// Retorna o tipo 
virtual CContorno::ETipoContorno Contorno ()const
  {
    return CContorno::WEST;
  }

   /**
    * @brief Função herdade da classe CParametroSolver usada para 
    * calcular o valor de x como é um objeto da fronteira, 
    * não altera seu valor de x 
   */
  virtual long double Go (long double d = 0)
  {
    return x;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend

//       friend ostream& operator<< (ostream& os, COGSitioEsqLRCM& obj);
//       friend istream& operator>> (istream& is, COGSitioEsqLRCM& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGSitioEsqLRCM& obj);
// istream& operator>> (istream& is, COGSitioEsqLRCM& obj);

#endif
