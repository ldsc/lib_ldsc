#ifndef COGSitio_h
#define COGSitio_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       COGSitio.h
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
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CObjetoGrafo_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>
#endif

// ===============================================================================
// Documentacao Classe: COGSitio
// ===============================================================================
/**
 * @brief  Representa um objeto sítio de um grafo.
 *
 * é herdeiro de CObjetoGrafo,
 * tendo uma propriedade x (herdada de CParametroSolver)
 * um rótulo e uma propriedade (herdadas de CObjetoGrafo).
 * A característica básica de um sítio é que este pode ter
 * n coneccao's, ou seja pode estar conectado a n objetos do tipo
 * CObjetoGrafo, mas esta conecção é 1 para 1 (uma garra).
 *
 * Exemplo:
 * Um sítio pode estar conectado a outro sítio.
 * Um sítio pode estar conectado a uma ligação.
 *
 * OBS:
 * Operações de escrita << e leitura >>.
 * Os objetos do tipo  CObjetoGrafo devem apenas salvar e ler
 * suas propriedades internas.
 * A montagem do grafo deve ser realizada pelo objeto CGrafo.
 *
 * @author:  André Duarte Bueno
 * @see:     grafos
 * @todo:    implementar operadores >>.
 * @ingroup  HCObjetoGrafo
*/
class COGSitio : public CObjetoGrafo {
// --------------------------------------------------------------Atributos
public:
     /**
     * @brief Lista de ponteiros para objetos do tipo CObjetoGrafo.
     * Ou seja, ponteiros para objetos da hierarquia CObjetoGrafo.
     * O vetor coneccao é o vetor dos objetos a quem estou conectado.
     * @todo: verificar vantagens de trocar vector por list.
     */
     std::vector < CObjetoGrafo * >coneccao; // default size()=0

// -------------------------------------------------------------Construtor

/// Construtor
     COGSitio () = default;
//   {
     // DEBUG numeroSitios++;
     // DEBUG if( numeroSitios == 1 )
     // DEBUG         {
     // DEBUG         fout.open("imagens/COGSitio.log.txt");
     // DEBUG         if(fout.bad())
     // DEBUG                 {
     // DEBUG       cerr << "\nerro abertura arquivo COGSitio.log.txt"<<endl;
     // DEBUG                 }
     // DEBUG         fout.precision(18);
     // DEBUG         }
//   }

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ COGSitio () = default;
//   {
     // DEBUG if( numeroSitios == 1 )
     // DEBUG         fout.close();
     // DEBUG numeroSitios--;
//   }

// ----------------------------------------------------------------Métodos
     /**
     * @brief Função herdada da classe CParametroSolver,
     * usada para calcular o novo valor de x (pressão).
     * Não muda ovalor de x, apenas gera novo valor!
     */
     virtual long double Go ( long double d = 0 ) override;

     /**
     * @brief Função herdada de CObjetoGrafo, recebe um ponteiro para
     * um CObjetoGrafo, e o inclue na lista de coneccoes's
     */
     virtual void Conectar ( CObjetoGrafo *objA, CObjetoGrafo *objB = nullptr ) override;

     /// Salva o objeto em disco
     virtual std::ostream &Write ( std::ostream &os ) const  override;

     /// Salva atributos do objeto em disco no formato antigo
     virtual std::ostream &Write_Liang_Format ( std::ostream &os ) const  override;

     /// Função que calcula o fluxo associado ao sitio
     virtual long double Fluxo () const  override;

     /// Deleta uma conexão. Recebe um rótulo de um link?
     virtual void DeletarConeccao ( unsigned int link );	// V1

     /**
     * @brief Deleta os links para objetos que foram marcados para deleção.
     * Recebe um número que identifica os objetos que foram marcados
     * para deleção
     */
     virtual bool DeletarConeccoesInvalidadas ( int deletado ); // novo V2

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
     friend std::ostream &operator<< ( std::ostream &os, COGSitio &obj );
//       friend istream& operator>> (istream& is, COGSitio& obj);

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream &operator<< ( std::ostream &os, COGSitio &obj );
// istream& operator>> (istream& is, COGSitio& obj);


// Cria o tipo COGSitio_CENTER, que é igual a COGSitio
// typedef COGSitio COGSitio_CENTER;
using COGSitio_CENTER = COGSitio;

#endif
