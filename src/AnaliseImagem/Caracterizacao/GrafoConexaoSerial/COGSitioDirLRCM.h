#ifndef COGSitioDirLRCM_h
#define COGSitioDirLRCM_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: COGSitioDirLRCM...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico 
            [LDSC].
@author     André Duarte Bueno
@file       COGSitioDirLRCM.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitioLRCM.h>

// ===============================================================================
// Documentacao CLASSE: COGSitioDirLRCM
// ===============================================================================
/** 
 * @brief  Representa um sítio que armazena a informação associada a conexão.
 * @short      	redefine funcao Contorno
 * @author 	André Duarte Bueno
 * @see		CGrafo
 * @Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitioLR->COGSitioDirLR->COGSitioDirLRCM
 */
class COGSitioDirLRCM : public COGSitioLRCM
{
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
/// Construtor
	COGSitioDirLRCM ()
  {
  }

// --------------------------------------------------------------Destrutor
/// Destrutor  
  virtual ~ COGSitioDirLRCM ()
  {
  }

// ----------------------------------------------------------------Métodos
public:
  // Redefinição de funções herdadas
   /// Retorna o tipo de contorno
  virtual CContorno::ETipoContorno Contorno ()const	// Tipo()
  {
    return CContorno::EST;
  }

   /** 
    * @brief Função herdada da classe CParametroSolver,
    * usada para calcular o valor de x.
    * Observe que retorna direto o valor de x, ou seja,
    * um objeto da fronteira não recalcula seu fluxo*/
  virtual long double Go (long double d = 0)
  {
    return x;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend ostream& operator<< (ostream& os, COGSitioDirLRCM& obj);
  // friend istream& operator>> (istream& is, COGSitioDirLRCM& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGSitioDirLRCM& obj);
// istream& operator>> (istream& is, COGSitioDirLRCM& obj);

#endif
