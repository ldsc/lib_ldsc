/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico 
	[LDSC].
@author:          André Duarte Bueno
File:             CRotulador3D.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Rotulador/CRotulador3D.h>
// using namespace std;

/*
-------------------------------------------------------------------------
Função: AreaObjetos
-------------------------------------------------------------------------
@short  :Calcula area de cada objeto
@author :André Duarte Bueno
@see    :rotulagem
@param  :void
@return :void
*/
void CRotulador3D::CalculaAreaObjetos ()
{
  if (rotulado == false)	// Só procede o calculo se a matriz rótulo já foi determinada
    Go (pm);			// Calcula a matriz rotulo

  if (rotulado == true)		// Só procede o calculo se a matriz rótulo foi determinada
    {
      if (areaObjetos != NULL)	// Se a areaObjetos já existe, apaga
		delete areaObjetos;

      areaObjetos = new CVetor (numeroObjetos);	// Cria vetor área dos objetos

      if (areaObjetos != NULL)	// Se alocado corretamente
	{
	  areaObjetos->Constante (0);	// Zera a área dos objetos
	  for (unsigned int i = 0; i < NX (); i++)	// Percorre direcao x
	    for (unsigned int j = 0; j < NY (); j++)	// Percorre direcao y
	      for (unsigned int k = 0; k < NZ (); k++)	// Percorre direcao z
		areaObjetos->data1D[data3D[i][j][k]]++;	// Incrementa vetor
	}
    }
}

