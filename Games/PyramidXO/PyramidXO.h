#pragma once

#include "../../header/BoardGame_Classes.h"

class PyramidXO_Board : public Board<char>
{
public:
    PyramidXO_Board();

    bool update_board(Move<char>* move) override;

    bool is_lose(Player<char>* player) override;

    bool is_win(Player<char>* player) override;

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;
};


class PyramidXO_UI : public UI<char>
{
public:
    PyramidXO_UI();
    ~PyramidXO_UI() {}

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    Move<char>* get_move(Player<char>* player) override;

    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;
};