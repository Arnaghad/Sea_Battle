#include <iostream>
#include <stdlib.h>     // for rand() and srand()
#include <time.h>       // for time()
#include <algorithm>
#include <cstdlib>
#include <string>

using namespace std;
struct Ships {
    int x_head;
    int y_head;
    int size;
    int orientation;
};

const char EMPTY_CHAR = '-';
const char SHIP_CHAR = 'S';
const char HIT_CHAR = 'X';
const char MISS_CHAR = 'O';
const int MAX_SHIP_SIZE = 4;
int isVertical = -1, Side = -1, first_row, first_col, computerCol, computerRow, hit_deck;
bool Shipsunk = true;
int Turn;
int Cnt_Player_Ships = 0, Cnt_Computer_Ships = 0;
int computerHits = 0;

bool isValidInput(int x, int y, int length, int width) {
    return x >= 0 && x < length&& y >= 0 && y < width;
}

void initializeBoard(char** board, int LENGTH_SIZE, int WIDTH_SIZE) {
    for (int i = 0; i < WIDTH_SIZE; i++) {
        for (int j = 0; j < LENGTH_SIZE; j++) {
            board[i][j] = EMPTY_CHAR;
        }
    }
}

int Amount_decks(int* shipSize, int NUM_SHIPS) {
    int amount;
    amount = 0;
    for (int i = 0; i < NUM_SHIPS; i++)
        amount = amount + shipSize[i];
    return amount;
}
void printBoard(char** board, int length, int width, bool playerBoard = true) {
    cout << "   ";
    for (int i = 0; i < length; i++) {
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < width; i++) {
        cout << i << " |";
        for (int j = 0; j < length; j++) {
            if (playerBoard || board[i][j] != SHIP_CHAR) {
                cout << board[i][j] << "|";
            }
            else {
                cout << EMPTY_CHAR << "|";
            }
        }
        cout << endl;
    }
}

void placePlayerShips(char** board, int* shipSizes, int numShips, int LENGTH_SIZE, int WIDTH_SIZE, Ships* Player_Ships) {
    for (int i = 0; i < numShips; i++) {
        printBoard(board, LENGTH_SIZE, WIDTH_SIZE);
        int size = shipSizes[i];
        cout << "Placing " << size << "-deck ship. Enter the starting cell (row, col): ";
        int row, col;
        while (true) {
            cin >> row >> col;
            if (cin.fail()) {
                cout << "Invalid input data.Please enter an integer." << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                break;
            }
        }
        cout << "Enter orientation (0 for horizontal, 1 for vertical): ";
        int orientation;
        cin >> orientation;

        bool isShipPlaced = false;
        while (!isShipPlaced) {
            bool isValidPlacement = true;

            for (int j = 0; j < size; j++) {
                int y = row + (orientation == 1) * j;
                int x = col + (orientation == 0) * j;

                if (!isValidInput(x, y, LENGTH_SIZE, WIDTH_SIZE) || board[y][x] != EMPTY_CHAR) {
                    isValidPlacement = false;
                    break;
                }

                // Check for adjacent ships horizontally, vertically, and diagonally
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int nx = x + dx;
                        int ny = y + dy;

                        if (isValidInput(nx, ny, LENGTH_SIZE, WIDTH_SIZE) && board[ny][nx] != EMPTY_CHAR) {
                            isValidPlacement = false;
                            break;
                        }
                    }
                }

                if (!isValidPlacement) {
                    break;
                }
            }

            if (isValidPlacement) {
                Player_Ships[Cnt_Player_Ships].x_head = col;
                Player_Ships[Cnt_Player_Ships].y_head = row;
                Player_Ships[Cnt_Player_Ships].size = shipSizes[i];
                Player_Ships[Cnt_Player_Ships].orientation = orientation;
                Cnt_Player_Ships++;
                for (int j = 0; j < size; j++) {
                    int y = row + (orientation == 1) * j;
                    int x = col + (orientation == 0) * j;
                    board[y][x] = SHIP_CHAR;

                }
                isShipPlaced = true;
            }
            else {
                cout << "Invalid placement. Please try again." << endl;
                cout << "Enter the starting cell (row, col): ";
                cin >> row >> col;
                cout << "Enter orientation (0 for horizontal, 1 for vertical): ";
                cin >> orientation;
            }
        }
    }
}

