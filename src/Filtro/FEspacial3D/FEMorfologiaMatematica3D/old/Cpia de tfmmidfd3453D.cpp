//�tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ].
#ifndef  TFMMIDFd3453D_cpp
#define  TFMMIDFd3453D_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
				Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:		Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TFMMIDFd3453D.cpp
Nome da classe:      TFMMIDFd3453D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a fun��o CriaMascara da classe TFMMIDFd3453D.
*/

//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------

#include "TFiltro\TFEspacial3D\TFEMorfologiaMatematica3D\TFMMIDFd3453D.h"  						//Classe base
#include "TMascara\TMCDiscreta3D\TBCd345.h"  						//ponteiro para
//#include "TMascara\TMascMascDiscretad34.h"  		//Cria objeto


/*
============================================================================
Documentacao 		CriaMascara
============================================================================
Descri��o:        Funcao que cria a mascara de chanfro adequada.
						O filtro TFMMIDF recebe a imagem pm, e o tamanho da mascara.
						E a fun��o Go recebe o raio m�ximo, define a vari�vel raioMaximo
						e chama cria mascara.
Pr�-condi��es:
Excess�es:        tipos de excessoes
Concorr�ncia:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
void TFMMIDFd3453D::CriaMascara(unsigned int _tamanhoMascara)
{
 if(mask)										//se existe uma mascara
	{
	 if(mask->GetNX()==_tamanhoMascara)	//e � do mesmo  tamanho
		  return;                      	//sai
	 delete mask;                     	//se  n�o � do mesmo tamanho apaga
	}                                   //e abaixo cria uma nova

 mask=new TBCd345(_tamanhoMascara); 		//se n�o existe a mascara, cria uma nova
}

/*
============================================================================
Documentacao      Go
============================================================================
Descri��o:
//Substitui o uso da mascara da mascara, que usava loop's e �ndices confusos
//pelo acesso direto aos valores da mascara.
//Ao lado do c�digo o esbo�o do ponto da mascara que esta sendo considerado.
//Da forma como esta o c�digo fica mais r�pido e compreensivo.
 //TIDF
 //Por uma quest�o de performance, deixar como esta.
 //Mas criar na classe base a mesma funcao mas com codigo generico
 //usando coisas do tipo  min (data3D[x][y][z] + masc->data3D[x][y][z])
 //Funcao IDFNosPlanosDeContorno();//preenche os planos de contorno com valor base
Programador:      Andre Duarte Bueno
*/
TMatriz3D *TFMMIDFd3453D::Go( TMatriz3D *& matriz, unsigned int _tamanhoMascara )
{
 InicializaIDF(matriz,_tamanhoMascara);		//armazena valores da matriz e _tamanhoMascara
															//verifica se pm->data2D e this->data2D tem as mesmas dimensoes
															//preenche os valores de data2D com os valores de pm
 //int register auxiliar;              		//
 //int register minimo;								//atributo da classe por causa funcao inline
 int x,y,z;												//Indices para percorrer a matriz
 IDFNosPlanosDeContorno(3);

 //-------------------------
 //MinimoIda
	for (z=1; z < nz-1 ;z++)						//inicio do 1 pois j� considerou planos 0 acima
	 for (y=1; y < ny-1 ;y++)
    	for (x=1; x < nx-1 ;x++)
			if (data3D[x][y][z]!=0)					//Testa a imagem, se nao for solido entra
				{                                //
				minimo = raioMaximo;					//assume valor maximo
				// 4  3  4	z=0
				//(3) x  3
				//(4)(3)(4)
				min(data3D[x-1][y]  [z] + 3);
				min(data3D[x-1][y-1][z] + 4); min(data3D[x][y-1][z] + 3); min(data3D[x+1][y-1][z] + 4);
				//(5)(4)(5)	z=-1
				//(4)(3)(4)
				//(5)(4)(5)
				min(data3D[x-1][y+1][z-1] + 5);   min(data3D[x][y+1][z-1] + 4);	min(data3D[x+1][y+1][z-1] + 5);
            min(data3D[x-1][y]  [z-1] + 4);   min(data3D[x][y]  [z-1] + 3);   min(data3D[x]  [y]  [z-1] + 4);
				min(data3D[x-1][y-1][z-1] + 5);   min(data3D[x][y-1][z-1] + 4);	min(data3D[x+1][y-1][z-1] + 5);
				data3D[x][y][z]=minimo;
				}

 //-------------------------
 //MinimoVolta
 for (z = nz-2; z > 0; z--)						  //-2 pois j� considerou plano z-1 acima
	 for (y = ny-2; y > 0; y--)
		for (x = nx-2; x > 0; x--)
			if (data3D[x][y][z]!=0)               //Se nao for solido
			  {
			  minimo = data3D[x][y][z];           //Armazena valor minimo da ida
				//(4)(3)(4)	z=0
				//(3) x  3
				// 4  3  4
			  min(data3D[x-1][y+1][z] + 4);  min(data3D[x][y+1][z]+3);  	min(data3D[x+1][y+1][z] + 4);//bug
																								min(data3D[x+1][y]  [z] + 3);
   			//(5)(4)(5)	z=+1
				//(4)(3)(4)
				//(5)(4)(5)
			  min(data3D[x-1][y+1][z+1]+5);   min(data3D[x][y+1][z+1]+4);  min(data3D[x+1][y+1][z+1]+5);
			  min(data3D[x-1][y  ][z+1]+4);	 min(data3D[x][y]  [z+1]+3);	min(data3D[x+1][y]  [z+1]+4);
			  min(data3D[x-1][y-1][z+1]+5);   min(data3D[x][y-1][z+1]+4);  min(data3D[x+1][y-1][z+1]+5);

			  data3D[x][y][z]=minimo;
			  }

return pm;
}

#endif




