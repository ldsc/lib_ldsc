/*
	===============================================================================
	PROJETO:          Biblioteca LIB_LDSC
	Ramo: TPadrao_ramo
	===============================================================================

	Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
		[LDSC].
	@author:          André Duarte Bueno
	File:             CRotulador.cpp
	begin:            Sat Sep 16 2000
	copyright:        (C) 2000 by André Duarte Bueno
	email:            andreduartebueno@gmail.com
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//Bibliotecas LIB_LDSC
//-----------------------------------------------------------------------
#include <Rotulador/CRotulador.h>
using namespace std;
/*
	-------------------------------------------------------------------------
	Função: Construtor
	-------------------------------------------------------------------------
	@short  :Zera ponteiros e inicializa variaveis.
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :
*/
CRotulador::CRotulador ( int _indice, int _fundo ) : INDICE(_indice), FUNDO(_fundo) {
	//Zera ponteiro para vetor conversão
	vConversao = NULL;
	//importante evita uso vetor área e perímetro sem rotular a imagem
	rotuloMaximoUtilizado = numeroObjetos = 0;
	//Vetor com a area dos objetos
	areaObjetos = NULL;
	//Vetor com os perimetros dos objetos
	perimetroObjetos = NULL;
	//Vetor com os raios hidraulicos
	//raioHidraulicoObjetos = NULL;
	//a matriz de dados ainda não foi rotulada
	rotulado = false;
	//rotuloInicial =0;
	//rotuloFinal = 0;
}

/*
	-------------------------------------------------------------------------
	Função:
	-------------------------------------------------------------------------
	@short  :Destrói objetos alocados dinamicamente
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :
*/
CRotulador::~CRotulador () {
	if (areaObjetos != NULL)
		delete areaObjetos; // objeto

	if (perimetroObjetos != NULL)
		delete perimetroObjetos;// objeto
	//if(raioHidraulicoObjetos!=NULL)
	//   delete          raioHidraulicoObjetos;// objeto

	//if(vConversao!=NULL)
	//   delete  vConversao;     //deletado na função de rotulagem // objeto
	//temporario
	//fout.close();
}

/*
	-------------------------------------------------------------------------
	Função: PesquisaRotulosValidosEOrdena
	-------------------------------------------------------------------------
	@short  :
	Este metodo é novo, e' mais rapido e eficiente.
	Pesquisa o vetor de rotulagem a procura de rotulos validos (positivos), e substitue os
	mesmos por rotulos validos e ordenados.
	Ou seja, substitue valores positivos com falhas na sequência por positivos sem falhas.
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :
*/

void CRotulador::PesquisaRotulosValidosEOrdena() {

	int contador = 1;		//0 sempre aponta para 0
	//LP-iniciando de 2 pois fundo pode ser 0 ou 1. for (int i = 2; i <= rotuloMaximoUtilizado; i++)	//Percorre o vetor vConversao até o maior rotulo utilizado
	for (int i = 1; i <= rotuloMaximoUtilizado; i++)	//Percorre o vetor vConversao até o maior rotulo utilizado
		if (vConversao->data1D[i] > 0)	//se for positivo é um rotulo válido
			vConversao->data1D[i] = contador++;	//Primeiro iguala depois incrementa
	//vConversao->data1D[0] = 0
	//vConversao->data1D[1] = 3
	//vConversao->data1D[2] = 3
	//..
	//vConversao->data1D[contador] = 3
	numeroObjetos = contador;	//Já considera o elemento 0
}

/*
	-------------------------------------------------------------------------
	Função:
	-------------------------------------------------------------------------
	@short  :
	Uniformiza a tabela de rótulos (para acesso direto)
	A tabela de rotulagem contem valores positivos se o rotulo é valido,
	e negativos quando não é um rotulo, mas uma referência indireta na
	propria tabela de rotulagem.
	Aqui, faz uma pesquisa nos valores de referencia indireta, deixando a tabela
	somente com rotulos válidos.
	Assim ao final desta função o vetor de conversao tem a forma:
	Posicao: 0,1,2,3,4, 5, 6, 7,...
	Valor:   0,1,3,6,8,12,15,20,...

	Obs: A função de rotulagem antiga, desenvolvida por Magnani, com base no algoritimo
	de Hoshem-Kopelman, realizava esta etapa na segunda passagem sobre
	a imagem para cada pixel (ou seja era mais lenta, pois para cada píxel
	a busca éra realizada).
	Aqui, a busca e substituição de rotulos indiretos por rotulos validos é feita
	uma única vez, percorrendo o vetor de rotulos.

	Aqui esta uma grande vantagem em relação ao Método de Hoshen e Kopelman,
	naquele algoritimo para cada pixel negativo da imagem faz esta busca.
	Agora, esta busca é realizada uma única vez.
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :
*/

