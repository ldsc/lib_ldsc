/**
----------------------------------------------------------------------------
PROJETO:		LIB_LDSC
   Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 * Liang Zirong, Paulo C. Philippi, Damiani,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CConfiguracoesEquilibrio3D.h
Nome da classe:      CConfiguracoesEquilibrio3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help

==================================================================================
Documentacao CLASSE:	  CConfiguracoesEquilibrio3D
==================================================================================
Assunto:               CConfiguracoesEquilibrio3D
Superclasse:
Descrição:             Declara a classe TConfiguracoesEquilibrio.
Acesso:
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
----------------------------------------------------------------------------
*/

#include <iomanip>
#include <string>
#include <sstream>
#include <cstdio>

using namespace std;

#include <ConEqu/CConfiguracoesEquilibrio3D.h>

#include <Base/COperacao.h>

#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMIDFd3453D.h>

#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/CFEMMIDFEuclidiana3D.h>

/** Metodo construtor.
 * Recebe uma ostream os, por default std::cout.
 * Seta os atributos da classe,
 * e cria alguns objetos dinâmicos, como a camara e os fluidos.
*/
CConfiguracoesEquilibrio3D::CConfiguracoesEquilibrio3D ( /*TCMatriz3D<int> * imagem */ ostream & out) {
   os = &out;				//  temporaria, para saida tela

   salvarResultadosParciaisDisco = 0;	//  temporario

   salvarResultadosFinaisDisco = 1;	// salvar resultados finais em disco

   corrigirAbertura = 0;

   visualizar = 0;				//  temporario

   contadorPassosExecutados = 0;		// zera o contador de passo executados. (necessário para Next)

   idf = NULL;

   rotulador = NULL;

   camara = NULL;

   camara = new CCamara3D ();		//  Cria objetos agregados

   COperacao::TestaAlocacao (camara, "objeto camara, construtor CConfiguracoesEquilibrio3");

   fluidoA = new CMFluido ();		//  que podem ser alterados usando

   fluidoB = new CMFluido ();		//  confEqui->fluidoA->atributo

   COperacao::TestaAlocacao (fluidoB, "objeto fluidoB, construtor CConfiguracoesEquilibrio3");

   tipoIDF = 345;				//  usa por default a métrica d345

   CConfiguracoesEquilibrio3D::DefineAtributos ();
}

/// Metodo que seta os atributos do objeto.
void CConfiguracoesEquilibrio3D::DefineAtributos () {
   //  0=solido ou paredes
   //  1=poros
   /*
      B0 = 4;			//  2;//  indices das diferentes regiões geométricas
      A0 = 5;
      B = 4;			//  magnani=4
      A = 5;			//  magnani=5
      Ai = 2;			//  ?nao usado
      G = 3;			//  6;
      G_ = 6;
      //  Y0=8;
      Yi = 3;			//  magnani=3
      KGB0 = 7;
      wbG__U_KGB0 = 10;		// verificar?
      KwbG__U_KGB0B0 = 9;
    */
   //  Y0=8;
   Yi 			   = 2;	//19660
   B0 			   = 3;	//26214
   B  			   = 3;	//26214
   G  			   = 4;	//19660
   G_ 			   = 5;	//39321
   A0 			   = 6;	//32768
   A 			      = 6;	//32768
   KGB0			   = 7;	//45875
   wbG__U_KGB0 	= 8;	//52428
   KwbG__U_KGB0B0 = 9;	// 65535
   Ai 			   = 10;	//13107 	// nao usado????
   raioMaximo     = 50;		//  Raio máximo e minimo a ser utilizado no processo
   raioMinimo     = 1;
}

