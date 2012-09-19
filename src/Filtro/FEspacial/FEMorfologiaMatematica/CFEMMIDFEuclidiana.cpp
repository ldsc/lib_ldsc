//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
				Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------
Desenvolvido por:		Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFEuclidiana.cpp
Nome da classe:      CFEMMIDFEuclidiana
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a função CriaMascara da classe CFEMMIDFEuclidiana.
*/
//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------

//  #include <math.h>      //  Classe base

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFEuclidiana.h"
//  Classe base

#include "Geometria/Bola/BCDiscreta/CBCEuclidiana.h"
//  ponteiro para


/*
==================================================================================
Documentacao 		CriaMascara
==================================================================================
Descrição:      Funcao que cria a mascara de chanfro adequada.
*/

void CFEMMIDFEuclidiana::CriaMascara (unsigned int _tamanhoMascara) {
  if (mask)
    delete mask;
  mask = new CBCEuclidiana (_tamanhoMascara);	//  valores mi,mj,rb definidos pelo construtor de CBCEuclidiana 
}


//  Ao lado do código o esboço do ponto da mascara que esta sendo considerado.
//  Da forma como esta o código fica mais rápido e compreensivo.
//  O valor M_SQRT2 é da biblioteca matematica é o valor da raiz de 2,sqrt(2).
//  Como os numeros inteiros sem sinal (unsigned int),  são usados para criar a matriz de
//  dados, e o espectro de valores dos unsigned int vai de 0 a 65535
//  posso multiplicar cada ponto por 100. Ao fazer isso estou permitindo o uso de unsigned int
//  para armazenar a distância euclidiana (sem o uso de flutuantes). Mas estou limitando
//  o tamanho da maior bola a ser considerada. A maior bola pode ter o tamanho de 65535/100=655 pixel's..
//  Que é maior que as imagens normalmente consideradas. Assim para imagens de até 655 píxel's, este
//  processo é válido. Acima disso podem ocorrer erros para bolas maior que 655.
//  TCMatriz2D< int > * CFEMMIDFEuclidiana::Go( TCMatriz2D< int >*& matriz)
/*
/*
TCMatriz2D< int > *CFEMMIDFEuclidiana::Go( TCMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
{
 InicializaIDF(matriz,_tamanhoMascara);		//  armazena valores da matriz e _tamanhoMascara
						//  verifica se pm->data2D e this->data2D tem as mesmas dimensoes
						//  preenche os valores de data2D com os valores de pm
 //  funcionava com int x,y;			//  Indices para percorrer a matriz
 int x,y;					//  Indices para percorrer a matriz
 float auxiliar,minimo;
 unsigned int R2X100 =M_SQRT2*100;		//  raiz de 2 multiplicada por 100

 //  Multiplico toda a matriz por 100, para usar mais duas casas decimais
 for (y=0; y < ny ;y++)
	for (x=0; x < nx ;x++)
			data2D[x][y]*=100;
 //  ida   MinimoIda
 //  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
	 for (y=1; y < ny ;y++)                  	 	  //  NY() é igual a ny, ny da matriz idf
		for (x=1; x < nx-1 ;x++)
			if (data2D[x][y]!=0)			  //  Testa a imagem, se nao for solido entra
				{                                     //  
				minimo = raioMaximo*100;              //  255*100=25500
					auxiliar=data2D[x-1][y-1]+R2X100 ; //    1  		x
					if (auxiliar < minimo)             //   (sqrt(2)) 1  sqrt(2)
						minimo = auxiliar;
					auxiliar=data2D[x][y-1]+100;    	  //      1 		x
					if (auxiliar < minimo)             //    sqrt(2) (1) sqrt(2)
						minimo = auxiliar;
					auxiliar=data2D[x+1][y-1]+R2X100 ; //    1  		x
					if (auxiliar < minimo)             //    sqrt(2)  1  (sqrt(2))
						minimo = auxiliar;
					auxiliar=data2D[x-1][y]+100;    	  //   (1)  		x
					if (auxiliar < minimo)             //    sqrt(2)  1  sqrt(2)
						minimo = auxiliar;
				data2D[x][y]=minimo;
				}
 //  volta    MinimoVolta
 //  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
	 for (y = ny-2; y >= 0; y--)						  //  -2 pois começa do zero e a mascara tem tamanho 1
		for (x = nx-2; x > 0; x--)
			if (data2D[x][y]!=0)                  	  //  Se nao for solido
				{
				minimo = data2D[x][y];		           //  Armazena valor minimo da ida,
									  //  como foi multiplicado por 100, divido por 100
					  auxiliar=data2D[x+1][y+1]+R2X100 ;//    sqrt(2)  1 (sqrt(2))
					  if (auxiliar < minimo)            //       		 x  	1
							minimo = auxiliar;
					  auxiliar=data2D[x][y+1]+100;    	//    sqrt(2) (1) sqrt(2)
					  if (auxiliar < minimo)            //       		 x  	 1
							minimo = auxiliar;
					  auxiliar=data2D[x-1][y+1]+R2X100 ;//   (sqrt(2)) 1  sqrt(2)
					  if (auxiliar < minimo)            //       		 x  	 1
							minimo = auxiliar;
					  auxiliar=data2D[x+1][y]+100;     	//    sqrt(2)  1  sqrt(2)
					  if (auxiliar < minimo)            //       		 x 	(1)
							minimo = auxiliar;
				data2D[x][y]=minimo;
				}
return pm;
}
*/


