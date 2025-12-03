#include <iostream>
#include <iomanip>
#include <cctype>
#include "Ultimate.h"

Ultimate_Board::Ultimate_Board() : Board(9, 9)
{
    for (auto &row : board)
    {
        for (auto &cell : row)
            cell = blank_symbol;
    }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            large_board[i][j] = blank_symbol;
        }
    }
    check=0;
  
}
void  Ultimate_Board::small_board_check(int x,int y, char sym){
 
    auto all_equal = [&](char a, char b, char c)
    {
        return a == b && b == c && a != blank_symbol;
    };
  
        
         bool win = false;
         bool full = true;
         int row_start = (x / 3) * 3; 
         int col_start = (y / 3) * 3;
          // Check rows and columns
        for (int i = 0; i < 3; ++i) {
                if ((all_equal(board[row_start + i][col_start], board[row_start + i][col_start + 1], board[row_start + i][col_start + 2])&& board[row_start + i][col_start] == sym ) ||
                    (all_equal(board[row_start][col_start + i], board[row_start + 1][col_start + i], board[row_start + 2][col_start + i])&& board[row_start][col_start + i] == sym))
                {
                    large_board[x/3][y/3] =sym;
                    win = true;
                }
            }

             // Check diagonals
        if ((all_equal(board[row_start][col_start], board[row_start + 1][col_start + 1], board[row_start + 2][col_start + 2])&& board[row_start][col_start] == sym)||
                (all_equal(board[row_start][col_start + 2], board[row_start + 1][col_start + 1], board[row_start + 2][col_start])&& board[row_start][col_start + 2] == sym)) {
                large_board[x / 3][y/3] = sym;
                win = true;
            }

            //check if draw
        for (int i = 0; i < 3; ++i){
            for (int j = 0; j < 3; ++j){
                if (board[row_start + i][col_start + j] == blank_symbol){
                        full = false;
                    }
                }
            }

          //if full and !win-->draw
        if (full && !win)
            {
                large_board[x / 3][y / 3] = '#'; 
            }
        
}
bool Ultimate_Board::all_small_boards_done() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (large_board[i][j] == blank_symbol)
                return false;
        }
    }
    return true;
}
bool Ultimate_Board::update_board(Move<char> *move)
{
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0) && large_board[x / 3][y / 3] == blank_symbol)
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
         small_board_check(x,y,mark);
        

      return true;
    }

    return false;
}
bool Ultimate_Board::is_win(Player<char> *player)
{

    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c)
    {
        return a == b && b == c && a != blank_symbol && a!='T';
    };

    // Check rows and columns
    for (int i = 0; i <3; ++i)
    {
        if ((all_equal(large_board[i][0], large_board[i][1], large_board[i][2]) && large_board[i][0] == sym) ||
            (all_equal(large_board[0][i], large_board[1][i], large_board[2][i]) && large_board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(large_board[0][0], large_board[1][1], large_board[2][2]) && large_board[1][1] == sym) ||
        (all_equal(large_board[0][2], large_board[1][1], large_board[2][0]) && large_board[1][1] == sym))
        return true;

    return false;
}

bool Ultimate_Board::is_draw(Player<char> *player)
{ 

    return (all_small_boards_done() && !is_win(player));
}

bool Ultimate_Board::game_is_over(Player<char> *player)
{
    return is_win(player) || is_draw(player);
}

//=====================UI===========
Ultimate_UI::Ultimate_UI() : UI<char>("Ultimate Tic_Tac_TOe", 3) {}

Player<char> *Ultimate_UI::create_player(string &name, char symbol, PlayerType type)
{
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}
void Ultimate_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
        if (matrix.empty() || matrix[0].empty()) return;

        int rows = matrix.size();
        int cols = matrix[0].size();

        cout << "\n    ";
        for (int j = 0; j < cols; ++j)
            cout << setw(cell_width + 1) << j;
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

        for (int i = 0; i < rows; ++i) {
            cout << setw(2) << i << " |";
            for (int j = 0; j < cols; ++j)
                cout << setw(cell_width) << matrix[i][j] << " |";
            cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
        }
        cout << endl;
    }

Move<char> *Ultimate_UI::get_move(Player<char> *player)
{
    int x, y;

    if (player->get_type() == PlayerType::HUMAN)
    {
        cout << "\nPlease enter your move x and y (0 to 8): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER)
    {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}