/// Destrutor, destrói atributos dinâmicos.
CConfiguracoesEquilibrio3D::~CConfiguracoesEquilibrio3D () {
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

/** Metodo que cria a camara. Usa a idf d345 para determinar o raioMaximo (usa somente a imagem),
    Observe que na imagem da camara o raio maximo pode ser maior em do espaço da própria camara.
*/
void CConfiguracoesEquilibrio3D::CriaCamara (TCMatriz3D<int> * &imagem) {
   if (camara) {
      delete camara;
      camara = NULL;
      camara = new CCamara3D ();
   }
   if (contadorPassosExecutados == 0)
      Salvar (imagem, "0-imagemInicial");
   {
      CFEMMIDFd3453D idfp (imagem); // BUG: verificar se nao esta deletando a imagem
      idfp.Go (imagem);
      int maiorValor = idfp.MaiorValor ();
      raioMaximo = (maiorValor % 3 == 0) ? maiorValor / 3 : 1 + maiorValor / 3;
      //  raioMaximo= ( maiorValor % idfp.Getmi() == 0)?
      //  maiorValor / idfp.Getmi():   1+maiorValor/idfp.Getmi();//  antes idf->Getmi()=3
   }

   //  Define as propriedades da camara
   camara->DimensoesPadroes (raioMaximo);				//    wb=0      //  wb=1
   camara->CamaraInferior   (fluidoB->Molhabilidade () == 0 ? 2 * raioMaximo + 1 : 1);	//  5=membrana+2
   camara->CamaraSuperior   (fluidoB->Molhabilidade () == 0 ? 1 : 2 * raioMaximo + 1);	//  5->1
   camara->MembranaInferior (fluidoB->Molhabilidade () == 0 ? 0 : 2);			//  3->2
   camara->MembranaSuperior (fluidoB->Molhabilidade () == 0 ? 2 : 0);			//  3->2

   //  Define os indices das camaras superior e inferior
   camara->IndiceCamaraInferior (B0);
   camara->IndiceCamaraSuperior (A0);

   //  Aloca a camara, define as paredes e copia imagem
   camara->CriaCamara (imagem);

   //  *os<<" \nDados objeto camara criacao camara\n";
   //  *os<<*camara<<endl;
   //  if(imagem!=NULL)
   //     delete imagem;
   //  imagem=NULL;//  Deleta a imagem recebida, economia.
}

/// Metodo que cria a idf
void CConfiguracoesEquilibrio3D::CriaIDF () {
   if (idf)
      delete idf;				//  Se já existe deleta
   idf = NULL;

	 TCMatriz3D<int> * ptr_camara = static_cast<TCMatriz3D<int> *>(camara);

	switch (tipoIDF) {			//  Cria objeto idf selecionado
   case 345:
      idf = new CFEMMIDFd3453D (ptr_camara);
      break;
   default:
      idf = new CFEMMIDFEuclidiana3D (ptr_camara);
      break;
   }

   COperacao::TestaAlocacao (idf,   "objeto idf, funcao CConfiguracoesEquilibrio3D::CriaIDF");

   idf->Go (ptr_camara);			//  Calcula a idf

   idf->IndiceAtivo (G);			//  Define indice da matriz apos operacao abertura

   int maiorValor = idf->MaiorValor ();	//  Calcula o raioMaximo

   //  abaixo usava idf->Getmi() = 3, verificar
   raioMaximo = (maiorValor % idf->Mi () == 0) ? maiorValor / idf->Mi () : 1 + maiorValor / idf->Mi ();

	 TCMatriz3D<int> * ptr_idf = static_cast<TCMatriz3D<int> *>(idf); // Salvar recebe CImagem3D*
   Salvar (ptr_idf, string("1-imagemIDF"));
}

///  Metodo que cria objeto de rotulagem
void CConfiguracoesEquilibrio3D::CriaRotulador () {
   if (rotulador)
      delete rotulador;				//  Deleta objeto de conectividade anterior
   rotulador = NULL;
   rotulador = new CConectividade3D (camara);	//  Cria novo objeto rotulador
   COperacao::TestaAlocacao (rotulador, "objeto rotulador, funcao CConfiguracoesEquilibrio3D::Go");
}

/// Abertura: Calcula a abertura sobre a IDF da camara
void CConfiguracoesEquilibrio3D::CalculaAbertura (int &raio) {
	 TCMatriz3D<int> * ptr_camara = static_cast<TCMatriz3D<int> *>(camara);
   idf->Abertura (ptr_camara, raio);	//  Determina as regiões abertura na camara
   Salvar (ptr_camara, "2.0-imagem-%d-G-(4)");	//  ....Tem na camara(0,G=4)...

   if ( corrigirAbertura ) {
      idf->CorrigeAbertura(ptr_camara, G);
      Salvar (ptr_camara, "2.1-imagem-%d-G-(corrigida)"); // imagem G corrigida para não apresentar erro físico.
   }

   //está salvando a máscara somente para estudo. Depois comentar...
	 TCMatriz3D<int> * ptr_mask = static_cast<TCMatriz3D<int> *>(idf->mask);
   ptr_mask->Path(ptr_camara->Path());
   Salvar (ptr_mask, "2.2-mask-%d");

   // Abaixo só é necessário se for usar G-
   // OBS: Como agora só calcula KGB0 ou wbG_U_KGB0 o passo abaixo é desnecesário.
   /* if(fluidoB->Molhabilidade()==1)  		//  se wb=1 considera G_
     for(int i=0;i<camara->NX();i++)        	//  Usa a propria IDF para armazenar o resultado da abertura.
       for(int j=0;j<camara->NY();j++)       //  se pertence a região verde complementar (G_)
         if(camara->data3D[i][j]==G)         //  inverte o sinal na idf
       idf->data3D[i][j]= - idf->data3D[i][j];//  idf com sinal negativo indica região G, + G_
       int larguraCampoOld=idf->larguraCampo;
       idf->larguraCampo=4;
       Salvar(idf,"3-imagemIDF-GG_-%d.dgm");     //  salva idf_G_G em disco
       idf->larguraCampo=larguraCampoOld;
    */
}

/// Metodo que determina conectividade entre K(G,B0)
// é chamada em next quando b for não molhante
void CConfiguracoesEquilibrio3D::ConectividadeKGB0 () {
   camara->DefineCamaraInferior ();		//  Redesenha camara inferior, eliminada na abertura
   //  ....Tem na camara(0,G=3,B0=4)...
	 TCMatriz3D<int> * ptr_camara = static_cast<TCMatriz3D<int> *>(camara);

   rotulador->Go (ptr_camara);					//  Realiza a rotulagem

	 TCMatriz3D<int> * ptr_rotulador = static_cast<TCMatriz3D<int> *> (rotulador);
   Salvar (ptr_rotulador, "4-imagem-%d-G+B0-Rotulada");	//  salva imagem rotulada

   //  Verifica a conectividade entre B0 e G, e pinta a regiao conexa com KGB0
   rotulador->VerificaConectividade (G, B0, KGB0);

   //  rotulador->VerificaConectividade(G,B0,B);//  Verifica a conectividade entre B0 e G, e pinta a regiao conexa com KGB0
   Salvar (ptr_camara, "5-imagem-%d-K(G,B0)-(0,3,7)");	//  ....Tem na camara(0,G,KGB0)...
   indiceParcialB = KGB0;	//  Define o indiceParcial para B
}

/// Metodo que determina a uniao    G_U K(G,B0)]
// é chamada em next quando b for molhante
void CConfiguracoesEquilibrio3D::UniaoG__U_KGB0 () {
   for (int i = 0; i < camara->NX (); i++)		//  Percorre a imagem
      for (int j = 0; j < camara->NY (); j++)
         for (int k = 0; k < camara->NZ (); k++)
            if (idf->data3D[i][j][k] > 0 && camara->data3D[i][j][k] == 0) //  complemento da abertura
               camara->data3D[i][j][k] = wbG__U_KGB0;	//  =8, a união pode ser pintada com um único índice wbG-_U_KGB0
            else
               camara->data3D[i][j][k] = 0;	//  o restante passa a ser zero
   //  ....Tem na camara(0,wbG-_U_KGB0)...
	 TCMatriz3D<int> * ptr_camara = static_cast<TCMatriz3D<int> *>(camara);
   Salvar (ptr_camara, "6-imagem-%d-wbG__U_K(G,B0)-(0,8)");	//  0,wbG-_U_KGB0=8,
}

/// Metodo que determina a conectividade entre K{ [G_U K(G,B0)],B0)}
// é chamada em next, após UniaoG__U_KGB0 (), quando b for molhante
void CConfiguracoesEquilibrio3D::ConectividadeKwbG__U_KGB0B0 () {
   //  redesenha camara inferior (B0), eliminada na verificacao da conectividade
   camara->DefineCamaraInferior ();
   //  ....Tem na camara(0,wbG__U_KGB0,B0)...

	 TCMatriz3D<int> * ptr_camara = static_cast<TCMatriz3D<int> *>(camara);

   rotulador->Go (ptr_camara);					//  2 rotulagem
	 TCMatriz3D<int> * ptr_rotulador = static_cast<TCMatriz3D<int> *> (rotulador);

   //  ....Tem na camara(0,wbG__U_KGB0=8,KwbG__U_KGB0B0=9,)...
   Salvar (ptr_rotulador, "7.0-imagem-%d-wbG__U_KGB0B0-rotulada");
   rotulador->VerificaConectividade (wbG__U_KGB0, B0, KwbG__U_KGB0B0);
   indiceParcialB = KwbG__U_KGB0B0;	// == 9
   Salvar (ptr_camara, "7.1-imagem-%d-KwbG__U_KGB0B0-Omega-(0,8,9)");
   //  ....Tem na camara(0,wbG__U_KGB0=8,KwbG__U_KGB0B0=9,)...
}

/**  Metodo que determina a solução de Omega isto é, os valores de B, o restante e Y
  	Aqui a imagem tem (B->indiceRegiaoB, 0, A0, um outro indice que representa Y)
*/
void CConfiguracoesEquilibrio3D::SolucaoOmega (int &indiceRegiaoB, TCMatriz3D<int> * &imagem) {
   //  Marca Yi como sendo os valores que estao na idf e nao pertencem a região B
   for (int i = 0; i < camara->NX (); i++)
      for (int j = 0; j < camara->NY (); j++)		//  Transformacao OPCIONAL
         for (int k = 0; k < camara->NZ (); k++) 		//  Se pertence a imagem (idf) e nao a região B então é Yi
            if (idf->data3D[i][j][k] != 0 && camara->data3D[i][j][k] != indiceRegiaoB)
               camara->data3D[i][j][k] = Yi;
            else
               camara->data3D[i][j][k] = 0;
	 TCMatriz3D<int> * ptr_camara = static_cast<TCMatriz3D<int> *>(camara);
   Salvar (ptr_camara, "7.2-imagem-%d-Yi-(0,2)");	//  Aqui tem 0,Yi

   //  Funcao DeterminaA0()
   //  Aqui determina a regiao conexa a A0, Definindo a forma final (A0,Y,B), sem considerar Yi anterior
   camara->DefineCamaraSuperior ();			//  redesenha camara superior   (indices: 0,B,A0,Y)
   Salvar (ptr_camara, "7.3-imagem-%d-Yi+camaraSuperior-(0,3,5)");	//  Aqui tem 0, B,Yi,A0
   //  Verificar conectividade (Y,A0,A0)
   rotulador->Go (ptr_camara);					//  precisa re-rotular a imagem,(3 rotulagem)
   rotulador->VerificaConectividade (Yi, A0, A0);	//  verifica a conectividade de Yi com A0 e marca como A0
   Salvar (ptr_camara, "8.1-imagem-%d-Yi+A0-(0,3,5)");//  Aqui tem 0, Yi,A0

   //  Funcao DeterminaB()
   //  1-Acima, eliminou B da camara,
   //  2- recoloca os valores de B na camara
   //  3-Ja considera B da imagem anterior.

   int DeslocX = camara->DeslocamentoNX ();		//  obtem deslocamento para posicao
   int DeslocY = camara->DeslocamentoNY ();		//  inicial da imagem na camara
   int DeslocZ = camara->DeslocamentoNZ ();		//
   int ti, tj, tk;					//  variaveis temporarias otimizacao

   for (int i = 0; i < imagem->NX (); i++) {			//  percorre a imagem
      ti = i + DeslocX;
      for (int j = 0; j < imagem->NY (); j++) {
         tj = j + DeslocY;
         for (int k = 0; k < imagem->NZ (); k++) {
            tk = k + DeslocZ;
            if (idf->data3D[ti][tj][tk] != 0 && 	// pertence a imagem (IDF) e não é Yi ou A0, entao é B.
                camara->data3D[ti][tj][tk] != Yi &&
                camara->data3D[ti][tj][tk] != A0 ||
                imagem->data3D[i][j][k] == B		//  ou, se esta na imagem anterior era B
                ) {
               camara->data3D[ti][tj][tk] = B;		//  pinta de B
				}
         }
      }
   }
   /* //  Recoloca valor de B
       for(int i=0;i<camara->NX();i++)
        for(int j=0;j<camara->NY();j++)       		//  Transformacao OPCIONAL
         for(int k=0;k<camara->NZ();k++)      		//  se pertence a imagem e nao é Yi ou A0
        if(idf->data3D[i][j][k]!=0 && camara->data3D[i][j][k]!=Yi && camara->data3D[i][j][k]!=A0)
              camara->data3D[i][j][k]=B;
    */
   Salvar (ptr_camara, "8.2-imagem-%d-Yi+B+A0-(0,3,4,5)");	//  Aqui tem 0, Yi,A0
}

/** Metodo que determina a  CorrecaocaxYi
  Determinacao da região Yi (ca=1)
 Aqui ja tenho a solução da região OMEGA,se ca=1 imcompressivel, precisa considerar ca*Yi
 Aqui a camara tem a solucao para Omega no instante de tempo atual,
 A imagem tem a solucao no instante anterior
 Devo eliminar Yi da solucao de Omega, B=Omega-ca*Yi ~ Omega-ca*(Yi-1)
*/
void CConfiguracoesEquilibrio3D::CorrecaocaxYi (TCMatriz3D<int> * &imagem) {
   int DeslocX = camara->DeslocamentoNX ();
   int DeslocY = camara->DeslocamentoNY ();
   int DeslocZ = camara->DeslocamentoNZ ();
   int valorAnteriorPixel;

   for (int i = 0; i < imagem->NX (); i++)
      for (int j = 0; j < imagem->NY (); j++)
         for (int k = 0; k < imagem->NZ (); k++)			//  Se for Yi,na imagem (instante de tempo anterior)
            if (imagem->data3D[i][j][k] == Yi) 			// pinta como Yi na camara (coisas que eram B passam a ser Yi)
               camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ] = Yi;
            else if (imagem->data3D[i][j][k] == B) 		// Novo, pinta B da imagem anterior, visto que B nao retrocede (processo primario)
               // corrigindo, erro do codigo original, que permitia retrocesso de fluido B
               camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ] = B;

	 TCMatriz3D<int> * ptr_camara = static_cast<TCMatriz3D<int> *>(camara);
   Salvar (ptr_camara, "9-imagem-%d-K{WBxG__U_K(G,B0),B0}-caYi");
   //  CorrecaoBolas();
}

