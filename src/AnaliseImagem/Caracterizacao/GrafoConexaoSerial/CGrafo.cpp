/*
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: TPadrao_ramo
  ===============================================================================

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
  	[LDSC].
  @author:          André Duarte Bueno
  File:             CGrafo.cpp
  begin:            Sat Sep 16 2000
  copyright:        (C) 2000 by André Duarte Bueno
  email:            andreduartebueno@gmail.com
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

#ifndef COGSitioEsquerda_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitioEsquerda.h>
#endif

#ifndef COGSitioDireita_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitioDireita.h>
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
// Função:   CalculoCondutancias
// -------------------------------------------------------------------------
/** Função:   CalculoCondutancias (ex: converte raio hidraulico em condutância).
    @short:   Todo objeto do grafo tem uma propriedade que é armazenada.
    A função CalculoCondutancias é uma função criada para alteração
    desta propriedade levando em conta os fenômenos que se deseja estudar.
					
    Específico:
    No caso específico do cálculo da permeabilidade de representações tridimensionais.
    Transforma a propriedades raioHidraulico dos "sítios" em condutâncias.
    Ou seja, percorre todo o grafo e transforma a propriedade raioHidraulico em condutância.
    Esta condutância é usada pelo objeto sítio para calcular a sua pressão (x).

    Ou seja:
    Quando se determina o grafo a propriedade armazenada nos objetos é o raio hidraulico.
    Quando se deseja determinar a permeabilidade a propriedade armazenada é a condutancia.

    Tarefa:
    Verificar uma forma de eliminar a dependencia destes parâmetros.
    
    Note que a função esta calculando a condutancia segundo a lei de Poiselle -> para ligações (eq 5.16 da tese Liang). 

    Abaixo a equacao 5.17 da tese do Liang, que usa condutancia segundo Koplik (1983)
    // Calcula a condutancia do sitio usando a equação ri^3/(3*viscosidade)
    //      objeto[k]->propriedade = (raio_hidraulico * raio_hidraulico * raio_hidraulico ) /
    // (3.0 * _viscosidade );

    @author : André Duarte Bueno
    @see    : CPermabilidade
    @param  : viscosidade, dimensão do pixel e fator de amplificação do pixel.
    @return : void
*/
void CGrafo::CalculoCondutancias (long double _viscosidade, long double _sizePixel, unsigned long int _fatorAmplificacao) {
  // Variáveis auxiliares
  long double raio_hidraulico;
  // diametro hidraulico
  long double dH;
  long double sizePixelXfatorAmplificacao = _sizePixel * _fatorAmplificacao;
	long double variavelAuxiliar = (M_PI) / (128.0 * _viscosidade * sizePixelXfatorAmplificacao);

  // Percorre  todos os objetos do  grafo
  for (unsigned long int k = 0; k < objeto.size (); k++)
  {
      // Pega o raio hidraulico do objeto k e já converte para metros
      raio_hidraulico = objeto[k]->propriedade * sizePixelXfatorAmplificacao;
      // Calcula a condutancia do "sitio" usando a equação 5.16 da tese Liang
      //  =pi*dH^4/(128*viscosidade*comprimentoL)
      // 4.0*(raio_hidraulico) ;
      dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;
      objeto[k]->propriedade = (variavelAuxiliar * dH * dH * dH * dH);
      // ( CMath::PI * dH * dH * dH * dH )
      //               /
      // (128.0 * _viscosidade * _sizePixel * _fatorAmplificacao );
  }
  return;
}

// -------------------------------------------------------------------------
/**	Função:	GetObjetoGrafo
   @short  : Cria objeto herdeiro de CObjetoGrafo, de acordo com o tipo solicitado.
   @author : André Duarte Bueno
   @see    : grafos
   @param  : CContorno::ETipoContorno identificando o tipo de sítio a ser criado.
   			Formato andre(old): esquerda(0) centro(1) direita(2)
   			Formato Liang: esquerda(1) centro(0) direita(2)
   			Formato novo: veja CContorno::ETipoContorno
   @return : Retorna um ponteiro para um sítio novo alocado
*/
CObjetoGrafo* CGrafo::GetObjetoGrafo (CContorno::ETipoContorno tipoContorno)
{
  CObjetoGrafo *data = nullptr;
  switch (tipoContorno)
  {
    case CContorno::CENTER:
      data = new COGSitioCentro ();
      break;
    case CContorno::WEST:
      data = new COGSitioEsquerda ();
      break;
    case CContorno::EST:
      data = new COGSitioDireita ();
      break;
    default:
      data = new COGSitioCentro ();
      break;
  }
  assert (data);
  return data;
}

// -------------------------------------------------------------------------
/** @short  : 	Salva em disco o grafo.
    A saída de dados era no mesmo formato do Liang
    Formato antigo Write_Liang_Format:
    NumerodeSitios
    Tipo de Sitio
    Numero De Links
    Lista dos rótulos dos objetos
    Lista das propriedades dos objetos

    Formato novo Write (Andre Format):
    NumeroTotalObjetos 	// salvo pelo grafo
    TipoDoObjeto
    RotuloDoObjeto
    propriedadeDoObjeto	// raio hidraulico ou condutancia
    x           	// propriedade a er calculada (ex:pressão).
    NumeroConeccoes
    Lista_dos_rotulos_das_coneccoes

    @author : André Duarte Bueno
    @see    : grafos
    @param  : Nome do arquivo de disco (string)
    @return : void
*/
void CGrafo::Write (string fileName)
{
  ofstream out (fileName.c_str ());
  if (out.fail ())
  {
      cerr << " Não conseguiu abrir o arquivo de disco " << fileName;
      return;
  }
  out.setf (ios::right);
  // Numero de objetos
  out << setw (5) << objeto.size () << endl;
  // Percorre os objetos e salva em disco as informações de cada objeto
  for (unsigned long int i = 0; i < objeto.size (); i++)
  {
      objeto[i]->Write (out);
      out << endl;		// fout << (*objeto[i]);
  }
  out.close ();
}

