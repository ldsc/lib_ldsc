/*
----------------------------------------------------------------------------
PROJETO: LIB_LDSC
	Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi, Marcos Damiani,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CConfiguracoesEquilibrio2D.h
Nome da classe:      	CConfiguracoesEquilibrio2D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------

==================================================================================
Documentacao Classe:	  CConfiguracoesEquilibrio2D
==================================================================================
Assunto:               CConfiguracoesEquilibrio2D
Superclasse:
Descrição:             Declara a classe CConfiguracoesEquilibrio.
Acesso:
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
----------------------------------------------------------------------------
*/

#include <cstdio>
#include <sstream>
#include <iomanip>
#include <AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CConfiguracoesEquilibrio2D.h>
#include <Base/COperacao.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd4.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd8.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd5711.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFEuclidiana.h>
using namespace std;

//  Cria alguns objetos agregados e zera ponteiros
CConfiguracoesEquilibrio2D::CConfiguracoesEquilibrio2D ( /*TCMatriz2D< int >* imagem */ ostream & out)
{
	// Flags de controle
	os = &out; 				// temporaria, para saida tela
	//salvarDisco = 0;//;			// temporaria
	salvarResultadosParciaisDisco = 0; 	// não salvar resultados parciais em disco;
	salvarResultadosFinaisDisco = 1;	// salvar resultados finais em disco
	visualizar = 0;				// temporaria
	contadorPassosExecutados = 0;		// zera o contador de passos executados. (necessário para Next)

	// Objetos agregados
	idf = nullptr;
	rotulador = nullptr;
	camara = nullptr;
	camara = new CCamara2D ();	//  Cria objetos agregados
	COperacao::TestaAlocacao (camara, "objeto camara, construtor CConfiguracoesEquilibrio2");
/* Falta implementar sobrecarga do operador << em TCMatriz2D
	if (visualizar == 1)
	{
		TCMatriz2D< int > *ptr_camara = static_cast<TCMatriz2D< int > *>(camara);
		out << (*ptr_camara) << endl;
	}
*/
	fluidoA = new CMFluido ();	//  Podem ser alterados usando
	fluidoB = new CMFluido ();	//  confEqui->fluidoA->atributo
	COperacao::TestaAlocacao (fluidoB, "objeto fluidoB, construtor CConfiguracoesEquilibrio2");
						
						//  define os atributos default deste objeto
	CConfiguracoesEquilibrio2D::DefineAtributos ();	
}

//  Define os valores dos indices das diferentes regiões geométricas determinadas
void CConfiguracoesEquilibrio2D::DefineAtributos()
{
	tipoIDF = 34;			//  usa por default a métrica d34
	//  0=solido ou paredes
	//  1=poros
	// Valores abaixo devem ser utilizados quando quizer gerar as imagens com ~65000 cores.
	// Pode-se usar qualquer conjunto de valores, desde que não ocorram repeticoes.
	// Para melhorar a qualidade da visualizacao da imagem gerada, usar TCMatriz2D< int >->NumCores(n);
	// sendo n maior que o maior valor encontrado na imagem.
	B0 = 65000;			//  2;//  indices das diferentes regiões geométricas
	A0 = 15000;
	B = 65000;			//  magnani=4
	A = 15000;			//  magnani=5
	Ai = 10000;			//  ?nao usado
	G = 20000;			//  6;
	G_ = 25000;
	//  Y0=8;
	Yi = 30000;			//  magnani=3
	KGB0 = 35000;
	wbG__U_KGB0 = 40000;
	KwbG__U_KGB0B0 =  45000;
/*
	Tarefa: Leandro, voltar a usar os indices de Magnani, na hora de salvar a imagem setar o numero de cores com NumCores (testar resultado).
	B0 = 4;//26214			//  2;//  indices das diferentes regiõees geométricas
	A0 = 5;//32768
	B = 4;//26214			//  magnani=4
	A = 5;//32768			//  magnani=5
	Ai = 2;//13107			//  ?nao usado
	G = 3;//19660			//  6;
	G_ = 6;//39321
	//  Y0=8;
	Yi = 3;//19660			//  magnani=3
	KGB0 = 7;//45875
	wbG__U_KGB0 = 88;//52428
	KwbG__U_KGB0B0 = 9;// 65535
*/
	Ai 	= 1;	//13107 	// nao usado????
	Yi 	= 2;	//19660
	B0 	= 3;	//26214
	B  	= 3;	//26214
	G  	= 4;	//19660
	G_ 	= 5;	//39321
	A0 	= 6;	//32768
	A  	= 6;	//32768
	KGB0	= 7;	//45875
	wbG__U_KGB0 	= 8;	//52428
	KwbG__U_KGB0B0 = 9;	// 65535
	//  Y0=8;
	raioMaximo = 5;	//  Raio máximo a ser utilizado no processo
	raioMinimo = 1;	//  Raio minimo a ser utilizado no processo
}

