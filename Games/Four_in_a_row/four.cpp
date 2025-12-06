
#include <iostream>
#include <iomanip>
#include <cctype> // for toupper()
#include <climits> //for INT_MAX INT_MIN
#include "four.h"

using namespace std;

//============= Board Implementation  =========

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
   //5 - last_row[y] gives the index of the lowest available row in this column
 
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

//====================================UI Implementation ==============
FOUR_UI::FOUR_UI() : UI<char>("Four In A Rwo", 3) {}

Player<char> *FOUR_UI::create_player(string &name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

Move<char> *FOUR_UI::get_move(Player<char> *player)
{ 
    int  y; int x;
    if (player->get_type() == PlayerType::HUMAN)
    {
        cout << "\nPlease enter your column (0 to 6): ";
        cin >>  y;
    }
     else if (player->get_type() == PlayerType::COMPUTER) {
        AI=player->get_symbol();
        if (AI=='X'){
             OOP='O';
        }
        else if (AI=='O'){
             OOP='X';
        }
       auto* b = dynamic_cast<FOUR_Board*>(player->get_board_ptr());
        y = FOUR_UI::findBestMove(b->get_board_matrix());
        
    }
    
    return new Move<char>(0, y, player->get_symbol());
}

//==================================AI Implementation===================

bool FOUR_UI::isMovesLeft(vector<vector<char>>& board)
{
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 7; j++)
            if (board[i][j] == '.')
                return true;
    return false;
}


int  FOUR_UI::evaluate(vector<vector<char>>& board)
{
    //Check for a win in  rows, columns, and diagonals.
    for (int i=0; i < 6; ++i) {
      for (int j = 0; j < 4 ; ++j) {  
        if (j+3<7&&(board[i][j] == board[i][j+1] && board[i][j+1] == board[i][j+2] && board[i][j+2] == board[i][j+3]&&board[i][j]!='.'))
            if (board[i][j]==AI)
                return +10;
            else if (board[i][j]==OOP)
                return -10;
       }
    }
   
    for (int i=0; i < 3; ++i) {
      for (int j = 0; j < 7 ; ++j) {  
        if (i+3<6&&(board[i][j]== board[i+1][j]&&board[i+1][j]== board[i+2][j]&&board[i+2][j]==board[i+3][j]&&board[i][j]!='.'))
            if (board[i][j]==AI)
                return +10;
            else if (board[i][j]==OOP)
                return -10;
       }
    }
     for (int i=0; i<3;++i){
       for (int j=0; j<4;++j){
        if (i-3>=0&&j+3<7 &&(board[i][j]== board[i-1][j+1]==board[i-2][j+2]==board[i-3][j+3]&&board[i][j]!='.') )
              if (board[i][j]==AI)
                  return +10;
              else if (board[i][j]==OOP)
                  return -10;
        if (i+3<6 && j+3<7 &&(board[i][j]== board[i+1][j+1]== board[i+2][j+2]==board[i+3][j+3]&&board[i][j]!='.') )
               if (board[i][j]==AI)
                  return +10;
              else if (board[i][j]==OOP)
                  return -10;
    }
   }
   return 0;
 
    
}
  
/*In Connect Four the last move should fall to the bottom
so this function returns the last available row*/

int FOUR_UI::get_next_available_row(const vector<vector<char>>& board, int col)
{
    for (int i = 5; i >= 0; --i)
        if (board[i][col] == '.')
            return i;
    return -1;
}

int FOUR_UI::minimax( vector<vector<char>>& board, int depth, int alpha, int beta, bool isMax)
{
    int score = evaluate(board);
    //Winning sooner → better score
    // losing later → less bad
    if (score == 10) {return score - depth;} 
    if (score == -10) {return score + depth;}
    if (!isMovesLeft(board)) return 0;
    if (depth > 6) return score;

    if (isMax)
    {
        int best = INT_MIN;
        for (int j = 0; j < 7; j++)
        {

               int i = get_next_available_row(board, j);
               if (i != -1){
                board[i][j] = AI;
                best = max(best, minimax(board, depth+1, alpha, beta, false));
                board[i][j] = '.';
                alpha = max(alpha, best);
                if (beta <= alpha) {break;}// prune this branch.
               }  
        }
        return best;
    }
    else
    {
        int best =INT_MAX;
        for (int j = 0; j < 7; j++)
        {
            int i = get_next_available_row(board, j);
            if (i != -1)
            {
                board[i][j] = OOP;
                best = min(best, minimax( board, depth+1, alpha, beta, true));
                board[i][j] = '.';
                beta = min(beta, best);
                if (beta <= alpha) break;
            }
        }
        return best;
    }
}

int  FOUR_UI::findBestMove(vector<vector<char>> board)
{
   int bestVal = INT_MIN;
   int bestMove=-1;
 

    int alpha = INT_MIN, beta = INT_MAX;

    for (int j = 0; j < 7; j++)
    {
        int i = get_next_available_row(board, j);
        if (i != -1)
        {
            board[i][j] = AI;
            int moveVal = minimax(board, 0, alpha, beta, false);
            board[i][j] = '.';
            if (moveVal > bestVal)
            {
               bestVal = moveVal;
               bestMove = j;
            }
            alpha = max(alpha, bestVal);
        }
    }

    return bestMove;
}
