// Nombre: Benjamín Quiroz Villanueva
// RUT: 20.265.703-6

#ifndef TRIE_HPP
#define TRIE_HPP

// #include "utils.hpp"

#include <array>
#include <string>
#include <cstdint>
#include <iostream>

using namespace std;

struct TrieNode {
    TrieNode* parent;
    array<TrieNode*,27> next;
    bool is_terminal;
    string* str;
    uint64_t priority;
    TrieNode* best_terminal;
    uint64_t best_priority;
    
    TrieNode();


    string get_str() const { return (str ? *str : ""); }
};

class Trie {
    public:
    // Variante de autocompletado: 0 = FREQUENCY, 1 = RECENT
    Trie(int variant_mode);
    
    int variant;
    
    void insert(const string& w);

    TrieNode* descend(TrieNode* v, const char c);

    TrieNode* autocomplete(TrieNode* v);

    void update_priority(TrieNode* v);

    // Getter para root
    TrieNode* get_root() const { return root; }

    // Setter para root (opcional)
    void set_root(TrieNode* r) { root = r; }

    void print_trie() const;                   // función de debug

    int get_size() const;
private:
    TrieNode* root;

    uint64_t global_counter;   // contador para la variante RECENT

    uint64_t size; // tamaño

    void print_trie_helper(TrieNode* node, std::string prefix) const;
};

#endif