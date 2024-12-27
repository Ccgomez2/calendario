#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

struct Machine {
    int a_x, a_y; // Botón A
    int b_x, b_y; // Botón B
    int p_x, p_y; // Posición del premio
};

// Implementación de una tabla hash manual
class HashTable {
private:
    static const int TABLE_SIZE = 10007; // Tamaño de la tabla
    vector<vector<pair<pair<int, int>, int>>> table; // Encadenamiento

    int hashFunction(int x, int y) {
        return (x * 31 + y) % TABLE_SIZE;
    }

public:
    HashTable() : table(TABLE_SIZE) {}

    void insert(int x, int y, int cost) {
        int index = hashFunction(x, y);
        for (auto& entry : table[index]) {
            if (entry.first.first == x && entry.first.second == y) {
                entry.second = min(entry.second, cost);
                return;
            }
        }
        table[index].emplace_back(make_pair(x, y), cost);
    }

    int get(int x, int y) {
        int index = hashFunction(x, y);
        for (auto& entry : table[index]) {
            if (entry.first.first == x && entry.first.second == y) {
                return entry.second;
            }
        }
        return numeric_limits<int>::max(); // Valor grande si no está
    }

    pair<int, vector<int>> getStatistics() const {
        int total_entries = 0;
        vector<int> bucket_sizes(TABLE_SIZE, 0);

        for (size_t i = 0; i < table.size(); ++i) {
            if (!table[i].empty()) {
                bucket_sizes[i] = table[i].size();
                total_entries += table[i].size();
            }
        }

        return make_pair(total_entries, bucket_sizes);
    }
};

int solveMachine(const Machine& machine, HashTable& visited) {
    const int MAX_PRESSES = 100; // Máximo de presiones permitido
    int min_cost = numeric_limits<int>::max();

    for (int a = 0; a <= MAX_PRESSES; ++a) {
        for (int b = 0; b <= MAX_PRESSES; ++b) {
            int x = a * machine.a_x + b * machine.b_x;
            int y = a * machine.a_y + b * machine.b_y;

            int cost = a * 3 + b * 1;
            visited.insert(x, y, cost);

            if (x == machine.p_x && y == machine.p_y) {
                min_cost = min(min_cost, cost);
            }
        }
    }

    return min_cost == numeric_limits<int>::max() ? -1 : min_cost;
}

vector<Machine> readMachinesFromFile(const string& filename) {
    vector<Machine> machines;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        if (line.find("Button A:") != string::npos) {
            int a_x, a_y, b_x, b_y, p_x, p_y;

            // Leer botón A
            sscanf(line.c_str(), "Button A: X+%d, Y+%d", &a_x, &a_y);

            // Leer botón B
            getline(file, line);
            sscanf(line.c_str(), "Button B: X+%d, Y+%d", &b_x, &b_y);

            // Leer premio
            getline(file, line);
            sscanf(line.c_str(), "Prize: X=%d, Y=%d", &p_x, &p_y);

            machines.push_back({a_x, a_y, b_x, b_y, p_x, p_y});
        }
    }

    return machines;
}

int main() {
    vector<Machine> machines = readMachinesFromFile("input_13.txt");

    int total_cost = 0;
    int prizes_won = 0;
    HashTable visited;

    for (const auto& machine : machines) {
        int cost = solveMachine(machine, visited);
        if (cost != -1) {
            total_cost += cost;
            prizes_won++;
        }
    }

    cout << "Prizes won: " << prizes_won << endl;
    cout << "Total cost: " << total_cost << endl;

    
}
