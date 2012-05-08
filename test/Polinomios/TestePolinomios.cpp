//---------------------------------------------------------------------------
//somente para compilacao da biblioteca
#include <cstdlib>		//por causa itoa exit()
//#include <cconio>		//clrscr();wherex,wherey,gotoxy,

#include <fstream>		//por causa ifstream e ofstream ponteiros arquivos

#include <TFuncao/poly.h>
#include <TFuncao/poli_1g.h>
#include <TFuncao/poli_2g.h>
#include <TFuncao/poli_3g.h>
#include <TFuncao/poli_4g.h>
#include <TFuncao/poli_5g.h>
#include <TFuncao/poli_6g.h>
#include <TFuncao/seno.h>
#include <TFuncao/cos.h>

int main ()
{
    double r1, r2;		//usadas para armazenar o resultado
    Polinomio *polinomio;
    polinomio = new Polinomio (5);	//cria polinomio de grau 5
    cout << "\nIniciou o programa:";
    cout << "\nVai mostrar como foi criado o polinomio de grau 5:";
    polinomio->saida ();		//mostra coeficientes
    cout << "\nVai solicitar a entrada dos coeficientes:";
    polinomio->entrada ();	//entrada de coefificientes
    cout << "\nVai mostrar os coeficientes que voce digitou:";
    polinomio->saida ();		//mostra coeficientes
    cout << "\nEntre com o parametro de calculo para o polinomio de 5 grau:";
    cin >> r1;
    r2 = polinomio->calcular (r1);
    cout << "\nResultado polinomio=" << r2;
    delete polinomio;
    cin.get ();			//faz uma pausa

    Senoide *senoide = new Senoide;
    cout << "\nVai mostrar como foi criada a senoide:";
    senoide->saida ();
    cout << "\nVai solicitar a entrada dos coeficientes:";
    senoide->entrada ();
    cout << "\nVai mostrar os coeficientes que voce digitou:";
    senoide->saida ();
    cout << "\nEntre com o parametro de calculo para a senoide: ";
    cin >> r2;
    r2 = senoide->calcular (r2);
    cout << "\nResultado senoide=" << r2;
    delete senoide;
    cin.get ();

    cout << "\nVai criar polinomios de grau 1,2,3,4: ";

    Polinomio *g1 = new Polinomio_1G;
    Polinomio *g2 = new Polinomio_2G;
    Polinomio *g3 = new Polinomio_3G;
    Polinomio *g4 = new Polinomio_4G;

    cout << "\nVai solicitar os coeficientes dos polinomios 1,2,3,4: ";
    g1->entrada ();
    g2->entrada ();
    g3->entrada ();
    g4->entrada ();
    cout << "\n\tEntre com o parametro de calculo para os polinomios: ";
    cin >> r1;
    cout << "\n1g resultado=" << g1->calcular (r1);	//calcula polinomios
    cout << "\n2g resultado=" << g2->calcular (r1);
    cout << "\n3g resultado=" << g3->calcular (r1);
    cout << "\n4g resultado=" << g4->calcular (r1);

// cout<<"\t\nTeste da sobrecarga de operator()";
// cout<<"\n1g resultado="<<&g1(x);//calcula polinomios
// cout<<"\n2g resultado="<<g2(x);

    delete g1;
    delete g2;
    delete g3;
    delete g4;
    return 0;
}

