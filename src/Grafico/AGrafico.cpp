
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	AGrafico.cpp
Nome da classe:		AGrafico

Descri��o: Classe base para o desenho de gr�ficos.
           Permite a cria��o de graficos simples, do tipo
           graficos de linhas e graficos de barra.
	   USA OWL

Criar um arquivo de base, uma biblioteca basica com fun��es muito usadas
que poderiam ser acessadas pelos objetos que voc� cria.
Logo abaixo uma paleta de cores de 16 cores, poderia criar numa classe basica,
e poder acessar de qualquer classe que use a OWL.
*/
#include <TGrafico/AGrafico.h>	//Inclui a classe
//----------------------------------------------------------------------------
// Valores de cor (assumes NUMEROCORES == 16)
//----------------------------------------------------------------------------
//Cria uma classe global para cores e paletas default(padr�es)
//static significa que so e visivel neste arquivo
const NUMEROCORESPALETA16 = 16;
static TColor paleta16[NUMEROCORESPALETA16] = {
  0x0000000L, 0x0FFFFFFL, 0x0FF0000L, 0x000FF00L,
  0x00000FFL, 0x0FFFF00L, 0x000FFFFL, 0x0FF00FFL,
  0x0880000L, 0x0008800L, 0x0000088L, 0x0888800L,
  0x0008888L, 0x0880088L, 0x0448844L, 0x0884488L
};

/*
============================================================================
Documentacao AGraficoPonto
============================================================================
Descri��o: Define os valores de x0 e y0
Programador:      Andre Duarte Bueno
*/
AGraficoPonto::AGraficoPonto ()
{
  x0 = y0 = 0;
}

/*
============================================================================
Documentacao AGraficoBox
============================================================================
Descri��o: Define os valores de w e h
Programador:      Andre Duarte Bueno
*/
AGraficoBox::AGraficoBox ()
{
  w = h = 0;
}

/*
============================================================================
Documentacao SetAll
============================================================================
Descri��o:        Fun��o usada para definir todos os valores do box ao mesmo tempo
Programador:      Andre Duarte Bueno
*/ void
AGraficoBox::SetAll (unsigned int _x0, unsigned int _y0, unsigned int _w,
		     unsigned int _h)
{
  x0 = _x0;
  y0 = _y0;
  w = _w;
  h = _h;
}

/*
============================================================================
Documentacao construtor da classe de titulos
============================================================================
*/
TWindow *
  AGraficoTitulo::ponteiroJanela = NULL;	//ponteiro para janela

//AGraficoTitulo::AGraficoTitulo(TMDIClient&pj,char* t)         //construtor
AGraficoTitulo::AGraficoTitulo (char *t)	//construtor
{
//ponteiroJanela=&pj;                                   //armazena ponteiro para janela
  strcpy (titulo, t);		//armazena titulo
  fonte = NULL;;		//zera ponteiro para fonte
  //preenche a estrutura de dados da estrutura MainFontRec
  MainFontRec.lfHeight = 19;	//26;
  MainFontRec.lfWidth = 8;	//10;
  MainFontRec.lfEscapement = 0;
  MainFontRec.lfOrientation = 0;
  MainFontRec.lfWeight = FW_BOLD;
  MainFontRec.lfItalic = 0;
  MainFontRec.lfUnderline = 0;
  MainFontRec.lfStrikeOut = 0;
  MainFontRec.lfCharSet = ANSI_CHARSET;
  MainFontRec.lfOutPrecision = OUT_DEFAULT_PRECIS;
  MainFontRec.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  MainFontRec.lfQuality = PROOF_QUALITY;
  MainFontRec.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
  strcpy (MainFontRec.lfFaceName, "Times New Roman");

  fonte = new TFont (&MainFontRec);	//cria fonte a partir do &LOGFONT
}

/*
============================================================================
Documentacao /*
============================================================================
Documentacao SetFonte
============================================================================
Descri��o:       Usada para definir o titulo
Programador:      Andre Duarte Bueno
*/

void
AGraficoTitulo::SetTitulo ()
{
  //wsprintf(texto, "%d", );
  TInputDialog dialogo (AGraficoTitulo::ponteiroJanela,	//ponteiro janela
			"Entre com o titulo",	//titulo
			titulo,	//prompt
			titulo,	//bufer
			sizeof (titulo), 0,	//tamanho bufer, modulo
			new TFilterValidator ("A-Za-z"));	//valida��o de dados tem de ser tudo emendado
  //new TFilterValidator("0-9")); //valida��o de dados
  dialogo.Execute ();
  //if (dialogo.Execute() == IDOK)
}

