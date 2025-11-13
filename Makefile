# =========================
# Makefile para Nana en Windows
# =========================

CXX = g++
CXXFLAGS = -std=c++17 -O2 -I/mingw64/include
LDFLAGS = -L/mingw64/lib -lnana -ljpeg -lpng

# Fuentes y objetos
MAIN_SRC = main.cpp trie.cpp
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)

GUI_SRC  = gui.cpp trie.cpp
GUI_OBJ  = $(GUI_SRC:.cpp=.o)

MAIN_EXE = tarea2.exe
GUI_EXE  = gui_app.exe

# Compilar ambos ejecutables
all: $(MAIN_EXE) $(GUI_EXE)

# Ejecutable consola
$(MAIN_EXE): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Ejecutable gráfico (sin consola)
$(GUI_EXE): $(GUI_OBJ)
	$(CXX) $(CXXFLAGS) -mwindows $^ -o $@ $(LDFLAGS)

# Regla genérica para compilar .cpp a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -f $(MAIN_OBJ) $(GUI_OBJ) $(MAIN_EXE) $(GUI_EXE)

.PHONY: all clean
