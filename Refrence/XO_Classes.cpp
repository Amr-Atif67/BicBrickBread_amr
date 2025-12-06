//--------------------------------------- IMPLEMENTATION
/*

for refrence for now


*/
#include <iostream>
#include <iomanip>
#include <cctype> // for toupper()
#include "../header/XO_Classes.h"

using namespace std;

//--------------------------------------- X_O_Board Implementation

X_O_Board::X_O_Board() : Board(3, 3)
{
    // Initialize all cells with blank_symbol
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol;
}

bool X_O_Board::update_board(Move<char>* move)
{
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Check bounds
    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    // Undo move
    if (mark == 0 || mark == blank_symbol) {
        if (board[x][y] != blank_symbol)
            n_moves--;
        board[x][y] = blank_symbol;
        return true;
    }

    // Apply move
    if (board[x][y] == blank_symbol) {
        board[x][y] = toupper(mark);
        n_moves++;
        return true;
    }

    return false;
}

bool X_O_Board::is_win(Player<char> *player)
{

    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c)
    {
        return a == b && b == c && a != blank_symbol;
    };

    // Check rows and columns
    for (int i = 0; i < rows; ++i)
    {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool X_O_Board::is_lose(Player<char> *player)
{

    const char sym = (player->get_symbol() == 'X'? 'O' : 'X');

    auto all_equal = [&](char a, char b, char c)
    {
        return a == b && b == c && a != blank_symbol;
    };

    // Check rows and columns
    for (int i = 0; i < rows; ++i)
    {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool X_O_Board::is_draw(Player<char> *player)
{
    return (n_moves == 9 && !is_win(player));
}

bool X_O_Board::game_is_over(Player<char> *player)
{
    return is_win(player) || is_draw(player) || is_lose(player);
}

//--------------------------------------- XO_UI Implementation

XO_UI::XO_UI() : Custom_UI<char>("Weclome to FCAI X-O Game by Dr El-Ramly", 3) {}


Move<char> *XO_UI::get_move(Player<char> *player)
{ 
    int r, c;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol()
        << ") enter your move (row col): ";
        cin >> r >> c;
    } else if (player->get_type() == PlayerType::COMPUTER) {
        r = std::rand()%5, c = std::rand()%5;
    } else if (player->get_type() == PlayerType::AI) {
        std::pair move = bestMove(player, 9);
        r = move.first, c = move.second;
    }
    
    return new Move<char>(r, c, player->get_symbol());
}
