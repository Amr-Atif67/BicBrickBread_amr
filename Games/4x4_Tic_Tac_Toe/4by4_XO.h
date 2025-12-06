#pragma once

#include "../../header/BoardGame_Classes.h"

class _4by4XO_Move : public Move<char> {
    int from_x, from_y;
public:
    _4by4XO_Move(int fx, int fy, int tx, int ty, char s) 
        : Move<char>(tx, ty, s), from_x(fx), from_y(fy) {}
    int get_from_x() const { return from_x; }
    int get_from_y() const { return from_y; }
};

class _4by4XO_Board : public Board<char>
{
public:
    _4by4XO_Board();

    bool update_board(Move<char>* move) override;

    bool is_win(Player<char>* player) override;

    bool is_lose(Player<char>* player) override;

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;
};


class _4by4XO_UI : public UI<char>
{
public:
    _4by4XO_UI();
    
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    Move<char>* get_move(Player<char>* player) override;
};