/*
============================================================================
Documentacao SetFonte
============================================================================
Descri��o:       Usada para definir o tipo de fonte, deve abrir um quadro de di�logo
		  default do windows para sele��o da fonte
                 O atributo ponteiroJanela foi recebido no construtor do  AGraficoTitulo
Programador:      Andre Duarte Bueno .
*/
void
AGraficoTitulo::SetFonte ()
{
  if (fonte)
    delete fonte;

  TChooseFontDialog::TData fontData;	//cria estrutura de dados para di�logo
  fontData.LogFont = MainFontRec;	//passa atributo LOGFONT para estrutura de dados
  //abaixo define mais alguns atributos da estrutura de dados
  fontData.Flags =
    CF_ANSIONLY | CF_TTONLY | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
  fontData.FontType = SCREEN_FONTTYPE;	//preenche alguns atributos da estrutura de dados
  fontData.SizeMin = 20;	//
  fontData.SizeMax = 20;	//
  //Cria di�logo passando a estrutura de dados
  TChooseFontDialog dialogo (AGraficoTitulo::ponteiroJanela, fontData);	//, "Di�logo de fontes");
  if (dialogo.Execute () == IDOK)	//"FontDlg"
    {
      //Pega somente o nome da fonte, o peso e se � italica- we don't care about size
      //strcpy(MainFontRec.lfFaceName, fontData.LogFont.lfFaceName);
      //MainFontRec.lfWeight = fontData.LogFont.lfWeight;
      //MainFontRec.lfItalic = fontData.LogFont.lfItalic;
      MainFontRec = fontData.LogFont;	//posso pegar os diversos �tens da estrutura (linhas acima)
      //ou pegar toda a estrutura como aqui
      fonte = new TFont (&MainFontRec);	//cria fonte a partir do &LOGFONT
      //Invalidate(TRUE);
    }
}

/*
TFont(THandle handle, TAutoDelete autoDelete = NoAutoDelete);
TFont(const LOGFONT far* logFont);	//Creates a TFont object from the given logFont.
TFont(const TFont& src);
*/
/*
============================================================================
Documentacao Paint
============================================================================
Descri��o:        Fun��o de desenho do t�tulo
Programador:      Andre Duarte Bueno
*/
void
AGraficoTitulo::Paint (TDC & dc, bool erase, TRect & rect)
{				//alinhamento
  //define o alinhamento como sendo central, e tendo como base o piso da linha
  unsigned int alinhamentoAntigo = dc.SetTextAlign (TA_CENTER | TA_BASELINE);

  if (fonte)			//se existe uma fonte
    {
      dc.SelectObject (*fonte);	//selecionar
      dc.TextOut (x0 + w / 2, y0 - h / 2, titulo, strlen (titulo));	//desenhar o texto
      dc.RestoreFont ();	//restaurar a fonte
    }
  else				//caso contrario s� desenhar com a fonte atual
    dc.TextOut (x0 + w / 2, y0 - h / 2, titulo, strlen (titulo));


  dc.SetTextAlign (alinhamentoAntigo);	//retorna ao alinhamento antigo
}

/*
============================================================================
Documentacao Paint
============================================================================
Descri��o:    Fun��o de desenho do marco
				  Deveria ser do tipo:
              objetoGeometria->Desenhar(cordenadas);
Programador:      Andre Duarte Bueno
*/
void
AGraficoMarco::Paint (TDC & dc, bool erase, TRect & rect)
{
  dc.Ellipse (x0 - 2, y0 - 2, x0 + 2, y0 + 2);	//elipse dentro do retangulo
}

/*
============================================================================
Documentacao Paint
============================================================================
Descri��o:       Fun��o de desenho do grid, vazia
Programador:      Andre Duarte Bueno
*/
void
AGraficoGrid::Paint (TDC & dc, bool erase, TRect & rect)
{
}

