//--------------------------------------- IMPLEMENTATION
/*

for refrence for now


*/
#include <iostream>
#include <iomanip>
#include <cctype> // for toupper()
#include "four.h"

using namespace std;

//--------------------------------------- X_O_Board Implementation

FOUR_Board::FOUR_Board() : Board(6, 7)
{
    // Initialize all cells with blank_symbol
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol;
}

bool FOUR_Board::update_board(Move<char> *move)
{   int y = move->get_y();
    int x = 5-last_row[y]++;
 
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0))
    {

        if (mark == 0)
        { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else
        { // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

bool FOUR_Board::is_win(Player<char> *player)
{

    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c,char d)
    {
        return a == b && b == c && c==d&& a != blank_symbol;
    };

    // Check rows and columns
 for (int i=0; i < rows; ++i) {
    for (int j = 0; j < columns ; ++j) {  
        if (j+3<7&&all_equal(board[i][j], board[i][j+1], board[i][j+2], board[i][j+3]))
            return true;
    }
}
for (int i = 0; i <rows ; ++i) { 
    for (int j = 0; j < columns; ++j) {
        if (i+3<6&&all_equal(board[i][j], board[i+1][j], board[i+2][j], board[i+3][j]))
            return true;
    }
}
    // Check diagonals
   for (int i=0; i<rows;++i){
    for (int j=0; j<columns;++j){
        if (i-3>=0&&j+3<7 &&all_equal(board[i][j], board[i-1][j+1], board[i-2][j+2],board[i-3][j+3]) && board[i][j] == sym)
        return true;
        if (i+3<6 && j+3<7 &&all_equal(board[i][j], board[i+1][j+1], board[i+2][j+2],board[i+3][j+3]) && board[i][j] == sym)
         return true;
    }
   }
      

    return false;
}

bool FOUR_Board::is_draw(Player<char> *player)
{
    return (n_moves == 42 && !is_win(player));
}

bool FOUR_Board::game_is_over(Player<char> *player)
{
    return is_win(player) || is_draw(player);
}

//--------------------------------------- XO_UI Implementation
FOUR_UI::FOUR_UI() : UI<char>("Weclome to FCAI X-O Game by Dr El-Ramly", 3) {}

Player<char> *FOUR_UI::create_player(string &name, char symbol, PlayerType type)
{
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char> *FOUR_UI::get_move(Player<char> *player)
{ 
    int  y;
    if (player->get_type() == PlayerType::HUMAN)
    {
        cout << "\nPlease enter your column (0 to 6): ";
        cin >>  y;
    
        // implement here
    }
    else if (player->get_type() == PlayerType::COMPUTER)
    {
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(0, y, player->get_symbol());
}
