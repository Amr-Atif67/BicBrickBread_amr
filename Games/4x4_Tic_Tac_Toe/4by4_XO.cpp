#include "4by4_XO.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

_4by4XO_Board::_4by4XO_Board() : Board<char>(4, 4) {
    n_moves = 0;
    
    board[0][0] = 'X'; board[0][1] = 'O'; board[0][2] = 'X'; board[0][3] = 'O';
    
    board[3][0] = 'O'; board[3][1] = 'X'; board[3][2] = 'O'; board[3][3] = 'X';
}

bool _4by4XO_Board::update_board(Move<char>* move) {
    _4by4XO_Move* m = static_cast<_4by4XO_Move*>(move);

    int fx = m->get_from_x();
    int fy = m->get_from_y();
    int tx = m->get_x();
    int ty = m->get_y();
    char s = m->get_symbol();

    
    if (fx < 0 || fx >= 4 || fy < 0 || fy >= 4 || tx < 0 || tx >= 4 || ty < 0 || ty >= 4) return false;

    
    if (board[fx][fy] != s) return false;

    
    if (board[tx][ty] != 0) return false;

    
    if (abs(fx - tx) + abs(fy - ty) != 1) return false;

    
    board[tx][ty] = s;
    board[fx][fy] = 0;
    n_moves++;
    return true;
}

bool _4by4XO_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();
    
    for (int i = 0; i < 4; i++) {
        
        if (board[i][0] == s && board[i][1] == s && board[i][2] == s) return true;
        if (board[i][1] == s && board[i][2] == s && board[i][3] == s) return true;
        
        
        if (board[0][i] == s && board[1][i] == s && board[2][i] == s) return true;
        if (board[1][i] == s && board[2][i] == s && board[3][i] == s) return true;
    }
    
   
    if (board[0][0] == s && board[1][1] == s && board[2][2] == s) return true;
    if (board[1][1] == s && board[2][2] == s && board[3][3] == s) return true;
    if (board[0][1] == s && board[1][2] == s && board[2][3] == s) return true;
    if (board[1][0] == s && board[2][1] == s && board[3][2] == s) return true;

    
    if (board[0][3] == s && board[1][2] == s && board[2][1] == s) return true;
    if (board[1][2] == s && board[2][1] == s && board[3][0] == s) return true;
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    if (board[1][3] == s && board[2][2] == s && board[3][1] == s) return true;

    return false;
}

bool _4by4XO_Board::is_lose(Player<char>* player) {
    return false;
}

bool _4by4XO_Board::is_draw(Player<char>* player) {
    return n_moves > 100; // Safety limit
}

bool _4by4XO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

_4by4XO_UI::_4by4XO_UI() : UI<char>("Welcome to 4x4 Tic-Tac-Toe. Move your tokens to align 3!", 4) {}

Player<char>* _4by4XO_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* _4by4XO_UI::get_move(Player<char>* player) {
    int fx, fy, tx, ty;
    cout << player->get_name() << " (" << player->get_symbol() << ")\n";
    cout << "Select token to move (row col): ";
    cin >> fx >> fy;
    cout << "Select destination (row col): ";
    cin >> tx >> ty;
    return new _4by4XO_Move(fx, fy, tx, ty, player->get_symbol());
}
