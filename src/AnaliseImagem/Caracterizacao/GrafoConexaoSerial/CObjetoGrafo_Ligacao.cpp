/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          André Duarte Bueno
@file:             CObjetoGrafo_Ligacao.cpp
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo_Ligacao.h>

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
void CObjetoGrafo_Ligacao::Conectar (CObjetoGrafo * objA, CObjetoGrafo * objB)
{
  this->coneccaoA.push_back (objA);
  this->coneccaoB.push_back (objB);
}

// -------------------------------------------------------------------------
// Função:       DeletarConeccao
// -------------------------------------------------------------------------
/** Deleta a coneccao de um ramo morto
    @short  :		Deleta a coneccao de um ramo morto
    @author :		André Duarte Bueno
    @see    :	
    @param  : 	unsigned int link
    @return :		void
*/
void CObjetoGrafo_Ligacao::DeletarConeccao (unsigned int link)
{
  this->coneccaoA.erase ( coneccaoA.begin() + link );
  this->coneccaoB.erase ( coneccaoB.begin() + link );
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
*/
bool CObjetoGrafo_Ligacao::DeletarConeccoesInvalidadas (int deletado)
{
 return DeletarConeccoesInvalidadas_aux ( deletado , coneccaoA ) &&
        DeletarConeccoesInvalidadas_aux ( deletado , coneccaoB );

// Como o códugo abaixo repete, mudando apenas coneccaoA e coneccaoB, criei função comum e movi para base.
//   unsigned int indice_rotulo_valido {0};
//   
//   // Percorre todas as coneccoes
//   for ( auto objeto_conectado: coneccaoA )
//     // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
//     // Se foi deletado vai ser pulado.
//     if (objeto_conectado->rotulo != deletado)
//       {
//        coneccaoA[indice_rotulo_valido++] = objeto_conectado;
//       }
//   // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
//   coneccaoA.resize (indice_rotulo_valido);
// 
//   // Percorre todas as coneccoes
//   for ( auto objeto_conectado: coneccaoB )
//     // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
//     // Se foi deletado vai ser pulado.
//     if (objeto_conectado->rotulo != deletado)
//       {
//        coneccaoB[indice_rotulo_valido++] = objeto_conectado;
//       }
//   // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
//   coneccaoB.resize (indice_rotulo_valido);
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
ostream & CObjetoGrafo_Ligacao::Write (ostream & out) const
{
    out.setf (ios::right);
    // Tipo de contorno
    /// @todo trocar por tipo ojeto grafo!
    out << setw (4) << static_cast<unsigned char>( Contorno() ) << '\n';

    // Rótulo de this
    out << ' ' << setw (5) << rotulo;

    // Numero de conexões do sítio     
    out << ' ' << setw (4) << coneccaoA.size ();

    // CONECCAO A
    // lista dos rótulos
    for ( auto objeto : coneccaoA )
      out << ' ' << setw (4) << objeto->rotulo;

    // CONECCAO B
    // lista dos rótulos
    for ( auto objeto : coneccaoB )
      out << ' ' << setw (4) << objeto->rotulo;
}

/**
-------------------------------------------------------------------------
Função:  operator<<
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em fout
@author : André Duarte Bueno
@see    :
@param  : ostream& fout, CObjetoGrafo_Ligacao& s
@return : ostream&
*/
ostream & operator<< (ostream & fout, CObjetoGrafo_Ligacao & s)
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
/*istream& operator>> (istream& is, CObjetoGrafo_Ligacao& s)
{
  s.Read(is);	
  return is;
}
*/
