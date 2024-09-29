#include <iostream> // for input and output
#include <vector>   // for dynamic arrays
#include <cstdlib>  // for system function like cls to clear the screen
#include <ctime>    // for time function
#include <cmath>    // for math functions like rand function

#define N 9             // Size of the board
#define MINI_BOX_SIZE 3 // Size of the mini box 3x3
#define EASY_LVL 13     // Number of empty cells for easy level
#define MEDIUM_LVL 29   // Number of empty cells for medium level
#define HARD_LVL 41     // Number of empty cells for hard level

using namespace std;

// Function to clear the screen
void clearScreen()
{
    system("cls");
}

class SudokuBoard
{
public:
    SudokuBoard()
    {
        solved.resize(N, vector<int>(N, 0));
        unsolved.resize(N, vector<int>(N, 0));
        emptyCells = 0;
    }

    int emptyCells;

    vector<vector<int>> solved;
    vector<vector<int>> unsolved;

    // Random number generator
    int randomGenerator(int num)
    {
        // num is the max limit of generated number
        return (rand() % num) + 1;
    }

    // Check if it is safe to put the number in a specific cell
    bool checkIfSafe(int i, int j, int num)
    {
        // first check the row, then check the column
        // then check the mini box
        return (isAbsentInRow(i, num) && isAbsentInCol(j, num) && isAbsentInBox(i - i % MINI_BOX_SIZE, j - j % MINI_BOX_SIZE, num));
    }

