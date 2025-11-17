/**
 * @file gui.cpp
 * @brief Interfaz gráfica usando Nana para probar el autocompletado con Trie.
 *
 * Contiene una ventana de selección de modo/dataset y un editor con sugerencias
 * de autocompletado. Las acciones principales son:
 * - Inicializar un `Trie` con un dataset seleccionado.
 * - Mostrar sugerencias basadas en el prefijo actual.
 * - Aceptar sugerencias con TAB o actualizar/insertar palabras con ENTER.
 *
 * Autor: Benjamín Quiroz Villanueva (RUT: 20.265.703-6)
 */

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <iostream>
#include "trie.hpp"
#include "utils.hpp"

using namespace nana;

/**
 * @brief Obtiene la última palabra (sin separadores) al final de un texto.
 *
 * Retorna una cadena vacía si no hay palabra válida. Los separadores considerados
 * son espacio, tabulación y salto de línea.
 *
 * @param texto Texto a analizar.
 * @return Última palabra encontrada o cadena vacía.
 */
std::string obtener_ultima_palabra(const std::string &texto) {
    if (texto.empty()) return "";
    
    auto pos = texto.find_last_not_of(" \t\n");
    if (pos == std::string::npos) return "";
    
    auto end = pos;
    auto start = texto.find_last_of(" \t\n", end);
    
    if (start == std::string::npos) start = 0;
    else start = start + 1;
    
    return texto.substr(start, end - start + 1);
}

/**
 * @brief Convierte la posición del caret (columna, línea) a índice absoluto en el texto.
 *
 * Si la posición excede el tamaño del texto, devuelve `texto.size()`.
 *
 * @param cp Posición del caret como `nana::upoint` (x=columna, y=línea).
 * @param texto Texto donde se calcula el índice.
 * @return Índice absoluto correspondiente en `texto`.
 */
size_t caret_a_indice(const nana::upoint &cp, const std::string &texto) {
    size_t linea_objetivo = static_cast<size_t>(cp.y);
    size_t idx = 0;
    size_t linea_actual = 0;
    
    while(linea_actual < linea_objetivo && idx < texto.size()) {
        if(texto[idx] == '\n') ++linea_actual;
        ++idx;
    }
    
    size_t col = static_cast<size_t>(cp.x);
    if(idx + col > texto.size()) return texto.size();
    
    return idx + col;
}

/**
 * @brief Convierte un índice absoluto en el texto a una posición de caret (columna, línea).
 *
 * Recorre el texto hasta `indice` contando saltos de línea para calcular línea y columna.
 *
 * @param indice Índice absoluto dentro de `texto`.
 * @param texto Texto base.
 * @return `nana::upoint` con la columna (x) y línea (y).
 */
nana::upoint indice_a_caret(size_t indice, const std::string &texto) {
    unsigned linea = 0;
    unsigned columna = 0;
    
    for(size_t i = 0; i < indice && i < texto.size(); ++i) {
        if(texto[i] == '\n') {
            ++linea;
            columna = 0;
        } else {
            ++columna;
        }
    }
    
    return nana::upoint{columna, linea};
}