/*
-------------------------------------------------------------------------
Função: 	PerimetroObjetos
-------------------------------------------------------------------------
@short  :	Calcula PerimetroObjetos de cada objeto
@author :	André Duarte Bueno
@see    : rotulagem
@param  : void
@return : void
PerimetroObjetos
*/
void CRotulador3D::CalculaPerimetroObjetos ()
{
// cout<<"\nCalculo PerimetroObjetos não implementado em 3D, valores de perimetro definidos como sendo 1";
  if (rotulado == false)	// Só procede o calculo se a matriz rótulo já foi determinada
    Go (pm);			// Calcula a matriz rotulo

  if (rotulado == true)		// Só procede o calculo se a matriz rótulo foi determinada
    {
      if (perimetroObjetos != NULL)	// Se a perimetroObjetos já existe, apaga
	delete perimetroObjetos;

      perimetroObjetos = new CVetor (numeroObjetos);	// Cria vetor perimetroObjetos

      if (perimetroObjetos != NULL)
	{
	  // Copiar de 2D para cá e levar em consideração o 3D
	  perimetroObjetos->Constante (1);	// temporario define como sendo 1

/*
  perimetroObjetos->Constante(0);		// zera os perrimetros como sendo 0
  perimetroObjetos->data1D[0]=1;		// Objeto 0 assume perimetro 1 (erro assumido)

  unsigned int   i,j;               // Variaveis auxiliares para loop's

  // --------------------------------
  // Região central, fora as bordas
	for(i=1; i < NX()-1; i++)   		// Percorre direcao x, fora bordas
		for(j=1; j< NY()-1; j++) 		// Percorre direcao y, fora bordas
			{
			if(data2D[i][j] != FUNDO)
			 {
			 if(data2D[i-1][j]==FUNDO)		// esquerdo
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i+1][j]==FUNDO)		// direito
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i][j-1]==FUNDO)		// acima
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i][j+1]==FUNDO)		// abaixo
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 }
			}
  // --------------------------------
  // Borda esquerda
	i=0;
		for(j=1; j< NY()-1; j++)     // Percorre direcao y,
			{
			if(data2D[i][j]!=FUNDO)
			 {
			 perimetroObjetos->data1D[data2D[i][j]]++;// considera esquerdo como perímetro [novo]
			 if(data2D[i+1][j]==FUNDO)				  // direito
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i][j-1]==FUNDO)				  // acima
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i][j+1]==FUNDO)				  // abaixo
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 }
			}
  // --------------------------------
  // Borda direita
	i= NX()-1;
		for(j=1; j< NY()-1; j++)         	  // Percorre direcao y,
			{
			if(data2D[i][j]!=FUNDO)
			 {
			 perimetroObjetos->data1D[data2D[i][j]]++;// considera direito como perímetro [novo]
			 if(data2D[i-1][j]==FUNDO)				  // esquerdo
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i][j-1]==FUNDO)				  // acima
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i][j+1]==FUNDO)				  // abaixo
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 }
			}
  // --------------------------------
  // Borda superior
	j=0;
	for(i=1; i < NX()-1; i++)           	  // Percorre direcao x,
			{
			if(data2D[i][j] != FUNDO)
			 {
			 perimetroObjetos->data1D[data2D[i][j]]++;// considera superior como perímetro [novo]
			 if(data2D[i-1][j]==FUNDO)				  // esquerdo
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i+1][j]==FUNDO)				  // direito
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i][j+1]==FUNDO)				  // abaixo
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 }
			}
  // --------------------------------
  // Borda inferior
	j=NY()-1;
	for(i=1; i < NX()-1; i++)           	  // Percorre direcao x,
			{
			if(data2D[i][j] != FUNDO)
			// if(data2D[i][j]>0)
			 {
			 perimetroObjetos->data1D[data2D[i][j]]++;// considera inferior como perímetro [novo]
			 if(data2D[i-1][j]==FUNDO)				  // esquerdo
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i+1][j]==FUNDO)				  // direito
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 if(data2D[i][j-1]==FUNDO)				  // acima
				 perimetroObjetos->data1D[data2D[i][j]]++;
			 }
			}

  // --------------------------------
	// 4 cantos (incluído na versão 7)
  // Falta considerar os 4 pontos dos 4 cantos da imagem
  // Canto superior esquerdo
   i=0;
   j=0;
   if(data2D[i][j]!=FUNDO)
   	{
		perimetroObjetos->data1D[data2D[i][j]]++;// considera  Canto superior
		perimetroObjetos->data1D[data2D[i][j]]++;// considera  Canto esquerdo
      if(data2D[i+1][j]==FUNDO)
         perimetroObjetos->data1D[data2D[i][j]]++;
   	if(data2D[i][j+1]==FUNDO)
         perimetroObjetos->data1D[data2D[i][j]]++;
      }

   // Canto superior direito
   i= NX()-1;
  	j= 0;
   if(data2D[i][j]!=FUNDO)
   	{
		perimetroObjetos->data1D[data2D[i][j]]++;// considera  Canto superior
		perimetroObjetos->data1D[data2D[i][j]]++;// considera  Canto direito
	   if(data2D[i-1][j]==FUNDO)
         perimetroObjetos->data1D[data2D[i][j]]++;
	   if(data2D[i][j+1]==FUNDO)
         perimetroObjetos->data1D[data2D[i][j]]++;
      }
   // Canto inferior esquerdo
   i= 0;
  	j= NY()-1;
   if(data2D[i][j]!=FUNDO)
   	{
		perimetroObjetos->data1D[data2D[i][j]]++;// considera  Canto inferior
		perimetroObjetos->data1D[data2D[i][j]]++;// considera  Canto esquerdo
	   if(data2D[i][j-1]==FUNDO)
         perimetroObjetos->data1D[data2D[i][j]]++;
   	if(data2D[i+1][j]==FUNDO)
         perimetroObjetos->data1D[data2D[i][j]]++;
      }

   // Canto inferior direito
   i= NX()-1;
  	j= NY()-1;
   if(data2D[i][j]!=FUNDO)
   	{
		perimetroObjetos->data1D[data2D[i][j]]++;// considera  Canto inferior
		perimetroObjetos->data1D[data2D[i][j]]++;// considera  Canto direito
	   if(data2D[i-1][j]==FUNDO)
         perimetroObjetos->data1D[data2D[i][j]]++;
   	if(data2D[i][j-1]==FUNDO)
         perimetroObjetos->data1D[data2D[i][j]]++;
      }
    	// Salvar(perimetroObjetos,"perimetroObjetos.peo");
*/

	}			// 
    }				// if rotulado==true
}

