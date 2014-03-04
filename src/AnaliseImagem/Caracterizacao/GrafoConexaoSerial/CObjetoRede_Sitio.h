#ifndef CObjetoRede_Sitio_h
#define CObjetoRede_Sitio_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_Sitio.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// Interface disco
#include <fstream>

// Classe container para vetores
#include <vector>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Definição de CObjetoGrafo
#ifndef CObjetoRede_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoRede_Sitio
// ===============================================================================
/**
 * @brief Representa um objeto sítio de uma rede.
 * Tendo uma variável rotulo (herdade de CObjetoGrafo)
 * x (herdada de CParametroSolver), e uma propriedade (herdada de CObjetoRede).
 * @author:  André Duarte Bueno
 * @see:     grafos
 * @todo:    implementar operadores >>.
 * @ingroup  HCObjetoGrafo
*/

class CObjetoRede_Sitio :  public CObjetoRede
{
public:
// --------------------------------------------------------------Atributos
     /**
     * @brief Lista de ponteiros para objetos do tipo CObjetoGrafo.
     * Ou seja, ponteiros para objetos da hierarquia CObjetoGrafo.
     * O vetor conexao é o vetor dos objetos a quem estou conectado.
     * @todo: verificar vantagens de trocar vector por list.
     */
     std::vector < CObjetoRede * >conexao;

// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoRede_Sitio () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoRede_Sitio () = default;

// ----------------------------------------------------------------Métodos
     /**
       * @brief Função que recebe um ponteiro para um CObjetoRede,
       * e o inclue na lista de conexões. Lista dos objetos a quem estou conectado.
	   * NOTA: mesmo código de CObjetoRede_Sitio
     */
     /*inline*/ virtual void Conectar ( CObjetoRede *objA, CObjetoRede */*objB = nullptr*/ ) override ;

     /// Deleta uma conexão. NOTA: mesmo código de CObjetoRede_Sitio
     /*inline*/ virtual void DeletarConeccao ( unsigned int link ) override ;

     /**
     * @brief Deleta os links para objetos que foram marcados para deleção.
     * Recebe um número que identifica os objetos que foram marcados
     * para deleção, se o rótulo dos objetos conectados é igual a este parâmetro
	 * a conexão é eliminada.
	 * NOTA: mesmo código de CObjetoGrafo_1VetorConexoes
     */
     /*inline*/ virtual bool DeletarConeccoesInvalidadas ( unsigned int deletado ) override ; 
	 
     /// @brief Salva atributos do objeto em disco.
     virtual std::ostream &Write ( std::ostream &os ) const override ;

     /**
     * @brief Função usada para calcular uma propriedade.
     */
     virtual long double Go ( long double d = 0 ) override;

	 /// Determina o fluxo associado a this, função herdada.
     virtual long double Fluxo () const override;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
     friend std::ostream &operator<< ( std::ostream &os, CObjetoRede_Sitio &obj );
//       friend istream& operator>> (istream& is, CObjetoRede_Sitio& obj);

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream &operator<< ( std::ostream &os, CObjetoRede_Sitio &obj );
// istream& operator>> (istream& is, CObjetoRede_Sitio& obj);


// Cria o tipo CObjetoRede_Sitio_CENTER, que é igual a CObjetoRede_Sitio
// typedef CObjetoRede_Sitio CObjetoRede_Sitio_CENTER;
using CObjetoRede_Sitio_CENTER = CObjetoRede_Sitio;

#endif
