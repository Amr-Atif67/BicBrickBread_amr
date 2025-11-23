#ifndef XO_num_H
#define XO_num_H

#include "BoardGame_Classes.h"
using namespace std;


class XO_NUM_Board : public Board<char> {
private:
    char blank_symbol = '.'; 
public:
   
    XO_NUM_Board();

    bool update_board(Move<char>* move);

    bool is_win(Player<char>* player);

    bool is_lose(Player<char>*) { return false; };


    bool is_draw(Player<char>* player);

    bool game_is_over(Player<char>* player);
};



class XO_NUM_UI : public UI<char> {
public:
    /**
     * @brief Constructs an XO_UI object.
     *
     * Initializes the base `UI<char>` class with the welcome message "FCAI X-O".
     */
    XO_NUM_UI();

    ~XO_NUM_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);

    virtual Move<char>* get_move(Player<char>* player);
};

#endif 