// Ex: grafo->Write(fout); ou fout <<grafo->Write<<endl;
/**ostream& CGrafo::Write(ostream& os)
   {
   fout.setf(ios::right);

   // Numero de objetos
   fout << setw(5) << objeto.size()<< endl;

   // Percorre os objetos e salva em disco as informações de cada objeto
   for( unsigned long int i = 0 ; i < objeto.size();  i++ )
   objeto[i]->Write(os); 	
   return os;
   }
*/

// -------------------------------------------------------------------------
// Função:                    LeVetorPropriedades()
// -------------------------------------------------------------------------
/**
    @short  : Lê os dados da propriedade x dos objetos do grafo que foram salvos no disco.
    O sistema de proteçao contra quedas de energia salva, a cada conjunto de iterações, 
    os dados dos objetos do grafo em disco (salva o vetor das pressões do grafo).
    Se o micro caiu (queda de luz), o programa pode solicitar a reinicialização da simulação, 
    criando o grafo, determinando o mesmo e então chamando a função LeDadosDisco(),
    que vai ler os dados do arquivo de disco "grafo.vector".
    @author :	André Duarte Bueno
    @see    : grafos
    @param  :
    @return :	bool // ostream&
*/
bool CGrafo::LeVetorPropriedades ()
{
  // Abre arquivo de disco
  // Tarefa: o nome do arquivo "grafo.vectorX" deve considerar o nome do arquivo pois pode processar
  // mais de um arquivo no mesmo diretório ao mesmo tempo.
  ifstream fin ("grafo.vectorX");
  if (fin.fail ())
  { cerr << "Falha abertura arquivo que tem os dados do grafo: grafo.vectorX.\n";
    return 0;
  }
  // Lê os dados do vetor (atributo propriedade).
  long double temp;
  for (unsigned long int k = 0; k < objeto.size (); k++)
    {
      fin >> temp;
      // objeto[k]->propriedade = temp;
      objeto[k]->x = temp;

      if (fin.eof ()) // Tarefa: não deve ocorrer! testar!
	continue;
    }
  return 1;
}

bool CGrafo::SalvaVetorPropriedades ()
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
/** Salva dados do objeto em os (disco),
    @short  : Salva dados do objeto em os (disco),
    Salva o número de objetos que fazem parte de grafo e
    a seguir os dados de cada objeto.
    @author :	André Duarte Bueno
    @see    : grafos
    @param  :
    @return :	ostream&
*/
ostream & operator<< (ostream & os, CGrafo & grafo)
{
  // Numero de objetos
  os << setw (5) << grafo.objeto.size () << endl;

  // Percorre os objetos e salva em disco as informações de cada objeto
  for (unsigned long int i = 0; i < grafo.objeto.size (); i++)
    grafo.objeto[i]->Write (os);	// os << (*(grafo.objeto[i]));

  return os;
}

// -------------------------------------------------------------------------
// Função:               SetMatrizAVetorB
// -------------------------------------------------------------------------
/** Recebe uma matriz A (vazia) e um vetor B (vazio) e 
    preenche os mesmos com os coeficientes necessários.
    @short  : Recebe uma matriz A (vazia) e um vetor B (vazio)
    e preenche os mesmos com os coeficientes necessários
    para determinação do sistema de equações.
    Condições:
    1- O grafo já deve ter sido determinado.
    2- Os valores iniciais de pressão já devem ter sido definidos
    (valores de contorno, normalmente Plano_0 = 1, Plano_n = 0).
    3- Deve receber uma matriz e um vetor vazios.
    @author : André Duarte Bueno
    @see    : grafos
    @param  :
    @return : bool indicando sucesso da operação.
    Tarefa: receber matriz de double!! eliminando multiplicador 1e17
*/
bool CGrafo::SetMatrizAVetorB (TCMatriz2D< int > * &A, CVetor * &B) const
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
      // Faz um cast para sítio derivado (em função do acesso a função Contorno e vetor coneccao.
      COGSitio *objeto_j = dynamic_cast < COGSitio * >(objeto[j]);
      assert (objeto_j);  // se não der certo o cast, vai lançcar excessão!

      switch (objeto_j->Contorno ())
	{
	  // Fronteira esquerda
	case CContorno::WEST:

	  // Fronteira direita
	case CContorno::EST:
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
	case CContorno::CENTER:
	  // Percorre as conecções do objeto       
	  for (i = 0; i < objeto_j->coneccao.size (); i++)
	    {
	      // Se o link for um objeto de centro (não contorno) entra
	      if (objeto_j->coneccao[i]->Contorno () == CContorno::CENTER)
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
		  A->data2D[mj][mj] -= static_cast < int >(Cij);	// LIXO o static
		}
	    }
	  break;
	}			// switch
    }				// for

  A->Write ("grafo.matrixA");
  B->Write ("grafo.vectorB");

  return 1;
}

/*ANTIGA, FUNCIONA, USA
bool CGrafo::SetMatrizAVetorB(TCMatriz2D< int >* &A, CVetor* &B) const
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
