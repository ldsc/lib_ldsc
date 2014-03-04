/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:    André Duarte Bueno
@file:      CObjetoRede_LigacaoMultipla.cpp
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_LigacaoMultipla.h>

/**
-------------------------------------------------------------------------
Função:
-------------------------------------------------------------------------
@short  : Adiciona os dois ponteiros recebidos objA e objB aos vetores de this.
@author : André Duarte Bueno
@see    : 
@param  : CObjetoRede * objA, CObjetoRede * objB
@return : void
*/
void CObjetoRede_LigacaoMultipla::Conectar (CObjetoRede * objA, CObjetoRede * objB)
{
  conexaoA.push_back (objA);
  conexaoB.push_back (objB);
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
void CObjetoRede_LigacaoMultipla::DeletarConeccao (unsigned int link)
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
bool CObjetoRede_LigacaoMultipla::DeletarConeccoesInvalidadas (unsigned int deletado)
{
 return DeletarConeccoesInvalidadas_aux ( deletado , conexaoA ) &&
        DeletarConeccoesInvalidadas_aux ( deletado , conexaoB );
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
*/
long double CObjetoRede_LigacaoMultipla::Fluxo () const 
{
  long double fluxo { 0.0 };
  static long double condutanciaEntreObjetosConectados;

  for (unsigned long int i = 0; i < conexaoA.size (); i++)
  {
   condutanciaEntreObjetosConectados =  1.0 /
   (1.0/conexaoA[i]->propriedade + 1.0/this->propriedade + 1.0/conexaoB[i]->propriedade);

   // o fluxo é a condutancia total entre objetos vezes a diferença de x(pressão) dos objetos
   // a quem this esta conectado.
   fluxo += condutanciaEntreObjetosConectados * (conexaoA[i]->x - conexaoB[i]->x);
  }
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
ostream & CObjetoRede_LigacaoMultipla::Write (ostream & out) const
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
     out << ' ' << setw ( 4 ) << conexaoA.size ();

     // Dados da conexaoA
     // lista dos rótulos
     for ( auto objeto_conectado : conexaoA )
          out << ' ' << setw ( 4 ) << objeto_conectado->rotulo;

     // Dados da conexaoB
     // lista dos rótulos
     for ( auto objeto_conectado : conexaoB )
          out << ' ' << setw ( 4 ) << objeto_conectado->rotulo;

	 return out;
}

/**
-------------------------------------------------------------------------
Função:  operator<<
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em fout
@author : André Duarte Bueno
@see    :
@param  : ostream& fout, CObjetoRede_LigacaoMultipla& s
@return : ostream&
*/
ostream & operator<< (ostream & out, CObjetoRede_LigacaoMultipla & s)
{
  s.Write(out);
  return out;
}