/*

Obs:

	 0-Criar classe base para  as funcoes c�clicas (seno, cossenno, tengente)
		Verificar se vale ou nao a pena.
	 1-Outras funcoes podem ser agora inclu�das, utilizando o mesmo processo
		ex: cossenos, tangentes, hiperbolicos,...
	 2-foi incluido o m�todo f_definir, que permite a entrada dos
		valores do polinomio, de modo que o programa
		principal pode criar um objeto polinomio e depois  passar os seus
		atributos atrav�s da funcao f_definir.
		Observe que isto nao impede que os mesmos sejam posteriormente
		modificados.

    4-Implementar funcoes f_salvar e f_ler
      Para salvamento e leitura em disco.

    5- Definir em todas as classes a heranca da interface
       como virtual, Classe X:virtual public Interface.

    6- Verificar porque foi necess�rio definir a sobrecarga de
       cin>> e cout<< com o prot�tipo das funcoes.
       A funcao template deveria ser gerada quando o objeto criado fosse
       usado.

       Observe que como os objetos sao dinamicos, o programa nao consegue
       identificar em tempo de compilacao qual o objeto que vai ser criado,
       logo nao consegue identificar qual funcao template deve ser criada.
       � por este motivo que existe a necessidade de se definir o prot�tipo
       da funcao template.


    7- A funcao f_definir pode ter como parametro um ponteiro para doubles
       e em funcao do grau do polinomio, definir cada coeficiente
       do polinomio pelo ponteiro passado
       ex:
       void f_definir(double* p)
       {
       for(ii=0;ii<=grau;ii++)
	  poli[ii]=p[ii];
       }
       A dificuldade � que voce ter� de definir um vetor de doubles
       dentro do programa e entao passar este vetor.
       ex:
       double *vv=new double[n];
       vv[0]=123; vv[1]=455; vv[2]=654;...
       polinomio.f_definir(vv);
       e como esta
       polinomio.f-definir(123,455,654,..) esta �timo.

     8-
	recebendo como parametro a variavel independente.
			double=objeto_polinomio.f_calcular(x);
	sobrecarga      double<<objeto_polinomio;
	friend double operator<<(double r,Polinomio p)

     9- A tentativa de sobrecarga do operador () para substituir
	a funcao de calcula nao d� certo.
	Isto se deve a dois motivos:
	1- O operador () s� pode ser sobrecarregado como funcao membro
	2- A sobrecarga como funcao membro recebe um objeto implicitamente
	   e nao um ponteiro para um objeto.
*/

//---------------------------------------------------------------------------
//somente para compilacao

int main ()
{
    double r1, r2;
    clrscr ();
    TPolinomio *polinomio;
    polinomio = new TPolinomio (5);


    polinomio->f_saida ();
    polinomio->f_entrada ();
    polinomio->f_saida ();
    cout <<
         "\n\tEntre com o parametro de calculo para o polinomio base de 5 grau:";
    cin >> r1;
    r2 = polinomio->f_calcular (r1);
    cout << "\n\tResultado polinomio=" << r2;
    //r2=polinomio(r1+1);
    //cout<<"\n\tResultado polinomio(r1+1)="<<r1;
    delete polinomio;

    getche ();
    Senoide *senoide;
    senoide = new Senoide;

    senoide->f_saida ();
    senoide->f_entrada ();
    senoide->f_saida ();
    cout << "\n\tEntre com o parametro de calculo para a senoide: ";
    cin >> r2;
    r2 = senoide->f_calcular (r2);
    cout << "\n\tResultado senoide=" << r2;
    delete senoide;

    TPolinomio *g1 = new Polinomio_1g;	//cria polinomios
    TPolinomio *g2 = new Polinomio_2g;	//de diferentes graus
    TPolinomio *g3 = new Polinomio_3g;	//observe o uso do polimorfismo
    TPolinomio *g4 = new Polinomio_4g;

    g1->f_entrada ();
    //g2->f_definir(1,2,3); nao � membro de TPolinomio
    g3->f_entrada ();
    g4->f_entrada ();
    cout << "\n\tEntre com o parametro de calculo para o polinomio: ";
    cin >> r1;
    cout << "\n1g resultado=" << g1->f_calcular (r1);	//calcula polinomios
    cout << "\n2g resultado=" << g2->f_calcular (r1);
    cout << "\n3g resultado=" << g3->f_calcular (r1);
    cout << "\n4g resultado=" << g4->f_calcular (r1);

// cout<<"\t\nTeste da sobrecarga de operator()";
// cout<<"\n1g resultado="<<&g1(x);//calcula polinomios
// cout<<"\n2g resultado="<<g2(x);

    delete g1;
    delete g2;
    delete g3;
    delete g4;
    return 0;
}
