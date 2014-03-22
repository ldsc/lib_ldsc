/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: Programa de teste da LIB_LDSC
  ===============================================================================

  Desenvolvido por:
  Laboratorio de Desenvolvimento de Software Cientifico
  [LDSC].
  @author     André Duarte Bueno
  @file       main.cpp
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by André Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

// ----------------------------------------------------------------------
// Bibliotecas C/C++
// ----------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <iostream>
#include <cstdlib>       // EXIT_SUCCESS;
using namespace std;

// ----------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------
#include <AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CConfiguracoesEquilibrio3D.h>
#include <MetNum/Matriz/TCMatriz3D.h>

/**
 * Função main() inclui teste da biblioteca
 * Cria imagem 3D a partir do argumento passado para o programa na linha de comando,
 * ou solicitando o nome do arquivo de disco
 * A seguir cria objeto para determinação das configurações de equilíbrio
 * e chama o método Go().
 * @param argc
 * @param argv[]
 * @return
 */
int main(int argc, char *argv[])
{
    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./confeq3D nomeImagem"
          << "\nou"
          << "\n./confeq3D"
          << "\n================================================================================" << endl;


    TCMatriz3D<int>* img3D {nullptr } ; // ponteiro para imagem

    if (argc==2)
    {
        // Cria imagem 3D
        img3D = new TCMatriz3D<int>(argv[1]);
        if (img3D == nullptr )
        {
            cerr << "\nFalha abertura arquivo: " << argv[1] << endl;
            return 0;
        }
    }
    else
    {
        string nomeArquivo;
        cout << "\nEntre com o nome da imagem 3D - binária (ex: P262_K70-2-45-50-lp-d6.pm3 ):";
        getline(cin,nomeArquivo);
        // Cria imagem 3D
        img3D = new TCMatriz3D<int>(nomeArquivo);
        if (img3D == NULL)
        {
            cerr << "\nFalha abertura arquivo: " << argv[1] << endl;
            return 0;
        }
    }
    
    // Cria arquivo de saída
    ofstream fout("confeq3D.out");
    // Cria objeto para determinação das configurações de equilíbrio
    CConfiguracoesEquilibrio3D *confeq { nullptr };
    confeq = new CConfiguracoesEquilibrio3D ( fout );
    if (confeq == nullptr)
    {
        cerr << "\nFalha criação confeq: saindo \n";
        return 0;
    }

    cout << "Deseja modificar atributos da simulação e dos fluidos (s)(n)? ";
    char resp;
    bool disco { false };
    cin >> resp;
    cin.get();
    if (resp == 's' || resp == 'S')
    {
        cout << "\n\nSalvarResultadosParciaisDisco (gera diversos arquivos com as camaras)? (0) não, (1) sim.";
        cin >> disco;
        cin.get();
        confeq->SalvarResultadosParciaisDisco(disco);

        int fluidoAcompressibilidade;
        cout << "\n\nEntre com a compressibilidade do fluidoA: (0) não compressível (1) compressível? ";
        cout << "\ndefault=" << confeq->fluidoA->Compressibilidade() << endl;
        cin >> fluidoAcompressibilidade;
        cin.get();
        confeq->fluidoA->Compressibilidade(fluidoAcompressibilidade);
        cout << "Entrou com o valor fluidoAcompressibilidade: " << fluidoAcompressibilidade << endl;

        int fluidoBmolhabilidade;
        cout << "\n\nEntre com a molhabilidade do fluidoB: (0) não molhante (1) molhante? ";
        cout << "\ndefault=" << confeq->fluidoB->Molhabilidade() << endl;
        cin >> fluidoBmolhabilidade;
        cin.get();
        confeq->fluidoB->Molhabilidade(fluidoBmolhabilidade);
        cout << "Entrou com o valor fluidoBmolhabilidade: " <<  fluidoBmolhabilidade << endl;
    }

    cout << "Propriedades: "  << endl;
    cout << "confeq->fluidoA->Compressibilidade(): " 	<< confeq->fluidoA->Compressibilidade() 		<< endl;
    cout << "confeq->fluidoB->Molhabilidade() : " 		<< confeq->fluidoB->Molhabilidade() 			<< endl;
    cout << "confeq->SalvarResultadosParciaisDisco()=" 	<< confeq->SalvarResultadosParciaisDisco() << endl;

	// Executa determinação das configurações de equilíbrio
    cout << "\n================> Processando dados (pode demorar)." << endl;
    confeq->Go( img3D );

	// Destroi objetos criados
    delete img3D;
    delete confeq;
    return EXIT_SUCCESS;
}

