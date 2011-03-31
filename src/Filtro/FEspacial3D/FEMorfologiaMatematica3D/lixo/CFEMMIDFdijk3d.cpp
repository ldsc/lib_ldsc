// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ].
#ifndef  CFEMMIDFdijk3D_cpp
#define  CFEMMIDFdijk3D_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFdijk3D.cpp
Nome da classe:      CFEMMIDFdijk3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a fun��o CriaMascara da classe CFEMMIDFdijk3D.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/FEMMIDFdijk3D.h"	// Classe base
#include "Geometria/Bola/BCDiscreta3D/CBCdijk3D.h"	// ponteiro para
// #include "TMascara\TMascMascDiscretad34.h"            // Cria objeto


/*
============================================================================
Documentacao 		CriaMascara
============================================================================
Descri��o:        Funcao que cria a mascara de chanfro adequada.
						O filtro CFEMMIDF recebe a imagem pm, e o tamanho da mascara.
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
void
CFEMMIDFdijk3D::CriaMascara (unsigned int _tamanhoMascara)
{
  if (mask)			// se existe uma mascara
    {
      if (mask->GetNX () == _tamanhoMascara)	// e � do mesmo  tamanho
	return;			// sai
      delete mask;		// se  n�o � do mesmo tamanho apaga
    }				// e abaixo cria uma nova

  mask = new CBCdijk3D (_tamanhoMascara, mi, mj, mk, raioBase);	// se n�o existe a mascara, cria uma nova
}

/*
============================================================================
Documentacao      Go
============================================================================
Descri��o:
// Substitui o uso da mascara da mascara, que usava loop's e �ndices confusos
// pelo acesso direto aos valores da mascara.
// Ao lado do c�digo o esbo�o do ponto da mascara que esta sendo considerado.
// Da forma como esta o c�digo fica mais r�pido e compreensivo.
 // TIDF
 // Por uma quest�o de performance, deixar como esta.
 // Mas criar na classe base a mesma funcao mas com codigo generico
 // usando coisas do tipo  min (data3D[x][y][z] + masc->data3D[x][y][z])
 // Funcao IDFNosPlanosDeContorno();// preenche os planos de contorno com valor base
Programador:      Andre Duarte Bueno
*/
TMatriz3D *
CFEMMIDFdijk3D::Go (TMatriz3D * &matriz, unsigned int _tamanhoMascara)
{
  ExecutadaPorGo (matriz);	// pm=matriz, copia valores, verfica

//  IDFNosPlanosDeContornoIDA(mi);
  // -------------------------
  int x, y, z;			// Indices para percorrer a matriz
  register int xm1;		// x+1
  register int x_1;		// x-1
  register int ym1;		// y+1
  register int y_1;		// y-1
  register int zm1;		// z+1
  register int z_1;		// z-1

  // MinimoIda
  for (z = 1; z < nz - 1; z++)	// inicio do 1 pois j� considerou planos 0 acima
    {
      z_1 = z - 1;
      for (y = 1; y < ny - 1; y++)
	{
	  ym1 = y + 1;
	  y_1 = y - 1;
	  for (x = 1; x < nx - 1; x++)
	    {
	      if (data3D[x][y][z] != 0)	// Testa a imagem, se nao for solido entra
		{		// 
		  xm1 = x + 1;
		  x_1 = x - 1;
		  minimo = raioMaximo;	// assume valor maximo
		  //  4  3  4      z=0
		  // (3) x  3
		  // (4)(3)(4)
		  min (data3D[x_1][y][z] + mi);	// ponto[x][y]
		  min (data3D[x_1][y_1][z] + mj);
		  min (data3D[x][y_1][z] + mi);
		  min (data3D[xm1][y_1][z] + mj);
		  // (5)(4)(5)     z=-1
		  // (4)(3)(4)
		  // (5)(4)(5)
		  min (data3D[x_1][ym1][z_1] + mk);
		  min (data3D[x][ym1][z_1] + mj);
		  min (data3D[xm1][ym1][z_1] + mk);
		  min (data3D[x_1][y][z_1] + mj);
		  min (data3D[x][y][z_1] + mi);
		  min (data3D[xm1][y][z_1] + mj);
		  min (data3D[x_1][y_1][z_1] + mk);
		  min (data3D[x][y_1][z_1] + mj);
		  min (data3D[xm1][y_1][z_1] + mk);
		  data3D[x][y][z] = minimo;
		}
	    }
	}
    }
  // -------------------------
//  IDFNosPlanosDeContornoVOLTA(mi);
  // MinimoVolta
  for (z = nz - 2; z > 0; z--)	// -2 pois j� considerou plano z-1 acima
    {
      zm1 = z + 1;
      for (y = ny - 2; y > 0; y--)
	{
	  ym1 = y + 1;
	  y_1 = y - 1;
	  for (x = nx - 2; x > 0; x--)
	    {
	      if (data3D[x][y][z] != 0)	// Se nao for solido
		{
		  xm1 = x + 1;
		  x_1 = x - 1;
		  minimo = data3D[x][y][z];	// Armazena valor minimo da ida
		  // (4)(3)(4)     z=0
		  // (3) x  3
		  //  4  3  4
		  min (data3D[x_1][ym1][z] + mj);
		  min (data3D[x][ym1][z] + mi);
		  min (data3D[xm1][ym1][z] + mj);	// bug
		  /*ponto[x][y] */ min (data3D[xm1][y][z] + mi);
		  // (5)(4)(5)     z=+1
		  // (4)(3)(4)
		  // (5)(4)(5)
		  min (data3D[x_1][ym1][zm1] + mk);
		  min (data3D[x][ym1][zm1] + mj);
		  min (data3D[xm1][ym1][zm1] + mk);
		  min (data3D[x_1][y][zm1] + mj);
		  min (data3D[x][y][zm1] + mi);
		  min (data3D[xm1][y][zm1] + mj);
		  min (data3D[x_1][y_1][zm1] + mk);
		  min (data3D[x][y_1][zm1] + mj);
		  min (data3D[xm1][y_1][zm1] + mk);

		  data3D[x][y][z] = minimo;
		}
	    }
	}
    }
  return pm;
}

#endif
