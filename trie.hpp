/**
 * @file trie.hpp
 * @brief Declaraciones del Trie (árbol de prefijos) y su nodo.
 *
 * Define `TrieNode` y la clase `Trie` con las operaciones públicas usadas
 * por la aplicación: inserción, navegación por prefijos y manejo de prioridades
 * para autocompletado.
 *
 * Autor: Benjamín Quiroz Villanueva (RUT: 20.265.703-6)
 */

#ifndef TRIE_HPP
#define TRIE_HPP

#include <array>
#include <string>
#include <cstdint>
#include <iostream>

using namespace std;

/**
 * @brief Nodo del Trie.
 *
 * Contiene punterros a hijos (`next`), puntero al padre, y campos usados
 * para indicar terminalidad y gestionar prioridades para el autocompletado.
 */
struct TrieNode {
    TrieNode* parent;                    /**< Puntero al padre */
    array<TrieNode*,27> next;            /**< Punteros a hijos (a-z + extras) */
    bool is_terminal;                    /**< True si el nodo marca el fin de una palabra */
    string* str;                         /**< Almacena la palabra completa en nodos terminales */
    uint64_t priority;                   /**< Prioridad del nodo (para autocompletar) */
    TrieNode* best_terminal;             /**< Mejor terminal en el subárbol */
    uint64_t best_priority;              /**< Prioridad del `best_terminal` */
    
    /**
     * @brief Constructor por defecto inicializa campos y arreglos.
     */
    TrieNode();

    /**
     * @brief Devuelve la cadena almacenada si es nodo terminal.
     * @return Cadena almacenada o cadena vacía si no existe.
     */
    string get_str() const { return (str ? *str : ""); }
};

/**
 * @brief Estructura Trie para autocompletado.
 *
 * Soporta dos variantes de priorización: FREQUENCY (conteo de usos) y RECENT
 * (prioriza por uso reciente). Proporciona las operaciones básicas necesarias
 * para insertar palabras, navegar por prefijos y obtener sugerencias.
 */
class Trie {
    public:
    /**
     * @brief Construye un Trie vacío.
     * @param variant_mode Variante de autocompletado: 0 = FREQUENCY, 1 = RECENT
     */
    Trie(int variant_mode);
    
    int variant; /**< Variante activa del autocompletado (FREQUENCY/RECENT) */
    
    /**
     * @brief Inserta la palabra `w` en el Trie.
     * @param w Palabra a insertar (por referencia constante).
     */
    void insert(const string& w);

    /**
     * @brief Desciende desde `v` por el carácter `c`.
     * @param v Nodo de partida.
     * @param c Carácter a seguir.
     * @return Nodo hijo o `nullptr` si no existe la rama.
     */
    TrieNode* descend(TrieNode* v, const char c);

    /**
     * @brief Obtiene la mejor sugerencia (terminal) en el subárbol de `v`.
     * @param v Nodo desde el cual se consulta.
     * @return Puntero a `TrieNode` terminal con mayor prioridad o `nullptr`.
     */
    TrieNode* autocomplete(TrieNode* v);

    /**
     * @brief Actualiza la prioridad del nodo terminal `v` y propaga cambios.
     * @param v Nodo terminal cuya prioridad se actualiza.
     */
    void update_priority(TrieNode* v);

    /**
     * @brief Devuelve la raíz del Trie.
     * @return Puntero a la raíz.
     */
    TrieNode* get_root() const { return root; }

    /**
     * @brief Establece la raíz (uso interno/experimental).
     * @param r Nuevo nodo raíz.
     */
    void set_root(TrieNode* r) { root = r; }

    /**
     * @brief Imprime el Trie en salida estándar (uso de depuración).
     */
    void print_trie() const;                   // función de debug

    /**
     * @brief Devuelve el número de nodos en el Trie (incluye raíz).
     * @return Número de nodos.
     */
    int get_size() const;
private:
    TrieNode* root;                    /**< Raíz del Trie */

    uint64_t global_counter;   /**< Contador usado por la variante RECENT */

    uint64_t size; // tamaño

    void print_trie_helper(TrieNode* node, std::string prefix) const;
};

#endif