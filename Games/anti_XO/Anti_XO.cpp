#include "Anti_XO.h"

#include <iostream>
using namespace std;


Anti_XO_Board::Anti_XO_Board():Board(3,3)
{
    board.assign(3, vector<char>(3, '.'));
}

bool Anti_XO_Board::update_board(Move<char>* move)
{
    
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();

    if (r < 0 || r >= 3 || c < 0 || c >= 3)
        return false;

    if (board[r][c] != '.')
        return false;

    board[r][c] = s;
    return true;
}

bool Anti_XO_Board::is_lose(Player<char>* player)
{
    char sym = player->get_symbol();

    for (int r = 0; r < 3; r++)
        if (board[r][0] == sym && board[r][1] == sym && board[r][2] == sym)
            return true;

    for (int c = 0; c < 3; c++)
        if (board[0][c] == sym && board[1][c] == sym && board[2][c] == sym)
            return true;

    if (board[0][0] == sym && board[1][1] == sym && board[2][2] == sym)
        return true;

    if (board[0][2] == sym && board[1][1] == sym && board[2][0] == sym)
        return true;

    return false;
}

bool Anti_XO_Board::is_draw(Player<char>* player)
{
    if (is_lose(player)) 
        return false;

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[r][c] == '.')
                return false;

    return true;
}

bool Anti_XO_Board::game_is_over(Player<char>* player)
{
    return is_lose(player) || is_draw(player);
}


Anti_XO_UI::Anti_XO_UI() 
    : UI<char>("anti_XO",3) {}

Player<char>* Anti_XO_UI::create_player(string& name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

Move<char>* Anti_XO_UI::get_move(Player<char>* player)
{
    int r, c;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol()
             << ") enter your move (row col): ";
        cin >> r >> c;
    } else if (player->get_type() == PlayerType::COMPUTER) {
        r = std::rand()%5, c = std::rand()%5;
    }

    return new Move<char>(r, c, player->get_symbol());
}
