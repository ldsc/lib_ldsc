/** 
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:    André Duarte Bueno
@file:      CObjetoRede_Ligacao.cpp
@begin:     Sat Sep 16 2000
@copyright: (C) 2000 by André Duarte Bueno
@email:     andreduartebueno@gmail.com
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Ligacao.h>

/**
-------------------------------------------------------------------------
Função:
-------------------------------------------------------------------------
@short  : Fas esta ligação apontar para os objetos recebidos objA e objB 
          Note que conexões pré existentes deixam de existir pois uma ligação
          só pode apontar para dois objetos!
@author : André Duarte Bueno
@see    : 
@param  : CObjetoRede * objA, CObjetoRede * objB
@return : void
*/
void CObjetoRede_Ligacao::Conectar (CObjetoRede * objA, CObjetoRede * objB)
{
  conexao[0] = objA;
  conexao[1] = objB;
}

// -------------------------------------------------------------------------
// Função:       DeletarConeccao
// -------------------------------------------------------------------------
/** Deleta as conexões. 
 *  Note que como esta ligação não sabe qual objeto conectado chamou esta função, deleta as duas conexões.
    @short  :		Deleta a conexao de um ramo morto
    @author :		André Duarte Bueno
    @see    :	
    @param  : 	unsigned int link
    @return :		void
    @todo   : testar funcionamento!
*/
void CObjetoRede_Ligacao::DeletarConeccao (unsigned int link)
{
// conexao.clear(); //deveria deletar logo as duas! zerando o size do vetor conexao.
  this->conexao.erase ( conexao.begin() + link );
}

/** Deleta as conexões.
    @short  : Deleta as conexões e marca esta ligação para deleção, pois se um dos objetos 
    ao qual estou conectado foi deletado, esta ligação deve ser deletada.
    @author : André Duarte Bueno
    @see    : 
    @param  : unsigned int link
    @return : void
    @todo   : testar funcionamento!
*/
bool CObjetoRede_Ligacao::DeletarConeccoesInvalidadas (unsigned int deletado)
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

/**
-------------------------------------------------------------------------
Função:
-------------------------------------------------------------------------
@short  :   Calcula condutancia considerando eq.XX, ou seja,
            cond = 1/(1/condutancia_sitio_esq + 1/condutancia_this + 1/condutancia_sitio_dir).
@author :   André Duarte Bueno
@see    :
@param  :   nada
@return :   long double, o fluxo associado a ligação
@bug    :   se as conexões foram deletadas aqui ocorre um bug!
*/
long double CObjetoRede_Ligacao::Fluxo () const 
{
  long double fluxo { 0.0 };
  static long double condutanciaEntreObjetosConectados;

   condutanciaEntreObjetosConectados =  1.0 /
   (1.0/conexao[0]->propriedade + 1.0/this->propriedade + 1.0/conexao[1]->propriedade);

   // o fluxo é a condutancia total entre objetos vezes a diferença de x(pressão) dos objetos
   // a quem this esta conectado.
   fluxo += condutanciaEntreObjetosConectados * (conexao[0]->x - conexao[1]->x);

  return fluxo;
}

/**
-------------------------------------------------------------------------
Função:     Write
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em fout
@author : André Duarte Bueno
@see    :
@param  : ofstream& fout
@return : ostream&
*/
ostream & CObjetoRede_Ligacao::Write (ostream & out) const
{
     out.setf ( ios::right );

     // Tipo de contorno
     out << setw ( 4 ) << static_cast<unsigned char> ( Contorno() ) << '\n';

     // Rótulo de this
     // out << ' ' << setw ( 5 ) << CObjetoRede::rotulo;
     out << ' ' << setw ( 5 ) << rotulo;

     // x de this (pressão)
     out << ' ' << setw ( 10 ) << x;

	 // propriedade de this (condutancia)
     out << ' ' << setw ( 10 ) << propriedade;

     // Numero de conexões
	 // como é ligação temos ramos duplos e o size de conexaoA é o mesmo de conexaoB
     out << ' ' << setw ( 4 ) << 2;

     // Dados da conexaoA
     out << ' ' << setw ( 4 ) << conexao[0]->rotulo;

     // Dados da conexaoB
     out << ' ' << setw ( 4 ) << conexao[1]->rotulo;

	 return out;
}

/**
-------------------------------------------------------------------------
Função:  operator<<
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em fout
@author : André Duarte Bueno
@see    :
@param  : ostream& fout, CObjetoRede_Ligacao& s
@return : ostream&
*/
ostream & operator<< (ostream & out, CObjetoRede_Ligacao & s)
{
  s.Write(out);
  return out;
}
