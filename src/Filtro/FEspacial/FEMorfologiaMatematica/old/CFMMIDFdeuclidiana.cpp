// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef  CFMMIDFEuclidiana_cpp
#define  CFMMIDFEuclidiana_cpp
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
				Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------
Desenvolvido por:		Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFEuclidiana.cpp
Nome da classe:      CFMMIDFEuclidiana
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a fun��o CriaMascara da classe CFMMIDFEuclidiana.
*/
// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

// #include <math.h>      // Classe base

#include "TFiltro\TFEspacial\TFEMorfologiaMatematica\CFMMIDFEuclidiana.h"	// Classe base
#include "TMascara\TMCDiscreta\TBCEuclidiana.h"	// ponteiro para

/*
============================================================================
Documentacao 		CriaMascara
============================================================================
Descri��o:      Funcao que cria a mascara de chanfro adequada.
		O filtro TIDF recebe a imagem pm, e o tamanho da mascara.
*/

void
CFMMIDFEuclidiana::CriaMascara (unsigned int _tamanhoMascara)
{
  if (mask)
    delete mask;
  mask = new TBCEuclidiana (_tamanhoMascara);	// valores mi,mj,rb definidos pelo construtor de TBCEuclidiana 
}


// Ao lado do c�digo o esbo�o do ponto da mascara que esta sendo considerado.
// Da forma como esta o c�digo fica mais r�pido e compreensivo.
// O valor M_SQRT2 � da biblioteca matematica � o valor da raiz de 2,sqrt(2).
// Como os numeros inteiros sem sinal (unsigned int),  s�o usados para criar a matriz de
// dados, e o espectro de valores dos unsigned int vai de 0 a 65535
// posso multiplicar cada ponto por 100. Ao fazer isso estou permitindo o uso de unsigned int
// para armazenar a dist�ncia euclidiana (sem o uso de flutuantes). Mas estou limitando
// o tamanho da maior bola a ser considerada. A maior bola pode ter o tamanho de 65535/100=655 pixel's..
// Que � maior que as imagens normalmente consideradas. Assim para imagens de at� 655 p�xel's, este
// processo � v�lido. Acima disso podem ocorrer erros para bolas maior que 655.
// CMatriz2D * CFMMIDFEuclidiana::Go( CMatriz2D*& matriz)
/*
/*
CMatriz2D *CFMMIDFEuclidiana::Go( CMatriz2D *& matriz, unsigned int _tamanhoMascara )
{
 InicializaIDF(matriz,_tamanhoMascara);		// armazena valores da matriz e _tamanhoMascara
						// verifica se pm->data2D e this->data2D tem as mesmas dimensoes
						// preenche os valores de data2D com os valores de pm
 // funcionava com int x,y;			// Indices para percorrer a matriz
 int x,y;					// Indices para percorrer a matriz
 float auxiliar,minimo;
 unsigned int R2X100 =M_SQRT2*100;		// raiz de 2 multiplicada por 100

 // Multiplico toda a matriz por 100, para usar mais duas casas decimais
 for (y=0; y < ny ;y++)
	for (x=0; x < nx ;x++)
			data2D[x][y]*=100;
 // ida   MinimoIda
 // Da forma como esta n�o percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
	 for (y=1; y < ny ;y++)                  	 	  // NY() � igual a ny, ny da matriz idf
		for (x=1; x < nx-1 ;x++)
			if (data2D[x][y]!=0)			  // Testa a imagem, se nao for solido entra
				{                                     // 
				minimo = raioMaximo*100;              // 255*100=25500
					auxiliar=data2D[x-1][y-1]+R2X100 ; //   1  		x
					if (auxiliar < minimo)             //  (sqrt(2)) 1  sqrt(2)
						minimo = auxiliar;
					auxiliar=data2D[x][y-1]+100;    	  //     1 		x
					if (auxiliar < minimo)             //   sqrt(2) (1) sqrt(2)
						minimo = auxiliar;
					auxiliar=data2D[x+1][y-1]+R2X100 ; //   1  		x
					if (auxiliar < minimo)             //   sqrt(2)  1  (sqrt(2))
						minimo = auxiliar;
					auxiliar=data2D[x-1][y]+100;    	  //  (1)  		x
					if (auxiliar < minimo)             //   sqrt(2)  1  sqrt(2)
						minimo = auxiliar;
				data2D[x][y]=minimo;
				}
 // volta    MinimoVolta
 // Da forma como esta n�o percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
	 for (y = ny-2; y >= 0; y--)						  // -2 pois come�a do zero e a mascara tem tamanho 1
		for (x = nx-2; x > 0; x--)
			if (data2D[x][y]!=0)                  	  // Se nao for solido
				{
				minimo = data2D[x][y];		           // Armazena valor minimo da ida,
									  // como foi multiplicado por 100, divido por 100
					  auxiliar=data2D[x+1][y+1]+R2X100 ;//   sqrt(2)  1 (sqrt(2))
					  if (auxiliar < minimo)            //      		 x  	1
							minimo = auxiliar;
					  auxiliar=data2D[x][y+1]+100;    	//   sqrt(2) (1) sqrt(2)
					  if (auxiliar < minimo)            //      		 x  	 1
							minimo = auxiliar;
					  auxiliar=data2D[x-1][y+1]+R2X100 ;//  (sqrt(2)) 1  sqrt(2)
					  if (auxiliar < minimo)            //      		 x  	 1
							minimo = auxiliar;
					  auxiliar=data2D[x+1][y]+100;     	//   sqrt(2)  1  sqrt(2)
					  if (auxiliar < minimo)            //      		 x 	(1)
							minimo = auxiliar;
				data2D[x][y]=minimo;
				}
return pm;
}
*/