void placeRandomShips(char** board, const int shipSizes[], int NUM_SHIPS, int flag, int LENGTH_SIZE, int WIDTH_SIZE, Ships* Player_Ships, Ships* Computer_Ships) {
    srand((unsigned)time(0) + flag);
    int cnt_attempt = 0;
    bool flStartAgain = false;
    do {
        flStartAgain = false;
        for (int i = 0; i < NUM_SHIPS; i++) {
            int row, col;
            int size_Ship = shipSizes[i];
            int orientation;
            row = rand() % WIDTH_SIZE;
            col = rand() % LENGTH_SIZE;
            orientation = rand() % 2;
            bool isShipPlaced = false;
            cnt_attempt = 0;
            while (!isShipPlaced) {
                cnt_attempt++;
                bool isValidPlacement = true;

                for (int j = 0; j < size_Ship; j++) {
                    int y = row + (orientation == 1) * j;
                    int x = col + (orientation == 0) * j;

                    if (!isValidInput(x, y, LENGTH_SIZE, WIDTH_SIZE) || board[y][x] != EMPTY_CHAR) {
                        isValidPlacement = false;
                        break;
                    }

                    // Check for adjacent ships horizontally, vertically, and diagonally
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            int nx = x + dx;
                            int ny = y + dy;

                            if (isValidInput(nx, ny, LENGTH_SIZE, WIDTH_SIZE) && board[ny][nx] != EMPTY_CHAR) {
                                isValidPlacement = false;
                                break;
                            }
                        }
                    }

                    if (!isValidPlacement) {
                        break;
                    }
                }

                if (isValidPlacement) {
                    if (Cnt_Player_Ships >= NUM_SHIPS) {
                        Computer_Ships[Cnt_Computer_Ships].x_head = col;
                        Computer_Ships[Cnt_Computer_Ships].y_head = row;
                        Computer_Ships[Cnt_Computer_Ships].size = shipSizes[i];
                        Computer_Ships[Cnt_Computer_Ships].orientation = orientation;
                        Cnt_Computer_Ships++;
                    }
                    else {
                        Player_Ships[Cnt_Player_Ships].x_head = col;
                        Player_Ships[Cnt_Player_Ships].y_head = row;
                        Player_Ships[Cnt_Player_Ships].size = shipSizes[i];
                        Player_Ships[Cnt_Player_Ships].orientation = orientation;
                        Cnt_Player_Ships++;
                    }
                    for (int j = 0; j < size_Ship; j++) {
                        int y = row + (orientation == 1) * j;
                        int x = col + (orientation == 0) * j;
                        board[y][x] = SHIP_CHAR;
                    }
                    isShipPlaced = true;

                }
                else {
                    row = rand() % WIDTH_SIZE;
                    col = rand() % LENGTH_SIZE;
                    orientation = rand() % 2;
                }
                if ((cnt_attempt >= 1000) && (isValidPlacement == false)) {
                    string answer;
                    if (Turn == 0) {
                        do {
                            cout << "The number of attempts has been exceeded 1000 times. Continue? (Y/N)" << endl;
                            cin >> answer;
                            if ((answer != "N") && (answer != "Y")) {
                                cout << "Invalid input." << endl;
                                continue;
                            }
                        } while ((answer != "N") && (answer != "Y"));
                        if (answer == "Y") {
                            initializeBoard(board, LENGTH_SIZE, WIDTH_SIZE);
                            cnt_attempt = 0;
                            Cnt_Player_Ships = 0;
                            flStartAgain = true;
                            break;
                        }
                        else {
                            cnt_attempt = 0;
                            flStartAgain = false;
                            break;
                        }
                    }
                    else {
                        answer = "Y";
                        initializeBoard(board, LENGTH_SIZE, WIDTH_SIZE);
                        Cnt_Computer_Ships = 0;
                        cnt_attempt = 0;
                        flStartAgain = true;
                        break;
                    }
                }
            }
            if (flStartAgain) {
                break;
            }
        }
    } while (flStartAgain);
}