CConfiguracoesEquilibrio2D::~CConfiguracoesEquilibrio2D ()
{
  if (camara)
   	delete camara;
  if (idf)
   	delete idf;
  if (rotulador)
   	delete rotulador;
  if (fluidoA)
   	delete fluidoA;
  if (fluidoB)
   	delete fluidoB;
}

//  ====================================
//  Construcão da Camara
//  ====================================
//  Usa a idf d34 para chutar o raioMaximo (usa somente a imagem), ou seja
//  na imagem da camara o raio maximo pode ser maior em funcao da camara
void CConfiguracoesEquilibrio2D::CriaCamara(TCMatriz2D< int > * &imagem)
{
  Salvar(imagem, "0-imagemInicial.pgm"); 		//  Salva a imagem inicial
  
  {							// Calcula estimativa do raio máximo
    // PODE SER OTIMIZADO, SUBSTITUIR POR NX/4?
    // Usa idfd34 apenas para estimar o raio máximo, depois vai criar a idf selecionada
		TCFEMMIDFd34<int> idfp(imagem);				//  calcula a idf da imagem
    idfp.Go( imagem );					//  e determina o raioMaximo

    int maiorValor = idfp.MaiorValor();			//  a ser utilizado na construcao da camara
    raioMaximo = (maiorValor % 3 == 0) ? maiorValor / 3 : 1 + maiorValor / 3;
    // raioMaximo= ( maiorValor % idfp.Mi() == 0)? maiorValor / idfp.Mi() : 1 + maiorValor /idfp.Mi() ;
  }
  
  //  Define as propriedades da camara
  camara->DimensoesPadroes( raioMaximo );		
  							//    wb=0        //  wb=1
  camara->CamaraInferior( fluidoB->Molhabilidade() == 0 ? 2 * raioMaximo + 1 : 1 );	//  5=membrana+2
  
  camara->MembranaInferior( fluidoB->Molhabilidade() == 0 ? 0 : 2 );	//  3->2
  
  camara->MembranaSuperior( fluidoB->Molhabilidade() == 0 ? 2 : 0 );	//  3->2
  
  camara->CamaraSuperior( fluidoB->Molhabilidade() == 0 ? 1 : 2 * raioMaximo + 1 );	//  5->1
  
  //  Define os indices das camaras superior e inferior
  camara->IndiceCamaraInferior( B0 );
  camara->IndiceCamaraSuperior( A0 );
  
  //  Aloca a camara, define as paredes e copia imagem
  camara->CriaCamara( imagem );	
/* Falta implementar sobrecarga do operador << em TCMatriz2D
  if (visualizar == 1)
  {
	*os << " \nDados objeto camara criacao camara\n";
	TCMatriz2D< int >* ptr_camara = static_cast<TCMatriz2D< int >*>(camara);
	*os << *ptr_camara << endl;
  }
*/
  //  if(imagem!=nullptr)
  //     delete imagem;
  //  imagem=nullptr;//  Deleta a imagem recebida, economia.
}

//  ====================================
//  Construcao da IDF
//  ====================================
void
CConfiguracoesEquilibrio2D::CriaIDF ()
{
  if (idf)
    delete idf;			//  Se a idf já existe, deleta
  idf = nullptr;

  TCMatriz2D< int >* ptr_camara = static_cast<TCMatriz2D< int >*>(camara);

  switch ( tipoIDF )		//  Cria objeto idf selecionado
  {
    case 4:
			idf = new TCFEMMIDFd4<int>( ptr_camara );
      break;
    case 8:
			idf = new TCFEMMIDFd8<int>( ptr_camara );
      break;
    case 34:
			idf = new TCFEMMIDFd34<int>( ptr_camara );
      break;
    case 5711:
			idf = new TCFEMMIDFd5711<int>( ptr_camara );
      break;
    case 1:
    default:
			idf = new TCFEMMIDFEuclidiana<int>( ptr_camara );
      break;
  }
  COperacao::TestaAlocacao (idf,"objeto idf, funcao CConfiguracoesEquilibrio2D:");
  
  //  Calcula a idf 					//  criar funcao separada?
  idf->Go ( ptr_camara );				//  Calcula a idf
  idf->IndiceAtivo (G);					//  Define o indice dos píxel's que pertencem a imagem aberta
  int maiorValor = idf->MaiorValor ();			//  Calcula o raioMaximo a ser utilizado
  raioMaximo = (maiorValor % idf->Mi() == 0) ? maiorValor / idf->Mi() : 1 + maiorValor / idf->Mi ();
  
  TCMatriz2D< int >* ptr_idf = static_cast<TCMatriz2D< int >*>( idf );
  Salvar (ptr_idf, string("1-imagemIDF.pgm"));
}