/// Metodo que determina a SolucaoFinal(), e salva em disco
//  Tarefa: Pensar em Criar: SetdeslocX(), SetdeslocY(), Write..
void CConfiguracoesEquilibrio3D::SolucaoFinal (TCMatriz3D<int> * &imagem) {
   int DeslocX = camara->DeslocamentoNX ();
   int DeslocY = camara->DeslocamentoNY ();
   int DeslocZ = camara->DeslocamentoNZ ();

   int Va = 0, Vb = 0, Vy = 0;				// volume dos fluídos A, B e Y. Para cálculo da saturação.

   for (int i = 0; i < imagem->NX (); i++) {
      for (int j = 0; j < imagem->NY (); j++) {
         for (int k = 0; k < imagem->NZ (); k++) {
            //  Copia a solucao final para a imagem
            imagem->data3D[i][j][k] = camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ];
            // Acumula valores de A e B (todos os poros da imagem são A ou Yi ou B)
            if (int r = imagem->data3D[i][j][k]) {
               if ( r == A ) {
                  Va++;
					} else if ( r == B ) {
                  Vb++;
					} else if ( r == Yi ) {
                  Vy++;
					}
            }
         }
		}
	}

   saturacaoA =  (double)Va / (double)(Va + Vb + Vy);
   saturacaoB =  (double)Vb / (double)(Va + Vb + Vy);

   if (salvarResultadosFinaisDisco) {
      ostringstream out;
      out << "10-imagemFinal-" << contadorPassosExecutados;
      fluidoB->Molhabilidade() == 1 ? out << "-wb1" : out << "-wb0";
      fluidoA->Compressibilidade () == 1 ? out <<"-ca1" : out <<"-ca0";
      out << ".dgm";

      fileName = out.str(); // não precisa do fileName, tirar?
      // novidade
			imagem->SetFormato (D2_X_Y_Z_GRAY_ASCII);
      imagem->NumCores( imagem->MaiorValor() );
      imagem->Write (fileName);	//  A cada passo, deve salvar a configuracao final de equilibrio
   }
}