void updateBoard(char** board, int& row, int& col, Ships Ships[], int& Num_Ships, int LENGTH_SIZE, int WIDTH_SIZE) {
    if (board[row][col] == SHIP_CHAR) {
        board[row][col] = HIT_CHAR; // Mark as hit ship
        if (Turn == 1) {
            cout << "Computer hit your ship" << endl;
            hit_deck++;
            Shipsunk = false;
        }
        else {
            cout << "You hit a ship!" << endl;
        }
        for (int i = 0; i < Num_Ships; i++) {
            int k = 0;
            if (Ships[i].orientation == 0) {
                for (int j = Ships[i].x_head; j < Ships[i].x_head + Ships[i].size; j++) {
                    ;
                    if (board[Ships[i].y_head][j] == HIT_CHAR) {
                        k++;
                    }
                }
                if (k == Ships[i].size) {
                    cout << Ships[i].size << "-deck ship was sunk" << endl;
                    for (int j = Ships[i].x_head; j < Ships[i].x_head + Ships[i].size; j++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                if ((Ships[i].y_head + dy < WIDTH_SIZE) && (j + dx < LENGTH_SIZE) && (Ships[i].y_head + dy > -1) && (j + dx > -1)) {
                                    if (board[Ships[i].y_head + dy][j + dx] == EMPTY_CHAR) {
                                        board[Ships[i].y_head + dy][j + dx] = MISS_CHAR;
                                    }
                                }
                            }
                        }
                    }
                    Ships[i].x_head = Ships[Num_Ships - 1].x_head;
                    Ships[i].y_head = Ships[Num_Ships - 1].y_head;
                    Ships[i].size = Ships[Num_Ships - 1].size;
                    Ships[i].orientation = Ships[Num_Ships - 1].orientation;
                    Num_Ships--;
                    if (Turn == 1) {
                        Shipsunk = true;
                        isVertical = -1;
                        Side = -1;
                        computerHits++;
                    }
                    break;
                }
            }
            else {
                for (int j = Ships[i].y_head; j < Ships[i].y_head + Ships[i].size; j++) {
                    if (board[j][Ships[i].x_head] == HIT_CHAR) {
                        k++;
                    }
                }
                if (k == Ships[i].size) {
                    cout << Ships[i].size << "-deck ship was sunk" << endl;
                    for (int j = Ships[i].y_head; j < Ships[i].y_head + Ships[i].size; j++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            for (int dy = -1; dy <= 1; dy++) {
                                if ((Ships[i].x_head + dx < LENGTH_SIZE) && (j + dy < WIDTH_SIZE) && (Ships[i].x_head + dx > -1) && (j + dy > -1)) {
                                    if (board[j + dy][Ships[i].x_head + dx] == EMPTY_CHAR) {
                                        board[j + dy][Ships[i].x_head + dx] = MISS_CHAR;
                                    }
                                }
                            }
                        }
                    }
                    Ships[i].x_head = Ships[Num_Ships - 1].x_head;
                    Ships[i].y_head = Ships[Num_Ships - 1].y_head;
                    Ships[i].size = Ships[Num_Ships - 1].size;
                    Ships[i].orientation = Ships[Num_Ships - 1].orientation;
                    Num_Ships--;
                    if (Turn == 1) {
                        Shipsunk = true;
                        isVertical = -1;
                        Side = -1;

                    }
                    break;
                }

            }
        }
    }
    else {
        board[row][col] = MISS_CHAR;// Mark as miss
        if (Turn == 1) {
            cout << "Computer miss!" << endl;
        }
        else {
            cout << "You miss!" << endl;
        }
    }
}