//  ====================================
//  Criação do objeto de rotulagem
//  ====================================
void
CConfiguracoesEquilibrio2D::CriaRotulador ()
{
  if(rotulador)
    delete rotulador;		//  Deleta objeto de rotulagem anterior
  rotulador = nullptr;
  TCMatriz2D< int >* ptr_camara = static_cast<TCMatriz2D< int >*>(camara);
  rotulador = new CConectividade2D (ptr_camara);	//  Cria novo objeto rotulador
  COperacao::TestaAlocacao (rotulador, "objeto rotulador, funcao CConfiguracoesEquilibrio2D::Go");
}

//  ==================================================
//  Abertura: Calcula a abertura sobre a IDF da camara
//  ==================================================
void
CConfiguracoesEquilibrio2D::CalculaAbertura ( int & raio )
{
  	TCMatriz2D< int >* ptr_camara = static_cast<TCMatriz2D< int >*>(camara);
  	idf->Abertura (ptr_camara, raio);	//  Determina as regiões abertura (G) na camara
  	Salvar (ptr_camara, "2-imagemG-(4)-%d.pgm");	//  ....Tem na camara(0,G=3)...
	
/*	
	// correção da abertura.
	TCMatriz2D< int > *pm2D = new TCMatriz2D< int >(*ptr_camara);
	ptr_camara->Constante(0);
	int nx = pm2D->NX();
	int ny = pm2D-> NY();
	bool pertence;
	for (int j = raio; j < ny - raio; j++) {	// Percorre a imagem idf
    		for (int i = raio; i < nx - raio; i++) {
    			if ( pm2D->data2D[i][j] == G ){
    				pertence = false;
    				for(int x = -raio; x <= raio; x++) {
    					if (pm2D->data2D[i+x][j] == G) {
    						pertence = true;
    					} else {
    						pertence = false;
    						break;
    					}
    				}
    				if ( pertence ){
    					for(int x = -raio; x <= raio; x++) {
    						ptr_camara->data2D[i+x][j] = G;
    					}
    				}
    			}
    		}
    	}
//    	repintar membranas eliminadas com correcao acima
 camara->DefineCamaraInferior ();	
 camara->DefineCamaraSuperior ();	
*/  	
    	
  
  idf->CorrigeAbertura(ptr_camara, G);
  Salvar (ptr_camara, "2.1-imagemG-(corrigida)-%d.dgm"); // imagem G corrigida para não apresentar erro físico.
  
  //TCMatriz2D< int >* ptr_mask = static_cast<TCMatriz2D< int >*>(idf->mask);
  //Salvar (ptr_mask, "2.2-mask-%d.pgm");

//  Abaixo só é necessário se for usar G_
//  OBS: Como agora só calcula KGB0 ou wbG_U_KGB0 o passo abaixo é desnecesário.
/*   if(fluidoB->Molhabilidade()==1)  		//  se wb=1 considera G_
	for(int i=0;i<camara->NX();i++)        //  Usa a propria IDF para armazenar o resultado da abertura.
	 for(int j=0;j<camara->NY();j++)       //  se pertence a regiï¿½o verde complementar (G_)
	  if(camara->data2D[i][j]==G)          	//  inverte o sinal na idf
		idf->data2D[i][j]= - idf->data2D[i][j];//  idf com sinal negativo indica regiï¿½o G, + G_
*/
  /*
     int larguraCampoOld=idf->larguraCampo;
     idf->larguraCampo=4;
     Salvar(idf,"3-imagemIDF-GG_-%d.pgm");     //  salva idf_G_G em disco
     idf->larguraCampo=larguraCampoOld;
   */
}