/// Metodo que reestabelece os valores positivos na IDF
void CConfiguracoesEquilibrio3D::RestabeleceIDFPositiva () {
   for (int i = 0; i < idf->NX (); i++) {		//  Percorre a imagem idf
      for (int j = 0; j < idf->NY (); j++) {	//  e restabelece os valores negativos->para positivos,
         for (int k = 0; k < idf->NZ (); k++) {
            if (idf->data3D[i][j][k] < 0) {	//  para evitar erro na operção de abertura
               idf->data3D[i][j][k] = -idf->data3D[i][j][k];
				}
			}
		}
	}
}

/// Metodo que determina a diferença em relação ao artigo
/// AQUI DIFERENÇA DA VERSAO DO ARTIGO PARA O CODIGO:
/// No artigo faz: K(G,B0)
/// No codigo faz: K(G-Yi,B0)
/// APÓS REALIZAR OPERACAO ABERTURA, MARCA PONTOS Y DA IMAGEM ANTERIOR
void CConfiguracoesEquilibrio3D::DiferencaEmRelacaoArtigo (TCMatriz3D<int> *&imagem) {
   int DeslocX = camara->DeslocamentoNX ();
   int DeslocY = camara->DeslocamentoNY ();
   int DeslocZ = camara->DeslocamentoNZ ();
   for (int i = 0; i < imagem->NX (); i++)
      for (int j = 0; j < imagem->NY (); j++)
         for (int k = 0; k < imagem->NZ (); k++)
            if (imagem->data3D[i][j][k] == Yi)				//  Se for Yi,na imagem (instante de tempo anterior)
               camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ] = 0;	//  pinta como Yi na camara (coisas que eram B passam a ser Yi)
}