void Computerturn(char** Player_board, int LENGTH_SIZE, int WIDTH_SIZE, Ships* Player_Ships) {
    int Previous_orientation, Previous_computerRow, Previous_computerCol;
    srand((unsigned)time(NULL) + 2);
    if (Shipsunk == true) {
        hit_deck = 0;
        do {
            computerRow = rand() % WIDTH_SIZE;
            computerCol = rand() % LENGTH_SIZE;
        } while (Player_board[computerRow][computerCol] == HIT_CHAR || Player_board[computerRow][computerCol] == MISS_CHAR);
        cout << computerRow << " " << computerCol << endl;
        updateBoard(Player_board, computerRow, computerCol, Player_Ships, Cnt_Player_Ships, LENGTH_SIZE, WIDTH_SIZE);
        cout << "Player's Board:" << endl;
        printBoard(Player_board, LENGTH_SIZE, WIDTH_SIZE, true);
    }
    else {
        if ((isVertical == -1) && (Side == -1)) {
            isVertical = rand() % 2;
            Side = rand() % 2;
            first_col = computerCol;
            first_row = computerRow;
        }
        Previous_computerCol = computerCol;
        Previous_computerRow = computerRow;
        do {
            computerCol = Previous_computerCol;
            computerRow = Previous_computerRow;
            if ((isVertical == 0) && (computerCol < LENGTH_SIZE) && (computerCol > -1)) {

                if (((Side == 0) && (computerCol > 0)) || (computerCol == LENGTH_SIZE - 1)) {
                    computerCol--;
                }
                else {
                    if (((Side == 1) && (computerCol < LENGTH_SIZE - 1)) || (computerCol == 0)) {
                        computerCol++;
                    }
                }
            }
            else {
                if (((Side == 0) && (computerRow > 0)) || (computerRow == WIDTH_SIZE - 1)) {
                    computerRow--;
                }
                else {
                    if (((Side == 1) && (computerRow < WIDTH_SIZE - 1)) || (computerRow == 0)) {
                        computerRow++;
                    }
                }
            }
            if (((Player_board[computerRow][computerCol] == MISS_CHAR) || ((computerRow < 0) || (computerRow > WIDTH_SIZE - 1) || (computerCol < 0) || (computerCol > LENGTH_SIZE - 1))) && (hit_deck == 1)) {
                isVertical = rand() % 2;
                Side = rand() % 2;
            }
            if (((Player_board[computerRow][computerCol] == MISS_CHAR) || ((computerRow < 0) || (computerRow > WIDTH_SIZE - 1) || (computerCol < 0) || (computerCol > LENGTH_SIZE - 1))) && (hit_deck > 1)) {
                Side = 1 - Side;
                computerCol = first_col;
                computerRow = first_row;
            }
            if (Player_board[computerRow][computerCol] == HIT_CHAR) {
                computerCol = first_col;
                computerRow = first_row;
                if (isVertical == 0) {
                    computerCol += (Side == 1) ? 1 : -1;
                }
                else {
                    computerRow += (Side == 1) ? 1 : -1;
                }
            }
        } while ((Player_board[computerRow][computerCol] == MISS_CHAR) || ((computerRow < 0) || (computerRow > WIDTH_SIZE - 1) || (computerCol < 0) || (computerCol > LENGTH_SIZE - 1)));
        if ((Player_board[computerRow][computerCol] == SHIP_CHAR) && (Shipsunk == false)) {
            updateBoard(Player_board, computerRow, computerCol, Player_Ships, Cnt_Player_Ships, LENGTH_SIZE, WIDTH_SIZE);
            cout << computerRow << " " << computerCol << endl;
            if (Player_board[computerRow][computerCol] == HIT_CHAR) {
                hit_deck++;
                cout << "Player's Board:" << endl;
                printBoard(Player_board, LENGTH_SIZE, WIDTH_SIZE, true);
                if ((((computerCol == 0) || (computerCol == LENGTH_SIZE - 1)) && (isVertical == 0)) || (((computerRow == 0) || (computerRow == WIDTH_SIZE - 1)) && (isVertical == 1))) {
                    Side = 1 - Side;
                    computerCol = first_col;
                    computerRow = first_row;
                }
            }
        }
        else {
            updateBoard(Player_board, computerRow, computerCol, Player_Ships, Cnt_Player_Ships, LENGTH_SIZE, WIDTH_SIZE);
            cout << computerRow << " " << computerCol << endl;
            if (hit_deck == 1) {
                Previous_orientation = isVertical;
                isVertical = rand() % 2;
                if ((isVertical == Previous_orientation)) {
                    Side = 1 - Side;
                }
                else {
                    Side = rand() % 2;
                }
                computerCol = first_col;
                computerRow = first_row;
            }
            else {
                computerCol = first_col;
                computerRow = first_row;
                Side = 1 - Side;
            }
            cout << "Player's Board:" << endl;
            printBoard(Player_board, LENGTH_SIZE, WIDTH_SIZE, true);
        }
    }
}