//  ====================================
//  Conectividade entre K(G,B0)
//  ====================================
void CConfiguracoesEquilibrio2D::ConectividadeKGB0 ()
{

  camara->DefineCamaraInferior ();					// Redesenha camara inferior, eliminada na abertura
									// no sistema novo, implementado com coilib, isto é desnecessário,
									// (não tem a membrana, a camara não entra no cálculo da abertura)
									//  ....Tem na camara(0,G=3,B0=4)...
  TCMatriz2D< int >* ptr_camara = static_cast<TCMatriz2D< int >*>( camara );
  rotulador->Go ( ptr_camara );						//  Realiza a rotulagem
  
  TCMatriz2D< int >* ptr_rotulador = static_cast<TCMatriz2D< int >*>(rotulador);
  Salvar (ptr_rotulador, string("4-imagemG+B0-Rotulada-%d.pgm"));	//  salva imagem rotulada
  
  // Tarefa: Leandro renomear VerificaConectividade , para PintaRegiaoConexa ou DefineRegiaoConexa 
  rotulador->VerificaConectividade (G, B0, KGB0);			//  Verifica a conectividade entre G e B0, e pinta a regiao conexa com KGB0
  //  rotulador->VerificaConectividade(G,B0,B);				//  Verifica a conectividade entre B0 e G, e pinta a regiao conexa com B
  Salvar (ptr_camara, "5-imagem-K(G,B0)-(0,3,7)-%d.pgm");		//  ....Tem na camara(0,G,KGB0)...
  indiceParcialB = KGB0;	//  define o indiceParcial para B
}

//  ====================================
//  Uniao    G_U K(G,B0)]
//  ====================================
void CConfiguracoesEquilibrio2D::UniaoG__U_KGB0 ()
{
  for (int i = 0; i < camara->NX (); i++)			//  Percorre a imagem
    for (int j = 0; j < camara->NY (); j++)
								//  Se esta na imagem inicial (idf>0) e nao esta na camara (G) entao é abertura complementar (G-)
      if (idf->data2D[i][j] > 0 && camara->data2D[i][j] == 0)
		camara->data2D[i][j] = wbG__U_KGB0;		//  =8, a uniao pode ser pintada com um unico indice wbG-_U_KGB0
      else
		camara->data2D[i][j] = 0;			//  o restante passa a ser zero
  
  //  ....Tem na camara(0,wbG-_U_KGB0)...
  //  if(fluidoB->Molhabilidade()==1)                 		//  se wb=1 a IDF tem valores negativos
  //  RestabeleceIDFPositiva();                 		//  A idf passa a ser positiva e representa a imagem inicial
  TCMatriz2D< int >* ptr_camara = static_cast<TCMatriz2D< int >*>(camara);
  Salvar (ptr_camara, "6-imagem-wbG__U_K(G,B0)-(0,8)-%d.pgm");	//  0,wbG-_U_KGB0=8,
}

//  =========================================
//  Conectividade entre K{ [G_U K(G,B0)],B0)}
//  =========================================
void CConfiguracoesEquilibrio2D::ConectividadeKwbG__U_KGB0B0()
{
  camara->DefineCamaraInferior ();					//  redesenha camara inferior (B0), eliminada na verificacao da conectividade
  
  									//  ....Tem na camara(0,wbG__U_KGB0,B0)...
  rotulador->Go (camara);						//  Antes: 2ï¿½ rotulagem Agora: 1ï¿½ rotulagem
  
  TCMatriz2D< int >* ptr_rotulador = static_cast<TCMatriz2D< int >*>(rotulador);
  Salvar (ptr_rotulador, "7.0-imagem-wbG__U_KGB0B0-rotulada-%d.pgm");	//  ....Tem na camara(0,wbG__U_KGB0=8,B0=4,)...
  
  rotulador->VerificaConectividade (wbG__U_KGB0, B0, KwbG__U_KGB0B0);	//  
  
  									//  rotulador->VerificaConectividade(wbG__U_KGB0,B0,B); //  
  indiceParcialB = KwbG__U_KGB0B0;					//  =9
  
  TCMatriz2D< int >* ptr_camara = static_cast<TCMatriz2D< int >*>(camara);
  Salvar (ptr_camara, "7.1-imagem-KwbG__U_KGB0B0-Omega-(0,8,9)-%d.pgm");//  ....Tem na camara(0,wbG__U_KGB0=8,KwbG__U_KGB0B0=9,)...
}

