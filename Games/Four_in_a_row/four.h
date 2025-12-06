
#ifndef FOUR_H
#define FOUR_H
#include "../../header/BoardGame_Classes.h"
using namespace std;


class FOUR_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.
    

public:
   int last_row[7]{};
    FOUR_Board();
    bool update_board(Move<char>* move)override;
    bool is_win(Player<char>* player)override;
    bool is_lose(Player<char>* player) { return false; }
    bool is_draw(Player<char>* player)override;
    bool game_is_over(Player<char>* player)override;
    int get_last_row(int y){
           return 5-last_row[y];
    }
    int increase_last_row(int y){
           return 5-last_row[y]++;
    }
     int decrease_last_row(int y){
           return 5-last_row[y]--;
    }

    
};


class FOUR_UI : public UI<char> {
private:
char AI, OOP;
public:
   
    FOUR_UI();
    ~FOUR_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type)override;
    Move<char>* get_move(Player<char>* player) override;
    //=====For AI====
    bool isMovesLeft( vector<vector<char>>& matrix);
    int evaluate( vector<vector<char>>& matrix);
    int minimax(vector<vector<char>>& matrix, int depth, int alpha, int beta, bool isMax);
    int findBestMove( vector<vector<char>> board);
    int get_next_available_row(const vector<vector<char>>& board, int col);
};

#endif 