/*
============================================================================
Documentacao Paint
============================================================================
Descri��o:        Fun��o de desenho do eixo X.
Programador:      Andre Duarte Bueno
No construtor recebe o numero de tickets, por enquanto recebe como numero de tickets
o n�mero de pontos do vetor.
Para um histograma de n�veis de cinza numeroTickets=255.
*/
void
AGraficoGridEixoX::Paint (TDC & dc, bool erase, TRect & rect)
{
//int
  float dx = (10.0 * w / numeroTickets) > 1.0 ? 10.0 * w / numeroTickets : 1.0;	//calcula espa�amento da dire��o x
  dc.MoveTo (x0, y0);		//desenha linha do eixo X
  dc.LineTo (x0 + w, y0);	//
  for (int px = x0 + dx; px <= x0 + w; px += dx)	//desenha os tickets
    {				//antes usava px=x0+dx/2
      dc.MoveTo (px, y0);	//ponto inicial
      dc.LineTo (px, y0 + 3);	//ponto final, altura de 3 p�xel's
    }
}

/*
============================================================================
Documentacao Paint
============================================================================
Descri��o:        Fun��o de desenho do eixo y
Programador:      Andre Duarte Bueno
*/
void
AGraficoGridEixoY::Paint (TDC & dc, bool erase, TRect & rect)
{
//int
  float dy = (10.0 * h / numeroTickets) > 1.0 ? 10.0 * h / numeroTickets : 1.0;	//calcula o espa�amento na dire��o y
  dc.MoveTo (x0, y0);		//move para ponto inicial do eixo
  dc.LineTo (x0, y0 - h);	//desenha o eixo y
  for (int py = y0 - dy; py >= y0 - h; py = py - dy)	//desenha os tickets do eixo y
    {				//antes usava py=y0-dy/2
      dc.MoveTo (abs (x0 - 3), py);	//ponto inicial
      dc.LineTo (x0, py);	//ponto final, largura 3
    }
}

/*
============================================================================
Documentacao AGraficoSub
============================================================================
Descri��o:     Construtor
Programador:      Andre Duarte Bueno
*/
AGraficoSub::AGraficoSub (TVetor * &vy)
{
  y = vy;			//armazena endere�o do vetor
  fatorEscalaY = 1.0;		//armazena fator de escala inicial
};

/*
============================================================================
Documentacao ~AGraficoSub
============================================================================
Descri��o:       Destrutor
Programador:      Andre Duarte Bueno
*/
AGraficoSub::~AGraficoSub ()
{
  if (y)
    delete y;			//apaga os vetores recebidos
}

/*
============================================================================
Documentacao CalculaFatoresDeEscala
============================================================================
Descri��o: Calcula os fatores de escala, que � a altura dispon�vel do grafico
				dividida pela diferen�a entre o maior e o menor valor do gr�fico.
            Por enquanto parte do zero at� o valor m�ximo.
Programador:      Andre Duarte Bueno
*/
void
AGraficoSub::CalculaFatoresDeEscala ()
{
  if (y)
    if (y->Maximo () != 0)
      fatorEscalaY = (float) h / (float) y->Maximo ();
    else
      fatorEscalaY = (float) h;	//assume fator de escala 1
}

/*
============================================================================
Documentacao Posicao
============================================================================
Descri��o:        Recebe a posicao do cursor do mouse no grafico
						Calcula a posicao relativa ao sub-gr�fico
                  Calcula a posicao relativa ao vetor
						Retorna a posi��o x e o valor y do vetor, num formato de char*
Programador:      Andre Duarte Bueno
*/
char *
AGraficoSub::Posicao (TPoint pt)	//pt � a posicao do mouse no grafico todo
{
  float dx = (float) w / (float) y->GetNX ();	//dimensao x de cada barra/linhas
  //
//int   posicaoY=(pt.y-0.1/0.7*h)/dy;             //posicaoY (0,0) no topo do grafico
  int posicaoX = (pt.x - x0) / dx;	//posi��o x real
  if (posicaoX < 0)
    posicaoX = 0;
  if (posicaoX >= y->GetNX ())
    posicaoX = y->GetNX () - 1;
  int valorY = y->data1D[posicaoX];	//valor do vetor
  char *resultado = new char[255];
//char resultado[255];                               //formata o resultado
  int acumulado = 0;
  for (int i = 0; i <= posicaoX; i++)	//percorre os valores e acumula
    acumulado += y->data1D[i];	//at� a posic�o atual
  wsprintf (resultado, "AGraficoSub: X=%d, Y=%d, acumulado=%d)", posicaoX,
	    valorY, acumulado);
  return resultado;
}