/*
-------------------------------------------------------------------------
Função: Go
-------------------------------------------------------------------------
@short  :Executa toda a sequencia de rotulagem
@author :André Duarte Bueno
@see    :
@param  :
@return :
*/
// bool CRotulador3D::Go(CMatriz3D*& matriz, int _rotuloInicial)
bool
CRotulador3D::Go (CMatriz3D * matriz)
{
  // this->rotuloInicial=rotuloInicial;                        // default=1, rotulo a ser dado para o primeiro objeto

  if (PreparaImagem (matriz) == false)	// verifica a imagem
    return false;

  // debug
  // {IProcessTime wait("IdentificaObjetos.tempo.txt");
  // IdentificaObjetos();}
  // vConversao->Write("1-VetorConversaoDepoisIdentificaObjetos.txt");
  // IdentificaObjetos(rotuloInicial);
  IdentificaObjetos ();

  // debug
  // {IProcessTime wait("PesquisaRotulosValidosEOrdena.tempo.txt");
  // PesquisaRotulosValidosEOrdena();}
  // vConversao->Write("2-VetorConversaoDepoisPesquisaRotulosValidosEOrdena.txt");
  PesquisaRotulosValidosEOrdena ();

  // debug
  // {IProcessTime wait("UniformizaTabelaRotulos.tempo.txt");
  // UniformizaTabelaRotulos();}
  // vConversao->Write("3-VetorConversaoDepoisUniformizaTabelaRotulos.txt");
  UniformizaTabelaRotulos ();

  // debug
  // {IProcessTime wait("RotulaImagem.txt");
  // RotulaImagem();}
  // RotulaImagem(/*_rotuloInicial*/);
  RotulaImagem ();

  // Define que a imagem já foi rotulada
  rotulado = true;

  // Retorna true informando que tudo ocoreu direito
  return true;
};

/*
-------------------------------------------------------------------------
Função: PreparaImagem
-------------------------------------------------------------------------
@short  :
	Funcao chamada por Go, para verificar a imagem passada.
   Ou seja antes de inicializar o procedimento de rotulagem,
	faz algumas verificações com a imagem.
   Armazena o ponteiro para a imagem e verifica se a imagem tem as mesmas dimensões
   caso contrário, realoza a imagem.
@author :André Duarte Bueno
@see    :rotulagem
@param  :CMatriz3D
@return :bool
*/
bool CRotulador3D::PreparaImagem (CMatriz3D * matriz)
{
  pm = matriz;			// Armazena endereço matriz
  // Verifica se a matriz tem as mesmas dimensoes do rotulador
  if (pm == NULL)
    return false;

  // Se o objeto rotulador3D foi criado sem passar a matriz 3D, ou nx,ny,nz
  // a matriz3D definida por data3D não foi criada, abaixo já Aloca a matriz3D
  if (this->nx != matriz->NX () || this->ny != matriz->NY () || this->nz != matriz->NZ ())
    {
      Desaloca ();		// Desaloca a matriz existende e depois
      this->nx = matriz->NX ();	// Define novo tamanho
      this->ny = matriz->NY ();
      this->nz = matriz->NZ ();
      Aloca ();			// Aloca de acordo com novo tamanho
    }

// Abaixo pode ser eliminado?? (desnecessário)
  for (unsigned int k = 0; k < nz; k++)
    for (unsigned int j = 0; j < ny; j++)
      for (unsigned int i = 0; i < nx; i++)
	// if(matriz->data3D[i][j][k]!=0)// Como a imagem recebida por ser uma outra copia os dados
	if (matriz->data3D[i][j][k] != FUNDO)
	  this->data3D[i][j][k] = 1;	// Define this com 0 e 1
	else
	  this->data3D[i][j][k] = 0;

// NOVO Versão 7
// Como pode chamar Go mais de uma vez, preciso apagar os vetores area e perimetro
// que foram alocados na chamada anterior a Go
  if (areaObjetos)
    delete areaObjetos;
  areaObjetos = NULL;

  if (perimetroObjetos)
    delete perimetroObjetos;
  perimetroObjetos = NULL;

  return true;
// Write("PreparaImagem.txt");
}

