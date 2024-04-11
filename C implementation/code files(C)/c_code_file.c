#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define ROW 9
#define COL 10

// Define Pair struct
struct Pair {
    int first, second;
};

// Define Node struct
struct Node {
    struct Pair parent;
    double f, g, h;
};

// Function to read grid from file
void readGridFromFile(int grid[][COL], char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            fscanf(file, "%d", &grid[i][j]);
        }
    }
    fclose(file);
}

// Function to read source-destination pairs from file
void readSourceDestFromFile(struct Pair *src, struct Pair *dest, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    fscanf(file, "%d %d %d %d", &src->first, &src->second, &dest->first, &dest->second);
    fclose(file);
}

// A* Search function
void aStarSearch(int grid[][COL], struct Pair src, struct Pair dest) {
    // If source is out of range
    if (src.first < 0 || src.first >= ROW || src.second < 0 || src.second >= COL) {
        printf("Source out of range.\n");
        return;
    }

    // If destination is out of range
    if (dest.first < 0 || dest.first >= ROW || dest.second < 0 || dest.second >= COL) {
        printf("Destination out of range.\n");
        return;
    }

    // If source or destination is blocked
    if (grid[src.first][src.second] == 0 || grid[dest.first][dest.second] == 0) {
        printf("Source or destination is blocked.\n");
        return;
    }

    // Create a closed list to store already evaluated nodes
    int closedList[ROW][COL];
    for (int i = 0; i < ROW; ++i) {
        for (int j = 0; j < COL; ++j) {
            closedList[i][j] = 0;
        }
    }

    // Declare a 2D array to hold the details of the nodes
    struct Node nodeDetails[ROW][COL];

    int i, j;

    // Initialize the node details
    for (i = 0; i < ROW; ++i) {
        for (j = 0; j < COL; ++j) {
            nodeDetails[i][j].f = INT_MAX;
            nodeDetails[i][j].g = INT_MAX;
            nodeDetails[i][j].h = INT_MAX;
            nodeDetails[i][j].parent.first = -1;
            nodeDetails[i][j].parent.second = -1;
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    nodeDetails[i][j].f = 0.0;
    nodeDetails[i][j].g = 0.0;
    nodeDetails[i][j].h = 0.0;
    nodeDetails[i][j].parent = src;

    // Set a flag to indicate if the destination is reached
    int foundDest = 0;

    // Create a priority queue to store open nodes
    struct Node openList[ROW * COL];
    int openListSize = 0;

    // Push the starting node into the open list
    openList[openListSize++] = (struct Node) {{i, j}, 0.0, 0.0, 0.0};

    // While the open list is not empty
    while (openListSize > 0) {
        // Find the node with the lowest f value in the open list
        int minIndex = 0;
        double minF = openList[0].f;
        for (int k = 1; k < openListSize; ++k) {
            if (openList[k].f < minF) {
                minF = openList[k].f;
                minIndex = k;
            }
        }

        struct Node currentNode = openList[minIndex];
        openList[minIndex] = openList[--openListSize];

        i = currentNode.parent.first;
        j = currentNode.parent.second;
        closedList[i][j] = 1;

        // Generate all possible successors
        double gNew, hNew, fNew;

        // Bottom cell
        if (i + 1 < ROW) {
            // If destination is found
            if (i + 1 == dest.first && j == dest.second) {
                nodeDetails[i + 1][j].parent = (struct Pair) {i, j};
                foundDest = 1;
                break;
            }
            // If successor is not in the closed list and is not blocked
            if (!closedList[i + 1][j] && grid[i + 1][j]) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = sqrt((i + 1 - dest.first) * (i + 1 - dest.first) + (j - dest.second) * (j - dest.second));
                fNew = gNew + hNew;
                // If this path is better than the previous one
                if (nodeDetails[i + 1][j].f == INT_MAX || nodeDetails[i + 1][j].f > fNew) {
                    openList[openListSize++] = (struct Node) {{i + 1, j}, fNew, gNew, hNew};
                    nodeDetails[i + 1][j].f = fNew;
                    nodeDetails[i + 1][j].g = gNew;
                    nodeDetails[i + 1][j].h = hNew;
                    nodeDetails[i + 1][j].parent = (struct Pair) {i, j};
                }
            }
        }

        // Left cell
        if (j - 1 >= 0) {
            // If destination is found
            if (i == dest.first && j - 1 == dest.second) {
                nodeDetails[i][j - 1].parent = (struct Pair) {i, j};
                foundDest = 1;
                break;
            }
            // If successor is not in the closed list and is not blocked
            if (!closedList[i][j - 1] && grid[i][j - 1]) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = sqrt((i - dest.first) * (i - dest.first) + (j - 1 - dest.second) * (j - 1 - dest.second));
                fNew = gNew + hNew;
                // If this path is better than the previous one
                if (nodeDetails[i][j - 1].f == INT_MAX || nodeDetails[i][j - 1].f > fNew) {
                    openList[openListSize++] = (struct Node) {{i, j - 1}, fNew, gNew, hNew};
                    nodeDetails[i][j - 1].f = fNew;
                    nodeDetails[i][j - 1].g = gNew;
                    nodeDetails[i][j - 1].h = hNew;
                    nodeDetails[i][j - 1].parent = (struct Pair) {i, j};
                }
            }
        }

        // Right cell
        if (j + 1 < COL) {
            // If destination is found
            if (i == dest.first && j + 1 == dest.second) {
                nodeDetails[i][j + 1].parent = (struct Pair) {i, j};
                foundDest = 1;
                break;
            }
            // If successor is not in the closed list and is not blocked
            if (!closedList[i][j + 1] && grid[i][j + 1]) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = sqrt((i - dest.first) * (i - dest.first) + (j + 1 - dest.second) * (j + 1 - dest.second));
                fNew = gNew + hNew;
                // If this path is better than the previous one
                if (nodeDetails[i][j + 1].f == INT_MAX || nodeDetails[i][j + 1].f > fNew) {
                    openList[openListSize++] = (struct Node) {{i, j + 1}, fNew, gNew, hNew};
                    nodeDetails[i][j + 1].f = fNew;
                    nodeDetails[i][j + 1].g = gNew;
                    nodeDetails[i][j + 1].h = hNew;
                    nodeDetails[i][j + 1].parent = (struct Pair) {i, j};
                }
            }
        }

        // Top cell
        if (i - 1 >= 0) {
            // If destination is found
            if (i - 1 == dest.first && j == dest.second) {
                nodeDetails[i - 1][j].parent = (struct Pair) {i, j};
                foundDest = 1;
                break;
            }
            // If successor is not in the closed list and is not blocked
            if (!closedList[i - 1][j] && grid[i - 1][j]) {
                gNew = nodeDetails[i][j].g + 1.0;
                hNew = sqrt((i - 1 - dest.first) * (i - 1 - dest.first) + (j - dest.second) * (j - dest.second));
                fNew = gNew + hNew;
                // If this path is better than the previous one
                if (nodeDetails[i - 1][j].f == INT_MAX || nodeDetails[i - 1][j].f > fNew) {
                    openList[openListSize++] = (struct Node) {{i - 1, j}, fNew, gNew, hNew};
                    nodeDetails[i - 1][j].f = fNew;
                    nodeDetails[i - 1][j].g = gNew;
                    nodeDetails[i - 1][j].h = hNew;
                    nodeDetails[i - 1][j].parent = (struct Pair) {i, j};
                }
            }
        }
    }

    // If destination is not found
    if (!foundDest) {
        printf("Destination not found.\n");
        return;
    }

    // Print the path
    printf("Path found!\n");
    int row = dest.first;
    int col = dest.second;
    struct Pair path[ROW * COL];
    int pathSize = 0;
    while (!(nodeDetails[row][col].parent.first == row && nodeDetails[row][col].parent.second == col)) {
        path[pathSize++] = (struct Pair) {row, col};
        int temp_row = nodeDetails[row][col].parent.first;
        int temp_col = nodeDetails[row][col].parent.second;
        row = temp_row;
        col = temp_col;
    }
    path[pathSize++] = (struct Pair) {row, col};
    for (int k = pathSize - 1; k >= 0; --k) {
        printf("(%d, %d) ", path[k].first, path[k].second);
    }
    printf("\n");
}

// Main function
int main() {
    int grid[ROW][COL];
    struct Pair src, dest;

    // Read grid from file
    readGridFromFile(grid, "C:\\Users\\siddh\\OneDrive\\Documents\\Daa Project\\C implementation\\input files\\grid.txt");

    // Read source-destination pairs from file
    readSourceDestFromFile(&src, &dest, "C:\\Users\\siddh\\OneDrive\\Documents\\Daa Project\\C implementation\\input files\\source_dest_input_3.txt");

    // Call A* search function
    aStarSearch(grid, src, dest);

    return 0;
}