/// Metodo que salva imagem em disco
/// Note que inclui informações como wb1 wb0 ca0 ca1
void CConfiguracoesEquilibrio3D::Salvar (TCMatriz3D<int> * &imagem, string msg) {
   char fileName[255];

   string buffer = msg;
   buffer +=  (fluidoB->Molhabilidade () 	 == 1)	? "-wb1" : "-wb0";
   buffer += (fluidoA->Compressibilidade () == 1)	? "-ca1" : "-ca0"; 		// 2007 estava trocado
   buffer += ".dgm";

   // Substitue o %d pelo contadorPassosExecutados
   //  porque nao usar o raio? porque o raio pode estar diminuindo.
   sprintf (fileName, buffer.c_str (), contadorPassosExecutados);

   if (salvarResultadosParciaisDisco == 1) {
			imagem->SetFormato(D2_X_Y_Z_GRAY_ASCII);
      int cores = imagem->MaiorValor();
      imagem->NumCores( (cores >= 2) ? cores : cores+2 );
      imagem->Write (fileName);
   }

   if (visualizar == 1)     {
      (*os) << "\nTarefa: Arrumar a linha 416 do arquivo CConfiguracoesEquilibrio3D.cpp:\n\n" ;
      //(*os) << (*imagem) << "\n" ;
      //(*os) << fileName;
      cin.get ();
   }
}

