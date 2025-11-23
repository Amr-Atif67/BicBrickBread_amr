#include "Large_Tic_Tac_Toe.h"

#include <iostream>


Large_XO_Board::Large_XO_Board():Board(5,5), emptyCell(' ')
{
    board.assign(5, vector<char>(5, emptyCell));
}

bool Large_XO_Board::update_board(Move<char>* move)
{
    
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();

    // Safety Check
    if (r < 0 || r >= board.size() || c < 0 || c >= board[0].size())
        return false;

    if (board[r][c] != emptyCell)
        return false;

    board[r][c] = s;
    return true;
}

bool Large_XO_Board::is_win(Player<char>* player)
{
    char sym = player->get_symbol();

    for (int r = 0; r < board.size(); r++) {   
        for (int c = 0; c < board.size() - 3; ++c) {
            if (board[r][c] == sym && board[r][c+1] == sym && board[r][c+2] == sym)
                return true;
        }
    }

    for (int c = 0; c < board[0].size(); c++) {   
        for (int r = 0; r < board.size() - 3; ++r) {
            if (board[r][c] == sym && board[r+1][c] == sym && board[r+2][c] == sym)
                return true;
        }
    }

    for(int rc = 0; rc < board.size() - 3; ++rc) {
        if (board[rc][rc] == sym && board[rc+1][rc+1] == sym && board[rc+2][rc+2] == sym)
            return true;
        if (board[rc][board.size()-rc-1] == sym && board[rc+1][board.size()-rc-2] == sym && board[rc+2][board.size()-rc-3] == sym)
            return true;
    }

    return false;
}

bool Large_XO_Board::is_lose(Player<char>* player) 
{
    return false;
}

bool Large_XO_Board::is_draw(Player<char>* player)
{
    if (is_win(player)) 
        return false;

    for (int r = 0; r < board.size(); r++)
        for (int c = 0; c < board[0].size(); c++)
            if (board[r][c] == emptyCell)
                return false;

    return true;
}

bool Large_XO_Board::game_is_over(Player<char>* player)
{
    return is_win(player) || is_draw(player);
}


Large_XO_UI::Large_XO_UI() 
    : UI<char>("5x5_XO",5) {}

Player<char>* Large_XO_UI::create_player(string& name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

Move<char>* Large_XO_UI::get_move(Player<char>* player)
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