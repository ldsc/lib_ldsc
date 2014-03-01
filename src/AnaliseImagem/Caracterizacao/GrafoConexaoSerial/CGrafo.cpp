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
// Função:   CalcularCondutancias
// -------------------------------------------------------------------------
/** Função:   CalcularCondutancias (ex: converte raio hidraulico em condutância).
    @short:   Todo objeto do grafo tem uma propriedade que é armazenada.
    A função CalcularCondutancias é uma função criada para alteração
    desta propriedade levando em conta os fenômenos que se deseja estudar.

    Específico:
    No caso específico do cálculo da permeabilidade de representações tridimensionais,
    transforma a propriedade raioHidraulico dos "sítios" em condutância.
    Esta condutância é usada pelo objeto "sítio" para calcular a sua pressão (x).

    Ou seja:
    Quando se determina o grafo a propriedade armazenada nos objetos é o raio hidraulico.
    Quando se deseja determinar a permeabilidade a propriedade armazenada é a condutancia.

    @todo    Verificar uma forma de eliminar a dependencia destes parâmetros.
    @todo    Verificar possibilidade de mover para classe CPermeabilidadeGrafo.

    Note que a função esta calculando a condutancia segundo a lei de Poiselle -> para ligações 
    (eq 5.16 da tese Liang).
    condutancia=       ( CMath::PI * dH * dH * dH * dH )
                                       /
                (128.0 * _viscosidade * _sizePixel * _fatorAmplificacao );

    Abaixo a equacao da condutancia para sitios segundo Koplik (1983), eq 5.17 da tese do Liang
    Calcula a condutancia do sitio usando a equação ri^3/(3*viscosidade)
    condutancia=(raio_hidraulico*raio_hidraulico*raio_hidraulico)/(3.0*_viscosidade);

    @author : André Duarte Bueno
    @see    : CPermabilidade
    @param  : viscosidade, dimensão do pixel e fator de amplificação do pixel.
    @return : void
*/
void CGrafo::CalcularCondutancias (long double _viscosidade, long double _sizePixel, 
				  unsigned long int _fatorAmplificacao)
{
  // Variáveis auxiliares
  // Raio hidraulico
  long double raio_hidraulico{0.0};
  // Diametro hidraulico
  long double dH{0.0};
  long double sizePixelXfatorAmplificacao = _sizePixel * _fatorAmplificacao;
  long double variavelAuxiliar = (CMath::PI) / (128.0 * _viscosidade * sizePixelXfatorAmplificacao);

  // Percorre  todos os objetos do grafo
  for (unsigned long int k = 0; k < objeto.size (); k++)
  {
      // Pega o raio hidraulico do objeto k e já converte para metros
      raio_hidraulico = objeto[k]->propriedade * sizePixelXfatorAmplificacao;
      dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;
      // Calcula condutancia a partir do diâmetro hidraulico.
      objeto[k]->propriedade = (variavelAuxiliar * dH * dH * dH * dH);
  }
  return;
}

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
  assert (data);
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
void CGrafo::Write (string nomeArquivo)
{
  string grafoName = nomeArquivo + ".cgrafo";
  ofstream out ( grafoName.c_str() );  //ofstream out { nomeArquivo.c_str() };
  if ( out.fail() )
  {
      cerr << " Não conseguiu abrir o arquivo de disco " << nomeArquivo;
      return;
  }
  out.setf (ios::right);

  // Numero de objetos
  out << setw (5) << objeto.size () << endl;
  
  // Percorre os objetos e salva em disco as informações de cada objeto.
  for ( unsigned long int i = 0; i < objeto.size (); i++ )
  {
      objeto[i]->Write (out);
      out << '\n';		// fout << (*objeto[i]) << '\n';
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
// Função:                    LerVetorPropriedades_x()
// -------------------------------------------------------------------------
/**
    @short  : Lê os dados da propriedade x dos objetos do grafo que foram salvos no disco.
    O sistema de proteçao contra quedas de energia salva, a cada conjunto de iterações, 
    os dados dos objetos do grafo em disco (salva o vetor das pressões do grafo).
    Se o micro caiu (queda de luz), o programa pode solicitar a reinicialização da simulação, 
    criando o grafo, determinando o mesmo e então chamando a função LeDadosDisco(),
    que vai ler os dados do arquivo de disco "grafo.vectorX".
    @author :	André Duarte Bueno
    @see    : grafos
    @param  :
    @return :	bool // ostream&
 
     @todo: o nome do arquivo "grafo.vectorX" deve considerar o nome da imagem pois pode 
     processar mais de um arquivo no mesmo diretório ao mesmo tempo.
*/
bool CGrafo::LerVetorPropriedades_x ()
{
  // Abre arquivo de disco
  ifstream fin ("grafo.vectorX"); // todo: considerar nome imagem
  if (fin.fail ())
  { cerr << "Falha abertura arquivo que tem os dados do grafo: grafo.vectorX.\n";
    return 0;
  }
  // Lê os dados do vetor (atributo propriedade).
  long double temp;
  for ( auto objeto_grafo : objeto ) // objeto é um vetor
    {
      fin >> temp;
      objeto_grafo->x = temp;
      
///  @bug: Teste do uso do tag bug do doxygen.
/**  @test: o código abaixo foi comentado pois não deve ocorrer, a ideia é que cada imagem 
tenha um diretório separado! Se colocar mais de uma imagem no mesmo diretório o código abaixo pode 
ser necessário, pois pode ter gerado grafo.vectorX para imagem pequena e depois tentar ler para 
imagem grande!
       if (fin.eof ()) 
 	return 0;
*/
    }
  return 1;
}

/// Salva no arquivo "grafo.vectorX" o valor da pressão de cada objeto.
bool CGrafo::SalvarVetorPropriedades_x ()
{
  // Abre arquivo de disco
  ofstream fout ("grafo.vectorX");
  if (fout.fail ())
    return 0;

  // Salva os dados do atributo x de cada objeto em disco (ex: pressão que esta sendo calculada).
  for (unsigned long int k = 0; k < objeto.size (); k++)
    // fout  <<              objeto[k]->propriedade  << endl;
    fout << objeto[k]->x << endl;
  return 1;
}

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

// -------------------------------------------------------------------------
// Função:               SetarMatrizAVetorB
// -------------------------------------------------------------------------
/** @short  : Recebe uma matriz A (vazia) e um vetor B (vazio)
    e preenche os mesmos com os coeficientes necessários
    para determinação do sistema de equações.
    Pré-condições:
    1- O grafo já deve ter sido determinado.
    2- Os valores iniciais de pressão já devem ter sido definidos
    (valores de contorno, normalmente Plano_0 = 1, Plano_n = 0).
    3- Deve receber uma matriz e um vetor vazios.
    @author : André Duarte Bueno
    @see    : grafos
    @param  :
    @return : bool indicando sucesso da operação.
    @todo: receber matriz de double !! eliminando multiplicador 1e17.
    @todo: transformar em template que recebe tipo : float, double, long double.
    @todo: transformar CVetor em template!
*/
bool CGrafo::SetarMatrizAVetorB (TCMatriz2D< int > * &A, CVetor * &B) const
{
  // vector< vector<double> > A;
  // vector<double> B;

  // Passo 0: Definição de variáveis auxiliares
  // índice i da matriz A (ou vetor B)
  unsigned long int mi;

  // índice j da matriz A
  unsigned long int mj;

  // Condutância total Cij = (Cii+Cjj)/2 para o modelo 2 ; Tarefa: calcular para demais modelos!
  long double Cij;

  // Passo 1:
  // Determinação da dimensão da matriz e do vetor
  // cout << "\nlastObjectOfSolver="               <<       lastObjectOfSolver;
  // cout << "\nfirstObjectOfSolver="      <<  firstObjectOfSolver;

  // A dimensão do sistema de equações considera os planos 1->n-1,
  // pois os planos 0 e n-1 tem pressões constantes.
  unsigned int dim = lastObjectOfSolver - firstObjectOfSolver + 1;
  // cout <<"\ndim="<<dim;

  // Redimensiona a matriz A
  A->Redimensiona (dim, dim);
  // Zera a matriz A
  A->Constante (0);

  // Redimensiona o vetor B
  B->Redimensiona (dim);
  // Zera o vetor B
  B->Constante (0);

  unsigned int i;
  for (unsigned long int j = 0; j < objeto.size (); j++)
    {
      // Faz um cast para sítio derivado (em função do acesso a função Contorno e vetor coneccao).
      COGSitio *objeto_j = dynamic_cast < COGSitio * >(objeto[j]);
      assert (objeto_j);  // se não der certo o cast, vai lançar excessão!

      switch (objeto_j->Contorno ())
	{
	  // Fronteira esquerda
    case CContorno::ETipoContorno::WEST:

	  // Fronteira direita
    case CContorno::ETipoContorno::EST:
	  // Percorre as conecções do objeto       
	  for (i = 0; i < objeto_j->coneccao.size (); i++)
	    {
	      // Calcula Cij - Tarefa: explicar a equacao usada.
	      Cij = (objeto_j->coneccao[i]->propriedade + objeto_j->propriedade) / 2.0;
	      Cij = Cij * 1.0e17;	// LIXO, para gerar int
	      // cij esta sendo armazenado em int por isto multiplico por e17

	      // Desloca o índice da matriz(vetor), pois só entram os sítios
	      // que não estão na interface.
	      mi = objeto_j->coneccao[i]->rotulo - firstObjectOfSolver;	// 3;

	      // Acumula Cij no vetor B[mi] -= Cij     * objeto_j->x,
	      // x deve estar definido
	      // B->data1D[ mi ] -= Cij * objeto_j->x; 
	      B->data1D[mi] -= static_cast < int >(Cij * objeto_j->x);

	      // Acumula -Cij na matriz A[mi][mi]
	      // A->data2D[mi][mi] -= Cij;
	      A->data2D[mi][mi] -= static_cast < int >(Cij);
	    }
	  break;
	  // Fronteira Centro
    case CContorno::ETipoContorno::CENTER:
	  // Percorre as conecções do objeto       
	  for (i = 0; i < objeto_j->coneccao.size (); i++)
	    {
	      // Se o link for um objeto de centro (não contorno) entra
          if (objeto_j->coneccao[i]->Contorno () == CContorno::ETipoContorno::CENTER)
		{
		  // Calcula Cij
		  Cij = (objeto_j->propriedade + objeto_j->coneccao[i]->propriedade) / 2.0;
		  Cij = Cij * 1.0e17;	// LIXO para gerar int
		  // cij esta sendo armazenado em int por isto multiplico por e17

		  // Desloca os índices da matriz
		  mi = objeto_j->coneccao[i]->rotulo - firstObjectOfSolver;
		  mj = objeto_j->rotulo - firstObjectOfSolver;

		  // Define A->data2D[mi][mj]      
		  A->data2D[mi][mj] = static_cast < int >(Cij);	// LIXO o static

		  // Acumula A->data2D[mj][mj]
		  A->data2D[mj][mj] -= static_cast < int >(Cij); // LIXO o static
		}
	    }
	  break;
	}			// switch
    }				// for

    /// @todo: abaixo deve considerar o nome do arquivo/imagem que foi processada!
  A->Write ("grafo.matrixA");
  B->Write ("grafo.vectorB");

  return 1;
}

/*ANTIGA, FUNCIONA, USA
bool CGrafo::SetarMatrizAVetorB(TCMatriz2D< int >* &A, CVetor* &B) const
{
  // Passo 0: Definição de variáveis auxiliares
  // índice i da matriz A (ou vetor B)
  unsigned long int mi;	

  // índice j da matriz A
  unsigned long int mj;
	
  // Condutância total Cij = (Cii+Cjj)/2 para o modelo 2
  long double Cij;
	
  // Passo 1:
  // Determinação da dimensão da matriz e do vetor
  // cout << "\nlastObjectOfSolver="		<<	 lastObjectOfSolver;
  // cout << "\nfirstObjectOfSolver="	<<  firstObjectOfSolver;

	
  unsigned int dim = lastObjectOfSolver - firstObjectOfSolver + 1 ;
  // cout <<"\ndim="<<dim;
	
  // Redimensiona a matriz A
  A->Redimensiona(dim,dim);
  // Zera a matriz A
  A->Constante(0);
	
  // Redimensiona o vetor B
  B->Redimensiona(dim);
  // Zera o vetor B
  B->Constante(0);
 	
  unsigned int i;
  for( unsigned long int j = 0 ; j < objeto.size();  j++ )
    {
      // Faz um cast para sítio derivado (em função do acesso a função Contorno e vetor coneccao.

      COGSitio* objeto_j = dynamic_cast<COGSitio*>(objeto[j]);
      assert(objeto_j);
  		
      switch( objeto_j->Contorno() )
	{
	  // Fronteira esquerda
	case CContorno::WEST :	
  			
	  // Fronteira direira
	case CContorno::EST :	
	  // Percorre as conecções do objeto	
	  for ( i = 0; i < objeto_j->coneccao.size(); i++)
	    {
	      // Calcula Cij
	      Cij = (objeto_j->coneccao[i]->propriedade +  objeto_j->propriedade   ) /2.0 ;
	      Cij = Cij* 1.0e17;	// LIXO, para gerar int
	      // cij esta sendo armazenado em int por isto multiplico por e17
	  							
	      // Desloca o índice da matriz(vetor), pois só entram os sítios
	      // que não estão na interface.
	      mi = objeto_j->coneccao[i]->rotulo - firstObjectOfSolver; // 3;
	  							
	      // Acumula Cij no vetor B[mi] -= Cij	* objeto_j->x,
	      // x deve estar definido
	      // B->data1D[ mi ] -= Cij * objeto_j->x;	
	      B->data1D[ mi ] -= static_cast<int>( Cij * objeto_j->x);	
	  							
	      // Acumula -Cij na matriz A[mi][mi]
	      // A->data2D[mi][mi] -= Cij;	
	      A->data2D[mi][mi] -= static_cast<int>(Cij);	
	    }
	  break;
  			
	  // Fronteira Centro
	case CContorno::CENTER :	
	  // Percorre as conecções do objeto	
	  for ( i = 0; i < objeto_j->coneccao.size(); i++)
	    {
	      // Se o link  for  um objeto de centro (não contorno) entra
	      if( objeto_j->coneccao[i]->Contorno() == CContorno::CENTER)
		{										
		  // Calcula Cij
		  Cij = ( objeto_j->propriedade + objeto_j->coneccao[i]->propriedade ) /2.0 ;
		  Cij = Cij* 1.0e17;// LIXO para gerar int
		  // cij esta sendo armazenado em int por isto multiplico por e17
    	  								
		  // Desloca os índices da matriz
		  mi = objeto_j->coneccao[i]->rotulo - firstObjectOfSolver ;
		  mj = objeto_j->rotulo  - firstObjectOfSolver ;
    				  					
		  // Define A->data2D[mi][mj]	
		  A->data2D[mi][mj]	 = static_cast<int>(Cij); // LIXO o static
    				  					
		  // Acumula A->data2D[mj][mj]
		  A->data2D[mj][mj]	-= static_cast<int>(Cij);// LIXO o static
		}
	    }
	  break;
	}// switch
    }// for
	
  A->Write("Matriz_A.txt");	
  B->Write("Vetor_B.txt");	
  return 1;
}
*/
