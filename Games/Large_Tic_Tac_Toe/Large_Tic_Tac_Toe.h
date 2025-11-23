#ifndef Large_Tic_Tac_Toe

#include "../../header/BoardGame_Classes.h"

class Large_XO_Board : public Board<char>
{
public:
    Large_XO_Board();

    bool update_board(Move<char>* move) override;

    bool is_lose(Player<char>* player) override;

    bool is_win(Player<char>* player) override;

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;

private:
    char emptyCell;
};


class Large_XO_UI : public UI<char>
{
public:
    Large_XO_UI();
    ~Large_XO_UI() {}

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    Move<char>* get_move(Player<char>* player) override;
};

#endif // Large_Tic_Tac_Toe