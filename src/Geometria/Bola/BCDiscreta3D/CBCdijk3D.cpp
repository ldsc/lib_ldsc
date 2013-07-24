
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCdijk3D.cpp
Nome da classe:      CBCdijk3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:Implementa as funções da classe CBCdijk3D.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
// Classe base
#include <Geometria/Bola/BCDiscreta3D/CBCdijk3D.h>

/*
==================================================================================
Documentacao Construtor
==================================================================================
Descrição:        O construtor chama as funções de calculo do raio bola e
			depois a funcao de preenchimento da mascara
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
// Divide o tamanho da mascara por dois, porque o método atual de calculo
// só usa a metade da mascara // :CBCDiscreta ((tamanhoMascara-1)/2+1)

// Mascara dijk  comparando coma d5711 fica da forma:
// mi=5 mj=7 mk=11    0<= raioBase <= 5-1, raiobase fica entre 0 e 4, sendo o default=2
CBCdijk3D::CBCdijk3D (unsigned int tm, int i, int j, int k, int rb)
	: CMCdijk (i, j, k, rb), CBCDiscreta3D (tm) { // no lugar de tm rb ??? verificar
	unsigned int dist;		// distancia calculada
	unsigned int raio = RaioX ();	// Como a bola é cúbica uso raio=raioX=raioY=raioZ

	// calculo raio bola inclusa e tangente
	raioBolaInclusa = mi * (raio);	//
	raioBolaTangente = mi * (raio + 1);	//

	// variaveis otimizacao, usadas calculo simetria
	unsigned int posxe, posxd;	// x esquerda e x direita
	unsigned int posys, posyn;	// y sul e y norte
	unsigned int poszf, poszb;	// z front , e z back
	unsigned int miXraio = mi * raio;	// otimização

	for (unsigned int xx = 0; xx <= raio; xx++) {	// Usa xx, metade da bola
		posxe = raio - xx;
		posxd = raio + xx;
		for (unsigned int yy = 0; yy <= raio; yy++) {	// só percorre meia bola por ser simétrica
			// calculo da distancia, Falta generalizar (valores 3 e 4), e o uso do raio base
			posys = raio - yy;
			posyn = raio + yy;
			for (unsigned int zz = 0; zz <= raio; zz++)	{ // calcula distancias
				poszf = raio + zz;
				poszb = raio - zz;
				if ((xx >= yy) && (xx >= zz))
					dist = mi * xx + yy + zz;	// dist=3*x+y+z;
				else if ((yy >= zz) && (yy > xx))
					dist = mi * yy + zz + xx;	// dist=3*y+z+x;
				else
					dist = mi * zz + xx + yy;	// dist=3*z+x+y;

				if (dist <= miXraio) {
					data3D[posxe][posyn][poszf] = data3D[posxe][posys][poszf] =
							data3D[posxd][posyn][poszf] =
							data3D[posxd][posys][poszf] =
							data3D[posxe][posyn][poszb] =
							data3D[posxe][posys][poszb] =
							data3D[posxd][posyn][poszb] =
							data3D[posxd][posys][poszb] = dist;
				} else {
					data3D[posxe][posyn][poszf] = data3D[posxe][posys][poszf] =
							data3D[posxd][posyn][poszf] =
							data3D[posxd][posys][poszf] =
							data3D[posxe][posyn][poszb] =
							data3D[posxe][posys][poszb] =
							data3D[posxd][posyn][poszb] =
							data3D[posxd][posys][poszb] = 0;
				}
			}
		}
	}
	data3D[raio][raio][raio] = 1;
}

/*
void CBCdijk3D::CalculaRaioBola()
{
	 unsigned int raio=RaioX();// Como a bola é quadrada uso raio=raioX=raioY
	raioBolaInclusa= mj*(raio-1);// 5
	raioBolaTangente=mj*(raio-1) +1;// 6
}    */
