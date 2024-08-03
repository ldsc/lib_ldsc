#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>

int main() {
    // Roda o gnuplot que fica em background
    std::string gnuplotCommand = "gnuplot -persist";
    system(gnuplotCommand.c_str());

    while (true) {
        // // Assumindo que o arquivo de dados é um CSV
        // std::ifstream dataFile("dados.csv");
        // std::string line;
        //
        // // Cria um arquivo temporário para armazenar os dados a serem plotados
        // std::ofstream tempFile("temp.dat");
        //
        // // Lê o arquivo e escreve os dados no arquivo temporário
        // while (std::getline(dataFile, line)) {
        //     tempFile << line << std::endl;
        // }
        //
        // dataFile.close();
        // tempFile.close();

        // Comando Gnuplot para plotar o arquivo temporário
        std::string plotCommand = "plot \"temp.dat\" using 1:2 with lines";
        // Comandos system são comandos para o gnuplot em background
        system(plotCommand.c_str());

        // Aguarda um tempo antes de repetir o processo
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

}