void CRotulador::UniformizaTabelaRotulos () {
	//Percorre o vetor de rotulos
	int aux;
	for (int cont = 0; cont <= rotuloMaximoUtilizado; cont++) {
		aux = vConversao->data1D[cont];
		if (aux < 0) {
			//Enquanto o rotulo for negativo (indireto),
			while (aux < 0)
				//Procurar um valor positivo a ser utilizado (direto)
				aux = vConversao->data1D[-aux];
			//Fica com rótulo válido (direto)
			vConversao->data1D[cont] = aux;
		}
	}
}


/*
	==================================================================================
	Documentacao	RaioHidraulicoObjetos
	==================================================================================
	Descrição: Chama as funções de cálculo da área e do perímetro
*/

void CRotulador::CalculaRaioHidraulicoObjetos () {
	//TWaitCursor w;
	//Cria cursor de espera
	//Se ainda não determinada,              calcula a área dos objetos
	if (areaObjetos == NULL)
		CalculaAreaObjetos ();

	//Se ainda não determinada,      calcula o perimetro dos objetos
	if (perimetroObjetos == NULL)
		CalculaPerimetroObjetos ();
	//só procede o calculo se a matriz rótulo foi determinada
}

/*
	-------------------------------------------------------------------------
	Função:RaioHidraulicoObjetos(unsigned int k)
	-------------------------------------------------------------------------
	@short  :Função nova, dado o indice retorna o raioHidraulico em double.
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :
*/
double CRotulador::RaioHidraulicoObjetos (int k) const {
	if (k < (numeroObjetos))
		return areaObjetos->data1D[k] / (double) perimetroObjetos->data1D[k];	//quando o BC trava aparece esta linha???
	return 0.0;
}

/*
	-------------------------------------------------------------------------
	Função: GetareaObjetos(unsigned int k)
	-------------------------------------------------------------------------
	@short  :
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :
*/
int CRotulador::AreaObjetos (int k) const {
	if (k < (numeroObjetos) && areaObjetos != NULL)
		//return areaObjetos[k - rotuloInicial];
		return areaObjetos->data1D[k];
	return 0;
}

/*
	-------------------------------------------------------------------------
	Função: GetperimetroObjetos
	-------------------------------------------------------------------------
	@short  :
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :
*/
int CRotulador::PerimetroObjetos (int k) const {
	if (k < (numeroObjetos) && perimetroObjetos != NULL)
		return perimetroObjetos->data1D[k];
	//return perimetroObjetos[k - rotuloInicial];
	return 1;			//Evita bug chamada a GetraioHidraulicoObjetos
	//arrumar posteriormente
}


/*
	-------------------------------------------------------------------------
	Função:
	-------------------------------------------------------------------------
	@short  :
	Verifica e corrige a tabela de rotulagem para o ponto de contorno.
	Que pode ser o ponto acima ou o ponto atrês (atrês só para caso 3D).

	Obs:
	Se ocorrer de 8->-10 e depois 8-> -15 deve fazer 8->-15 e 10-> -15
	Em função disto, o valor anterior (-10) é armazenado em aux.

	Obs:
	Se ocorrer 10->-10 ocorre loop infinito, por isso verifica com o if(aux!=rotuloAtual)

	Contorno é uma posição do vetor de conversão
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :
*/

//void CRotulador::VerificaContorno( int contorno,  int  rotuloAtual)
void CRotulador::VerificaContorno (int contorno, int rotuloAtual) {
	//variável estatica criada uma única vez
	static int aux;
	//se o contorno não for fundo, deve ser verificado
	if (contorno != 0) {
		//armazena rotulo anterior do contorno
		aux = vConversao->data1D[contorno];
		//verifica se é válido, se não for,
		while (aux < 0) //localiza rótulo válido.
			aux = vConversao->data1D[-aux];
		//o contorno aponta para o rotulo atual
		vConversao->data1D[contorno] = -rotuloAtual;
		//se forem diferentes, então
		//o antigo rótulo do contorno aponta para o rotulo atual
		if (aux != rotuloAtual)
			vConversao->data1D[aux] = -rotuloAtual;
	}
}