// retorna ponteiro para uma CMatriz3D que aponta para uma imagem da região informada, extraída da câmara.
// esta função deve ser chamada após Next ou Go.
TCMatriz3D<int> * CConfiguracoesEquilibrio3D::GetImagem(int regiao) const {
   TCMatriz3D<int> * imagem = NULL;
	 imagem = new TCMatriz3D<int>(camara->NxImg(), camara->NyImg(), camara->NzImg()); // aloca matriz 3D com as dimensões da imagem da câmara
   if ( ! imagem ) return NULL;

   if ( GetImagem( imagem , regiao ) )
      return imagem;
   else
      return NULL;
}

// Altera os valores da matriz passsada como parametro para corresponder a imagem binária referente a regiao passada como parâmetro.
// esta função deve ser chamada após Next ou Go.
bool CConfiguracoesEquilibrio3D::GetImagem(TCMatriz3D<int> * &imagem, int regiao) const {
   if ( ! imagem ) return false;
   int DeslocX = camara->DeslocamentoNX ();
   int DeslocY = camara->DeslocamentoNY ();
   int DeslocZ = camara->DeslocamentoNZ ();

   // verifica se as dimensões da imagem passada como parâmetro são as mesmas da imagem na camara
   if ( ( imagem->NX() != camara->NxImg() ) || ( imagem->NY() != camara->NyImg() ) || ( imagem->NZ() != camara->NzImg() ) ) {
      imagem->Redimensiona(camara->NxImg(), camara->NyImg(), camara->NzImg()); // aloca matriz 3D com as dimensões da imagem da câmara
      if ( ! imagem ) return false;
   }

   int nx = imagem->NX();
   int ny = imagem->NY();
   int nz = imagem->NZ();

   for (int i = 0; i < nx; i++)
      for (int j = 0; j < ny; j++)
         for (int k = 0; k < nz; k++)
         {							// Se o pixel analizado na camara corresponder ao valor da região informada.
            if (camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ] == regiao)
               // Atribui o valor da região ao pixel da imagem.
               imagem->data3D[i][j][k] = 1; 		// Tarefa: verificar se funciona retornar a regiao = regiao
            else						// Se o pixel analizado não corresponder a região.
               // Atribui o valor 0 ao pixel da imagem.
               imagem->data3D[i][j][k] = 0;
         }
   return true;
}

