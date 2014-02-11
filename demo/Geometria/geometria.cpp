#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd4.h>
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd8.h>
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd34.h>
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd5711.h>
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd3453D.h>
using namespace std;

/**
 * Exemplo simples de uso da biblioteca de objetos geometricos.
 *
 * Exercício: Gerar saida em modo texto e gerar gráficos no display.
 */
int main (int argc, char *argv[]) {

	cout  << "\n================================================================================"
				<< "\n========= Programa para criar bolas de acordo com a métrica escolhida =========="
				<< "\n================================================================================"
				<< "\nUSO: ./criarBola"
				<< "\n================================================================================" << endl;
	int dimensao;
	bool sair = false;
	do {
		cout << "Qual a dimensão da bola que deseja criar (impar)? \n";
		cin >>  dimensao;
		if ((dimensao % 2) == 0) {
			cout << "A dimensão informada deve ser impar! \n";
		} else {
			sair = true;
		}
	} while( ! sair);

	cout << "Qual métrica utilizar para criar a bola? \n"
			 << "CDCd4        (1)\n"
			 << "CDCd8        (2)\n"
			 << "CDCd34       (3)\n"
			 << "CDCd5711     (4)\n"
			 << "CDCd345 (3D) (5)\n";
	int opcao;
	cin >> opcao;

	CBCDiscreta* bola = NULL;
	CBCDiscreta3D * bola3D = NULL;
	string nomebola;
	switch (opcao) {
		case 1 :
			bola = new CBCd4(dimensao);
			nomebola = "CBCd4";
			break;
		case 2 :
			bola = new CBCd8(dimensao);
			nomebola = "CBCd8";
			break;
		case 3 :
			bola = new CBCd34(dimensao);
			nomebola = "CBCd34";
			break;
		case 4 :
			bola = new CBCd8(dimensao);
			nomebola = "CBCd5711";
			break;
		case 5 :
			bola3D = new CBCd3453D(dimensao);
			nomebola = "CBCd3453D";
			break;
	}

	ostringstream os;
	os << "../../data/images/" << nomebola << "-" << dimensao;
	switch (opcao) {
		case 1 :
		case 2 :
		case 3 :
		case 4 :
			os << ".pgm";
			cout << "Salvando a bola em disco com o nome " << os.str() << endl;
			bola->SetFormato(P2_X_Y_GRAY_ASCII);
			bola->NumCores(bola->MaiorValor());
			bola->salvarAlinhado = true;
			bola->Path("");
			bola->Write( os.str() );
			break;
		case 5 :
			os << ".dgm";
			cout << "Salvando a bola em disco com o nome " << os.str() << endl;
			bola3D->SetFormato(D2_X_Y_Z_GRAY_ASCII);
			bola3D->NumCores(bola3D->MaiorValor());
			bola3D->salvarAlinhado = true;
			bola3D->Path("");
			bola3D->Write( os.str() );
			break;
		default : cout << "Opção inválida! Saindo...";
	}

	if ( bola ) delete bola;
	if (bola3D) delete bola3D;
	return 0;
}