/*
	-------------------------------------------------------------------------
	Função:
	-------------------------------------------------------------------------
	@short  :
	A tabela de rotulagem, mesmo depois de uniformizada, pode ter descontinuidades.
	Aqui, faz com que a tabela de rotulagem passe a ter valores crescentes
	Antes 0,1,2,5,7,12,....    -> Depois 0,1,2,3,4,5,6,7.....

	Da forma como esta é muito lenta, pois acaba fazendo um for dentro de outro.
	Obs: Em-4-12-98 Acrescentei a variavel   maiorRotuloUtilizado que deixa mais eficiente.
	Esta função foi descartada, só esta sendo utilizada por Go_Old.
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :
*/
/*
	void CRotulador::OrdenaTabelaRotulos()             // * * * * * * * * * * * * * * * * *
	{                                                  // D E S A T I V A D A (usa em Go_Old)
	int rotulosOrdenados=0;                            // * * * * * * * * * * * * * * * * *
	int rotulosDesordenados;

	//for(int i=0; i < vConversao->GetNX(); i++)//Percorre todo  vetor rotulo (vConversao)
	for(int i=0; i < rotuloMaximoUtilizado; i++)//Percorre todos os rotulos utilizados
	{
	rotulosDesordenados=0;
	//for(int k=0;k<vConversao->GetNX();k++)//pesquisa todo o vetor a procura de i
	for(int k=0;k< rotuloMaximoUtilizado;k++)//pesquisa até o maior rotulo utilizado a procura de i
	if(vConversao->data1D[k]==i)//se existe algum valor i
	{
	vConversao->data1D[k]=rotulosOrdenados;
	rotulosDesordenados++;//se ocorrerem trocas, acumula o numero de trocas realizadas
	}
	if(rotulosDesordenados>=1)
	rotulosOrdenados++;//incremente o contador de rotulosSequenciais
	}

	numeroObjetos = rotulosOrdenados;
	}
*/
/*
	==================================================================================
	Documentacao Go
	==================================================================================
	Descrição:			Executa a sequencia de rotulagem
	Pré-condições:    Os objetos alocados dinamicamente (vConversao, data..)
	devem ser alocados e testados dentro das respectivas funções.
	Na função identifica objetos aloca o vetor vConversao, utilizando
	um determinado numero de pontos para o vetor conversão. Se tentar alocar
	uma posição no vetor maior que aquela alocada ocorre estouro de pilha.
	Excessões:        tipos de excessoes
	Concorrência:
	Tempo processamento(s):
	Tamanho(bits):
	Comentarios:
	Programador:      Andre Duarte Bueno
	Descrição:
	O presente algoritimo é baseado no algoritimo de rotulagem de Hoshen-Kopelman.
	Porem, este codigo é um pouco mais eficiente, e retorna uma imagem final com rotulos sequenciais,
	ou seja 0,1,2,3,4....

	O aumento de eficiencia se deve aos fatores:
	1-Utilização de um vetor de conversão previamente definido. O algoritimo de Hoshen-Kopelman
	retorna no final a area dos objetos no vetor de rotulagem, aqui não. Para obter a area dos
	objetos use a funcao AreaObjetos, que cria e calcula o vetor areaObjetos

	2-Utilização de um contador para rotulagem , que é incrementado a cada nova linha.
	Ou seja a cada nova linha um mesmo objeto vai ter
	um rotulo ao final da primeira passagem maior que o último utilizado.

	3-Utilização do proprio vetor de conversao para deixar os rotulos finais sequenciais e ordenados.
	Isto é feito procurando-se no vetor de rotulagem os rotulos positivos e os colocando
	em ordem crescente e ordenada.

	4-Utilização do proprio vetor de conversão para correção dos indices indiretos

	5- Utilização do próprio vetor de conversão para correção dos rótulos.

	As diferenças são:
	-Uso de um vetor de conversão com valores iniciais previamente definidos 0->0, 1->1,...
	-Uso de um contador que cresce a cada nova linha, para definição dos rotulos na primeira passagem
	Este conceito reduz o número de verificações a serem realizadas.
	-Uso do proprio vetor de conversão para deixar os rotulos em sequência (0,1,2,3...)
	-Uso do proprio vetor de conversao para uniformização dos valores de rotulagem
	(eliminação de indices indiretos)

*/
/*
	void CRotulador::Go()
	{
	//{IProcessTime wait("IdentificaObjetos.tempo.txt");
	//IdentificaObjetos();}
	//vConversao->Write("1-VetorConversaoDepoisIdentificaObjetos.txt");
	IdentificaObjetos(rotuloInicial);

	//{IProcessTime wait("PesquisaRotulosValidosEOrdena.tempo.txt");
	//PesquisaRotulosValidosEOrdena();}
	//vConversao->Write("2-VetorConversaoDepoisPesquisaRotulosValidosEOrdena.txt");
	PesquisaRotulosValidosEOrdena();

	//{IProcessTime wait("UniformizaTabelaRotulos.tempo.txt");
	//UniformizaTabelaRotulos();}
	//vConversao->Write("3-VetorConversaoDepoisUniformizaTabelaRotulos.txt");
	UniformizaTabelaRotulos();

	//{IProcessTime wait("RotulaImagem.txt");
	//RotulaImagem();}
	RotulaImagem();

	rotulado=true;       //Define que a imagem já foi rotulada
	};*/

