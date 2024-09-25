/*
Bryce Taylor
bktaylor2@crimson.ua.edu
CS 581
Homework #1

To compile (on Windows):
    g++ -Wall -O life.cpp -o life.exe

To run:
    ./life.exe (Size of board) (Max generations)
    ./life.exe 1000 1000
*/

#include <chrono>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    int boardSize; // Board size (N)
    int maxGenerations; // Maximum number of generations
    vector<vector<bool>> curBoard; // Stores the state of the current board
    vector<vector<bool>> newBoard; // Needed so that current board is not modified during calculation
    vector<bool> row; // Helper array to store a row of a board
    int numNeighbors; // Helper variable to track number of neighbors a cell has

    srand(time(0)); // Seed RNG

    // Input processing and error checking
    if (argc != 3) {
        cout << "Usage: ./life.exe (Size of board) (Max number of generations)" << endl;
        return 1;
    }
    boardSize = atoi(argv[1]);
    maxGenerations = atoi(argv[2]);
    if (boardSize <= 0 || maxGenerations <= 0) {
        cout << "Arguments should be positive integers." << endl;
        return 1;
    }
    cout << "Board size: " << boardSize << ", Max generations: " << maxGenerations << endl;

    // Initialize empty board
    for (int i = 0; i < boardSize + 2; i++) {
        row.push_back(0);
    }
    for (int i = 0; i < boardSize + 2; i++) {
        curBoard.push_back(row);
    }
    row.clear();

    // Initialize board (minus borders) with pseudo-random values
    for (int i = 1; i < boardSize + 1; i++) {
        for (int j = 1; j < boardSize + 1; j++) {
            curBoard[i][j] = rand() % 2;
        }
    }

    /*
    for (int i = 0; i < boardSize + 2; i++) {
        for (int j = 0; j < boardSize + 2; j++) {
            cout << curBoard[i][j] << " ";
        }
        cout << endl;
    }
    */

    // Start timer
    auto start = chrono::high_resolution_clock::now();

    // Main algorithm loop
    for (int n = 1; n <= maxGenerations; n++) {
        // Calculate new board
        newBoard = curBoard;
        // Note: does not calculate border "ghost" cells since these will never change
        for (int i = 1; i < boardSize + 1; i++) {
            for (int j = 1; j < boardSize + 1; j++) {
                // Calculate number of alive neighbors this cell has
                numNeighbors = 0;
                numNeighbors += curBoard[i-1][j-1] + curBoard[i-1][j] + curBoard[i-1][j+1] + \
                                curBoard[i+1][j-1] + curBoard[i+1][j] + curBoard[i+1][j+1] + \
                                curBoard[i][j-1] + curBoard[i][j+1];

                // If cell is dead...
                if (curBoard[i][j] == 0) {
                    // 3 neighbors means it becomes alive
                    if (numNeighbors == 3) {
                        newBoard[i][j] = 1;
                    }
                    // Otherwise cell is still dead
                    else {
                        newBoard[i][j] = 0;
                    }
                }
                // If cell is alive...
                else if (curBoard[i][j] == 1) {
                    // 0 or 1 neighbors dies
                    if (numNeighbors < 2) {
                        newBoard[i][j] = 0;
                    }
                    // 4 or more neighbors dies
                    else if (numNeighbors > 3) {
                        newBoard[i][j] = 0;
                    }
                    // 2 or 3 neighbors survives
                    else {
                        newBoard[i][j] = 1;
                    }
                }
            }
        }

        /*
        // Print out the board state (for testing)
        cout << "Gen " << n << ": " << endl;
        for (int i = 0; i < boardSize + 2; i++) {
            for (int j = 0; j < boardSize + 2; j++) {
                cout << newBoard[i][j] << " ";
            }
            cout << endl;
        }
        */

        // Stop if there is no change between generations
        if (newBoard == curBoard) {
            break;
        }

        // Update the current board this generation
        curBoard = newBoard;
    }

    // End timer and calculate time taken
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Time taken: " << duration.count() << " ms" << endl;

    return 0;
}
