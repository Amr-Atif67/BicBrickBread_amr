#ifndef SUS
#define SUS
#include "../../header/BoardGame_Classes.h"
using namespace std;

class SUS_Board: public Board<char>{
private:
   char blank_symbol='.';

   int s_score{};
   int u_score{};
  

public:
  SUS_Board();
  bool update_board(Move<char>* move);
  bool is_win(Player<char>* player) override;
  bool is_lose(Player<char>* player)override;
  bool is_draw(Player<char>* player) override;
  bool game_is_over(Player<char>* player) override;
  void score(int x,int y, char sym);
};
class SUS_UI: public UI<char> {
private:
 char OOP='U';
 char AI='S';

public:
 SUS_UI();
 ~SUS_UI() {};
 Player<char>** setup_players() override;
 Player<char>* create_player(string& name, char symbol, PlayerType type) override;
 Move<char>* get_move(Player<char>* player) override;
 //=====For AI====
    struct moove{ int row, col;};
    bool isMovesLeft( vector<vector<char>>& matrix);
    int evaluate( vector<vector<char>>& board, int lastRow, int lastCol, char lastPlayer);
    int minimax( vector<vector<char>>& board, int depth, int alpha, int beta, bool isMax, int lastRow, int lastCol, char lastPlayer);
    moove findBestMove(Player<char>* player, vector<vector<char>> board);



};

#endif //SUS
