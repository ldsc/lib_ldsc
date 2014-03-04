/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          André Duarte Bueno
@file:             CObjetoGrafo_2VetoresConexoes.cpp
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo_2VetoresConexoes.h>

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
void CObjetoGrafo_2VetoresConexoes::Conectar (CObjetoGrafo * objA, CObjetoGrafo * objB)
{
//   conexaoA = objA;
//   conexaoB = objB;
  conexao[0] = objA;
  conexao[1] = objB;
  
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
void CObjetoGrafo_2VetoresConexoes::DeletarConeccao (unsigned int link)
{
// conexao.clear(); //deveria deletar logo as duas! zerando o size do vetor conexao.
  this->conexao.erase ( conexao.begin() + link );	
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
bool CObjetoGrafo_2VetoresConexoes::DeletarConeccoesInvalidadas (unsigned int deletado)
{
//    if( conexao[0]->rotulo == deletado or conexao[1]->rotulo == deletado )
//      { 
//  		conexao[0] = nullptr;
//  		conexao[1] = nullptr;
//  		rotulo = deletado;  // como as conexões foram deletadas, this deve ser marcado para deleção
//  	}
//   return 1;
   return DeletarConeccoesInvalidadas_aux ( deletado , conexao );
}

// /**
// -------------------------------------------------------------------------
// Função:     Write
// -------------------------------------------------------------------------
// @short  : Escreve propriedades do objeto em out
// @author : André Duarte Bueno
// @see    :
// @param  : ofstream& out
// @return : ostream&
// */
// ostream & CObjetoGrafo_2VetoresConexoes::Write (ostream & out) const
// {
//     out.setf (ios::right);
//     // Tipo de contorno
//     /// @todo trocar por tipo ojeto grafo!
//     out << setw (4) << static_cast<unsigned char>( Contorno() ) << '\n';
// 
//     // Rótulo de this
//     out << ' ' << setw (5) << rotulo;
// 
//     // Numero de conexões do sítio     
//     out << ' ' << setw (4) << conexao[0].size ();
// 
//     // CONECCAO 
//     // lista dos rótulos
//     for ( auto objeto : conexao )
//       out << ' ' << setw (4) << objeto->rotulo;
// 
// //     // CONECCAO B
// //     // lista dos rótulos
// //     for ( auto objeto : conexao[1] )
// //       out << ' ' << setw (4) << objeto->rotulo;
// 
//     return out;	
// }

/**
-------------------------------------------------------------------------
Função:  operator<<
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em fout
@author : André Duarte Bueno
@see    :
@param  : ostream& fout, CObjetoGrafo_2VetoresConexoes& s
@return : ostream&
*/
ostream & operator<< (ostream & fout, CObjetoGrafo_2VetoresConexoes & s)
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
/*istream& operator>> (istream& is, CObjetoGrafo_2VetoresConexoes& s)
{
  s.Read(is);	
  return is;
}
*/