void playGame(char** playerBoard, char** computerBoard, int shipSizes[], int numDecks, int LENGTH_SIZE, int WIDTH_SIZE, Ships* Player_Ships, Ships* Computer_Ships) {

    while (Cnt_Computer_Ships != 0 && Cnt_Player_Ships != 0) {
        // player's turn
        system("cls");
        cout << "Your turn!" << endl;
        Turn = 0;
        // print player's board
        cout << "Player's board:" << endl;
        printBoard(playerBoard, LENGTH_SIZE, WIDTH_SIZE, true);
        // print computer's board
        cout << "Computer's board:" << endl;
        printBoard(computerBoard, LENGTH_SIZE, WIDTH_SIZE, false);
        int row, col;
        cout << "Enter the cell to attack (row, col): ";
        cin >> row >> col;
        system("cls");
        if (cin.fail()) {
            cout << "Invalid input data. Please enter an integer." << endl;
            cin.clear();// clear the error flag
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getchar();
            continue;
        }
        if (!isValidInput(col, row, LENGTH_SIZE, WIDTH_SIZE)) {
            cout << "Invalid input. Please try again." << endl;
            cin.ignore();
            getchar();
            system("cls");
            continue;
        }
        if (computerBoard[row][col] == SHIP_CHAR) {
            updateBoard(computerBoard, row, col, Computer_Ships, Cnt_Computer_Ships, LENGTH_SIZE, WIDTH_SIZE);
            cout << "Player's board:" << endl;
            printBoard(playerBoard, LENGTH_SIZE, WIDTH_SIZE, true);
            cout << "Computer's board:" << endl;
            printBoard(computerBoard, LENGTH_SIZE, WIDTH_SIZE, false);
        }
        else if (computerBoard[row][col] == HIT_CHAR || computerBoard[row][col] == MISS_CHAR) {
            cout << "You already attacked that cell. Please try again..." << endl;
            cin.ignore();
            getchar();
            continue;
        }
        else {
            updateBoard(computerBoard, row, col, Computer_Ships, Cnt_Computer_Ships, LENGTH_SIZE, WIDTH_SIZE);
            cout << "Player's board:" << endl;
            printBoard(playerBoard, LENGTH_SIZE, WIDTH_SIZE, true);
            cout << "Computer's board:" << endl;
            printBoard(computerBoard, LENGTH_SIZE, WIDTH_SIZE, false);
        }
        if (Cnt_Computer_Ships == 0) {
            cout << "Congratulations! You won!" << endl;
        }
        else {
            cout << "Press Enter to continue..." << endl;
            cin.ignore();
            getchar();
            system("cls");
            // computer's turn
            cout << "Computer's turn!" << endl;
            Turn = 1;
            Computerturn(playerBoard, LENGTH_SIZE, WIDTH_SIZE, Player_Ships);
            cout << Cnt_Player_Ships << endl;
            cout << "Press Enter to continue..." << endl;
            cin.ignore();
            getchar();
        }
        if (Cnt_Player_Ships == 0) {
            cout << "Computer win!!!" << endl;
            break;
        }
    }
}

