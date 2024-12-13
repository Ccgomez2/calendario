#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cctype>
#include <cstdlib>

static bool canAchieveTarget(
    size_t i, 
    long long current_value, 
    const std::vector<long long> &numbers, 
    long long target, 
    std::unordered_map<std::string, bool> &memo
) {
    // Si llegamos al final del vector
    if (i == numbers.size()) {
        return (current_value == target);
    }

    // Generar clave para memo
    std::string key = std::to_string(i) + "," + std::to_string(current_value);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }

    // Intentar sumar
    bool res = canAchieveTarget(i + 1, current_value + numbers[i], numbers, target, memo);
    // Intentar multiplicar sólo si no se encontró solución sumando
    if (!res) {
        res = canAchieveTarget(i + 1, current_value * numbers[i], numbers, target, memo);
    }

    memo[key] = res;
    return res;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long total = 0;
    std::string line;

    while (true) {
        if(!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        
        // Buscar la posición de ':'
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) continue;
        
        std::string target_str = line.substr(0, colon_pos);
        std::string nums_str = line.substr(colon_pos + 1);

        // Función para recortar espacios
        auto trim = [](std::string &s) {
            while(!s.empty() && std::isspace((unsigned char)s.back())) s.pop_back();
            while(!s.empty() && std::isspace((unsigned char)s.front())) s.erase(s.begin());
        };
        
        trim(target_str);
        trim(nums_str);

        long long target = std::atoll(target_str.c_str());
        
        // Obtener los números
        std::vector<long long> numbers;
        {
            std::stringstream ss(nums_str);
            long long x;
            while (ss >> x) {
                numbers.push_back(x);
            }
        }

        if (numbers.empty()) continue;

        // Memo para almacenamiento de estados: (i, current_value) -> bool
        std::unordered_map<std::string, bool> memo;

        // Iniciamos desde el segundo número, teniendo como valor actual el primero.
        // Porque el primer número no requiere operador antes de él.
        bool possible = false;
        if (numbers.size() == 1) {
            // Si solo hay un número, la respuesta es inmediata
            possible = (numbers[0] == target);
        } else {
            possible = canAchieveTarget(1, numbers[0], numbers, target, memo);
        }

        if (possible) {
            total += target;
        }
    }
    
    std::cout << total << "\n";

    return 0;
}


