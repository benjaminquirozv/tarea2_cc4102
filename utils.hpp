// Nombre: Benjamín Quiroz Villanueva
// RUT: 20.265.703-6

#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <array>
#include <fstream>
#include <string>
#include <iostream>
#include "trie.hpp"
#include <chrono>

#include <stdexcept>

using namespace std;

enum Variant { FREQUENCY = 0, RECENT = 1 };

// Forward declaration de Trie para evitar dependencia circular
class Trie;
struct TrieNode;

// Función para convertir char a índice (0-25 para 'a'-'z', 26 para otros)
inline int charToIndex(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    return 26; // índice para caracteres especiales
}

// Función simple para cargar palabras sin debug
inline void cargarArchivoPalabras(Trie& trie, const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + rutaArchivo);
    }
    
    std::string palabra;


    int i = 0;
    int e = 0;
    int group_counter = 0;
    int word_mark = 16384;

    while (archivo >> palabra) {
        auto start = std::chrono::high_resolution_clock::now();
        trie.insert(palabra);
        i++;
        group_counter++;
        
        if (group_counter == word_mark) {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;
            cout << "Tiempo en insertar 16384 palabras:" << elapsed_seconds.count() << "\n";
            group_counter = 0;
        }

        uint64_t marca = (1ULL << e);

        if ( i == marca) {
            cout << "Insercion numero: " << i << "\n";
            cout << "Cantidad de nodos del trie: " << trie.get_size() << "\n";
            e++;
        }
    }
    
    archivo.close();
    cout << " === === \n";

}

inline void recorrer(Trie& trie, const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + rutaArchivo);
    }
    
    uint64_t total_char = 0;      // caracteres totales del texto
    uint64_t total_escrito = 0;   // caracteres que el usuario escribe
    int i = 0;
    int e = 0;
    
    std::string palabra;
    
    while (archivo >> palabra) {
        trie.insert(palabra);
        i++;
        
        total_char += palabra.length();  // siempre cuenta el largo total
        
        TrieNode* node = trie.get_root();
        int chars_escritos_palabra = 0;  // contador para esta palabra específica
        bool autocompletado = false;

        for (char c : palabra) {
            node = trie.descend(node, c);
            chars_escritos_palabra++;

            if (node == nullptr) {
                // no existe el prefijo → el usuario escribe todo
                total_escrito += palabra.length();
                autocompletado = false;
                break;
            }

            TrieNode* best = trie.autocomplete(node);
            if (best != nullptr && best->get_str() == palabra) {
                // se pudo autocompletar
                total_escrito += chars_escritos_palabra;
                autocompletado = true;
                break;
            }
        }

        // si recorrió toda la palabra sin autocompletar ni fallar
        if (!autocompletado && node != nullptr) {
            total_escrito += palabra.length();
        }

        // actualiza prioridad si la palabra existe
        if (node != nullptr && node->is_terminal) {
            trie.update_priority(node);
        }

        uint64_t marca = (1ULL << e);

        if (i == marca) {
            cout << "Insercion numero: " << i << "\n";
            cout << "2 elevado a: " << e << "\n";
            cout << "Total de caracteres escritos: " << total_escrito << "\n";
            cout << "Total de caracteres en el texto: " << total_char << "\n";
            cout << "Porcentaje de caracteres escritos respecto al total del texto: " 
                 << (double)total_escrito / total_char * 100 << "%\n";
            e++;
        }
    }

    archivo.close();

    cout << "\n=== RESULTADOS FINALES ===\n";
    cout << "Total de caracteres escritos: " << total_escrito << "\n";
    cout << "Total de caracteres: " << total_char << "\n";
    cout << "Porcentaje final: " << (double)total_escrito / total_char * 100 << "%\n\n";
}

// Función con debug para mostrar pasos de inserción
inline void cargarArchivoPalabrasConDebug(Trie& trie, const std::string& rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + rutaArchivo);
    }
    
    std::string palabra;
    int contador = 0;
    
    std::cout << "========================================\n";
    std::cout << "INICIANDO CARGA DE caracteres DESDE: " << rutaArchivo << "\n";
    std::cout << "========================================\n\n";
    
    while (archivo >> palabra) {
        contador++;
        std::cout << "--- Paso " << contador << " ---\n";
        std::cout << "Insertando palabra: \"" << palabra << "\"\n";
        
        trie.insert(palabra);
        
        std::cout << "Estado del Trie después de insertar:\n";
        trie.print_trie();
        std::cout << "\n";
    }
    
    archivo.close();
    
    std::cout << "========================================\n";
    std::cout << "CARGA COMPLETADA\n";
    std::cout << "Total de palabras procesadas: " << contador << "\n";
    std::cout << "========================================\n\n";
}

#endif // UTILS_HPP
