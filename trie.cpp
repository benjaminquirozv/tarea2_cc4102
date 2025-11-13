// Nombre: Benjamín Quiroz Villanueva
// RUT: 20.265.703-6

#include "trie.hpp"
#include "utils.hpp" 

TrieNode::TrieNode():
    parent(nullptr),
    is_terminal(false),
    str(nullptr),
    priority(0),
    best_terminal(nullptr),
    best_priority(0) {
        next.fill(nullptr);
    }

Trie::Trie(int variant_mode) {
    root = new TrieNode();
    root->parent = nullptr;
    root->best_terminal = nullptr;
    root->best_priority = 0;
    global_counter = 1;
    variant = variant_mode;           // FREQUENCY por defecto
    size = 1;
}

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

TrieNode* Trie::descend(TrieNode* v, const char c) {
    if (!v) return nullptr;
    int idx = charToIndex(c);
    return v->next[idx];
}

TrieNode* Trie::autocomplete(TrieNode* v) {
    if (!v) return nullptr;
    return v->best_terminal;
}

// update_priority: actualiza priority y propaga best_terminal/best_priority hacia la raíz
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

void Trie::print_trie() const {
    print_trie_helper(root, "");
}

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

int Trie::get_size() const {
    return size;
}