//  ====================================
//  Definicao dos valores de B, o restante e' Y
//  Aqui a imagem tem (B->indiceRegiaoB, 0, A0, um outro indice que representa Y)
void CConfiguracoesEquilibrio2D::SolucaoOmega (int &indiceRegiaoB, TCMatriz2D< int > * &imagem)
{
  //  Função: MarcaYi()
  //  Transformacao OPCIONAL
  //  Marca Yi como sendo os valores que estao na idf e nao pertencem a regiao B
  for (int i = 0; i < camara->NX (); i++)
    for (int j = 0; j < camara->NY (); j++)

      if ( idf->data2D[i][j] != 0 && camara->data2D[i][j] != indiceRegiaoB )
	camara->data2D[i][j] = Yi;					//  Se pertence a imagem (idf) e nao a regiao B entao e Yi
      else
	camara->data2D[i][j] = 0;					//  caso contrario nao e Yi
  
  TCMatriz2D< int >* ptr_camara = static_cast<TCMatriz2D< int >*>(camara);
  Salvar (ptr_camara, "7.2-imagem-Yi-(0,3)-%d.pgm");			//  ....Tem na camara(0,Yi)...

  //  Funcao DeterminaA0()
  //  Aqui determina a regiao conexa a A0, definindo a forma final (A0,Y,B), sem considerar Yi anterior
  camara->DefineCamaraSuperior ();					//  redesenha camara superior   //  ....Tem na camara(0,Yi,A0)...
  Salvar (ptr_camara, "7.3-imagem-Yi+camaraSuperior-(0,3,5)-%d.pgm");
  									//  Verificar conectividade (Y,A0,A0)
  rotulador->Go (camara);						//  precisa re-rotular a imagem,(Antes: 3ï¿½ rotulagem, Agora 2ï¿½ rotulagem)
  rotulador->VerificaConectividade (Yi, A0, A0);			//  verifica a conectividade de Yi com A0 e marca como A0
  Salvar (ptr_camara, "8.1-imagem-Yi+A0-(0,3,5)-%d.pgm");		//  Aqui tem 0, Yi,A0

//  Funcao DeterminaB()
//  1-Acima, eliminou B da camara, 2-?? recoloca os valores de B na camara
//  3-Ja considera B da imagem anterior.
  int deslocX = camara->DeslocamentoNX ();				//  obtem deslocamento para posicao
  int deslocY = camara->DeslocamentoNY ();				//  inicial da imagem na camara
  int ti, tj;								//  variaveis temporarias (otimizacao)
  for (int i = 0; i < imagem->NX (); i++)				//  percorre a imagem
    {
      ti = i + deslocX;
      for (int j = 0; j < imagem->NY (); j++)
	{
	  tj = j + deslocY;
	  if (	idf->data2D[ti][tj] != 0 &&				//  Se pertence a imagem (IDF) e
		camara->data2D[ti][tj] != Yi 				//  nao e Yi 
		&& camara->data2D[ti][tj] != A0 			// ou A0, entao e B.
		|| imagem->data2D[i][j] == B				//  ou, se esta na imagem anterior era B
	     )
	    camara->data2D[ti][tj] = B;					//  pinta de B
	}
    }
    
  Salvar (ptr_camara, "8.2-imagem-Yi+B+A0-(0,3,4,5)-%d.pgm");		//  Aqui tem 0, Yi,A0

}

//  Obs: Quando existe um && ou um || que nao e satisfeito sai, nao verificando os demais.
//  Desta forma, quando usar if( ..&&..||..) sempre coloque no inicio aqueles que sao mais usados.

//  ====================================
//  CorrecaocaxYi
//  ====================================
//  Determinação da regiao Yi (ca=1)
//  Aqui ja tenho a solucao da regiao OMEGA, se ca = 1 imcompressivel, precisa considerar ca*Yi
//  Aqui a camara tem a solucao para Omega no instante de tempo atual,
//  A imagem tem a solucao no instante anterior
//  Devo eliminar Yi da solucao de Omega, B = Omega - ca*Yi ~ Omega -ca * (Yi-1)
void CConfiguracoesEquilibrio2D::CorrecaocaxYi (TCMatriz2D< int > * &imagem)
{

  //  if(imagem) delete imagem; imagem = nullptr;
  //  imagem = new TCMatriz2D< int >();                                         //  camara->nxImg(),camara->nyImg()
  //  sprintf(nomeArquivo, "10-imagem-%d.dat",     contadorPassosExecutados-1);
  //  imagem->Read( nomeArquivo );                				//  Le a imagem do disco

  int DeslocX = camara->DeslocamentoNX ();
  int DeslocY = camara->DeslocamentoNY ();
  for (int i = 0; i < imagem->NX (); i++)
    for (int j = 0; j < imagem->NY (); j++)
      if ( imagem->data2D[i][j] == Yi )					//  Se for Yi,na imagem (instante de tempo anterior)
	camara->data2D[i + DeslocX][j + DeslocY] = Yi;			//  pinta como Yi na camara

  //  if(imagem) delete imagem; imagem = nullptr;
  TCMatriz2D< int >* ptr_camara = static_cast<TCMatriz2D< int >*>(camara);
  Salvar (ptr_camara, "9-imagem-K{WBxG__U_K(G,B0),B0}-caYi-%d.pgm");
  //  CorrecaoBolas(); ??
}

