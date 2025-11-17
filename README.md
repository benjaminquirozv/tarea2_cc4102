# Tarea 2: Tries para Autocompletado

Nombre: Benjamín Quiroz Villanueva
RUT: 20.265.703-6


## Requisitos

Este proyecto se implementó utilizando la interfaz MSYS2 MinGW64 desde Windows 11 versión 25H2. 
Utiliza la librería Nana C++ GUI para la creación de interfaces gráficas de usuario. Se instala con los siguientes comandos:

```
pacman -S mingw-w64-x86_64-nana

pacman -S mingw-w64-x86_64-libjpeg-turbo mingw-w64-x86_64-libpng
```

### 

- GNU Make
- Compilador C++ (compatible con C++11 o superior)
- Librerías estándar de C++
- Nana C++ GUI


## Estructura del proyecto

La organización de carpetas y archivos es la siguiente:

```
├── Makefile                            # Reglas para compilar, ejecutar y limpiar el proyecto
├── README.md                           # Documentación general y guía de uso
├── main.cpp                            # Programa principal, ejecuta pruebas y experimentos
├── utils.hpp                           # Funciones y estructuras auxiliares
├── trie.cpp                            # Implementación del trie
├── trie.hpp                            # Declaración de la clase Trie y funciones asociadas
├── gui.cpp                             # Implementación de la interfaz gráfica
└── datasets/                           # Datasets de prueba
    ├── wikipedia.txt                   # Contiene texto real de distintas páginas de Wikipedia
    ├── words.txt                       # Dataset que contiene 2^18 palabras
    ├── random.txt                      # Dataset que contiene las palabras de words con distribución uniforme
    └── random_with_distribution.txt    #  Dataset generado de forma aleatoria pero con una distribución más realista
```

El archivo `Makefile` automatiza la compilación y ejecución.

## Para ejecutar el experimento
Para realizar el experimento: 
1) Clonar este repositorio.
2) Desde la raíz del directorio, simplemente ejecutar el comando `make`.
3) Una vez compilado, se puede ejecutar el programa con: `./tarea2.exe`.
4) Además, se añade una interfaz interactiva, la cual se puede acceder con: `./gui_app.exe`.
5) Para limpiar los archivos generados: `make clean`

## Comentarios y Documentación (Doxygen)
Para las firmas y documentación se utilizó la IA de Copilot.