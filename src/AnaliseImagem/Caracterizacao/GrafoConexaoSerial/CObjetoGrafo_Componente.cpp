/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          André Duarte Bueno
@file:             CObjetoGrafo_Componente.cpp
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo_Componente.h>

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
void CObjetoGrafo_Componente::Conectar (vector < CObjetoGrafo * >obj_vetor)
{
  coneccao.push_back (obj_vetor);
}

// -------------------------------------------------------------------------
// Função:       DeletarConeccao
// -------------------------------------------------------------------------
/** Deleta a coneccao de um ramo morto, note que esta deletando um vetor inteiro.
    @short  : Deleta a coneccao de um ramo morto
    @author : André Duarte Bueno
    @see    : 
    @param  : unsigned int link
    @return : void
    @test   : testar!
*/
void CObjetoGrafo_Componente::DeletarConeccao (unsigned int ivetor)
{
  this->coneccao.erase ( coneccao.begin() + ivetor );
}

void CObjetoGrafo_Componente::DeletarConeccao ( unsigned int ivetor, unsigned int link ) 
{
  coneccao[ivetor].erase( coneccao[ivetor].begin() + link );
}

/** Marca e deleta as conecções para objetos invalidados (marcados para deleção).
 * Funciona assim: percorre os objetos das conecções,
 * se o rótulo do objeto correspond	e a rótulo válido (não deletado),
 * então a conexão é preservada.
 * Já os objetos que foram marcados para deleção são desconsiderados(deletados);
 * isto é, se a conecção foi deletada, aqui ela é desconsiderada (apagada).
    @short  : Deleta a coneccao de um ramo morto
    @author : André Duarte Bueno
    @see    : 
    @param  : unsigned int link
    @return : void
    @todo   : Pode-se otimizar o consumo de memória eliminando objetos deletados após resize.
    @test   : testar!
*/
bool CObjetoGrafo_Componente::DeletarConeccoesInvalidadas (int deletado)
{
  unsigned int indice_rotulo_valido {0};

  // Percorre todas as coneccoes
  for ( auto vobjeto_conectado: coneccao )
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
ostream & CObjetoGrafo_Componente::Write (ostream & out) const
{
    // Tipo de contorno
    out << setw (4) << static_cast<unsigned char>( Contorno() ) << '\n';


    // Rótulo de this
    out << ' ' << setw (5) << rotulo;
	
    // Número de vetores coneccoes
    out << ' ' << setw (4) << coneccao.size ();

    // Para cada vetor percorrer a lista
    for (unsigned long int cont_coneccoes= 0; cont_coneccoes < coneccao.size (); cont_coneccoes++)
      {
        // Para cada vetor percorrer os objetos
        // coneccao[cont_vector] retorna um ponteiro para um vetor
        out << " " << setw (4) << coneccao[cont_coneccoes].size ();

        // lista dos rótulos
        for ( auto objeto_conectado :  coneccao[cont_coneccoes] )
          out << " " << setw (4) << objeto_conectado->rotulo;

//         // lista das propriedades (condutancias)
//         for ( auto objeto_conectado :  coneccao[cont_coneccoes] )
//           out << " " << setw (4) << objeto_conectado->propriedade;
  }
  return out;
}

// ------------------------------------------------------------------------------
// Funcao:     operator<<
// ------------------------------------------------------------------------------
ostream& operator<< (ostream& out, CObjetoGrafo_Componente& obj)
{
 obj.Write(out);
 return out;
}