//  ====================================
//  SolucaoFinal(), salva em disco
//  ====================================
//  Pensar em Criar: SetdeslocX(), SetdeslocY(), Write..
void CConfiguracoesEquilibrio2D::SolucaoFinal (TCMatriz2D< int > * &imagem) 
{
  //  verificar como esta a imagem
  int DeslocX = camara->DeslocamentoNX ();
  int DeslocY = camara->DeslocamentoNY ();
  
  for (int i = 0; i < imagem->NX (); i++)
    for (int j = 0; j < imagem->NY (); j++)
      imagem->data2D[i][j] = camara->data2D[i + DeslocX][j + DeslocY];	//  Copia valores da camara atual para a imagem
  
  if ( salvarResultadosFinaisDisco ) 
  {
	//  sprintf (nomeArquivo, "10-imagem-%d.pgm", contadorPassosExecutados);
	ostringstream os;
	os.setf(ios::right);
	os << "10-SolucaoFinal-Passo-" << setw(4) << right << setfill('0') << contadorPassosExecutados << ".pgm";
	nomeArquivo = os.str();
									// novidade
	imagem->SetFormato (P2_X_Y_GRAY_ASCII);
 	imagem->NumCores (imagem->MaiorValor());
	imagem->Write (os.str().c_str());	//  A cada passo, deve salvar a configuracao final de equilibrio
  }
}

// Retorna ponteiro para uma matriz 2D que aponta para uma imagem da região informada (A ou B) extraída da câmara.
// esta função deve ser chamada após Next ou Go.
// Quem recebe o ponteiro tem a responsabilidade de deletar a imagem após o uso, evitando-se vazamento de memória.

// Tarefa: Leandro, o indice deve ser qualquer indice trabalhado, isto e, A, B,Y, A0, BO, e valores parciais,
// e não apenas A e B. Isto deixa o código mais genérico e util.
// TCMatriz2D< int > * GetImagem ( const int regiao ) const;
TCMatriz2D< int > * CConfiguracoesEquilibrio2D::GetImagem( const int regiao ) const
{
	TCMatriz2D< int > * imagem = new TCMatriz2D< int >(camara->NxImg(), camara->NyImg());	// aloca matriz 2D com as dimensões da imagem da câmara

	return GetImagem( imagem , regiao );
}

// A funcao deve ter uma outra versão, uma versao que recebe uma imagem e apenas pinta a regiao de interesse
//TCMatriz2D< int > * GetImagem( CImagem2D* imagem, const int regiao ) const;
TCMatriz2D< int > * CConfiguracoesEquilibrio2D::GetImagem( TCMatriz2D< int >* imagem, const int regiao) const
{
	int DeslocX = camara->DeslocamentoNX ();
	int DeslocY = camara->DeslocamentoNY ();
/*
	int valor = 0;
	switch ( regiao )
	{
		case 'A': 	valor = A;
				break; 
		case 'B': 	valor = B; 
				break;
		default : 	return imagem;	
	}	
*/
	int nxMin = std::min( imagem->NX(), static_cast<int>( camara->NxImg() ) );	
	int nyMin = std::min( imagem->NY(), static_cast<int>( camara->NyImg() ) );	
	
	for (int i = 0; i < nxMin; i++)
		for (int j = 0; j < nyMin; j++)
	{
		if(camara->data2D[i + DeslocX][j + DeslocY] == regiao)		// Se o pixel analizado na camara corresponder ao valor da região informada.
			imagem->data2D[i][j] = 1;				// Atribui o valor 1 ao pixel da imagem que corresponde a cor da regiao.
		else								// Se o pixel analizado não corresponder a região.
			imagem->data2D[i][j] = 0;				// Atribui o valor 0 ao pixel da imagem.
	}
	
	return imagem;
}

