
#ifndef FOUR_H
#define FOUR_H
#include "../../header/BoardGame_Classes.h"
using namespace std;


class FOUR_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.
    int last_row[7]{};

public:
    FOUR_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
    // void score();
};


class FOUR_UI : public UI<char> {
public:

    FOUR_UI();

   
    ~FOUR_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

#endif 