/*
============================================================================
Fun��o Abertura(unsigned int raioMascaraAtual)
============================================================================
Executa abertura a partir da idf
// Esta fun��o s� deve ser acessada depois de calculada a idf
// idf->Go() calcula a idf.
// S� usa valores da mascara, n�o usa valores da mascmasc (que foi eliminada).

// Para cada RaioBola apaga a mascara atual e cria uma nova.
// A altera��o do tamanho da mascara, implica num calculo da PreencheMascara diferente, pois a
// PreencheMascara usa valores de raio da mascara.

// Da forma como foi definida a mascara s� usa valores em que y=0, maskd->data2D[x][0]
// ou seja a mascara funciona como um vetor.
// Observe que usa o tamanhoMascara para criar a mascara.
// O construtor da mascara recebe o tamanhoMascara (igual as demais mascaras)
// mas como as mascaras de chanfro foram desenvolvidas para uso de somente um quadrante,
// o primeiro quadrante. O construtor das mascaras reduz o tamanho da mascara para somente
// um quadrante.
// Assim a mascara n�o precisa ser do tamanho de  tamanhoMascara, mas sim de raioMascara+1.
// embora isso seja mais confuso � mais r�pido pois s� acessa pontos v�lidos da mascara.

// Obs: observe que n�o precisa testar se a mascara estoura o intervalo de pontos da imagem
// pois as bolas s�o todas inclusas na imagem
// for(int j=maskd->Getraio(); j < ny-maskd->Getraio(); j++)
// for(int i=maskd->Getraio(); i< nx-maskd->Getraio(); i++)
*/