//  ========================================
//  Reestabelece os valores positivos na IDF
//  ========================================
// não esta sendo chamada
void CConfiguracoesEquilibrio2D::RestabeleceIDFPositiva ()
{
	for (int i = 0; i < idf->NX (); i++)					//  Percorre a imagem idf
		for (int j = 0; j < idf->NY (); j++)				//  e restabelece os valores negativos->para positivos,
			if (idf->data2D[i][j] < 0)				//  para evitar erro na operaï¿½ï¿½o de abertura
				idf->data2D[i][j] = -idf->data2D[i][j];		//  
}


// Salva a matriz imagem em disco, monta o nome considerando parametros da simulação.
void CConfiguracoesEquilibrio2D::Salvar (TCMatriz2D< int > * &imagem, string msg)
{
  char nomeArquivo[255];

  string buffer;
  buffer = (fluidoB->Molhabilidade () == 1) 	? "wb1-" : "wb0-";
  buffer += (fluidoA->Compressibilidade () == 1)? "ca1-" : "ca0-"; 		// 2007 estava trocado
  buffer += msg;

  // Substitue o %d pelo contadorPassosExecutados
  //  porque nao usar o raio? porque o raio pode estar diminuindo.
  sprintf (nomeArquivo, buffer.c_str (), contadorPassosExecutados);

  if (salvarResultadosParciaisDisco == 1) { 
		imagem->SetFormato( P2_X_Y_GRAY_ASCII );
  	int cores = imagem->MaiorValor();
  	imagem->NumCores( (cores >= 2) ? cores : cores+2 );
    	imagem->Write (nomeArquivo);
  }
/* Falta implementar sobrecarga do operador << em TCMatriz2D
  if (visualizar == 1)
    {
      (*os) << (*imagem) << "\n" << nomeArquivo;
      cin.get ();
    }
*/
}

//  *********************************************************************
//  AQUI DIFERENCA DA VERSAO DO ARTIGO PARA O CODIGO:
//  No artigo faz: K(G,B0)
//  No codigo faz: K(G-Yi,B0)
//  APÓS REALIZAR OPERACAO ABERTURA, MARCA PONTOS Y DA IMAGEM ANTERIOR
//  Não esta sendo utilizada
void CConfiguracoesEquilibrio2D::DiferencaEmRelacaoArtigo (TCMatriz2D< int > * &imagem)
{
  int DeslocX = camara->DeslocamentoNX ();
  int DeslocY = camara->DeslocamentoNY ();
  for (int i = 0; i < imagem->NX (); i++)
    for (int j = 0; j < imagem->NY (); j++)
      if (imagem->data2D[i][j] == Yi)				//  Se for Yi na imagem (instante de tempo anterior)
	   camara->data2D[i + DeslocX][j + DeslocY] = 0;	//  pinta como Yi na camara (coisas que eram B passam a ser Yi)
}

//  ====================================
//  FUNCAO Next
//  ====================================
//   Funcao:    bool CConfiguracoesEquilibrio2D::Next(TCMatriz2D< int >* imagem)
//   Objetivo:  Determinar passo-a-passo as configurações de equilíbrio geométricas em um processo de interação entre
//               dois fluidos (fluidoA e fluidoB) em uma "camara" de um "porosimetro".
//   Comentário: Por uma questão de economia de memória, a imagem idf é usada para armazenar os índices de G e G_
//               ou seja, as regiões verde (abertura) e verde complementar. Isto é realizado pintando com valor negativo a região G_

