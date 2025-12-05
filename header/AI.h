#pragma once

#include "../../header/BoardGame_Classes.h"
#include <climits>
#include <utility>
#include <cstdlib>
#include <algorithm>

#define INF INT_MAX

/**
 * @class AI
 * @brief Implements a general AI with minimax algorithm and alpha-beta pruning.
 *
 * Supports:
 *  - Board evaluation for wins/losses
 *  - Minimax search with depth limit
 *  - Alpha-beta pruning to improve performance
 *  - Computing the optimal move for a given player
 */
class AI {
public:

    /**
     * @brief Evaluates the board state from the perspective of the AI player.
     *
     * @param board Pointer to the game board
     * @param player AI player
     * @return +10 for win, -10 for loss, 0 otherwise
     */
    virtual int evaluate(Board<char>* board, Player<char>* player) {
        if (board->is_win(player)) return 10;
        if (board->is_lose(player)) return -10;
        return 0;
    }

    /**
     * @brief Minimax algorithm with alpha-beta pruning.
     *
     * @param aiTurn True if it's the AI player's turn
     * @param player The AI player
     * @param alpha Current alpha value
     * @param beta Current beta value
     * @param depth Maximum search depth
     * @return Evaluation score of the board
     */
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
                    if (board->get_cell(r, c) == '.') {
                        Move<char> move(r, c, ai);
                        board->update_board(&move);

                        int score = minimax(false, player, alpha, beta, depth - 1);

                        // Undo move
                        Move<char> undo(r, c, 0);
                        board->update_board(&undo);

                        bestScore = std::max(bestScore, score);
                        alpha = std::max(alpha, score);

                        // Alpha-beta pruning
                        if (beta <= alpha) break;
                    }
                }
                if (beta <= alpha) break;
            }
            return bestScore;
        } else {
            int bestScore = INF;
            for (int r = 0; r < board->get_rows(); ++r) {
                for (int c = 0; c < board->get_columns(); ++c) {
                    if (board->get_cell(r, c) == '.') {
                        Move<char> move(r, c, opp);
                        board->update_board(&move);

                        int score = minimax(true, player, alpha, beta, depth - 1);

                        // Undo move
                        Move<char> undo(r, c, 0);
                        board->update_board(&undo);

                        bestScore = std::min(bestScore, score);
                        beta = std::min(beta, score);

                        // Alpha-beta pruning
                        if (beta <= alpha) break;
                    }
                }
                if (beta <= alpha) break;
            }
            return bestScore;
        }
    }

    /**
     * @brief Determines the optimal move for the AI player using minimax.
     *
     * @param player AI player
     * @param depth Maximum search depth (default 6)
     * @return Pair of row and column representing the best move
     */
    virtual std::pair<int, int> bestMove(Player<char>* player, int depth = 6) {
        auto* board = player->get_board_ptr();
        char ai = player->get_symbol();

        int bestScore = -INF;
        std::pair<int,int> move{-1, -1};

        for (int r = 0; r < board->get_rows(); ++r) {
            for (int c = 0; c < board->get_columns(); ++c) {
                if (board->get_cell(r, c) == '.') {
                    Move<char> m(r, c, ai);
                    board->update_board(&m);

                    int score = minimax(false, player, -INF, INF, depth - 1);

                    // Undo move
                    Move<char> undo(r, c, 0);
                    board->update_board(&undo);

                    if (score > bestScore) {
                        bestScore = score;
                        move = {r, c};
                    }
                }
            }
        }

        return move;
    }
};