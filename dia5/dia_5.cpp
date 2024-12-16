#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <cstring>
using namespace std;

// En el input no hay numeros mayores que 100
// Así evitamos estructuras dinámicas más complejas
const int MAX_NODES = 100;
/*
Vamos a implementar los grafos con una lista de adyacencia 
de este tipo (array de listas)
1: {3, 4}
2: {7}
3: {1}
4: {1}
5: {}
*/

// Implementamos los nodos con una lista de adyacencia
struct Grafo {
    array<vector<int>, MAX_NODES> adjList; // Array listas
    bool existe[MAX_NODES]; // Lista de booleanos para ver si un valor existe
    
    // Constructor que pone valores del booleano a false
    Grafo(){
        memset(existe, false, sizeof(existe));
    }

    void addEdge(int from, int to){
        adjList[from].push_back(to); // Añadimos arista de from a to
        existe[from] = true;
        existe[to] = true;
    }

    bool isNode(int nodo) const {
        return existe[nodo];
    }
};

// Función que lee el documento, lo separa en las reglas por ||
// y las actualizaciones por comas
// Las reglas y las actualizaciones están separadas por una línea en blanco
// así que usamos eso para separar 
void parseInput(const string& filename, Grafo& grafo, vector<vector<int>>& updates) {
    ifstream file(filename);
    string line;
    bool parsingUpdates = false;

    while (getline(file, line)) {
        // Si no es línea vacía continua
        // Si lo es, pon a true que ya has acabado las reglas.
        if (line.empty()) {
            parsingUpdates = true;
            continue;
        }

        // Si no has acabado las reglas
        // Separas en x e y y las metes al grafo
        if (!parsingUpdates) {
            int x, y;
            char delimiter;
            stringstream ss(line);
            ss >> x >> delimiter >> y;
            grafo.addEdge(x, y);
        } else {
            // Obtener actualizaciones
            vector<int> actualizaciones;
            stringstream ss(line);
            string token;
            while (getline(ss, token, ',')) {
                actualizaciones.push_back(stoi(token));
            }
            updates.push_back(actualizaciones); 
        }
    }
}

// Función que te verifica si la actualización sigue la regla
bool esCorrecto(const vector<int>& actualizacion, const Grafo& grafo) {
    // Crear un mapa de índices 
    array<int, MAX_NODES> pageIndex = {};
    fill(pageIndex.begin(), pageIndex.end(), -1); // Llenamos a -1

    for (size_t i = 0; i < actualizacion.size(); ++i) {
        pageIndex[actualizacion[i]] = i;
    }

    // Comprueba si los índices siguen la norma que nos da la regla almacenada en el grafo
    for (int node = 0; node < MAX_NODES; ++node) {
        if (grafo.isNode(node) && pageIndex[node] != -1) { // Si el nodo está en la actualización
            for (int neighbor : grafo.adjList[node]) {
                if (pageIndex[neighbor] != -1 && pageIndex[node] > pageIndex[neighbor]) {
                    return false; // Regla violada
                }
            }
        }
    }
    return true;
}

// Función para calcular la suma de los valores medios, lo que pide el advent
int sumaValoresMedios(const vector<vector<int>>& actualizaciones, const Grafo& grafo) {
    int suma = 0;

    for (const auto& actualizacion : actualizaciones) {
        if (esCorrecto(actualizacion, grafo)) {
            int medio = actualizacion.size() / 2; // Índice del valor medio
            suma += actualizacion[medio];        // Sumar el valor medio
        }
    }

    return suma;
}

int main() {
    // Creamos el grafo y las act.
    Grafo grafo;
    vector<vector<int>> actualizaciones;

    // Leer y procesar el archivo
    string filename = "input_5.txt";
    parseInput(filename, grafo, actualizaciones);

    // Suma de valores medios
    int suma_final = sumaValoresMedios(actualizaciones, grafo);

    // Imprimir el resultado
    cout << "Suma de las paginas centrales: " << suma_final << endl;

    return 0;
}
