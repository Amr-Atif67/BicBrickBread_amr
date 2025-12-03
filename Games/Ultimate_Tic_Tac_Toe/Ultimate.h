
#ifndef Ultimate_H
#define Ultmate_H
#include "../../header/BoardGame_Classes.h"
using namespace std;


class Ultimate_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.
    char large_board[3][3];
    int small_board=0;
    bool full=true;
    bool win=false;
    int check=0;
 
  
public:
    Ultimate_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
    void small_board_check(int x,int y, char sym);
    bool all_small_boards_done();
    
};


class Ultimate_UI : public UI<char> {
public:

    Ultimate_UI();

   
    ~Ultimate_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
    
};

#endif  //Ultimate_Tic_Tac_Toe
