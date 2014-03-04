/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          André Duarte Bueno
@file:             CObjetoGrafo_LigacaoMultipla.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <iomanip>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo_LigacaoMultipla.h>

/**
-------------------------------------------------------------------------
Função:
-------------------------------------------------------------------------
@short  : Adiciona os dois ponteiros recebidos objA e objB aos vetores de this.
@author : André Duarte Bueno
@see    : 
@param  : CObjetoGrafo * objA, CObjetoGrafo * objB
@return : void
*/
void CObjetoGrafo_LigacaoMultipla::Conectar (CObjetoGrafo * objA, CObjetoGrafo * objB)
{
  this->conexaoA.push_back (objA);
  this->conexaoB.push_back (objB);
}

// -------------------------------------------------------------------------
// Função:       DeletarConeccao
// -------------------------------------------------------------------------
/** Deleta a conexao de um ramo morto
    @short  :		Deleta a conexao de um ramo morto
    @author :		André Duarte Bueno
    @see    :	
    @param  : 	unsigned int link
    @return :		void
*/
void CObjetoGrafo_LigacaoMultipla::DeletarConeccao (unsigned int link)
{
  this->conexaoA.erase ( conexaoA.begin() + link );
  this->conexaoB.erase ( conexaoB.begin() + link );
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
*/
bool CObjetoGrafo_LigacaoMultipla::DeletarConeccoesInvalidadas (unsigned int deletado)
{
 return DeletarConeccoesInvalidadas_aux ( deletado , conexaoA ) &&
        DeletarConeccoesInvalidadas_aux ( deletado , conexaoB );

// Como o códugo abaixo repete, mudando apenas conexaoA e conexaoB, criei função comum e movi para base.
//   unsigned int indice_rotulo_valido {0};
//   
//   // Percorre todas as coneccoes
//   for ( auto objeto_conectado: conexaoA )
//     // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
//     // Se foi deletado vai ser pulado.
//     if (objeto_conectado->rotulo != deletado)
//       {
//        conexaoA[indice_rotulo_valido++] = objeto_conectado;
//       }
//   // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
//   conexaoA.resize (indice_rotulo_valido);
// 
//   // Percorre todas as coneccoes
//   for ( auto objeto_conectado: conexaoB )
//     // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
//     // Se foi deletado vai ser pulado.
//     if (objeto_conectado->rotulo != deletado)
//       {
//        conexaoB[indice_rotulo_valido++] = objeto_conectado;
//       }
//   // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
//   conexaoB.resize (indice_rotulo_valido);
return 1;
}

/**
-------------------------------------------------------------------------
Função:     Write
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em out
@author : André Duarte Bueno
@see    :
@param  : ofstream& out
@return : ostream&
*/
ostream & CObjetoGrafo_LigacaoMultipla::Write (ostream & out) const
{
    out.setf (ios::right);
    // Tipo de contorno
    /// @todo trocar por tipo ojeto grafo!
    out << setw (4) << static_cast<unsigned char>( Contorno() ) << '\n';

    // Rótulo de this
    out << ' ' << setw (5) << rotulo;

    // Numero de conexões do sítio     
    out << ' ' << setw (4) << conexaoA.size ();

    // CONECCAO A
    // lista dos rótulos
    for ( auto objeto : conexaoA )
      out << ' ' << setw (4) << objeto->rotulo;

    // CONECCAO B
    // lista dos rótulos
    for ( auto objeto : conexaoB )
      out << ' ' << setw (4) << objeto->rotulo;

    return out;	
}

/**
-------------------------------------------------------------------------
Função:  operator<<
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em fout
@author : André Duarte Bueno
@see    :
@param  : ostream& fout, CObjetoGrafo_LigacaoMultipla& s
@return : ostream&
*/
ostream & operator<< (ostream & fout, CObjetoGrafo_LigacaoMultipla & s)
{
  s.Write(fout);
  return fout;
}

/*
-------------------------------------------------------------------------
Função:    operator>>
-------------------------------------------------------------------------
@short  : Lê as propriedades do objeto a partir de arquivo os 		
					(normalmente de disco)
@author :	André Duarte Bueno
@see    :
@param  :
@return :
*/
/*istream& operator>> (istream& is, CObjetoGrafo_LigacaoMultipla& s)
{
  s.Read(is);	
  return is;
}
*/