//A função abaixo deve ser  descartada, pois é muito lenta.
//A lentidão esta na função OrdenaTabelaRotulos.
/*
	void CRotulador::Go_Old()
	{
	//{IProcessTime wait("IdentificaObjetos.tempo.txt");
	//IdentificaObjetos();}
	//vConversao->Write("1 (nova_old) vetor conversao depois de identificar objetos.txt");
	IdentificaObjetos();

	//{IProcessTime wait("UniformizaTabelaRotulos.tempo.txt");
	//UniformizaTabelaRotulos();}
	//vConversao->Write("2 (nova_old) vetor conversao depois de uniformizar.txt");
	UniformizaTabelaRotulos();

	//{IProcessTime wait("OrdenaTabelaRotulos.tempo.txt");
	//OrdenaTabelaRotulos();}
	//vConversao->Write("3 (nova_old) vetor conversao depois de ordenar.txt");
	OrdenaTabelaRotulos();


	//{IProcessTime wait("RotulaImagem.tempo.txt");
	//RotulaImagem();}
	RotulaImagem();

	rotulado=true;       //Define que a imagem já foi rotulada
	};
*/
/*
	==================================================================================
	Documentacao CmCalculaRaioHidraulicoObjetos
	==================================================================================
	Descrição:        Calcula o raio hidraulico*100
	Como raio hidraulico é area sobre perimetro, e atualmente use
	vetor de inteiros, multiplica por 100.
	extraido do arquivo  hidraulico.cpp
	PS: Função eliminada, elimina-se a necessidade do vetor raioHidraulicoObjetos
	agora sempre calcula o raioHidraulico.


	Pré-condições:
	Excessões:        tipos de excessoes
	Concorrência:
	Tempo processamento(s):
	Tamanho(bits):
	Comentarios:
	Programador:      Andre Duarte Bueno
*/
/*
	void 	CRotulador::RaioHidraulicoObjetos()
	{
	//TWaitCursor w;//cria cursor de espera
	if(areaObjetos==NULL)//Se ainda não determinada, determina
	AreaObjetos();//calcula a área dos objetos
	if(perimetroObjetos==NULL)//Se ainda não determinada, determina
	PerimetroObjetos();//calcula o perimetro dos objetos
	//só procede o calculo se a matriz rótulo foi determinada
	if(areaObjetos && perimetroObjetos)
	{
	if(raioHidraulicoObjetos)
	delete raioHidraulicoObjetos;//cria vetor raio hidraulico

	raioHidraulicoObjetos = new TVetor (numeroObjetos);//(areaObjetos->GetNX() );

	if(raioHidraulicoObjetos)//Calculo do RAIO HIDRAULICO
	{
	raioHidraulicoObjetos->Constante(0);
	for(unsigned int k = 0; k < raioHidraulicoObjetos->GetNX() ; k++)
	{
	if(perimetroObjetos->data1D[k]!=0)//Desnecessário, o perímetro nunca será zero?
	raioHidraulicoObjetos->data1D[k]=100 * areaObjetos->data1D[k]/perimetroObjetos->data1D[k];
	}
	//Salvar(raioHidraulicoObjetos,"raioHidraulicoObjetos.rho");
	}
	}
	}
*/