/*
============================================================================
Documentacao Paint
============================================================================
Descri��o:
Deve desenhar as barras verticais
x � o vetor para o eixoX (=0 pois � igualmente espa�ado)
y � o vetor para o eixoy(dados a serem plotados)
x0 � a posi��o inicial do eixo,
as barras devem ser centradas em rela��o ao ponto de interesse.
Se quero desenhar na posi��o x=5, as barras iniciam em x=4,8 at� x=5,2 (exemplo)
Programador:      Andre Duarte Bueno
*/

void
AGraficoSubBarra::Paint (TDC & dc, bool erase, TRect & rect)
{
  int indicePaleta = 0;
  float dx = (float) w / (float) y->GetNX ();	//dimensao de cada barra
  if (dx > 1)
    for (int i = 0; i < y->GetNX (); i++)	//percorre o eixoX
      {				//desenha retangulos
	dc.SelectObject (TBrush (paleta16[indicePaleta]));
	dc.Rectangle (x0 + i * dx, y0, x0 + (i + 1) * dx,
		      abs (y0 - y->data1D[i] * fatorEscalaY));
	dc.RestoreBrush ();

	indicePaleta++;
	if (indicePaleta > 15)
	  indicePaleta = 0;
      }
  else				//se o retangulo for somente uma linha
    for (int i = 0; i < y->GetNX (); i++)	//percorre o eixoX
      {				//desenha linhas
	dc.SelectObject (TBrush (paleta16[indicePaleta]));
	dc.MoveTo (x0 + i * dx, y0);
	dc.LineTo (x0 + (i + 1) * dx, abs (y0 - y->data1D[i] * fatorEscalaY));
	dc.RestoreBrush ();

	indicePaleta++;
	if (indicePaleta > 15)
	  indicePaleta = 0;
      }
}

/*
============================================================================
Documentacao Paint
============================================================================
Descri��o:
Deve desenhar as linhas
x � o vetor para o eixoX (=0 pois � igualmente espa�ado)
y � o vetor para o eixoy(dados a serem plotados)
x0 � a posi��o inicial do eixo,
as linhas devem ser centradas em rela��o ao ponto de interesse.
Programador:      Andre Duarte Bueno
*/
void
AGraficoSubLinha::Paint (TDC & dc, bool erase, TRect & rect)
{
//int indicePaleta=0;
  float dx = (float) w / (float) y->GetNX ();	//dimensao x de cada barra
//if(dx<1)
//      dx=1;                                      //garante valor m�nimo para dx, para poder avan�ar
//dc.MoveTo(x0+dx,abs(y0- y->data1D[0]*fatorEscalaY)); //move para posi��o inicial
  dc.MoveTo (x0, abs (y0 - y->data1D[0] * fatorEscalaY));	//move para posi��o inicial
  for (int i = 0; i < y->GetNX (); i++)	//percorre o eixoX
    {				//desenha linhas entre os pontos
      //dc.MoveTo(x0 +i*dx, y0);
      dc.LineTo (x0 + (i + 1) * dx, abs (y0 - y->data1D[i] * fatorEscalaY));
    }
}

/*
============================================================================
Documentacao AGraficoSubXY
============================================================================
Descri��o:      Recebe uma matriz, vai desenhar sobre os dados da matriz recebida
Programador:      Andre Duarte Bueno
*/
AGraficoSubXY::AGraficoSubXY (TMatriz2D * &_my)
{
  this->my = _my;
  fatorEscalaY = 1.0;
  fatorEscalaX = 1.0;
};

/*
============================================================================
Documentacao ~AGraficoSubXY
============================================================================
Descri��o:        Destrutor, apaga os vetores recebidos
Programador:      Andre Duarte Bueno
*/
AGraficoSubXY::~AGraficoSubXY ()
{
  if (my)
    delete my;
}


/*
============================================================================
Documentacao CalculaFatoresDeEscala
============================================================================
Descri��o:
Programador:      Andre Duarte Bueno
*/
void
AGraficoSubXY::CalculaFatoresDeEscala ()
{
  AGraficoSub::CalculaFatoresDeEscala ();
//resolver: precisa retornar o m�ximo da primeira linha da matriz
//fatorEscalaX=(float)w/(float)mx->Maximo();       //calcula fator de escala (float)
}

