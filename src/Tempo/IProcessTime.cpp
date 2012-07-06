
/*
----------------------------------------------------------------------------
PROJETO:		Teste objeto rotulagem
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Damiani,Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	IProcessTime.cpp
Nome da classe:         IProcessTime
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Observações:		Implementa um objeto de marcação de tempo de execução de uma função
*/
//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------
#include <TTempo/IProcessTime.h>

//Construtor, recebe uma string com o nome da função
//e cria uma variavel com a hora atual
IProcessTime::IProcessTime (const IString & _msg)	//Construtor
{
  inicio = oSys->Time ();
  msg = _msg;
}

//Destrutor, apresenta o tempo de execução da função
IProcessTime::~IProcessTime ()	//destrutor
{
  TSysTime fim = oSys->Time ();	//
  long int totalInicio =
    inicio.hour * 360000 + inicio.minute * 6000 + inicio.second * 100 +
    inicio.milliseconds;
  long int totalFim =
    fim.hour * 360000 + fim.minute * 6000 + fim.second * 100 +
    fim.milliseconds;

  long int tempoDecorrido =
    (totalFim - totalInicio) >
    0 ? (totalFim - totalInicio) : (totalInicio - totalFim);
  cout << "\nTempo decorrido processo [" << msg << "]:" << (tempoDecorrido) <<
    " (ms)";
  cout.flush ();
}
