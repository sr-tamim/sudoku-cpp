/**
 * @file sudoku.cpp
 * @brief Sudoku game implementation in C++
 *
 * This program allows the user to play the game of Sudoku. It provides a
 * command-line interface where the user can input their moves and see the
 * current state of the Sudoku board. The program creates a random Sudoku board every time.
 *
 * @date 26-Nov-2023
 * @version 1.0
 * @authors
 * - SR Tamim <https://sr-tamim.vercel.app>
 *
 * @section LICENSE
 * This program is licensed under MIT License. See the LICENSE file in the root of this repository for details.
 *
 * @section Dependencies
 * - iostream
 * - vector
 * - cstdlib
 * - ctime
 *
 * @section NOTES
 * This program is tested on Windows 11 using MinGW compiler.
 *
 * @section Usage
 * - Compile the program using a C++ compiler (g++, clang++, etc)
 * - Run the executable file
 */

#include <iostream>   // for input and output
#include <vector>     // for dynamic arrays
#include <cstdlib>    // for system function like cls to clear the screen
#include <ctime>      // for time function
#include <cmath>      // for math functions like rand function

#define N 9             // Size of the board
#define MINI_BOX_SIZE 3 // Size of the mini box 3x3
#define EASY_LVL 13     // Number of empty cells for easy level
#define MEDIUM_LVL 29   // Number of empty cells for medium level
#define HARD_LVL 41     // Number of empty cells for hard level

class SudokuBoard {
public:
    SudokuBoard() {
        solved.resize(N, std::vector<int>(N, 0));
        unsolved.resize(N, std::vector<int>(N, 0));
        emptyCells = 0;
    }

    void clearScreen();
    int randomGenerator(int num);
    bool checkIfSafe(int i, int j, int num);
    bool isAbsentInBox(int rowStart, int colStart, int num);
    bool isAbsentInRow(int i, int num);
    bool isAbsentInCol(int j, int num);
    void fillValues();
    void fillDiagonal();
    void fillBox(int row, int col);
    bool fillRemaining(int i, int j);
    void addEmptyCells();
    void printSudoku();
    bool isBoardSolved();
    void resetBoard();

    int emptyCells;

private:
    std::vector<std::vector<int>> solved;
    std::vector<std::vector<int>> unsolved;
};

// Function to clear the screen
void SudokuBoard::clearScreen() {
    std::system("cls");
}

// Random number generator
int SudokuBoard::randomGenerator(int num) {
    return (std::rand() % num) + 1;
}

// Check if it is safe to put the number in a specific cell
bool SudokuBoard::checkIfSafe(int i, int j, int num) {
    return (isAbsentInRow(i, num) && isAbsentInCol(j, num) && isAbsentInBox(i - i % MINI_BOX_SIZE, j - j % MINI_BOX_SIZE, num));
}

// Check if the number is absent in the 3x3 box
bool SudokuBoard::isAbsentInBox(int rowStart, int colStart, int num) {
    for (int i = 0; i < MINI_BOX_SIZE; i++) {
        for (int j = 0; j < MINI_BOX_SIZE; j++) {
            if (unsolved[rowStart + i][colStart + j] == num) {
                return false;
            }
        }
    }
    return true;
}

// Check if the number is absent in the row
bool SudokuBoard::isAbsentInRow(int i, int num) {
    for (int j = 0; j < N; j++) {
        if (unsolved[i][j] == num) {
            return false;
        }
    }
    return true;
}

// Check if the number is absent in the column
bool SudokuBoard::isAbsentInCol(int j, int num) {
    for (int i = 0; i < N; i++) {
        if (unsolved[i][j] == num) {
            return false;
        }
    }
    return true;
}

// Fill the board with values
void SudokuBoard::fillValues() {
    fillDiagonal(); // Fill the diagonal MINI_BOX_SIZE x MINI_BOX_SIZE matrices
    fillRemaining(0, MINI_BOX_SIZE); // Fill remaining blocks

    // Copy the unsolved board to solved
    solved = unsolved;

    addEmptyCells(); // Remove the K no. of digits from the board
}

// Fill the diagonal MINI_BOX_SIZE number of MINI_BOX_SIZE x MINI_BOX_SIZE matrices
void SudokuBoard::fillDiagonal() {
    for (int i = 0; i < N; i += MINI_BOX_SIZE) {
        fillBox(i, i); // Fill a 3 x 3 matrix
    }
}

// Fill a 3 x 3 matrix
void SudokuBoard::fillBox(int row, int col) {
    int num;
    for (int i = 0; i < MINI_BOX_SIZE; i++) {
        for (int j = 0; j < MINI_BOX_SIZE; j++) {
            do {
                num = randomGenerator(N);
            } while (!isAbsentInBox(row, col, num));
            unsolved[row + i][col + j] = num;
        }
    }
}