//----------------------------------------------------------------------------
//Definicao da tabela de resposta
//agora usa TMDIChild, antes usava TFrameWindow
DEFINE_RESPONSE_TABLE1 (AGrafico, TMDIChild) EV_WM_SIZE,	//alteracao do tamanho da janela
  EV_WM_GETMINMAXINFO,		//dimens�es m�nimas da janela
  //     EV_WM_LBUTTONDOWN,                                       //
  EV_WM_MOUSEMOVE,		//movimento mouse
  //     EV_WM_LBUTTONUP,                                         //bot�o esquerdo do mouse solto
  EV_WM_RBUTTONDOWN,		//bot�o direito do mouse pressinado
  END_RESPONSE_TABLE;

/*
============================================================================
Documentacao AGrafico
============================================================================
Descri��o:
		Herdeira publica da TMDIChild e virtual da TWindow
		recebe como ultimos par�metros os vetores vx e vy, que s�o passados para a
		AGraficoSubBarra. A classe AGraficoSubBarra deve eliminar os dois vetores no seu destrutor
Programador:      Andre Duarte Bueno
*/
AGrafico::AGrafico (TMDIClient & parent, const char far * title,
		    TVetor * &vY):
TMDIChild (parent, title)
//, TWindow(parent,title)                                                                       //aqui novidade acrescentei o acesso direto a TWindow
{
  Attr.X = GetSystemMetrics (SM_CXSCREEN) / 8;	//obtem dimens�es
  Attr.Y = GetSystemMetrics (SM_CYSCREEN) / 8;
  Attr.H = Attr.Y * 6;
  Attr.W = Attr.X * 6;
  //cria objetos agregados (partes do todo)
  titulo = new AGraficoTitulo ("Titulo do Grafico");	//passando os titulos
  tituloEixoX = new AGraficoTitulo ("Titulo eixo x");
  tituloEixoY = new AGraficoTitulo ("Titulo eixo y");
  legenda = new AGraficoTitulo ("Legenda");

  gridX = new AGraficoGridEixoX (vY->GetNX ());	//resolver?? deveria ser vX?
  gridY = new AGraficoGridEixoY (vY->GetNX ());
  subGrafico = NULL;		//vai ser criado na classe filha

  unsigned int espessuraCaneta = 1;	//espessura da caneta
  caneta = new TPen (TColor::Black, espessuraCaneta, PS_SOLID);	//cor da caneta
  pincel = new TBrush (TColor::Black);	//cor do pincel
};

/*
============================================================================
Documentacao AGrafico
============================================================================
Descri��o:       Destrutor, destr�e objetos criados no construtor
Programador:      Andre Duarte Bueno
*/
AGrafico::~AGrafico ()
{
  delete titulo;
  delete tituloEixoX;
  delete tituloEixoY;
  delete legenda;
  delete gridX;

  delete gridY;
  delete subGrafico;

  delete caneta;
  delete pincel;
}

/*void AGrafico::SetupWindow()
{
  TFrameWindow::SetupWindow();				//Executa inicializacao da classe base
  //DefineCordenadasMaximas();
}     */

/*
============================================================================
Documentacao Paint
============================================================================
Descri��o:    A fun��o Paint do grafico (de todo o grafico),
				simplesmente chama as fun��es Paint de cada parte do gr�fico.
            Assim cada objeto agregado se auto desenha.
Programador:      Andre Duarte Bueno
*/
void
AGrafico::Paint (TDC & dc, bool erase, TRect & rect)
{
  dc.SelectObject (*caneta);	//caneta preta
  dc.SelectObject (*pincel);	//pincel preto
  titulo->Paint (dc, erase, rect);	//desenho do titulo
  tituloEixoX->Paint (dc, erase, rect);	//desenho do eixo x
  tituloEixoY->Paint (dc, erase, rect);	//desenho do eixo y
  legenda->Paint (dc, erase, rect);	//desenho da legenda
  gridX->Paint (dc, erase, rect);	//desenho do grid x
  gridY->Paint (dc, erase, rect);	//desenho do grid y
  dc.RestoreBrush ();		//restaura o brush
  //Dentro da paint de subGrafico cria um brush colorido
  subGrafico->Paint (dc, erase, rect);	//Desenha a parte do gr�fico em s�
  dc.RestorePen ();		//restaura a caneta
}

