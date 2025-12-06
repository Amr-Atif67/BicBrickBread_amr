
#ifndef Ultimate_H
#define Ultmate_H
#include "../../header/BoardGame_Classes.h"
using namespace std;


class Ultimate_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.
    // char large_board[3][3];
    int small_board=0;
    bool full=true;
    bool win=false;
    vector<vector<char>> large_board = vector<vector<char>>(3, vector<char>(3, '.'));
 
  
public:
    Ultimate_Board();
    bool update_board(Move<char>* move)override;
    bool is_win(Player<char>* player)override;
    bool is_draw(Player<char>* player) override;
    bool is_lose(Player<char>* player) { return false; }
    bool game_is_over(Player<char>* player) override;
    void small_board_check(int x,int y, char sym);
    bool all_small_boards_done();
   

    
};


class Ultimate_UI : public UI<char> {
public:

    Ultimate_UI();

   
    ~Ultimate_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
    
};

#endif  //Ultimate_Tic_Tac_Toe
