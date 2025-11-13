// Nombre: Benjamín Quiroz Villanueva
// RUT: 20.265.703-6

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include "trie.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

int main() {
    
    
    try {
        Trie words_freq(FREQUENCY);
        Trie words_rec(RECENT);

        Trie words_freq_2(FREQUENCY);
        Trie words_rec_2(FREQUENCY);

        Trie words_freq_3(FREQUENCY);
        Trie words_rec_3(FREQUENCY);


        cout << " === Analisis de consumo de memoria y tiempo, dataset: Words === \n\n";    
        cargarArchivoPalabras(words_freq, "datasets/words.txt");

        cout << " === Analisis de consumo de memoria y tiempo, dataset: Words === \n\n";    
        cargarArchivoPalabras(words_rec, "datasets/words.txt");


        cout << " === Analisis de autocompletado para dataset: Wikipedia [Variante: Frecuencia] === \n\n";

        auto start1 = std::chrono::high_resolution_clock::now();        
        recorrer(words_freq, "datasets/wikipedia.txt");
        auto end1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds1 = end1 - start1;
        cout << "Tiempo en simular analisis: " << elapsed_seconds1.count() << " segundos \n";

        cout << " === Analisis de autocompletado para dataset: Wikipedia [Variante: Reciente] === \n\n";

        auto start2 = std::chrono::high_resolution_clock::now();        
        recorrer(words_rec, "datasets/wikipedia.txt");
        auto end2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds2 = end2 - start2;
        cout << "Tiempo en simular analisis: " << elapsed_seconds2.count() << " segundos \n";

        cout << " === Analisis de autocompletado para dataset: Random [Variante: Frecuencia]=== \n\n";

        auto start3 = std::chrono::high_resolution_clock::now();        
        recorrer(words_freq_2, "datasets/random.txt");
        auto end3 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds3 = end3 - start3;
        cout << "Tiempo en simular analisis: " << elapsed_seconds3.count() << " segundos \n";

        cout << " === Analisis de autocompletado para dataset: Random [Variante: Reciente]=== \n\n";

        auto start4 = std::chrono::high_resolution_clock::now();        
        recorrer(words_rec_2, "datasets/random.txt");
        auto end4 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds4 = end4 - start4;
        cout << "Tiempo en simular analisis: " << elapsed_seconds4.count() << " segundos \n";

        cout << " === Analisis de autocompletado para dataset: Random with distribution [Variante: Frecuencia]=== \n\n";

        auto start5 = std::chrono::high_resolution_clock::now();        
        recorrer(words_freq_3, "datasets/random_with_distribution.txt");
        auto end5 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds5 = end5 - start5;
        cout << "Tiempo en simular analisis: " << elapsed_seconds5.count() << " segundos \n";

        cout << " === Analisis de autocompletado para dataset: Random with distribution [Variante: Reciente]=== \n\n";

        auto start6 = std::chrono::high_resolution_clock::now();        
        recorrer(words_rec_3, "datasets/random_with_distribution.txt");
        auto end6 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds6 = end6 - start6;
        cout << "Tiempo en simular analisis: " << elapsed_seconds6.count() << " segundos \n";

    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}