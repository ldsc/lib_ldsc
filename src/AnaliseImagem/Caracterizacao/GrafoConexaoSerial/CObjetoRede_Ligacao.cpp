/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          André Duarte Bueno
@file:             CObjetoRede_Ligacao.cpp
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Ligacao.h>

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
void CObjetoRede_Ligacao::Conectar (CObjetoRede * objA, CObjetoRede * objB)
{
  coneccaoA.push_back (objA);
  coneccaoB.push_back (objB);
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
void CObjetoRede_Ligacao::DeletarConeccao (unsigned int link)
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
bool CObjetoRede_Ligacao::DeletarConeccoesInvalidadas (int deletado)
{
  unsigned int indice_rotulo_valido {0};
  
  // Percorre todas as coneccoes
  for ( auto objeto_conectado: coneccaoA )
    // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
    // Se foi deletado vai ser pulado.
    if (objeto_conectado->rotulo != deletado)
      {
       coneccaoA[indice_rotulo_valido++] = objeto_conectado;
      }
  // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
  coneccaoA.resize (indice_rotulo_valido);

  // Percorre todas as coneccoes
  for ( auto objeto_conectado: coneccaoB )
    // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
    // Se foi deletado vai ser pulado.
    if (objeto_conectado->rotulo != deletado)
      {
       coneccaoB[indice_rotulo_valido++] = objeto_conectado;
      }
  // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
  coneccaoB.resize (indice_rotulo_valido);
  
  /// @todo: aqui pode apagar, usando erase, os objetos além do size().
  return 1;
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
long double CObjetoRede_Ligacao::Fluxo () const 
{
  long double fluxo { 0.0 };
  static long double condutanciaEntreObjetosConectados;

  CObjetoRede* objeto_conectado_a = nullptr;
  CObjetoRede* objeto_conectado_b = nullptr;
  
  for (unsigned long int i = 0; i < coneccaoA.size (); i++)
  {
   objeto_conectado_a  = dynamic_cast<CObjetoRede*>(coneccaoA[i]);
   objeto_conectado_b  = dynamic_cast<CObjetoRede*>(coneccaoB[i]);
   condutanciaEntreObjetosConectados =  1.0 / 
   (1.0/objeto_conectado_a->propriedade + 1.0/this->propriedade + 1.0/objeto_conectado_b->propriedade);
   // o fluxo é a condutancia total entre objetos vezes a diferença de x(pressão) dos objetos
   // a quem this esta conectado.
   fluxo += condutanciaEntreObjetosConectados * (objeto_conectado_a->x - objeto_conectado_b->x);
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
ostream & CObjetoRede_Ligacao::Write (ostream & out) const
{
     out.setf ( ios::right );

     // Tipo de contorno
     /// @todo trocar por tipo ojeto grafo!
     // fout << setw (4) << Contorno ();
     out << setw ( 4 ) << static_cast<unsigned char> ( CObjetoRede::Contorno() ) << '\n';

     // Rótulo de this
     out << ' ' << setw ( 5 ) << CObjetoRede::rotulo;

     // propriedade de this (condutancia)
     out << ' ' << setw ( 10 ) << propriedade;

     // x de this (pressão)
     out << ' ' << setw ( 10 ) << x;

     // Numero de links do sítio
     out << ' ' << setw ( 4 ) << coneccaoA.size ();

     // CONECCAO A
     // lista dos rótulos
     for ( auto objeto_conectado : coneccaoA )
          out << ' ' << setw ( 4 ) << objeto_conectado->rotulo;

     CObjetoRede* objeto_conectado = nullptr;
     // lista das propriedades (condutancias)
     for ( auto objeto : coneccaoA ) {
          objeto_conectado  = dynamic_cast<CObjetoRede*> ( objeto );
          out << ' ' << setw ( 10 ) << objeto_conectado->propriedade;
     }

     // CONECCAO B
     // lista dos rótulos
     for ( auto objeto_conectado : coneccaoB )
          out << ' ' << setw ( 4 ) << objeto_conectado->rotulo;

     // lista das propriedades (condutancias)
     for ( auto objeto : coneccaoB ) {
          objeto_conectado  = dynamic_cast<CObjetoRede*> ( objeto );
          out << ' ' << setw ( 10 ) << objeto_conectado->propriedade;
     }
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
