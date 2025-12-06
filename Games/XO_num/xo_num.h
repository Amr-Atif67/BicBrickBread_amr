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

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
  
};



class XO_NUM_UI : public UI<char> {
public:

    XO_NUM_UI();

    ~XO_NUM_UI() {};
   char num;
   vector<char> odd{'1','3','5','7','9'}; 
   vector<char> even{'2','4','6','8'}; 
   Player<char>* create_player(string& name,char symbol, PlayerType type) override ;
   Player<char>** setup_players() override ;
   Move<char>* get_move(Player<char>* player) override;
};

#endif 