int main() {
    
    // ==========================================
    // VENTANA 1: Selección de modo
    // ==========================================
    
    form fm_selector{API::make_center(400, 300)};
    fm_selector.caption("Autocompletado con Trie");
    
    label titulo{fm_selector, "Selecciona el modo de autocompletado"};
    titulo.text_align(align::center);
    titulo.typeface(paint::font{"", 14, {600}});
    
    // Selector de dataset
    label lbl_dataset{fm_selector, "Dataset:"};
    lbl_dataset.text_align(align::center);
    
    combox cmb_dataset{fm_selector};
    cmb_dataset.push_back("Words");
    cmb_dataset.push_back("Wikipedia");
    cmb_dataset.push_back("Random");
    cmb_dataset.push_back("Random with Distribution");
    cmb_dataset.option(0); // Seleccionar el primero por defecto
    
    button btn_reciente{fm_selector, "Modo Reciente"};
    button btn_frecuencia{fm_selector, "Modo Frecuencia"};
    
    place layout_selector{fm_selector};
    layout_selector.div(
        "<vertical margin=20 gap=15 "
        "  <titulo weight=40> "
        "  <dataset_label weight=25> "
        "  <dataset_combo weight=30> "
        "  <weight=10> "
        "  <buttons arrange=[150,150] gap=20> "
        ">"
    );

    layout_selector.field("titulo") << titulo;
    layout_selector.field("dataset_label") << lbl_dataset;
    layout_selector.field("dataset_combo") << cmb_dataset;
    layout_selector.field("buttons") << btn_reciente << btn_frecuencia;
    layout_selector.collocate();
    
    // ==========================================
    // VENTANA 2: Editor de texto
    // ==========================================
    form fm_editor{API::make_center(700, 500)};
    fm_editor.caption("Editor con Autocompletado");
    
    // Panel de información del modo
    label lbl_modo{fm_editor, "Modo: (ninguno seleccionado)"};
    lbl_modo.typeface(paint::font{"", 11, {700}});
    lbl_modo.fgcolor(colors::dark_blue);
    
    // Área de texto principal
    textbox txt_editor{fm_editor, true}; // true para multilinea
    txt_editor.typeface(paint::font{"", 12});
    txt_editor.line_wrapped(true);
    txt_editor.enable_caret();
    
    // Panel de sugerencias
    label lbl_sugerencia_titulo{fm_editor, "Sugerencia:"};
    lbl_sugerencia_titulo.typeface(paint::font{"", 10, {600}});
    
    textbox txt_sugerencia{fm_editor, false}; // false para una sola línea
    txt_sugerencia.typeface(paint::font{"", 11});
    txt_sugerencia.editable(false);
    txt_sugerencia.fgcolor(colors::dark_green);
    
    // Panel de ayuda/instrucciones
    label lbl_ayuda{fm_editor, 
        "TAB: Aceptar sugerencia | ENTER: Ignorar y continuar | BACKSPACE: Borrar carácter"};
    lbl_ayuda.typeface(paint::font{"", 9});
    lbl_ayuda.fgcolor(colors::gray);
    lbl_ayuda.text_align(align::center);
    
    place layout_editor{fm_editor};
    layout_editor.div(
        "<vertical margin=15 gap=10 "
        "  <modo weight=30> "
        "  <editor> "
        "  <sugerencias weight=80 <vertical gap=5 "
        "    <titulo weight=25> "
        "    <texto weight=40> "
        "  >> "
        "  <ayuda weight=25> "
        ">"
    );
    layout_editor.field("modo") << lbl_modo;
    layout_editor.field("editor") << txt_editor;
    layout_editor.field("sugerencias") << lbl_sugerencia_titulo << txt_sugerencia;
    layout_editor.field("ayuda") << lbl_ayuda;
    layout_editor.collocate();
    
    
    // ==========================================
    // VARIABLES DEL TRIE
    // ==========================================
    Trie* trie = nullptr;
    std::string dataset_actual = "datasets/words.txt"; // Dataset por defecto
    
        // Evento para cambiar el dataset
    cmb_dataset.events().selected([&](const arg_combox& arg) {
        switch(cmb_dataset.option()) {
            case 0:
                dataset_actual = "datasets/words.txt";
                break;
            case 1:
                dataset_actual = "datasets/wikipedia.txt";
                break;
            case 2:
                dataset_actual = "datasets/random.txt";
                break;
            case 3:
                dataset_actual = "datasets/random_with_distribution.txt";
                break;
        }
        std::cout << "Dataset seleccionado: " << dataset_actual << std::endl;
    });

    // Función para inicializar el Trie con el dataset
    auto inicializar_trie = [&](int variante) {
        if (trie != nullptr) {
            delete trie;
        }
        trie = new Trie(variante);
        recorrer(*trie, dataset_actual);
        std::cout << "Trie inicializado con variante " << variante 
                << " y dataset: " << dataset_actual << std::endl;
    };
    
    // ==========================================
    // EVENTOS DE SELECCIÓN DE MODO
    // ==========================================
    
    btn_reciente.events().click([&] {
        inicializar_trie(1); // 1 = RECENT
        lbl_modo.caption("Modo: Reciente");
        txt_sugerencia.caption("(Escribe para ver sugerencias)");
        fm_selector.hide();
        fm_editor.show();
        txt_editor.focus();
    });
    
    btn_frecuencia.events().click([&] {
        inicializar_trie(0); // 0 = FREQUENCY
        lbl_modo.caption("Modo: Frecuencia");
        txt_sugerencia.caption("(Escribe para ver sugerencias)");
        fm_selector.hide();
        fm_editor.show();
        txt_editor.focus();
    });
    
    
    // ==========================================
    // FUNCIÓN: Actualizar sugerencia
    // ==========================================
    auto actualizar_sugerencia = [&]() {
        if (!trie) return;

        std::string texto_completo = txt_editor.text();
        std::string prefijo = obtener_ultima_palabra(texto_completo);

        // No mostrar sugerencias si no hay prefijo válido o si el texto termina en separador
        if (prefijo.empty() || texto_completo.empty() ||
            texto_completo.back() == ' ' ||
            texto_completo.back() == '\n' ||
            texto_completo.back() == '\t') {
            txt_sugerencia.caption("(No hay prefijo)");
            return;
        }

        // Navegar por el Trie con el prefijo
        TrieNode* nodo = trie->get_root();
        for (char c : prefijo) {
            nodo = trie->descend(nodo, c);
            if (!nodo) break;
        }

        if (nodo) {
            TrieNode* mejor = trie->autocomplete(nodo);
            if (mejor)
                txt_sugerencia.caption(mejor->get_str());
            else
                txt_sugerencia.caption("(Sin sugerencia)");
        } else {
            txt_sugerencia.caption("(Palabra no encontrada)");
        }
    };
    
    
    // ==========================================
    // EVENTO: Cambio de texto
    // ==========================================
    txt_editor.events().text_changed([&]{
        actualizar_sugerencia();
    });
    
    
    // ==========================================
    // EVENTO: Teclas especiales (TAB y ENTER)
    // ==========================================
    txt_editor.events().key_press([&](const arg_keyboard& arg) -> bool {
        if (!trie) return false;

        // ========== TAB: Autocompletar ==========
        if (arg.key == keyboard::tab) {
            std::string texto_completo = txt_editor.text();
            std::string prefijo = obtener_ultima_palabra(texto_completo);

            // Si no hay prefijo, no hacer nada
            if (prefijo.empty()) {
                txt_sugerencia.caption("(No hay prefijo)");
                arg.ignore = true;
                return true;
            }

            // Buscar el nodo del prefijo
            TrieNode* nodo = trie->get_root();
            for (char c : prefijo) {
                nodo = trie->descend(nodo, c);
                if (!nodo) break;
            }

            if (nodo) {
                TrieNode* mejor = trie->autocomplete(nodo);
                if (mejor) {
                    // Obtener posición del caret
                    nana::upoint caret = txt_editor.caret_pos();
                    size_t pos_absoluta = caret_a_indice(caret, texto_completo);

                    // Dividir texto antes y después del caret
                    std::string antes = texto_completo.substr(0, pos_absoluta);
                    std::string despues = texto_completo.substr(pos_absoluta);

                    // Buscar inicio de la última palabra antes del caret
                    auto inicio = antes.find_last_of(" \t\n");
                    if (inicio == std::string::npos) inicio = 0;
                    else inicio += 1;

                    // Construir nuevo texto con la palabra completada
                    std::string nuevo_texto = antes.substr(0, inicio) + mejor->get_str() + " " + despues;
                    txt_editor.caption(nuevo_texto);

                    // Insertar palabra en el trie y actualizar prioridad
                    trie->insert(mejor->get_str());
                    trie->update_priority(mejor);

                    // Reposicionar el caret al final de la palabra insertada
                    size_t nueva_pos = inicio + mejor->get_str().size() + 1;
                    nana::upoint nuevo_caret = indice_a_caret(nueva_pos, nuevo_texto);
                    txt_editor.caret_pos(nuevo_caret);

                    // Limpiar sugerencia
                    txt_sugerencia.caption("(Palabra aceptada)");
                    txt_editor.focus();

                    std::cout << "TAB: Palabra completada -> " << mejor->get_str() << std::endl;
                }
            }

            // Evitar que el tab inserte un carácter tabulación
            arg.ignore = true;
            return true;
        }

        // ========== ENTER: Insertar o actualizar palabra ==========
        if (arg.key == keyboard::enter) {
            std::string texto_completo = txt_editor.text();
            std::string prefijo = obtener_ultima_palabra(texto_completo);

            if (!prefijo.empty()) {
                TrieNode* nodo = trie->get_root();
                bool existe = true;

                for (char c : prefijo) {
                    nodo = trie->descend(nodo, c);
                    if (!nodo) {
                        existe = false;
                        break;
                    }
                }

                if (!existe || !nodo->is_terminal) {
                    trie->insert(prefijo);
                    std::cout << "ENTER: Palabra nueva insertada -> " << prefijo << std::endl;
                } else {
                    trie->update_priority(nodo);
                    std::cout << "ENTER: Prioridad actualizada -> " << prefijo << std::endl;
                }
            }

            // Limpiar sugerencia y dejar que Nana maneje el salto de línea
            txt_sugerencia.caption("(Nueva línea)");
            return false; // deja que el textbox inserte '\n'
        }

        return false; // otras teclas normales
    });
    
    // ==========================================
    // Eventos de cierre
    // ==========================================
    fm_selector.events().unload([&](const arg_unload&){
        if (trie != nullptr) {
            delete trie;
        }
        API::exit();
    });
    
    fm_editor.events().unload([&](const arg_unload&){
        if (trie != nullptr) {
            delete trie;
        }
        API::exit();
    });
    
    
    // Mostrar ventana inicial
    fm_selector.show();
    
    exec();
    
    return 0;
}

 