/*
// Usada para inverter o sentido do fluxo
void CConfiguracoesEquilibrio3D::InverterFluxo()
{
 camara->SetFormato(WRITEFORM_DI_X_Y_Z_GRAY_ASCII);
     int cores = camara->MaiorValor();
     camara->NumCores( (cores >= 2) ? cores : cores+2 );
     camara->Write ("imagemCamaraAntesInverterFluxo.dgm");

 // Pega valores usados no método
 int nximg = camara->NxImg();
 int nyimg = camara->NyImg();
 int nzimg = camara->NzImg();

 int DeslocX = camara->DeslocamentoNX ();
  	int DeslocY = camara->DeslocamentoNY ();
  	int DeslocZ = camara->DeslocamentoNZ ();

 // Faz uma cópia da imagem do meio poroso contida na camara
 CMatriz3D pmCopia( nximg, nyimg, nzimg );
 for (int i = 0; i < nximg; i++)
  for (int j = 0; j < nyimg; j++)
   for (int k = 0; k < nzimg; k++)
    pmCopia.data3D[i][j][k] = camara->data3D[i+DeslocX][j+DeslocY][k+DeslocZ];

 // Inverte as propriedades dos fluidos (compressibilidade e molhabilidade)
 bool compressibilidadeTmp = fluidoB->Compressibilidade();
 bool molhabilidadeTmp = fluidoB->Molhabilidade();

 fluidoB->Compressibilidade( fluidoA->Compressibilidade() );
 fluidoB->Molhabilidade( fluidoA->Molhabilidade() );

 fluidoA->Compressibilidade( compressibilidadeTmp );
 fluidoA->Molhabilidade( molhabilidadeTmp );

 // se a molhabilidade do Fluido B foi alterada, redefine valores de dimensões das membranas e camaras inferiores e superiores
 // sempre muda : if ( fluidoB->Molhabilidade () != molhabilidadeTmp )
 {
  //raioMaximo foi setado com outro valor. Não pode usar a mesma regra. Resolvido com a implementação abaixo.
  //camara->CamaraInferior   (fluidoB->Molhabilidade () == 0 ? 2 * raioMaximo + 1 : 1);
  //camara->CamaraSuperior   (fluidoB->Molhabilidade () == 0 ? 1 : 2 * raioMaximo + 1);
  //camara->MembranaInferior (fluidoB->Molhabilidade () == 0 ? 0 : 2);
  //camara->MembranaSuperior (fluidoB->Molhabilidade () == 0 ? 2 : 0);
  int camaraInferiorTmp = camara->CamaraInferior();
  int membranaInferiorTmp = camara->MembranaInferior();
  camara->CamaraInferior ( camara->CamaraSuperior() );
  camara->CamaraSuperior ( camaraInferiorTmp );
  camara->MembranaInferior ( camara->MembranaSuperior() );
  camara->MembranaSuperior ( membranaInferiorTmp );
 }

 // Zera a camara e redefine...
 camara->Constante( 0 );
 camara->DefineCamara();

 // Atualiza valores de deslocamento
 DeslocX = camara->DeslocamentoNX ();
  	DeslocY = camara->DeslocamentoNY ();
  	DeslocZ = camara->DeslocamentoNZ ();

 // Inverte a Imagem e redefine na nova camara. [ (B vira A) e (A ou Yi vira B) o restante é 0 ]
 for (int i = 0; i < nximg; i++)
  for (int j = 0; j < nyimg; j++)
   for (int k = 0; k < nzimg; k++)
    if ( pmCopia.data3D[i][j][k] == B )
     camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ] = A;
    else if ( ( pmCopia.data3D[i][j][k] == A ) or ( pmCopia.data3D[i][j][k] == Yi ) )
     camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ] = B;
    //else
    //	camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ] = 0;

 // Recalcula a IDF da camara
 CriaIDF();
 CriaRotulador();			// Cria o objeto de rotulagem, passando a camara
//	raio = fluidoB->Molhabilidade() == 0 ? raioMaximo : raioMinimo;

 //só corrige a abertura na drenagem.
 corrigirAbertura = false;

 camara->Write ("imagemCamaraDepoisInverterFluxo.dgm");
}
*/
// Usada para inverter o sentido do fluxo
void CConfiguracoesEquilibrio3D::InverterFluxo( TCMatriz3D<int> * &imagem ) {
	 camara->SetFormato(D2_X_Y_Z_GRAY_ASCII);
   int cores = camara->MaiorValor();
   camara->NumCores( (cores >= 2) ? cores : cores+2 );
   camara->Write ("imagemCamaraAntesInverterFluxo.dgm");

   // Pega valores usados no método
   int nximg = camara->NxImg();
   int nyimg = camara->NyImg();
   int nzimg = camara->NzImg();

   int DeslocX = camara->DeslocamentoNX ();
   int DeslocY = camara->DeslocamentoNY ();
   int DeslocZ = camara->DeslocamentoNZ ();

   // Faz também uma cópia da imagem do meio poroso contida na camara de forma que o que for diferente de 0 (solido) será 1 (poro)
   // esta imagem será usado para recriar a camara, depois de criada a imagem é redefinida (verificar necessidade!).
	 TCMatriz3D<int> *pmCopiaBin = new TCMatriz3D<int>( nximg, nyimg, nzimg );
   pmCopiaBin->Path(camara->Path());
   for ( int i = 0; i < nximg; i++ ) {
      for ( int j = 0; j < nyimg; j++ ) {
         for ( int k = 0; k < nzimg; k++ ) {
            if ( imagem->data3D[i][j][k] == 0 ) {
               pmCopiaBin->data3D[i][j][k] = 0;
            } else {
               pmCopiaBin->data3D[i][j][k] = 1;
            }
         }
      }
   }

   // Inverte as propriedades dos fluidos (compressibilidade e molhabilidade)
   bool compressibilidadeTmp = fluidoB->Compressibilidade();
   bool molhabilidadeTmp = fluidoB->Molhabilidade();

   fluidoB->Compressibilidade( fluidoA->Compressibilidade() );
   fluidoB->Molhabilidade( fluidoA->Molhabilidade() );

   fluidoA->Compressibilidade( compressibilidadeTmp );
   fluidoA->Molhabilidade( molhabilidadeTmp );

   // Recria a camara baseada na imagem binária do meio poroso.
   CriaCamara( pmCopiaBin );
   camara->Path(pmCopiaBin->Path());
   // Recalcula a IDF da camara
   CriaIDF();

   // Cria o objeto de rotulagem, passando a camara
   CriaRotulador();
   raio = fluidoB->Molhabilidade() == 0 ? raioMaximo : raioMinimo;

   // Atualiza valores de deslocamento
   DeslocX = camara->DeslocamentoNX ();
   DeslocY = camara->DeslocamentoNY ();
   DeslocZ = camara->DeslocamentoNZ ();

   // Inverte a imagem original e redefine na nova camara. [ (B vira A) e (A ou Yi vira B) o restante é 0 ]
   for (int i = 0; i < imagem->NX(); i++) {
      for (int j = 0; j < imagem->NY(); j++) {
         for (int k = 0; k < imagem->NZ(); k++) {
            if ( imagem->data3D[i][j][k] == B ) {
               camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ] = A;
               imagem->data3D[i][j][k] = A;
            } else if ( ( imagem->data3D[i][j][k] == A ) or ( imagem->data3D[i][j][k] == Yi ) ) {
               camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ] = B;
               imagem->data3D[i][j][k] = B;
            } else {
               camara->data3D[i + DeslocX][j + DeslocY][k + DeslocZ] = 0;
               imagem->data3D[i][j][k] = 0;
            }
         }
      }
   }

   //só corrige a abertura na drenagem.
   corrigirAbertura = false;

	 camara->SetFormato(D2_X_Y_Z_GRAY_ASCII);
   cores = camara->MaiorValor();
   camara->NumCores( (cores >= 2) ? cores : cores+2 );
   camara->Write ("imagemCamaraDepoisInverterFluxo.dgm");
   delete pmCopiaBin;
}