//------------------------------------------------------------------------------
//Fun��o herdada da TMDIChild que � redefinida aqui
//A redefini��o � necess�ria, pois sempre que o usuário alterar as dimens�es do gr�fico
//aqui vai atualizar as cordenadas de cada objeto agregado do grafico
//e depois solicitar o redesenho da cada objeto
void
AGrafico::EvSize (UINT sizeType, TSize & size)
{
  TFrameWindow::EvSize (sizeType, size);	//Chama classe base
  DefineCoordenadasObjetos ();	//Calcula as cordenadas dos diversos objetos
  Invalidate ();		//Solicita redesenho da janela
  UpdateWindow ();		//Opcional (redesenhar j�)
}

//------------------------------------------------------------------------------
//Fun��o herdada da TMDIChild que � redefinida aqui
//retorna os valores m�nimos da janela
void
AGrafico::EvGetMinMaxInfo (MINMAXINFO far & mminfo)
{
  mminfo.ptMinTrackSize.x = 300;
  mminfo.ptMinTrackSize.y = 300;
}

/*
============================================================================
Documentacao EvRButtonDown
============================================================================
Descri��o:
*/
void
AGrafico::EvRButtonDown (UINT modKeys, TPoint & ponto)
{
  TWindow::EvMouseMove (modKeys, ponto);

  TRect rect;			//define uma estrutura de retangulo
  GetClientRect (rect);		//obtem o retangulo da janela
  unsigned int h = rect.Height ();	//obtem altura e
  unsigned int w = rect.Width ();	//largura

  char msg[255];		//mensagem para titulo da janela do gr�fico
  AGraficoTitulo::ponteiroJanela = (TWindow *) this;	//passa para classe de titulos o ponteiro para janela
  if (ponto.y <= h * 0.1)
    {
      wsprintf (msg, "titulo");	//              titulo
      SetCaption (msg);		//altera o titulo da janela de acordo com objeto selecionado
      titulo->SetProperties ();
    }
  else if (ponto.y < h * 0.8 && ponto.x < w * 0.15)
    {
      wsprintf (msg, "tituloEixoY");	//              titulo
      SetCaption (msg);		//altera o titulo da janela de acordo com objeto selecionado
      tituloEixoY->SetProperties ();
    }
  else if (ponto.y < h * 0.8 && ponto.x >= w * 0.15)
    {
      wsprintf (msg, "subGrafico");	//              subGrafico
      SetCaption (msg);		//altera o titulo da janela de acordo com objeto selecionado
    }
  else if (ponto.y < h * 0.9)
    {
      wsprintf (msg, "tituloEixoX");	//              titulo
      SetCaption (msg);		//altera o titulo da janela de acordo com objeto selecionado
      tituloEixoX->SetProperties ();
    }
  else
    {
      wsprintf (msg, "legenda");	//              titulo
      SetCaption (msg);		//altera o titulo da janela de acordo com objeto selecionado
      legenda->SetProperties ();
    }

  SetCaption (msg);		//altera o titulo da janela de acordo com objeto selecionado
  Invalidate ();		//Como objetos podem ter sido alterados Solicita redesenho da janela
  UpdateWindow ();		//Opcional (redesenhar j�)
}

/*
============================================================================
Documentacao EvMouseMove
============================================================================
Descri��o:        Funcao de resposta ao  movimento do mouse  EvMouseMove
						continua selecionando area para zoom
Programador:      Andre Duarte Bueno
*/
void
AGrafico::EvMouseMove (UINT modKeys, TPoint & pt_movendo)
{
  TWindow::EvMouseMove (modKeys, pt_movendo);	//fun��o da classe base
  TRect rect;			//define uma estrutura de retangulo
  GetClientRect (rect);		//obtem o retangulo da janela
  unsigned int h = rect.Height ();	//obtem altura e
  unsigned int w = rect.Width ();	//largura

  char msg[255];		//mensagem para titulo da janela do gr�fico
  if (pt_movendo.y <= h * 0.1)
    wsprintf (msg, "titulo");	//              titulo
  else if (pt_movendo.y < h * 0.8 && pt_movendo.x < w * 0.15)
    wsprintf (msg, "tituloEixoY");	//              tituloEixoY
  else if (pt_movendo.y < h * 0.8 && pt_movendo.x >= w * 0.15)
    {
      //wsprintf(msg,subGrafico->Posicao(TPoint (pt_movendo.x-0.15*w ,pt_movendo.y-h*0.1)));
      wsprintf (msg, subGrafico->Posicao (pt_movendo));
    }
  else if (pt_movendo.y < h * 0.9)
    wsprintf (msg, "tituloEixoX");	//              tituloEixoX
  else
    wsprintf (msg, "legenda");	//              legenda

  SetCaption (msg);		//altera o titulo da janela de acordo com objeto selecionado
}

