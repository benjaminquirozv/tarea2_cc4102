/**
 * @file trie.cpp
 * @brief Implementación de la estructura Trie (árbol de prefijos) y sus operaciones.
 *
 * Este archivo contiene la implementación de los métodos declarados en
 * `trie.hpp`. El Trie soporta dos variantes de prioridad para autocompletado:
 * FREQUENCY (incrementa contador por cada uso) y RECENT (prioriza por uso reciente).
 *
 * Autor: Benjamín Quiroz Villanueva (RUT: 20.265.703-6)
 */

#include "trie.hpp"
#include "utils.hpp" 

/**
 * @brief Constructor por defecto de un nodo del Trie.
 *
 * Inicializa punteros a `nullptr`, marca `is_terminal` como false y pone
 * prioridades en 0. El arreglo `next` se rellena con `nullptr`.
 */
TrieNode::TrieNode():
    parent(nullptr),
    is_terminal(false),
    str(nullptr),
    priority(0),
    best_terminal(nullptr),
    best_priority(0) {
        next.fill(nullptr);
    }

/**
 * @brief Constructor del Trie.
 *
 * @param variant_mode Modo de prioridad para el autocompletado (FREQUENCY o RECENT).
 *
 * Inicializa la raíz y variables internas como `global_counter` y `size`.
 */
Trie::Trie(int variant_mode) {
    root = new TrieNode();
    root->parent = nullptr;
    root->best_terminal = nullptr;
    root->best_priority = 0;
    global_counter = 1;
    variant = variant_mode;           // FREQUENCY por defecto
    size = 1;
}

/**
 * @brief Inserta una palabra en el Trie.
 *
 * Si parte del camino no existe, crea nodos nuevos. Marca el nodo terminal
 * correspondiente y guarda la cadena (alloca un `string`).
 *
 * @param w Referencia a la palabra a insertar.
 */
void Trie::insert(const string& w){
    TrieNode* current = root;

    for (char c : w) {
        int index = charToIndex(c);
        // crear nodos de ser necesario
        if (current->next[index] == nullptr) {
            current->next[index] = new TrieNode();
            current->next[index]->parent = current;
            size++;
        }

        current = current->next[index];
    }

    if (!current->is_terminal) {
        current->is_terminal = true;
        current->str = new string(w);
        current->priority = 0;
    }
}

/**
 * @brief Desciende un nivel en el Trie usando el caracter dado.
 *
 * @param v Nodo desde el que se desea descender.
 * @param c Caracter usado para seleccionar la arista de descendencia.
 * @return Puntero al nodo hijo correspondiente, o `nullptr` si no existe.
 */
TrieNode* Trie::descend(TrieNode* v, const char c) {
    if (!v) return nullptr;
    int idx = charToIndex(c);
    return v->next[idx];
}

/**
 * @brief Devuelve el terminal con mayor prioridad en el subárbol de `v`.
 *
 * Este método aprovecha el campo `best_terminal` mantenido durante las
 * actualizaciones de prioridad para obtener la mejor sugerencia de autocompletado.
 *
 * @param v Nodo desde el cual se consulta el mejor terminal.
 * @return Puntero al `TrieNode` terminal con mayor prioridad, o `nullptr`.
 */
TrieNode* Trie::autocomplete(TrieNode* v) {
    if (!v) return nullptr;
    return v->best_terminal;
}

/**
 * @brief Actualiza la prioridad de un nodo terminal y propaga la mejor opción hacia la raíz.
 *
 * Dependiendo de la variante (`FREQUENCY` o `RECENT`) actualiza `v->priority`.
 * Luego, si corresponde, actualiza `best_priority` y `best_terminal` en los
 * ancestros hasta que ya no sea necesario propagar.
 *
 * @param v Nodo terminal cuya prioridad se debe actualizar.
 */
void Trie::update_priority(TrieNode* v) {
    if (!v || !v->is_terminal) return;  // seguridad

    // Actualizar la prioridad según la variante
    if (variant == FREQUENCY) {
        v->priority += 1;
    } else if (variant == RECENT) {
        v->priority = global_counter++;
    }

    // Propagar hacia la raíz
    TrieNode* node = v->parent;
    while (node != nullptr) {
        if (node->best_priority < v->priority) {
            node->best_priority = v->priority;
            node->best_terminal = v;
            node = node->parent;
        } else {
            break;  // ya no se necesita subir más
        }
    }
}

/**
 * @brief Imprime el contenido del Trie en texto (uso para debug).
 */
void Trie::print_trie() const {
    print_trie_helper(root, "");
}

/**
 * @brief Auxiliar recursiva para `print_trie`.
 *
 * Muestra cada palabra terminal encontrada y sus prioridades.
 *
 * @param node Nodo actual en la recursión.
 * @param prefix Prefijo acumulado (no usado para la salida actual, pero útil si se extiende).
 */
void Trie::print_trie_helper(TrieNode* node, string prefix) const {
    if (!node) return;

    if (node->is_terminal) {
        cout << "Palabra: " << *(node->str)
                << " | priority: " << node->priority
                << " | best_terminal: "
                << (node->best_terminal ? *(node->best_terminal->str) : "NULL")
                << "\n";
    }

    for (int i = 0; i < 27; ++i) {
        if (node->next[i]) {
            char c = (i < 26) ? ('a' + i) : '$';
            print_trie_helper(node->next[i], prefix + c);
        }
    }
}

/**
 * @brief Devuelve la cantidad de nodos actualmente en el Trie.
 * @return Número de nodos (incluye la raíz).
 */
int Trie::get_size() const {
    return size;
}

