#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <conio.h> // getch() dla Windows.

static const char WALL  = '#';         
static const char BOX   = '$';          
static const char GOAL  = '.';          
static const char PLAYER = '@';         
static const char PLAYER_ON_GOAL = '+'; 
static const char BOX_ON_GOAL = '*';    
static const char FLOOR = ' ';          

struct Position {
    int row;
    int col;
};

class Sokoban {
private:
    std::vector<std::string> board; // Plansza gry
    Position playerPos;             // Pozycja gracza
    int rows;                       // Liczba wierszy planszy
    int cols;                       // Liczba kolumn planszy

public:
    Sokoban(const std::string& filename) {
        std::ifstream inFile(filename);
        if(!inFile.is_open()) {
            std::cerr << "Nie mozna otworzyc pliku: " << filename << std::endl;
            exit(1);
        }

        std::string line;
        while(std::getline(inFile, line)) {
            board.push_back(line);
        }
        inFile.close();

        rows = board.size();
        cols = rows > 0 ? board[0].size() : 0;

        for(int r = 0; r < rows; ++r) {
            for(int c = 0; c < cols; ++c) {
                if(board[r][c] == PLAYER || board[r][c] == PLAYER_ON_GOAL) {
                    playerPos = {r, c};
                }
            }
        }
    }

    void printBoard() {
        system("cls"); 
        for(const auto& line : board) {
            std::cout << line << std::endl;
        }
        std::cout << "\n Sterowanie: W/S/A/D lub strzalki; Q - wyjscie \n";
    }

    bool isCompleted() {
        for(int r = 0; r < rows; ++r) {
            for(int c = 0; c < cols; ++c) {
                if(board[r][c] == GOAL) {
                    return false; 
                }
                if(board[r][c] == PLAYER_ON_GOAL) {
                    return false; 
                }
            }
        }
        return true; 
    }

    void movePlayer(int dx, int dy) {
        int newRow = playerPos.row + dx;
        int newCol = playerPos.col + dy;

        if(newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) { // Sprawdzamy czy nie wychodzimy poza plansze
            return; 
        }

        char destination = board[newRow][newCol]; // Sprawdzamy co jest na polu docelowym 
        char playerSymbol = board[playerPos.row][playerPos.col] == PLAYER_ON_GOAL ? PLAYER_ON_GOAL : PLAYER;

        if(destination == WALL) {
            return; 
        }
        else if(destination == FLOOR || destination == GOAL) {
            board[playerPos.row][playerPos.col] = (playerSymbol == PLAYER_ON_GOAL) ? GOAL : FLOOR;
            board[newRow][newCol] = (destination == GOAL) ? PLAYER_ON_GOAL : PLAYER;
            playerPos = {newRow, newCol};
        }
        else if(destination == BOX || destination == BOX_ON_GOAL) {
            int boxNewRow = newRow + dx;
            int boxNewCol = newCol + dy;

            if(boxNewRow < 0 || boxNewRow >= rows || boxNewCol < 0 || boxNewCol >= cols) { // Sprawdzamy czy skrzynia nie wydzie poza plansze
                return; 
            }

            char boxDestination = board[boxNewRow][boxNewCol];

            if(boxDestination == FLOOR || boxDestination == GOAL) {
                board[boxNewRow][boxNewCol] = (boxDestination == GOAL) ? BOX_ON_GOAL : BOX;
                board[newRow][newCol] = (destination == BOX_ON_GOAL) ? PLAYER_ON_GOAL : PLAYER;
                board[playerPos.row][playerPos.col] = (playerSymbol == PLAYER_ON_GOAL) ? GOAL : FLOOR;
                playerPos = {newRow, newCol};
            }
        }
    }

   void runGame() {

    printBoard();

    while (true) {
        if (isCompleted()) {
            std::cout << "Gratulacje! Poziom ukonczony!\n";
            break;
        }

        int ch = _getch();
        if (ch == 'q' || ch == 'Q') {
            std::cout << "Koniec gry\n";
            break;
        }

        if (ch == 224) {
            ch = _getch();
            if (ch == 72) {
                movePlayer(-1, 0); 
            } else if (ch == 80) {
                movePlayer(1, 0);  
            } else if (ch == 75) {
                movePlayer(0, -1); 
            } else if (ch == 77) {
                movePlayer(0, 1);  
            }
        } else {
            if (ch == 'w' || ch == 'W') {
                movePlayer(-1, 0); 
            } else if (ch == 's' || ch == 'S') {
                movePlayer(1, 0);  
            } else if (ch == 'a' || ch == 'A') {
                movePlayer(0, -1); 
            } else if (ch == 'd' || ch == 'D') {
                movePlayer(0, 1);  
            }
        }

        printBoard();
    }
}

};

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cerr << "Uzycie: " << argv[0] << " <nazwa_pliku_z_poziomem>\n";
        return 1;
    }

    std::string levelFile = argv[1];
    Sokoban game(levelFile);
    game.runGame();

    return 0;
}
