/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================
  Desenvolvido por:
                    Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:          André Duarte Bueno
  @file:             CGrafo.cpp
  @begin:            Sat Sep 16 2000
  @copyright:        (C) 2000 by André Duarte Bueno
  @email:            andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>
#include <iomanip>
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Objetos relacionados
#ifndef CMath_h
#include <Base/CMath.h>
#endif

#ifndef CGrafo_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafo.h>
#endif

#ifndef CObjetoGrafo_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>
#endif

#ifndef COGSitio_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio.h>
#endif

#ifndef COGSitio_EST_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_EST.h>
#endif

#ifndef COGSitio_WEST_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_WEST.h>
#endif

#ifndef TCMatriz2D_H
#include <Matriz/TCMatriz2D.h>
#endif

#ifndef CVetor_h
#include <Matriz/CVetor.h>
#endif
using namespace std;

/*
  -------------------------------------------------------------------------
  Função     Construtor
  -------------------------------------------------------------------------
  Define os valores default dos atributos internos.
*/
/*CGrafo::CGrafo( )
  {
  };
*/

// -------------------------------------------------------------------------
/**	Função: CriarObjetoGrafo
   @short  : Cria objeto herdeiro de CObjetoGrafo, de acordo com o tipo solicitado.
   CriarObjetoGrafo será herdada, cada modelo de grafo cria um conjunto diferente de 
   objetos do grafo.
   @author : André Duarte Bueno
   @see    : grafos
   @param  : CContorno::ETipoContorno identificando o tipo de sítio a ser criado.
   			Formato Liang: esquerda(1) centro(0) direita(2)
   			Formato andre(old): esquerda(0) centro(1) direita(2)
   			Formato novo: veja CContorno::ETipoContorno
   @return : Retorna um ponteiro para um sítio novo.

   @todo  Criar enumeração para objetos do grafo, receber como parâmetro o tipo do objeto.
   @todo  Ver livro que fala de padrões de projeto, classe padrão para criar objetos.
*/
CObjetoGrafo* CGrafo::CriarObjetoGrafo (CContorno::ETipoContorno tipoContorno)
{
  CObjetoGrafo *data = nullptr;
  switch ( tipoContorno )
  {
    case CContorno::ETipoContorno::CENTER :
      data = new COGSitio_CENTER ();
      break;

    case CContorno::ETipoContorno::WEST :
      data = new COGSitio_WEST ();
      break;

    case CContorno::ETipoContorno::EST :
      data = new COGSitio_EST ();
      break;

    default:
      data = new COGSitio_CENTER ();
      break;
  }
  assert (data); /// @todo: Implementar bloco try..catch para controle das alocações? static_assert?
  return data;
}

// -------------------------------------------------------------------------
/** @short  : Salva em disco o grafo.
    A saída de dados era no mesmo formato do Liang
    Formato antigo Write_Liang_Format:
    NumerodeSitios
    Tipo de Sitio
    Numero De Links
    Lista dos rótulos dos objetos
    Lista das propriedades dos objetos

    Formato novo (Andre):
    NumeroTotalObjetos 	// salvo pelo grafo
    TipoDoObjeto        // tipo do contorno
    RotuloDoObjeto
    propriedadeDoObjeto	// raio hidraulico ou condutancia
    x           	// propriedade a ser calculada (ex:pressão).
    NumeroConeccoes     // vetor com endereço dos objetos a quem esta conectado.
    Lista_dos_rotulos_das_coneccoes  // note que ainda falta as propriedades das conecções.

    @author : André Duarte Bueno
    @see    : grafos
    @param  : Nome do arquivo de disco (string)
    @return : void
    
    @todo sobrecarregar operador << ??; 
    @todo Write() deve chamar operador;
    @todo implementar versão que recebe uma ostream. 
          Ex: grafo->Write(fout); ou fout <<grafo->Write;
*/
void CGrafo::Write ()
{
  ofstream out ( NomeGrafo().c_str() );
  if ( out.fail() )  {
      cerr << " Não conseguiu abrir o arquivo de disco " << nomeArquivo;
      return;
  }
  out.setf (ios::right);

  // Numero de objetos
  out << setw (5) << objeto.size () << endl;
  
  // Percorre os objetos e salva em disco as informações de cada objeto.
  for ( unsigned long int i = 0; i < objeto.size (); i++ )
  {
      objeto[i]->Write (out);	  // out << (*objeto[i]) << '\n';
      out << '\n';
  }
  out.close ();
}

/** ostream& operator<<(std::ostream& os, CGrafo& grafo)
{
  // Numero de objetos
  out << setw (5) << grafo.objeto.size () << endl;
  
  // Percorre os objetos e salva em disco as informações de cada objeto.
  for ( objeto : grafo.objeto )
  {
      objeto->Write (out);
      out << '\n';
  }
}
*/

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** @short  : Salva dados do objeto em os (disco).
    Salva o número de objetos que fazem parte de grafo e a seguir os dados de cada objeto.
    @author : André Duarte Bueno
    @see    : grafos
    @param  : Objeto do tipo CGrafo.
    @return : ostream&
    @test   : Testar/verificar os diferentes tipos de arquivos de grafo gerados.
    @todo   : O nome do grafo defe indicar o tipo de grafo(nome classe que o gerou).
*/
ostream & operator<< (ostream & os, CGrafo & grafo)
{
  // Numero de objetos do grafo
  os << setw (5) << grafo.objeto.size () << endl;

  // Percorre os objetos e salva em disco as informações de cada objeto
  for ( auto objeto_grafo : grafo.objeto )
    objeto_grafo->Write (os);	// os << (*objeto_grafo);

  return os;
}