    // Check if the number is absent in the 3x3 box
    bool isAbsentInBox(int rowStart, int colStart, int num)
    {
        for (int i = 0; i < MINI_BOX_SIZE; i++)
        {
            for (int j = 0; j < MINI_BOX_SIZE; j++)
            {
                if (unsolved[rowStart + i][colStart + j] == num)
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Check if the number is absent in the row
    bool isAbsentInRow(int i, int num)
    {
        for (int j = 0; j < N; j++)
        {
            if (unsolved[i][j] == num)
            {
                return false;
            }
        }
        return true;
    }

    // Check if the number is absent in the column
    bool isAbsentInCol(int j, int num)
    {
        for (int i = 0; i < N; i++)
        {
            if (unsolved[i][j] == num)
            {
                return false;
            }
        }
        return true;
    }

    // Fill the board with values
    void fillValues()
    {
        // To improve the efficiency we would fill the diagonal mini boxes first

        fillDiagonal();                  // Fill the diagonal MINI_BOX_SIZE x MINI_BOX_SIZE matrices
        fillRemaining(0, MINI_BOX_SIZE); // Fill remaining blocks

        // Copy the unsolved board to solved
        solved = unsolved;

        // unsolved board is fully filled now, create empty cells in the board
        addEmptyCells(); // Remove the K no. of digits from the board
    }

    // Fill the diagonal MINI_BOX_SIZE number of MINI_BOX_SIZE x MINI_BOX_SIZE matrices
    void fillDiagonal()
    {
        for (int i = 0; i < N; i += MINI_BOX_SIZE)
        {
            fillBox(i, i); // Fill a 3 x 3 matrix
        }
    }

    // Fill a 3 x 3 matrix
    void fillBox(int row, int col)
    {
        int num;
        for (int i = 0; i < MINI_BOX_SIZE; i++)
        {
            for (int j = 0; j < MINI_BOX_SIZE; j++)
            {
                do
                {
                    num = randomGenerator(N);
                } while (!isAbsentInBox(row, col, num));
                unsolved[row + i][col + j] = num;
            }
        }
    }

    // Fill the remaining cells recursively
    bool fillRemaining(int i, int j)
    {
        if (j >= N && i < N - 1)
        {
            i++;
            j = 0;
        }
        if (i >= N && j >= N)
        {
            return true;
        }
        if (i < MINI_BOX_SIZE)
        {
            if (j < MINI_BOX_SIZE)
            {
                j = MINI_BOX_SIZE;
            }
        }
        else if (i < N - MINI_BOX_SIZE)
        {
            if (j == (i / MINI_BOX_SIZE) * MINI_BOX_SIZE)
            {
                j += MINI_BOX_SIZE;
            }
        }
        else
        {
            if (j == N - MINI_BOX_SIZE)
            {
                i++;
                j = 0;
                if (i >= N)
                {
                    return true;
                }
            }
        }

        for (int num = 1; num <= N; num++)
        {
            if (checkIfSafe(i, j, num))
            {
                unsolved[i][j] = num;
                if (fillRemaining(i, j + 1))
                {
                    return true;
                }
                unsolved[i][j] = 0;
            }
        }
        return false;
    }

    // Remove some digits from the board to create empty cells
    void addEmptyCells()
    {
        int count = emptyCells;
        while (count != 0)
        {
            int cellId = randomGenerator(N * N) - 1;
            int i = cellId / N;
            int j = cellId % N;

            if (unsolved[i][j] != 0)
            {
                count--;
                unsolved[i][j] = 0;
            }
        }
    }

    // Print Sudoku board
    void printSudoku()
    {
        cout << "  X";
        for (int i = 1; i <= N; i++)
        {
            cout << " " << i << " ";
            if (i % MINI_BOX_SIZE == 0)
                cout << " ";
        }
        cout << endl;
        cout << "Y  ";
        for (int k = 0; k < N + 2 * MINI_BOX_SIZE; k++)
        {
            cout << "--";
        }
        cout << endl;

        for (int i = 0; i < N; i++)
        {
            cout << (i + 1) << " ";
            for (int j = 0; j < N; j++)
            {
                if (j % MINI_BOX_SIZE == 0)
                    cout << "|";
                if (unsolved[i][j] == 0)
                    cout << " . ";
                else
                    cout << " " << unsolved[i][j] << " ";
            }
            cout << "|" << endl;
            if ((i + 1) % MINI_BOX_SIZE == 0)
            {
                cout << "   ";
                for (int k = 0; k < N + 2 * MINI_BOX_SIZE; k++)
                {
                    cout << "--";
                }
                cout << endl;
            }
        }
    }

    // Check if the board is solved
    bool isBoardSolved()
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (unsolved[i][j] == 0 || unsolved[i][j] != solved[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Reset the board to all 0s
    void resetBoard()
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                unsolved[i][j] = 0;
            }
        }
    }
};

void howToPlay()
{
    clearScreen();
    cout << "==== How to Play ====\n\n";
    cout << "Sudoku is a logic-based, combinatorial number-placement puzzle.\n\n";
    cout << "The objective is to fill a 9x9 grid with digits so that each column, each row, and each of the nine 3x3 subgrids that compose the grid contain all of the digits from 1 to 9.\n\n";
    cout << "The puzzle setter provides a partially completed grid, which for a well-posed puzzle has a single solution.\n";
    cout << "Completed puzzles are always a type of Latin square with an additional constraint on the contents of individual regions.\n\n";
    cout << "For more information, visit: https://en.wikipedia.org/wiki/Sudoku \n\n";
    system("pause");
}

void aboutDevelopers()
{
    clearScreen();
    cout << "==== About Developers ====\n\n";
    cout << "Developed by: \n";
    cout << "1. SR Tamim - ID: 41230201087\n";
    cout << "2. Mahatab Hossain - ID: 41230201189\n";
    cout << "3. Tousif Mahabub - ID: 41230201026\n";
    system("pause");
}

int main()
{
    SudokuBoard board;
    cout << "Welcome to Sudoku!\n\n";
    system("pause");

HomeScreen:
    clearScreen();
    cout << "==== Main Menu ====\n\n";
    cout << "1. Start Game\n";
    cout << "2. How to Play\n";
    cout << "3. About Developers\n";
    cout << "4. Exit\n\n";
    cout << "Your choice: ";
    int choice;
    cin >> choice;
    switch (choice)
    {
    case 1:
        goto StartGame;
        break;
    case 2:
        howToPlay();
        goto HomeScreen;
        break;
    case 3:
        aboutDevelopers();
        goto HomeScreen;
        break;
    case 4:
        goto ExitGame;
        break;
    default:
        cout << "Invalid choice! Try again.\n";
        system("pause");
        goto HomeScreen;
    }

StartGame:
    while (true)
    {
        srand(static_cast<unsigned int>(time(0)));

        clearScreen();

        // Ask for difficulty level
        cout << "Choose difficulty level:\n";
        cout << "1. Easy\n";
        cout << "2. Medium\n";
        cout << "3. Hard\n";
        cout << "Your choice: ";
        int choice;
        cin >> choice;

        switch (choice)
        {
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
            cout << "Invalid choice! Defaulting to Easy level.\n";
            board.emptyCells = EASY_LVL;
            break;
        }

        board.resetBoard();
        board.fillValues();

        while (!board.isBoardSolved())
        {
            clearScreen();
            board.printSudoku();

            int row, col, val;
            cout << "\nEnter row (1-9) (or 0 to quit): ";
            cin >> row;

            if (row == 0)
                goto HomeScreen;

            cout << "Enter column (1-9) (or 0 to quit): ";
            cin >> col;

            if (col == 0)
                goto HomeScreen;

            if (board.unsolved[row - 1][col - 1] != 0)
            {
                cout << "Cell is already filled! Try another one.\n";
                system("pause");
                continue;
            }

            cout << "Enter value (1-9) (or 0 to quit): ";
            cin >> val;

            if (val == 0)
                goto HomeScreen;

            if (row < 1 || row > N || col < 1 || col > N || val < 1 || val > N)
            {
                cout << "Invalid input! Try again.\n";
                system("pause");
                continue;
            }
            board.unsolved[row - 1][col - 1] = val;

            if (board.isBoardSolved())
            {
                clearScreen();
                board.printSudoku();
                cout << "\n\nCongratulations! You've solved the Sudoku puzzle!\n\n\n";
                system("pause");
                break;
            }
        }

        goto HomeScreen;

    ExitGame:
        cout << "Thanks for playing! Goodbye.\n";
        return 0;
    }

    return 0;
}