// Fill the remaining cells recursively
bool SudokuBoard::fillRemaining(int i, int j) {
    if (j >= N && i < N - 1) {
        i++;
        j = 0;
    }
    if (i >= N && j >= N) {
        return true;
    }
    if (i < MINI_BOX_SIZE) {
        if (j < MINI_BOX_SIZE) {
            j = MINI_BOX_SIZE;
        }
    } else if (i < N - MINI_BOX_SIZE) {
        if (j == (i / MINI_BOX_SIZE) * MINI_BOX_SIZE) {
            j += MINI_BOX_SIZE;
        }
    } else {
        if (j == N - MINI_BOX_SIZE) {
            i++;
            j = 0;
            if (i >= N) {
                return true;
            }
        }
    }

    for (int num = 1; num <= N; num++) {
        if (checkIfSafe(i, j, num)) {
            unsolved[i][j] = num;
            if (fillRemaining(i, j + 1)) {
                return true;
            }
            unsolved[i][j] = 0;
        }
    }
    return false;
}

// Remove some digits from the board to create empty cells
void SudokuBoard::addEmptyCells() {
    int count = emptyCells;
    while (count != 0) {
        int cellId = randomGenerator(N * N) - 1;
        int i = cellId / N;
        int j = cellId % N;

        if (unsolved[i][j] != 0) {
            count--;
            unsolved[i][j] = 0;
        }
    }
}

// Print Sudoku board
void SudokuBoard::printSudoku() {
    std::cout << "   ";
    for (int i = 1; i <= N; i++) {
        std::cout << " " << i << " ";
    }
    std::cout << std::endl;
    
    for (int i = 0; i < N; i++) {
        std::cout << (i + 1) << " ";
        for (int j = 0; j < N; j++) {
            if (j % MINI_BOX_SIZE == 0) std::cout << "|";
            if (unsolved[i][j] == 0)
                std::cout << " . ";
            else
                std::cout << " " << unsolved[i][j] << " ";
        }
        std::cout << "|" << std::endl;
        if ((i + 1) % MINI_BOX_SIZE == 0) {
            std::cout << "  ";
            for (int k = 0; k < N + 2 * MINI_BOX_SIZE; k++) {
                std::cout << "-";
            }
            std::cout << std::endl;
        }
    }
}

// Check if the board is solved
bool SudokuBoard::isBoardSolved() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (unsolved[i][j] == 0 || unsolved[i][j] != solved[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Reset the board to all 0s
void SudokuBoard::resetBoard() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            unsolved[i][j] = 0;
        }
    }
}

int main() {
    SudokuBoard board;
    
    while (true) {
        std::srand(static_cast<unsigned int>(std::time(0)));

        board.clearScreen();
        std::cout << "Welcome to Sudoku!\n\n";

        // Ask for difficulty level
        std::cout << "Choose difficulty level:\n";
        std::cout << "1. Easy\n";
        std::cout << "2. Medium\n";
        std::cout << "3. Hard\n";
        std::cout << "Your choice: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                board.emptyCells = EASY_LVL;
                break;
            case 2:
                board.emptyCells = MEDIUM_LVL;
                break;
            case 3:
                board.emptyCells = HARD_LVL;
                break;
            default:
                std::cout << "Invalid choice! Defaulting to Easy level.\n";
                board.emptyCells = EASY_LVL;
                break;
        }

        board.resetBoard();
        board.fillValues();

        while (!board.isBoardSolved()) {
            board.clearScreen();
            board.printSudoku();

            int row, col, val;
            std::cout << "\nEnter row (1-9), column (1-9), and value (1-9) to fill (or 0 0 0 to quit): ";
            std::cin >> row >> col >> val;

            if (row == 0 && col == 0 && val == 0) {
                std::cout << "Thanks for playing! Goodbye.\n";
                return 0;
            }

            if (row < 1 || row > N || col < 1 || col > N || val < 1 || val > N) {
                std::cout << "Invalid input! Try again.\n";
                std::system("pause");
                continue;
            }

            if (board.unsolved[row - 1][col - 1] != 0) {
                std::cout << "Cell is already filled! Try another one.\n";
                std::system("pause");
                continue;
            }

            board.unsolved[row - 1][col - 1] = val;

            if (board.isBoardSolved()) {
                board.clearScreen();
                board.printSudoku();
                std::cout << "Congratulations! You've solved the Sudoku puzzle!\n";
                std::system("pause");
                break;
            }
        }
    }

    return 0;
}