/*
==================================================================================
Função Abertura(unsigned int raioMascaraAtual)
==================================================================================
Executa abertura a partir da idf
//  Esta função só deve ser acessada depois de calculada a idf
//  idf->Go() calcula a idf.
//  Só usa valores da mascara, não usa valores da mascmasc (que foi eliminada).

//  Para cada RaioBola apaga a mascara atual e cria uma nova.
//  A alteração do tamanho da mascara, implica num calculo da PreencheMascara diferente, pois a
//  PreencheMascara usa valores de raio da mascara.

//  Da forma como foi definida a mascara só usa valores em que y=0, maskd->data2D[x][0]
//  ou seja a mascara funciona como um vetor.
//  Observe que usa o tamanhoMascara para criar a mascara.
//  O construtor da mascara recebe o tamanhoMascara (igual as demais mascaras)
//  mas como as mascaras de chanfro foram desenvolvidas para uso de somente um quadrante,
//  o primeiro quadrante. O construtor das mascaras reduz o tamanho da mascara para somente
//  um quadrante.
//  Assim a mascara não precisa ser do tamanho de  tamanhoMascara, mas sim de raioMascara+1.
//  embora isso seja mais confuso é mais rápido pois só acessa pontos válidos da mascara.

//  Obs: observe que não precisa testar se a mascara estoura o intervalo de pontos da imagem
//  pois as bolas são todas inclusas na imagem
//  for(int j=maskd->Getraio(); j < ny-maskd->Getraio(); j++)
//  for(int i=maskd->Getraio(); i< nx-maskd->Getraio(); i++)
*/

