#pragma once

 #include "../../header/BoardGame_Classes.h"

class Anti_XO_Board : public Board<char>
{
public:
    Anti_XO_Board();

    bool update_board(Move<char>* move) override;

    bool is_lose(Player<char>* player) override;

    bool is_win(Player<char>* player) override { return false; }

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;
};


class Anti_XO_UI : public UI<char>
{
public:
    Anti_XO_UI();
    ~Anti_XO_UI() {}

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    Move<char>* get_move(Player<char>* player) override;
};