// O calculo da fun��o  Abertura nesta classe � diferente do da classe base.
// A primeira diferen�a � que a idf calculada usando a CFMMIDFEuclidiana, esta multiplicada por 100.
// Logo preciso considerar isso, e o fa�o multiplicando as fun��es Getraio.. por 100.
// A segunda � que a mascara TBCEuclidiana � uma mascara preenchida corretamente, e n�o  um vetor
// como usado na TIDFd34. Assim percorro toda a mascara e verifico se o valor da mascara � 1 ou 0
// se for 1 seto o valor da matriz auxiliar pm.
// virtual CMatriz2D* Abertura(unsigned int _raioBola);// redefinida,
/*
CMatriz2D*  CFMMIDFEuclidiana::Abertura(CMatriz2D*& matriz,unsigned int _RaioBola)	// 
{
 pm=matriz;
 int i,j;
					// chamar antes de criar a mascara
 tamanhoMascara=2*_RaioBola+1;         // O tamanho da mascara � 2* o raio+1
 CriaMascara(tamanhoMascara);		// Cria a mascara adequada,do tamanho de tamanhoMascara
					// Se a bola tem raio=1 o tamanhoMascara=3
 pm->Constante(0);			// zera a matriz imagem
 CBCDiscreta* maskd     = dynamic_cast<CBCDiscreta*>(mask);// Converte a mascara

					// Percorre a idf (desconsidera as bordas, por causa da mascara)
  for( j=maskd->RaioY(); j < ny-maskd->RaioY(); j++)
	  for( i=maskd->RaioX(); i< nx-maskd->RaioX(); i++)
		  {
		  // este if executa de fato a erosao, se o valor do pixel � maior
		  // que determinado valor ele fica, sen�o sai
		  if (data2D[i][j] >  maskd->GetraioBolaTangente()*100)
					pm->data2D[i][j]=1;     // seta ponto da imagem como pertencente a bola

		  // este else if executa ent�o a dilata��o
		  // se o pixel tem um valor que pode sofrer dilata��o, executa a dilata��o
		  else if (data2D[i][j] > maskd->GetraioBolaInclusa()*100)
			for (int x=0; x < maskd->NX(); x++)	// percorre a mascara
			  for (int y=0; y < maskd->NY();y++)
					if(maskd->data2D[x][y]!=0)			// se o ponto da mascara for ativo, setar na imagem auxiliar como ativo
					pm->data2D[i+x-maskd->RaioX()][j+y-maskd->RaioY()]=1;// seta ponto da imagem como pertencente a bola
		  }
  // aqui pm � uma imagem de 0 e 1, os n�meros 1 formam as bolas para o raio da mascara atual
return pm;
}
*/
/*
============================================================================
Documentacao 		Erosao
============================================================================
Descricao: A fun��o abaixo implementa o conceito de erosao

Observe que n�o � preciso criar a mascara, visto que a idf j� � euclidiana
e a defini��o dos p�xel ativos ap�s a eros�o pode ser realizada sem a mascara
ou seja n�o preciso das fun��es GetraioBolaInclusa e GetraioBolaTangente
*/
/*
CMatriz2D*  CFMMIDFEuclidiana::Erosao(CMatriz2D*& matriz,unsigned int _RaioBola)	// 
{
 pm=matriz;
 int i,j;
 unsigned int  RaioBolaTangenteX100=(_RaioBola+1)*100;
 // tamanhoMascara=2*_RaioBola+1;
 // CriaMascara();
 // CBCDiscreta* maskd     = dynamic_cast<CBCDiscreta*>(mask);// converte ponteiro para mascara discreta
  for( j=0; j < ny; j++)               // percorre a imagem
	  for( i=0; i< nx; i++)
		  {
		  // if (data2D[i][j] >  maskd->GetraioBolaTangente())
		  if (data2D[i][j] > RaioBolaTangenteX100 )  // > ou >=?
					pm->data2D[i][j]=1;     // seta ponto ativo
		  else
					pm->data2D[i][j]=0;     // seta ponto inativo
		  }
return pm;
}
  */
/*
============================================================================
Documentacao 		Dilatacao
============================================================================
Descricao: A fun��o abaixo implementa o conceito de Dilatacao
*/
/*
CMatriz2D*  CFMMIDFEuclidiana::Dilatacao(CMatriz2D*& matriz,unsigned int _RaioBola)	// 
{
 pm=matriz;
 int i,j;
				// chamar antes de criar a mascara
 tamanhoMascara=2*_RaioBola+1;         // O tamanho da mascara � 2* o raio+1
 CriaMascara(tamanhoMascara);		// Cria a mascara adequada,do tamanho de tamanhoMascara
					// Se a bola tem raio=1 o tamanhoMascara=3
 CBCDiscreta* maskd     = dynamic_cast<CBCDiscreta*>(mask);// Converte a mascara
					// Percorre a idf (desconsidera as bordas, por causa da mascara)
  for( j=maskd->RaioY(); j < ny-maskd->RaioY(); j++)
	  for( i=maskd->RaioX(); i< nx-maskd->RaioX(); i++)
		  {
		  // este else if executa a dilata��o
		  // se o pixel tem um valor que pode sofrer dilata��o, executa a dilata��o
		  if (data2D[i][j] > maskd->GetraioBolaInclusa()*100)  // _RaioBola*100
			for (int x=0; x < maskd->NX(); x++)	// percorre a mascara
			  for (int y=0; y < maskd->NY();y++)
					if(maskd->data2D[x][y]!=0)			// se o ponto da mascara for ativo, setar na imagem auxiliar como ativo
					pm->data2D[i+x-maskd->RaioX()][j+y-maskd->RaioY()]=1;// seta ponto da imagem como pertencente a bola
		  }
  // aqui pm � uma imagem de 0 e 1, os n�meros 1 formam as bolas para o raio da mascara atual
return pm;
}
*/

#endif
