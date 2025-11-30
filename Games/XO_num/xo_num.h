#ifndef XO_num_H
#define XO_num_H

#include "../../header/BoardGame_Classes.h"
#include <vector>
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

    XO_NUM_UI();

    ~XO_NUM_UI() {};
   char num;
   vector<char> odd{'1','3','5','7','9'}; 
  vector<char> even{'2','4','6','8'}; 
    Player<char>* create_player(string& name,char symbol, PlayerType type) ;
    Player<char>** setup_players() override ;
    virtual Move<char>* get_move(Player<char>* player);
};

#endif 
