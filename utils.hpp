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

/**
 * @file utils.hpp
 * @brief Utilidades y funciones auxiliares para cargar datasets y medir rendimiento.
 *
 * Contiene funciones para convertir caracteres a índices del Trie, cargar
 * archivos de palabras en el Trie y recorrer un dataset para calcular métricas
 * de autocompletado. Está pensado como apoyo a la interfaz gráfica y pruebas.
 */

/**
 * @brief Variante de priorización para autocompletado.
 *
 * - `FREQUENCY` (0): incrementa contador por cada uso de la palabra.
 * - `RECENT` (1): asigna prioridades según un contador global para priorizar lo más reciente.
 */
enum Variant { FREQUENCY = 0, RECENT = 1 };

// Forward declaration de Trie para evitar dependencia circular
class Trie;
struct TrieNode;

/**
 * @brief Convierta un carácter a índice para el arreglo `next` del `TrieNode`.
 *
 * Mapea `a`..`z` a 0..25 y cualquier otro carácter al índice 26.
 *
 * @param c Carácter de entrada (se asume minúscula para letras inglesas).
 * @return Índice en rango 0..26.
 */
inline int charToIndex(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    return 26; // índice para caracteres especiales
}

/**
 * @brief Cargar un archivo de palabras en el `Trie` (modo no-verbose).
 *
 * Lee el archivo `rutaArchivo`, inserta cada palabra leída en `trie` y muestra
 * información periódica sobre el progreso y tiempos. Lanza excepción si no
 * puede abrir el archivo.
 *
 * @param trie Trie donde se insertan las palabras.
 * @param rutaArchivo Ruta al archivo de palabras.
 */
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

/**
 * @brief Recorre un dataset, inserta palabras en el Trie y calcula métricas.
 *
 * Calcula cuántos caracteres habría que escribir si el autocompletado
 * funciona con la política actual del `Trie`. Actualiza prioridades a medida
 * que encuentra palabras (simula que el usuario las acepta/usa).
 *
 * @param trie Trie a utilizar.
 * @param rutaArchivo Ruta del archivo de palabras.
 */
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

/**
 * @brief Cargar un archivo de palabras con salida de depuración paso a paso.
 *
 * Muestra en consola cada palabra insertada y el estado del Trie tras
 * cada inserción. Útil para depuración y para entender cómo crecen los nodos.
 *
 * @param trie Trie donde se insertan las palabras.
 * @param rutaArchivo Ruta al archivo a leer.
 */
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
