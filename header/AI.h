#pragma once

#include "../../header/BoardGame_Classes.h"
#include <climits>
#include <utility>
#include <cstdlib>
#include <algorithm>

#define INF INT_MAX
// General AI
class AI {
public:

    // Evaluate the board from AI perspective
    virtual int evaluate(Board<char>* board, Player<char>* player) {
        if (board->is_win(player)) return 10;
        if (board->is_lose(player)) return -10;
        return 0;
    }

    // Minimax with alpha-beta pruning
    virtual int minimax(bool aiTurn, Player<char>* player, int alpha, int beta, int depth) {
        auto* board = player->get_board_ptr();
        char ai = player->get_symbol();
        char opp = (ai == 'X' ? 'O' : 'X');

        if (depth == 0 || board->game_is_over(player)) {
            return evaluate(board, player);
        }

        if (aiTurn) {
            int bestScore = -INF;
            for (int r = 0; r < board->get_rows(); ++r) {
                for (int c = 0; c < board->get_columns(); ++c) {
                    if (board->get_cell(r,c) == '.') {
                        Move<char> move(r, c, ai);
                        board->update_board(&move);

                        int score = minimax(false, player, alpha, beta, depth - 1);

                        Move<char> undo(r, c, '.');
                        board->update_board(&undo);

                        bestScore = std::max(bestScore, score);
                    }
                }
            }
            return bestScore;
        } else {
            int bestScore = INF;
            for (int r = 0; r < board->get_rows(); ++r) {
                for (int c = 0; c < board->get_columns(); ++c) {
                    if (board->get_cell(r,c) == '.') {
                        Move<char> move(r, c, opp);
                        board->update_board(&move);

                        int score = minimax(true, player, alpha, beta, depth - 1);

                        Move<char> undo(r, c, '.');
                        board->update_board(&undo);

                        bestScore = std::min(bestScore, score);
                    }
                }
            }
            return bestScore;
        }
    }

    virtual std::pair<int, int> bestMove(Player<char>* player, int depth = 6) {
        auto* board = player->get_board_ptr();
        char ai = player->get_symbol();

        int bestScore = -INF;
        std::pair<int,int> move{-1,-1};

        for (int r = 0; r < board->get_rows(); ++r) {
            for (int c = 0; c < board->get_columns(); ++c) {
                if (board->get_cell(r,c) == '.') {
                    Move<char> m(r, c, ai);
                    board->update_board(&m);

                    int score = minimax(false, player, -INF, INF, depth - 1);

                    Move<char> undo(r, c, '.');
                    board->update_board(&undo);

                    if (score > bestScore) {
                        bestScore = score;
                        move = {r,c};
                    }
                }
            }
        }

        return move;
    }
};