/*
============================================================================
Documentacao GetBitmapRect
============================================================================
Descri��o:      Retorna um retangulo com a �rea a ser impressa
*/
TRect
AGrafico::GetBitmapRect ()
{
  TRect rect;			//define uma estrutura de retangulo
  GetClientRect (rect);		//obtem o retangulo da janela
  return rect;
}

/*
============================================================================
Documentacao CmTMMDeslocamentos
============================================================================
Descri��o: Fun��o utilizada para calcular as cordenadas dos diferentes objetos do gr�fico.
Programador:      Andre Duarte Bueno
*/
void
AGrafico::DefineCoordenadasObjetos ()
{
  TRect rect;			//define uma estrutura de retangulo
  GetClientRect (rect);		//obtem o retangulo da janela
  unsigned int h = rect.Height ();	//obtem altura e
  unsigned int w = rect.Width ();	//largura

//para cada objeto passa a informa��o da posicao inicial altura e largura
//titulo->SetAll                (x0,            y0,             w,                      h);
  titulo->SetAll (0, h / 10, w, h / 10);

  subGrafico->SetAll (w * 0.15, h * 0.8, w * 0.85, h * 0.7);
  subGrafico->CalculaFatoresDeEscala ();	//caso contr�rio fatores escala=1;

  gridX->SetAll (w * 0.15, h * 0.8, w * 0.85, 4);
  tituloEixoX->SetAll (0, h * 0.9, w, h / 10);
  legenda->SetAll (0, h, w, h / 10);

  tituloEixoY->SetAll (0, h * 0.8, w * 0.15, h * 0.7);
  gridY->SetAll (w * 0.15, h * 0.8, 4, h * 0.7);
}

/*
Rela��es inicial para os retangulos, do grafico

Posi��o:
0,0 (da tela).
------------------------------------------------
							titulo                 10%
-------------------------------------------------
15%  |Y
Y    |d
t	  |i					subGrafico             70%
i    |r
t    |g              gridX
-------------------------------------------------
                  	tituloEixoX            10%
-------------------------------------------------
                     legenda                10%
-------------------------------------------------
Obs: Lembre-se: A posi��o 0,0 � no canto superior esquerdo.
*/

/*
============================================================================
Documentacao AGraficoBarra
============================================================================
Descri��o: Construtor
Programador:      Andre Duarte Bueno
*/
AGraficoBarra::AGraficoBarra (TMDIClient & parent, const char far * title,
			      TVetor * &vY):
AGrafico (parent, title, vY)
{
  if (vY)
    subGrafico = new AGraficoSubBarra (vY);
  else
    {
      TVetor *vetorAuxiliar = new TVetor (5);
      vetorAuxiliar->Constante (10);
      subGrafico = new AGraficoSubBarra (vetorAuxiliar);
      MessageBox
	("Vetor de dados inexistente\nGrafico de barras de vetor auxiliar",
	 "Erro", MB_OK | MB_ICONERROR);
    }
}

/*
============================================================================
Documentacao AGraficoLinha
============================================================================
Descri��o:     Construtor
Programador:      Andre Duarte Bueno
*/
AGraficoLinha::AGraficoLinha (TMDIClient & parent, const char far * title,
			      TVetor * &vY):
AGrafico (parent, title, vY)
{
  if (vY)
    subGrafico = new AGraficoSubLinha (vY);
  else
    {
      TVetor *vetorAuxiliar = new TVetor (5);
      vetorAuxiliar->Constante (10);
      subGrafico = new AGraficoSubLinha (vetorAuxiliar);
      MessageBox
	("Vetor de dados inexistente\nGrafico de barras de vetor auxiliar",
	 "Erro", MB_OK | MB_ICONERROR);
    }
}