//  O calculo da função  Abertura nesta classe é diferente do da classe base.
//  A primeira diferença é que a idf calculada usando a CFEMMIDFEuclidiana, esta multiplicada por 100.
//  Logo preciso considerar isso, e o faço multiplicando as funções Getraio.. por 100.
//  A segunda é que a mascara CBCEuclidiana é uma mascara preenchida corretamente, e não  um vetor
//  como usado na TIDFd34. Assim percorro toda a mascara e verifico se o valor da mascara é 1 ou 0
//  se for 1 seto o valor da matriz auxiliar pm.
//  virtual TCMatriz2D< int >* Abertura(unsigned int _raioBola);//  redefinida,
/*
TCMatriz2D< int >*  CFEMMIDFEuclidiana::Abertura(TCMatriz2D< int >*& matriz,unsigned int _RaioBola)	//  
{
 pm=matriz;
 int i,j;
					//  chamar antes de criar a mascara
 tamanhoMascara=2*_RaioBola+1;         //  O tamanho da mascara é 2* o raio+1
 CriaMascara(tamanhoMascara);		//  Cria a mascara adequada,do tamanho de tamanhoMascara
					//  Se a bola tem raio=1 o tamanhoMascara=3
 pm->Constante(0);			//  zera a matriz imagem
 CBCDiscreta* maskd     = dynamic_cast<CBCDiscreta*>(mask);//  Converte a mascara

					//  Percorre a idf (desconsidera as bordas, por causa da mascara)
  for( j=maskd->RaioY(); j < ny-maskd->RaioY(); j++)
	  for( i=maskd->RaioX(); i< nx-maskd->RaioX(); i++)
		  {
		  //  este if executa de fato a erosao, se o valor do pixel é maior
		  //  que determinado valor ele fica, senão sai
		  if (data2D[i][j] >  maskd->GetraioBolaTangente()*100)
					pm->data2D[i][j]=1;     //  seta ponto da imagem como pertencente a bola

		  //  este else if executa então a dilatação
		  //  se o pixel tem um valor que pode sofrer dilatação, executa a dilatação
		  else if (data2D[i][j] > maskd->GetraioBolaInclusa()*100)
			for (int x=0; x < maskd->NX(); x++)	//  percorre a mascara
			  for (int y=0; y < maskd->NY();y++)
					if(maskd->data2D[x][y]!=0)			//  se o ponto da mascara for ativo, setar na imagem auxiliar como ativo
					pm->data2D[i+x-maskd->RaioX()][j+y-maskd->RaioY()]=1;//  seta ponto da imagem como pertencente a bola
		  }
  //  aqui pm é uma imagem de 0 e 1, os números 1 formam as bolas para o raio da mascara atual
return pm;
}
*/
/*
==================================================================================
Documentacao 		Erosao
==================================================================================
Descricao: A função abaixo implementa o conceito de erosao

Observe que não é preciso criar a mascara, visto que a idf já é euclidiana
e a definição dos píxel ativos após a erosão pode ser realizada sem a mascara
ou seja não preciso das funções GetraioBolaInclusa e GetraioBolaTangente
*/
/*
TCMatriz2D< int >*  CFEMMIDFEuclidiana::Erosao(TCMatriz2D< int >*& matriz,unsigned int _RaioBola)	//  
{
 pm=matriz;
 int i,j;
 unsigned int  RaioBolaTangenteX100=(_RaioBola+1)*100;
 //  tamanhoMascara=2*_RaioBola+1;
 //  CriaMascara();
 //  CBCDiscreta* maskd     = dynamic_cast<CBCDiscreta*>(mask);//  converte ponteiro para mascara discreta
  for( j=0; j < ny; j++)               //  percorre a imagem
	  for( i=0; i< nx; i++)
		  {
		  //  if (data2D[i][j] >  maskd->GetraioBolaTangente())
		  if (data2D[i][j] > RaioBolaTangenteX100 )  //  > ou >=?
					pm->data2D[i][j]=1;     //  seta ponto ativo
		  else
					pm->data2D[i][j]=0;     //  seta ponto inativo
		  }
return pm;
}
  */
/*
==================================================================================
Documentacao 		Dilatacao
==================================================================================
Descricao: A função abaixo implementa o conceito de Dilatacao
*/
/*
TCMatriz2D< int >*  CFEMMIDFEuclidiana::Dilatacao(TCMatriz2D< int >*& matriz,unsigned int _RaioBola)	//  
{
 pm=matriz;
 int i,j;
				//  chamar antes de criar a mascara
 tamanhoMascara=2*_RaioBola+1;         //  O tamanho da mascara é 2* o raio+1
 CriaMascara(tamanhoMascara);		//  Cria a mascara adequada,do tamanho de tamanhoMascara
					//  Se a bola tem raio=1 o tamanhoMascara=3
 CBCDiscreta* maskd     = dynamic_cast<CBCDiscreta*>(mask);//  Converte a mascara
					//  Percorre a idf (desconsidera as bordas, por causa da mascara)
  for( j=maskd->RaioY(); j < ny-maskd->RaioY(); j++)
	  for( i=maskd->RaioX(); i< nx-maskd->RaioX(); i++)
		  {
		  //  este else if executa a dilatação
		  //  se o pixel tem um valor que pode sofrer dilatação, executa a dilatação
		  if (data2D[i][j] > maskd->GetraioBolaInclusa()*100)  //  _RaioBola*100
			for (int x=0; x < maskd->NX(); x++)	//  percorre a mascara
			  for (int y=0; y < maskd->NY();y++)
					if(maskd->data2D[x][y]!=0)			//  se o ponto da mascara for ativo, setar na imagem auxiliar como ativo
					pm->data2D[i+x-maskd->RaioX()][j+y-maskd->RaioY()]=1;//  seta ponto da imagem como pertencente a bola
		  }
  //  aqui pm é uma imagem de 0 e 1, os números 1 formam as bolas para o raio da mascara atual
return pm;
}
*/
