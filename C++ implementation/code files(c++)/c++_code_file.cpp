#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <limits> // for numeric_limits
#include<stack>
using namespace std;

#define ROW 9
#define COL 10

// Define Pair struct
struct Pair {
    int first, second;
};

// Define Node struct
struct Node {
    Pair parent;
    double f, g, h;
};

// Comparison operator for Node structs
struct CompareNode {
    bool operator()(const Node& a, const Node& b) const {
        return a.f > b.f;
    }
};

// Function to read grid from file
void readGridFromFile(int grid[][COL], string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file " << filename << endl;
        exit(1);
    }
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            file >> grid[i][j];
        }
    }
    file.close();
}

// Function to read source-destination pairs from file
void readSourceDestFromFile(Pair& src, Pair& dest, string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file " << filename << endl;
        exit(1);
    }
    file >> src.first >> src.second >> dest.first >> dest.second;
    file.close();
}

// A* Search function
void aStarSearch(int grid[][COL], Pair src, Pair dest) {
    // If source is out of range
    if (src.first < 0 || src.first >= ROW || src.second < 0 || src.second >= COL) {
        cout << "Source out of range." << endl;
        return;
    }

    // If destination is out of range
    if (dest.first < 0 || dest.first >= ROW || dest.second < 0 || dest.second >= COL) {
        cout << "Destination out of range." << endl;
        return;
    }

    // If source or destination is blocked
    if (grid[src.first][src.second] == 0 || grid[dest.first][dest.second] == 0) {
        cout << "Source or destination is blocked." << endl;
        return;
    }

    // Create a closed list to store already evaluated nodes
    vector<vector<bool>> closedList(ROW, vector<bool>(COL, false));

    // Declare a 2D array to hold the details of the nodes
    vector<vector<Node>> nodeDetails(ROW, vector<Node>(COL));

    int i, j;

    // Initialize the node details
    for (i = 0; i < ROW; ++i) {
        for (j = 0; j < COL; ++j) {
            nodeDetails[i][j].f = numeric_limits<double>::max();
            nodeDetails[i][j].g = numeric_limits<double>::max();
            nodeDetails[i][j].h = numeric_limits<double>::max();
            nodeDetails[i][j].parent = {-1, -1};
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    nodeDetails[i][j].f = 0.0;
    nodeDetails[i][j].g = 0.0;
    nodeDetails[i][j].h = 0.0;
    nodeDetails[i][j].parent = {i, j};

    // Create a priority queue to store open nodes
    priority_queue<Node, vector<Node>, CompareNode> openList;

    // Push the starting node into the open list
    openList.push({{i, j}, 0.0, 0.0, 0.0});

    // Set a flag to indicate if the destination is reached
    bool foundDest = false;

    // While the open list is not empty
    while (!openList.empty()) {
        Node currentNode = openList.top();
        openList.pop();

        i = currentNode.parent.first;
        j = currentNode.parent.second;
        closedList[i][j] = true;

        // Generate all possible successors
        double gNew, hNew, fNew;

        // Bottom cell
        if (i + 1 < ROW) {
            // If destination is found
            if (i + 1 == dest.first && j == dest.second) {
                nodeDetails[i + 1][j].parent = {i, j};
                foundDest = true;
                break;
            }
            // If successor is not in the closed list and is not blocked
            if (!closedList[i + 1][j] && grid[i + 1][j]) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = sqrt((i + 1 - dest.first) * (i + 1 - dest.first) + (j - dest.second) * (j - dest.second));
                fNew = gNew + hNew;
                // If this path is better than the previous one
                if (nodeDetails[i + 1][j].f == numeric_limits<double>::max() || nodeDetails[i + 1][j].f > fNew) {
                    openList.push({{i + 1, j}, fNew, gNew, hNew});
                    nodeDetails[i + 1][j].f = fNew;
                    nodeDetails[i + 1][j].g = gNew;
                    nodeDetails[i + 1][j].h = hNew;
                    nodeDetails[i + 1][j].parent = {i, j};
                }
            }
        }

        // Left cell
        if (j - 1 >= 0) {
            // If destination is found
            if (i == dest.first && j - 1 == dest.second) {
                nodeDetails[i][j - 1].parent = {i, j};
                foundDest = true;
                break;
            }
            // If successor is not in the closed list and is not blocked
            if (!closedList[i][j - 1] && grid[i][j - 1]) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = sqrt((i - dest.first) * (i - dest.first) + (j - 1 - dest.second) * (j - 1 - dest.second));
                fNew = gNew + hNew;
                // If this path is better than the previous one
                if (nodeDetails[i][j - 1].f == numeric_limits<double>::max() || nodeDetails[i][j - 1].f > fNew) {
                    openList.push({{i, j - 1}, fNew, gNew, hNew});
                    nodeDetails[i][j - 1].f = fNew;
                    nodeDetails[i][j - 1].g = gNew;
                    nodeDetails[i][j - 1].h = hNew;
                    nodeDetails[i][j - 1].parent = {i, j};
                }
            }
        }

        // Right cell
        if (j + 1 < COL) {
            // If destination is found
            if (i == dest.first && j + 1 == dest.second) {
                nodeDetails[i][j + 1].parent = {i, j};
                foundDest = true;
                break;
            }
            // If successor is not in the closed list and is not blocked
            if (!closedList[i][j + 1] && grid[i][j + 1]) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = sqrt((i - dest.first) * (i - dest.first) + (j + 1 - dest.second) * (j + 1 - dest.second));
                fNew = gNew + hNew;
                // If this path is better than the previous one
                if (nodeDetails[i][j + 1].f == numeric_limits<double>::max() || nodeDetails[i][j + 1].f > fNew) {
                    openList.push({{i, j + 1}, fNew, gNew, hNew});
                    nodeDetails[i][j + 1].f = fNew;
                    nodeDetails[i][j + 1].g = gNew;
                    nodeDetails[i][j + 1].h = hNew;
                    nodeDetails[i][j + 1].parent = {i, j};
                }
            }
        }

        // Top cell
        if (i - 1 >= 0) {
            // If destination is found
            if (i - 1 == dest.first && j == dest.second) {
                nodeDetails[i - 1][j].parent = {i, j};
                foundDest = true;
                break;
            }
            // If successor is not in the closed list and is not blocked
            if (!closedList[i - 1][j] && grid[i - 1][j]) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = sqrt((i - 1 - dest.first) * (i - 1 - dest.first) + (j - dest.second) * (j - dest.second));
                fNew = gNew + hNew;
                // If this path is better than the previous one
                if (nodeDetails[i - 1][j].f == numeric_limits<double>::max() || nodeDetails[i - 1][j].f > fNew) {
                    openList.push({{i - 1, j}, fNew, gNew, hNew});
                    nodeDetails[i - 1][j].f = fNew;
                    nodeDetails[i - 1][j].g = gNew;
                    nodeDetails[i - 1][j].h = hNew;
                    nodeDetails[i - 1][j].parent = {i, j};
                }
            }
        }
    }

    // If destination is not found
    if (!foundDest) {
        cout << "Destination not found." << endl;
        return;
    }

    // Print the path
    cout << "Path found!" << endl;
    int row = dest.first;
    int col = dest.second;
    stack<Pair> path;
    while (!(nodeDetails[row][col].parent.first == row && nodeDetails[row][col].parent.second == col)) {
        path.push({row, col});
        int temp_row = nodeDetails[row][col].parent.first;
        int temp_col = nodeDetails[row][col].parent.second;
        row = temp_row;
        col = temp_col;
    }
    path.push({row, col});
    while (!path.empty()) {
        Pair p = path.top();
        path.pop();
        cout << "(" << p.first << ", " << p.second << ") ";
    }
    cout << endl;
}

// Main function
int main() {
    int grid[ROW][COL];
    Pair src, dest;

    // Read grid from file
    readGridFromFile(grid, "C:\\Users\\siddh\\OneDrive\\Documents\\Desktop\\VS Code\\DAA PROJECT\\input files\\grid.txt");

    // Read source-destination pairs from file
    readSourceDestFromFile(src, dest, "C:\\Users\\siddh\\OneDrive\\Documents\\Desktop\\VS Code\\DAA PROJECT\\input files\\source_dest_input_3.txt");

    // Call A* search function
    aStarSearch(grid, src, dest);

    return 0;
}
