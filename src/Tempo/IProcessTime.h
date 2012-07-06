#if !defined(IProcessTime_h)
#define IProcessTime_h
/*
----------------------------------------------------------------------------
PROJETO:		Biblioteca LDSC/COILIB2.0
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Damiani,Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	IProcessTime.h
Nome da classe:      IProcessTime
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Observações:        Representa um objeto de marcação de tempo de execução de uma função
*/
//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------
//Bibliotecas da COI-LIB incluidas com um obj

#include <sys/system.h>
#include <sys/string.h>
#include <sys/datetime.h>

//----------------------------------------------------------------------------
//Classe de controle de tempo de execucao de uma função qualquer
//Exemplo de uso:
//No inicio da função cria um objeto : IProcessTime wait("nomeFuncao");
//Quando a função sai de escopo o objeto wait do tipo IProcessTime é destruido
//e o tempo decorrido no processamento da função é passado para a tela,
//utilizando cout.
class IProcessTime
{
//Atributos
  IString msg;			//string com o nome da função executada
  TSysTime inicio;		//hora em que a função iniciou
//Metodos
public:
    IProcessTime (const IString & _msg);	//Construtor
   ~IProcessTime ();		//Destrutor
};
#endif //IProcessTime
