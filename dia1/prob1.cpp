#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

class Datos {
public:
    
    void load(const string& filename, vector<int>& v1, vector<int>& v2) {
        ifstream file(filename); //leer

        if (!file) {
            cerr << "Error al abrir el archivo: " << filename << endl;
            exit(1);
        }

        string line, data;

        while (getline(file, line)) {
            stringstream iss(line); //Para extraer los valores
            iss >> data;
            v1.push_back(stoi(data)); //añadir un elemento al final del contenedor
            iss >> data;
            v2.push_back(stoi(data));
        }

        file.close();
    }
};

class DyV{
public:

    int div (const vector<int>& v1, const vector<int>& v2, int ini, int fin) {
        
        if (ini == fin) {
            return abs(v1[ini] - v2[ini]);
        }
        if (ini > fin) {
            return 0;
        }

        int m = (ini + fin) / 2;
        int izq = div(v1, v2, ini, m);
        int der = div(v1, v2, m + 1, fin);

        return izq + der;
    }
};

int main() {
    vector<int> v1, v2; 
    Datos datos;
    datos.load("input.txt", v1, v2);

    // Ordenar los vectores
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    DyV dyv;
    int dist_total = dyv.div(v1, v2, 0, v1.size() - 1);

    cout << "La distancia total es: " << dist_total << endl;

    return 0;
}