/*
-------------------------------------------------------------------------
Função:IdentificaObjetos
-------------------------------------------------------------------------
@short  :Executa a primeira passagem pela imagem, identifica os píxeis ativos
e define os rotulos indiretos no vetor de conversão.
@author :André Duarte Bueno
@see    :rotulagem
@param  :matriz 3D
@return :void

Executa rotulagem diretamente em 3D
1-Verifica o primeiro plano (Z=0) (Extamente igual ao 2D, incluindo k=0)
	Ponto 0 0 0
	Primeira linha i 0 0
	Demais linhas e colunas
		Tratamento especial para primeira coluna  0 j 0
		Demais linhas e colunas i j 0

2-Verifica demais planos Z=1,2,3...(necessita verificar atras)
	Ponto 0 0 k
		verifica atras
	Primeira linha i 0 k
		verifica atras
	Demais linhas e colunas
		Tratamento especial para primeira coluna  0 j k
			verifica atras
		Demais linhas e colunas i j k
			verifica atras
*/
// void CRotulador3D::IdentificaObjetos(/*int rotuloInicial*/)
void
CRotulador3D::IdentificaObjetos ()
{
  // usados nos for
  unsigned int i, j, k;
  // valores dos pixel's acima, a esquerda e atras (plano anterior)
  int acima, esquerda, atras;

  // indice do rotulo atual
  unsigned long int rotuloAtual = 0;

  // unsigned long int   rotuloAtual=rotuloInicial;// indice do rotulo atual

  // usar menos pontos abaixo ou alocação dinamica se precisar de mais pontos
  // A cada chamada de Go, o numerodeObjetos é o encontrado na imagem.
  // se tiver mais de 1000 objetos vai dar pau
  rotuloMaximoUtilizado = numeroObjetos = (NX () * NY () * NZ () / 4);	// 100000;

  // Cria vetor de conversão vConversao
  if (vConversao != NULL)
    delete vConversao;
  vConversao = new CVetor (numeroObjetos);

  // Os valores do vetor de conversao sao pre definidos
  // a ser uma sequencia 0->0,1->1,2->2,3->3,...
  // Assim o indice 5 aponta para o rotulo 5, o 6 para o rotulo 6,..
  for (i = 0; i < vConversao->NX (); i++)
    vConversao->data1D[i] = i;
  // --------------------------------------
  // ************PLANO Z=0*****************
  // --------------------------------------
  // Adota exatamente o mesmo procedimento do caso 2D, considerando [i][j][0]
  // ---------------------------------------------
  // PONTO 0 0 0
  // ---------------------------------------------
  if (data3D[0][0][0] != FUNDO)
    data3D[0][0][0] = ++rotuloAtual;	// Se preto rotula

  // ---------------------------------------------
  // PRIMEIRA LINHA [i][0][0] [x][y][z]
  // ---------------------------------------------
  for (i = 1; i < NX (); i++)	// verifica a primeira linha
    if (data3D[i][0][0] != FUNDO)	// definindo os rotulos
      if (data3D[i - 1][0][0] != FUNDO)	// se a esquerda for preto
	data3D[i][0][0] = rotuloAtual;	// fica com o mesmo rotulo
      else
	data3D[i][0][0] = ++rotuloAtual;	// senão incrementa e usa

  // ---------------------------------------------
  // Demais LINHAs [i][j][0]
  // ---------------------------------------------
  for (j = 1; j < NY (); j++)
    {				// ------------------------------------------
      // PRIMEIRA COLUNA [0][j][0]         // A primeira coluna deve ter
      if (data3D[0][j][0] != FUNDO)	// um tratamento separado para
	{			// evitar estouro da pilha.
	  data3D[0][j][0] = ++rotuloAtual;	// linha nova sempre usa rotulo novo
	  acima = data3D[0][j - 1][0];
	  VerificaContorno (acima, rotuloAtual);	// verifica o contorno
	}
      // ---------------------------------------------
      // ABAIXO PONTOS [i][j][0]  (k=0)
      for (i = 1; i < NX (); i++)	// Varre as linhas x=i
	{
	  if (data3D[i][j][0] != FUNDO)	// se for poro (preto)
	    {
	      esquerda = data3D[i - 1][j][0];	// e da esquerda
	      if (esquerda != FUNDO)	// se a esquerda e acima forem != FUNDO
		data3D[i][j][0] = rotuloAtual;	// fica com o rotulo atual
	      else		// esquerda==FUNDO
		data3D[i][j][0] = ++rotuloAtual;	// incrementa rotulo e usa
	      acima = data3D[i][j - 1][0];	// armazena informação do ponto acima
	      VerificaContorno (acima, rotuloAtual);
	    }			// fim if
	}			// fim  do              for i
    }				// fim do       for j
  // ------------------------------------------
  // ************PLANO  Z=1,2,3...*************
  // ------------------------------------------
  for (k = 1; k < NZ (); k++)
    {
      // ---------------------------------------------
      // PONTO 0 0 k
      // ---------------------------------------------
      if (data3D[0][0][k] != FUNDO)
	{
	  data3D[0][0][k] = ++rotuloAtual;	// assume rotulo novo
	  atras = data3D[0][0][k - 1];	// 
	  VerificaContorno (atras, rotuloAtual);
	}
      // ---------------------------------------------
      // PRIMEIRA LINHA [i][0][0]
      // ---------------------------------------------
      for (i = 1; i < NX (); i++)
	{
	  if (data3D[i][0][k] != FUNDO)	// se for preto
	    {
	      if (data3D[i - 1][0][k] != FUNDO)	// e a esquerda preto
		data3D[i][0][k] = rotuloAtual;	// assume rotulo da esquerda
	      else
		data3D[i][0][k] = ++rotuloAtual;	// senão incrementa e usa
	      atras = data3D[i][0][k - 1];
	      VerificaContorno (atras, rotuloAtual);
	    }
	}
      // ---------------------------------------------

      // Demais LINHAs [i][j][k]
      // ---------------------------------------------
      for (j = 1; j < NY (); j++)
	{
	  // ---------------------------------------------
	  // PRIMEIRA COLUNA [0][j][k]
	  if (data3D[0][j][k] != FUNDO)
	    {
	      data3D[0][j][k] = ++rotuloAtual;
	      acima = data3D[0][j - 1][k];
	      VerificaContorno (acima, rotuloAtual);
	      atras = data3D[0][j][k - 1];
	      VerificaContorno (atras, rotuloAtual);
	    }

	  // ---------------------------------------------
	  // Demais LINHAs [i][j][k]
	  // ---------------------------------------------
	  for (i = 1; i < NX (); i++)	// Varre as linhas x=i
	    {
	      if (data3D[i][j][k] != FUNDO)	// se for poro (preto)
		{
		  esquerda = data3D[i - 1][j][k];	// da esquerda
		  if (esquerda != FUNDO)	// se a esquerda e acima forem != FUNDO
		    data3D[i][j][k] = rotuloAtual;	// fica com o rotulo atual
		  else		// esquerda==FUNDO
		    data3D[i][j][k] = ++rotuloAtual;	// incrementa rotulo e usa
		  acima = data3D[i][j - 1][k];	// armazena informação do ponto acima
		  VerificaContorno (acima, rotuloAtual);
		  atras = data3D[i][j][k - 1];	// e de atrês
		  VerificaContorno (atras, rotuloAtual);
		}		// fim if
	    }			// fim  do              for i

	}			// fim do       for j
    }				// fim do for k

  rotuloMaximoUtilizado = rotuloAtual;	// necessario, define o maior rotulo utilizado
// Write("identificaObjetos.ma3.txt");
}

/*
-------------------------------------------------------------------------
Função: RotulaImagem
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
@see    :
@param  :
@return :

2a passagem
Ex:
5 objetos: Rotulos: 0,1,2,3,4
rotuloInicial = 0
numeroObjetos = 5
rotuloFinal   = 4 =  rotuloInicial+numeroObjetos-1
rotuloInicial  =  _rotuloInicial;
*/
// void CRotulador3D::RotulaImagem(/*int _rotuloInicial*/)
void CRotulador3D::RotulaImagem ()
{
  // rotuloFinal    =  rotuloInicial  +   numeroObjetos - 1;
  for (unsigned int i = 0; i < NX (); i++)
    for (unsigned int j = 0; j < NY (); j++)
      for (unsigned int k = 0; k < NZ (); k++)
	   data3D[i][j][k] = vConversao->data1D[data3D[i][j][k]];	// + rotuloInicial;
  
  if (vConversao != NULL)
    delete vConversao;		// Elimina o vetor porque o mesmo é grande e desnecessario daqui p/frente
  vConversao = NULL;

  rotulado = true;		// Define que o objeto ja esta rotulado
// Write("rotulaImagem.ma3.txt");
}
