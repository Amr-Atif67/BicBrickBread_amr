#include "PyramidXO.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;



PyramidXO_Board::PyramidXO_Board() : Board<char>(3, 5) {
    n_moves = 0;
}

bool PyramidXO_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    // Check bounds
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }

   
    bool valid_shape = false;
    if (x == 0 && y == 2) valid_shape = true;
    else if (x == 1 && (y >= 1 && y <= 3)) valid_shape = true;
    else if (x == 2 && (y >= 0 && y <= 4)) valid_shape = true;

    if (!valid_shape) {
        return false;
    }

    
    if (board[x][y] != 0) {
        return false;
    }

    board[x][y] = symbol;
    n_moves++;
    return true;
}

bool PyramidXO_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();
   
    if (board[1][1] == s && board[1][2] == s && board[1][3] == s) return true;
    
    if (board[2][0] == s && board[2][1] == s && board[2][2] == s) return true;
    if (board[2][1] == s && board[2][2] == s && board[2][3] == s) return true;
    if (board[2][2] == s && board[2][3] == s && board[2][4] == s) return true;

    
    // Col 2 is the only one with 3 cells: (0,2), (1,2), (2,2)
    if (board[0][2] == s && board[1][2] == s && board[2][2] == s) return true;

    
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    
    if (board[0][2] == s && board[1][3] == s && board[2][4] == s) return true;

    return false;
}

bool PyramidXO_Board::is_lose(Player<char>* player) {
    return false;
}

bool PyramidXO_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool PyramidXO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


PyramidXO_UI::PyramidXO_UI() : UI<char>("Welcome to Pyramid XO Game", 3) {
}

Player<char>* PyramidXO_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* PyramidXO_UI::get_move(Player<char>* player) {
    int x, y;
    cout << player->get_name() << " (" << player->get_symbol() << "), enter your move (row col): ";
    cin >> x >> y;
    return new Move<char>(x, y, player->get_symbol());
}

void PyramidXO_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
      



if (matrix.empty() || matrix[0].empty()) return;

int rows = matrix.size();
int cols = matrix[0].size();
int center_col = cols / 2; 

// 1. Column Numbers
cout << "\n    ";
for (int j = 0; j < cols; ++j)
    cout << setw(cell_width + 1) << j; 
cout << "\n";

for (int i = 0; i < rows; ++i) {

    cout << "    "; 
    for (int j = 0; j < cols; ++j) {
        bool isValid = (j >= center_col - i) && (j <= center_col + i);
        
        if (isValid) {
            
            cout << " " << string(cell_width, '-');
            
          
            if (j == center_col + i) cout << "-"; 
        } else {
            
            cout << string(cell_width + 1, ' ');
        }
    }
    cout << "\n";

    
    cout << setw(2) << i << "  "; // Row Index

    for (int j = 0; j < cols; ++j) {
        bool isValid = (j >= center_col - i) && (j <= center_col + i);

        if (isValid) {
            char c = matrix[i][j];
            if (c == '\0') c = ' ';

            cout << "|" << setw(cell_width) << c;

            
            if (j == center_col + i) {
                cout << "|";
            }
        } 
        else {
            cout << string(cell_width + 1, ' ');
        }
    }
    cout << "\n";
    
    
    if (i == rows - 1) {
        cout << "    ";
        for (int j = 0; j < cols; ++j) {
             if (j >= center_col - i && j <= center_col + i) {
                 cout << " " << string(cell_width, '-');
                 if (j == center_col + i) cout << "-";
             }
        }
    }
}
cout << endl;
    

}