//  ====================================
//  FUNCAO Next
//  ====================================
//   Funcao:     bool CConfiguracoesEquilibrio2D::Next(TCMatriz2D< int >* imagem)
//   Objetivo:   Determinar passo-a-passo as configurações de equilíbrio geométricas em um processo de interação entre
//               dois fluidos (fluidoA e fluidoB) em uma "camara" de um "porosimetro".
//   Comentário: Por uma questão de economia de memória, a imagem idf é usada para armazenar os índices de G e G_
//               ou seja, as regiões verde (abertura) e verde complementar. Isto é realizado pintando com valor negativo a região G_
//   Next irá retornar verdadeiro enquanto existirem passos a serem executados.
bool CConfiguracoesEquilibrio3D::Next(TCMatriz3D<int> * &imagem) {
   if (contadorPassosExecutados == 0)	{					// só irá entrar na primeira vez
      CriaCamara(imagem);			// Cria a camara considerando a imagem e a Molhabilidade do fluido B
      CriaIDF();				// Cria a IDF, a partir da camara (calcula a idf e o raioMaximo)
      CriaRotulador();			// Cria o objeto de rotulagem, passando a camara
      raio = fluidoB->Molhabilidade() == 0 ? raioMaximo : raioMinimo;
   }
   if ( fluidoB->Molhabilidade() == 0 ? raio >= raioMinimo : raio <= raioMaximo ) {
      contadorPassosExecutados++;			//  Incrementa o numero de passos executados (inicialmente=1)
      cout << "ConfEq - calculando passo " << contadorPassosExecutados << endl;
      CalculaAbertura(raio);			//  Realiza abertura
      //  se wb=1 o raio cresce, em função da membrana KGB0 ï¿½ sempre B0.
      if (fluidoB->Molhabilidade() == 0) {
			//  DiferencaEmRelacaoArtigo(imagem);
         ConectividadeKGB0();		//  Calcula a relacao K(G,B0)
      }
      if (fluidoB->Molhabilidade() == 1) {
			//  se wb=1 considera G_
         UniaoG__U_KGB0();		//  Realiza a uniao de G_ com K(G,B0)
         //  DiferencaEmRelacaoArtigo(imagem);
         ConectividadeKwbG__U_KGB0B0();	//  Calcula a relacao KwbG__U_KGB0B0
      }
      //  Solucao final para Omega, 0,B,Yi,A0
      SolucaoOmega(indiceParcialB, imagem);	//  MarcaYi   //  DeterminaA0   //  ReestabeleceB
      //  Se ca=1, e nao for a primeira passagem
      if (fluidoA->Compressibilidade() == 0 && contadorPassosExecutados != 1) {
         cout << "Calculando CorrecaocaxYi..." << endl;
         CorrecaocaxYi(imagem);	//  Se for inCompressivel e nao for a primeira passagem entra
      }
      cout << "Calculando SolucaoFinal..." << endl;
      SolucaoFinal(imagem);	//  Copia solucao final para imagem e salva em disco
      raio += fluidoB->Molhabilidade() == 0 ? -1 : 1; // incrementa ou decrementa o raio.
      return true;
   } else {
      return false;
   }
}


///  Metodo que determina toda sequência
///  Determinar as configurações de equilíbrio geométricas em um processo de interação entre dois
///  fluidos (fluidoA e fluidoB) em uma "camara" de um "porosimetro".

///  Comentário: Por uma questão de economia de memória, a imagem idf é usada para armazenar
///  os índices de G e G_ , ou seja, as regiões verde (abertura) e verde complementar.
///  Isto é realizado pintando com valor negativo a região G_
void CConfiguracoesEquilibrio3D::Go (TCMatriz3D<int> * &imagem) {
   while ( Next(imagem) ) { } // Next irá executar todo os passos que foram comentados abaixo.
   //InverterFluxo();
   //while( Next(imagem) ) { }
}