// Porque esta recebendo a imagem??
bool CConfiguracoesEquilibrio2D::Next(TCMatriz2D< int > * &imagem)
{
	if (contadorPassosExecutados == 0) 		// só irá entrar na primeira vez
  		{
    		CriaCamara(imagem);			//  Cria a camara considerando a imagem e a Molhabilidade do fluido B
    		CriaIDF();				//  Cria a IDF, a partir da camara (calcula a idf e o raioMaximo)
    		CriaRotulador();			//  Cria o objeto de rotulagem, passando a camara
    		raio = fluidoB->Molhabilidade() == 0 ? raioMaximo : raioMinimo;
  		}
	
							// Next irá retornar verdadeiro enquanto existirem passos a serem executados.
	if ( fluidoB->Molhabilidade() == 0 ? raio >= raioMinimo : raio <= raioMaximo ) 
		{
		contadorPassosExecutados++;		//  Incrementa o numero de passos executados (inicialmente=1)
		CalculaAbertura(raio);			//  Realiza abertura para o raio atual
		
							//  se wb=1 o raio cresce, em função da membrana KGB0 e sempre B0.
		if (fluidoB->Molhabilidade() == 0)
		{ 					//  DiferencaEmRelacaoArtigo(imagem);
			ConectividadeKGB0();		//  Calcula a relacao K(G,B0)
		}
		
		if (fluidoB->Molhabilidade() == 1)
		{					//  se wb=1 considera G_
			UniaoG__U_KGB0();		//  Realiza a uniao de G_ com K(G,B0)
							//  DiferencaEmRelacaoArtigo(imagem);
			ConectividadeKwbG__U_KGB0B0();	//  Calcula a relacao KwbG__U_KGB0B0
		}
							//  Solucao final para Omega, 0,B,Yi,A0
		SolucaoOmega(indiceParcialB, imagem);	//  MarcaYi   //  DeterminaA0   //  ReestabeleceB
							//  Se ca=1, e nao for a primeira passagem
		
		if (fluidoA->Compressibilidade() == 0 && contadorPassosExecutados != 1)
			CorrecaocaxYi(imagem);		//  Se for inCompressivel e nao for a primeira passagem entra
		
		SolucaoFinal(imagem);			//  Copia solucao final para imagem e salva em disco
		
		raio += fluidoB->Molhabilidade() == 0 ? -1 : 1; // incrementa ou decrementa o raio.
     		return true;
     		} 
	else 
		{
     		return false;
     		}
}

//  ====================================
//  FUNCAO GO
//  ====================================
//    Funcao:    void CConfiguracoesEquilibrio2D::Go(TCMatriz2D< int >* imagem)
//    Objetivo:  Determinar as configurações de equilíbrio geométricas em um processo de interação entre dois
//               fluidos (fluidoA e fluidoB) em uma "camara" de um "porosimetro".
//    Comentário: Por uma questão de economia de memória, a imagem idf é usada para armazenar os índices de G e G_
//               ou seja, as regiões verde (abertura) e verde complementar. Isto é realizado pintando com valor negativo a região G_
void CConfiguracoesEquilibrio2D::Go(TCMatriz2D< int > * &imagem)
{
	while( Next(imagem) ){} 			// Next irá executar todo os passos que foram comentados abaixo.
	/* 
	CriaCamara(imagem);				//  Cria a camara considerando a imagem e a Molhabilidade do fluido B
	CriaIDF();					//  Cria a IDF, a partir da camara (calcula a idf e o raioMaximo)
	CriaRotulador();				//  Cria o objeto de rotulagem, passando a camara
	contadorPassosExecutados = 0;			//  Faz parte de this
	for (raio = fluidoB->Molhabilidade() == 0 ? raioMaximo : raioMinimo; 
		fluidoB->Molhabilidade() == 0 ? raio >= raioMinimo : raio <= raioMaximo;
		raio += fluidoB->Molhabilidade() == 0 ? -1 : 1)
	{
		contadorPassosExecutados++;		//  Incrementa o numero de passos executados (inicialmente=1)
		CalculaAbertura(raio);			//  Realiza abertura
							//  se wb=1 o raio cresce, em função da membrana KGB0 ï¿½ sempre B0.
		if (fluidoB->Molhabilidade() == 0)
		{ 					//  DiferencaEmRelacaoArtigo(imagem);
			ConectividadeKGB0();		//  Calcula a relacao K(G,B0)
		}
		if (fluidoB->Molhabilidade() == 1)
		{					//  se wb=1 considera G_
			UniaoG__U_KGB0();		//  Realiza a uniao de G_ com K(G,B0)
							//  DiferencaEmRelacaoArtigo(imagem);
			ConectividadeKwbG__U_KGB0B0();	//  Calcula a relacao KwbG__U_KGB0B0
		}
							//  Solucao final para Omega, 0,B,Yi,A0
		SolucaoOmega(indiceParcialB, imagem);	//  MarcaYi   //  DeterminaA0   //  ReestabeleceB
							//  Se ca=1, e nao for a primeira passagem
		if (fluidoA->Compressibilidade() == 0 && contadorPassosExecutados != 1)
			CorrecaocaxYi(imagem);		//  Se for inCompressivel e nao for a primeira passagem entra
		SolucaoFinal(imagem);			//  Copia solucao final para imagem e salva em disco
	}						//  fim for
	*/
}
