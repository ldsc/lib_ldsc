/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          André Duarte Bueno
@file:             CObjetoGrafo_MatrizConexoes.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iomanip>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo_MatrizConexoes.h>

// -------------------------------------------------------------------------
// Função:       Conectar
// -------------------------------------------------------------------------
/** Função : Conectar
 * @see    : Adiciona ao vetor conexão o vetor passado
 * @author:  André Duarte Bueno
 * @param  : recebe um vetor de  CObjetoGrafo*
 * @return : void
 * @test   : testar!
*/
void CObjetoGrafo_MatrizConexoes::Conectar (vector < CObjetoGrafo * >obj_vetor)
{
  conexao.push_back (obj_vetor);
}

// -------------------------------------------------------------------------
// Função:       DeletarConeccao
// -------------------------------------------------------------------------
/** Deleta a conexao de um ramo morto, note que esta deletando um vetor inteiro.
    @short  : Deleta a conexao de um ramo morto
    @author : André Duarte Bueno
    @see    : 
    @param  : unsigned int link
    @return : void
    @test   : testar!
*/
void CObjetoGrafo_MatrizConexoes::DeletarConeccao (unsigned int ivetor)
{
  this->conexao.erase ( conexao.begin() + ivetor );
}

void CObjetoGrafo_MatrizConexoes::DeletarConeccao ( unsigned int ivetor, unsigned int link ) 
{
  conexao[ivetor].erase( conexao[ivetor].begin() + link );
}

/** Marca e deleta as conexões para objetos invalidados (marcados para deleção).
 * Funciona assim: percorre os objetos das conexões,
 * se o rótulo do objeto correspond	e a rótulo válido (não deletado),
 * então a conexão é preservada.
 * Já os objetos que foram marcados para deleção são desconsiderados(deletados);
 * isto é, se a conexão foi deletada, aqui ela é desconsiderada (apagada).
    @short  : Deleta a conexao de um ramo morto
    @author : André Duarte Bueno
    @see    : 
    @param  : unsigned int link
    @return : void
    @todo   : Pode-se otimizar o consumo de memória eliminando objetos deletados após resize.
    @test   : testar!
*/
bool CObjetoGrafo_MatrizConexoes::DeletarConeccoesInvalidadas (unsigned int deletado)
{
  unsigned int indice_rotulo_valido {0};

  // Percorre todas as coneccoes
  for ( auto vobjeto_conectado: conexao )
  {
   indice_rotulo_valido = 0 ;
   for ( auto objeto_conectado: vobjeto_conectado )
    // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
    // Se foi deletado vai ser pulado.
    if (objeto_conectado->rotulo != deletado)
      {
       vobjeto_conectado[indice_rotulo_valido++] = objeto_conectado;
      }
  // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
  vobjeto_conectado.resize (indice_rotulo_valido);
  }
  return 1;
}

// ------------------------------------------------------------------------------
// Funcao:     Write
// ------------------------------------------------------------------------------
/** Salva o grafo em disco (em fout)
@see  		:
@author:    André Duarte Bueno
@param		: Recebe uma ofstream& fout
@return   : ostream&
@test   : testar!
*/
ostream & CObjetoGrafo_MatrizConexoes::Write (ostream & out) const
{
    // Tipo de contorno
    out << setw (4) << static_cast<unsigned char>( Contorno() ) << '\n';


    // Rótulo de this
    out << ' ' << setw (5) << rotulo;
	
    // Número de vetores coneccoes
    out << ' ' << setw (4) << conexao.size ();

    // Para cada vetor percorrer a lista
    for (unsigned long int cont_coneccoes= 0; cont_coneccoes < conexao.size (); cont_coneccoes++)
      {
        // Para cada vetor percorrer os objetos
        // conexao[cont_vector] retorna um ponteiro para um vetor
        out << " " << setw (4) << conexao[cont_coneccoes].size ();

        // lista dos rótulos
        for ( auto objeto_conectado :  conexao[cont_coneccoes] )
          out << " " << setw (4) << objeto_conectado->rotulo;

//         // lista das propriedades (condutancias)
//         for ( auto objeto_conectado :  conexao[cont_coneccoes] )
//           out << " " << setw (4) << objeto_conectado->propriedade;
  }
  return out;
}

// ------------------------------------------------------------------------------
// Funcao:     operator<<
// ------------------------------------------------------------------------------
ostream& operator<< (ostream& out, CObjetoGrafo_MatrizConexoes& obj)
{
 obj.Write(out);
 return out;
}