int main() {
    int LENGTH_SIZE, WIDTH_SIZE;
    int NUM_SHIPS;
    int i;
    int* shipSizes;
    string answer;
    int amount;
    int sum = 0;
    cout << "Welcome to Sea Battle game!" << endl;
    do {
        cout << "Do you want classic or custom game? (CL/CU)" << endl;
        cin >> answer;
        if (answer == "CL") {
            LENGTH_SIZE = 10;
            WIDTH_SIZE = 10;
            NUM_SHIPS = 10;
        }
        else {
            if (answer == "CU") {
                cout << "Please, write length of board" << endl;
                cin >> LENGTH_SIZE;
                cout << "Please, write width of board" << endl;
                cin >> WIDTH_SIZE;
                cout << "Please, write amount of ships" << endl;
                cin >> NUM_SHIPS;
               
                if (sum >= (LENGTH_SIZE * WIDTH_SIZE) / 2) {
                    cout << LENGTH_SIZE * WIDTH_SIZE << " " << sum << endl;
                    cout << "Invalid input. Too many ships" << endl;
                    answer = "";
                    continue;
                }

            }
            else {
                cout << "Invalid input. Please write CL or CU" << endl;
                continue;
            }
        }

    } while ((answer != "CL") && (answer != "CU"));
    shipSizes = new int[NUM_SHIPS];
    if (answer == "CU") {
        cout << "Please, write size of ships" << endl;
        i = 0;
        while (i < NUM_SHIPS) {
            cin >> shipSizes[i];
            if (shipSizes[i] > max(LENGTH_SIZE, WIDTH_SIZE)) {
                cout << "Invalid input. Size of the ship bigger then size of the board" << endl;
            }
            else {
                sum = sum + shipSizes[i];
                i++;
            }
        }
    }
    else {
        shipSizes = new int[NUM_SHIPS] { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    }
    char** playerBoard = new char* [WIDTH_SIZE];
    for (int i = 0; i < WIDTH_SIZE; i++)
        playerBoard[i] = new char[LENGTH_SIZE];
    char** computerBoard = new char* [WIDTH_SIZE];
    for (int i = 0; i < WIDTH_SIZE; i++)
        computerBoard[i] = new char[LENGTH_SIZE];
    initializeBoard(playerBoard, LENGTH_SIZE, WIDTH_SIZE);
    initializeBoard(computerBoard, LENGTH_SIZE, WIDTH_SIZE);

    // place player's ships
    Ships* Player_Ships = new Ships[NUM_SHIPS];
    Ships* Computer_Ships = new Ships[NUM_SHIPS];

    amount = Amount_decks(shipSizes, NUM_SHIPS);
    cout << "Do you want to place manually or automatically (M/A)?" << endl;
    do {
        cin >> answer;
        if (answer == "M") {
            cout << "Place your ships on the board:" << endl;
            placePlayerShips(playerBoard, shipSizes, NUM_SHIPS, LENGTH_SIZE, WIDTH_SIZE, Player_Ships);
        }
        else {
            if (answer == "A") {
                cout << "Just a second..." << endl;
                Turn = 0;
                placeRandomShips(playerBoard, shipSizes, NUM_SHIPS, 0, LENGTH_SIZE, WIDTH_SIZE, Player_Ships, Computer_Ships);
            }
            else {
                cout << "Invalid input. Please write M or A" << endl;
                continue;
            }
        }
    } while ((answer != "M") && (answer != "A"));
    // place computer's ships
    cout << "Computer is placing its ships on the board..." << endl;
    Turn = 1;
    placeRandomShips(computerBoard, shipSizes, NUM_SHIPS, 1, LENGTH_SIZE, WIDTH_SIZE, Player_Ships, Computer_Ships);
    playGame(playerBoard, computerBoard, shipSizes, amount, LENGTH_SIZE, WIDTH_SIZE, Player_Ships, Computer_Ships);
    return 0;
}
