#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct Position {
    int x, y, cost, direction;
    Position(int x, int y, int cost, int direction) : x(x), y(y), cost(cost), direction(direction) {}
    Position() : x(0), y(0), cost(0), direction(0) {} // Constructor predeterminado
};

// Nodo del BST
struct BSTNode {
    Position position;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Position pos) : position(pos), left(nullptr), right(nullptr) {}
};

// Insertar un nodo en el BST
BSTNode* insertBST(BSTNode* root, Position position) {
    if (!root) return new BSTNode(position);
    if (position.cost < root->position.cost) {
        root->left = insertBST(root->left, position);
    } else {
        root->right = insertBST(root->right, position);
    }
    return root;
}

// Encontrar y eliminar el nodo con el menor coste en el BST
BSTNode* extractMinBST(BSTNode*& root, Position& minPosition) {
    if (!root) return nullptr;
    if (!root->left) {
        minPosition = root->position;
        BSTNode* rightSubtree = root->right;
        delete root;
        return rightSubtree;
    }
    root->left = extractMinBST(root->left, minPosition);
    return root;
}

// Liberar memoria del BST
void deleteBST(BSTNode* root) {
    if (!root) return;
    deleteBST(root->left);
    deleteBST(root->right);
    delete root;
}

// Parse the maze and find the start and end positions
void parseMaze(ifstream& file, vector<string>& maze, Position& start, Position& end) {
    string line;
    int y = 0;
    while (getline(file, line)) {
        maze.push_back(line);
        for (int x = 0; x < line.size(); ++x) {
            if (line[x] == 'S') {
                start = Position(x, y, 0, 0); // Start facing East
            } else if (line[x] == 'E') {
                end = Position(x, y, 0, -1); // End direction doesn't matter
            }
        }
        y++;
    }
}

int findLowestScore(vector<string>& maze, Position start, Position end) {
    int height = maze.size();
    int width = maze[0].size();

    // Directions: East, South, West, North
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    // BST root
    BSTNode* bstRoot = nullptr;
    bstRoot = insertBST(bstRoot, start);

    // Visited map with direction-specific tracking
    unordered_map<string, int> visited;

    while (bstRoot) {
        Position current;
        bstRoot = extractMinBST(bstRoot, current);

        // Generate a unique key for the current position and direction
        string key = to_string(current.x) + "," + to_string(current.y) + "," + to_string(current.direction);

        // Skip if already visited with a lower cost
        if (visited.count(key) && visited[key] <= current.cost) {
            continue;
        }
        visited[key] = current.cost;

        // Check if we reached the end
        if (current.x == end.x && current.y == end.y) {
            deleteBST(bstRoot);
            return current.cost;
        }

        // Explore neighbors in all directions
        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];
            int newCost = current.cost + 1; // Moving forward costs 1 point

            // Check bounds and walls
            if (nx < 0 || nx >= width || ny < 0 || ny >= height || maze[ny][nx] == '#') {
                continue;
            }

            // Calculate turn cost
            if (i != current.direction) {
                newCost += 1000; // Turning costs 1000 points
            }

            Position neighbor(nx, ny, newCost, i);
            bstRoot = insertBST(bstRoot, neighbor);
        }
    }

    return -1; // If no path is found
}

int main() {
    ifstream file("input_16.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open input_16.txt" << endl;
        return 1;
    }

    vector<string> maze;
    Position start(0, 0, 0, 0);
    Position end(0, 0, 0, 0);

    parseMaze(file, maze, start, end);

    int lowestScore = findLowestScore(maze, start, end);

    cout << "The lowest score a Reindeer could possibly get: " << lowestScore << endl;

    return 0;
}

