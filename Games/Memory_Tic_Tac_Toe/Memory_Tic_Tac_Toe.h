/**
 * @file Memory_TicTacToe.h
 * @brief Defines the specialized Tic-Tac-Toe variant using Memory class names.
 *
 * This variant implements:
 *  - Custom board logic with player moves (X/O placement)
 *  - Win, draw, and game-over detection
 *  - Custom board display through `display_board_matrix`
 *
 * Classes:
 *  - `Memory_Board` : Manages board state and move application
 *  - `Memory_UI`    : Handles board rendering and player input
 */

#ifndef MEMORY_TIC_TAC_TOE_H
#define MEMORY_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"

using namespace std;

/**
 * @class Memory_Board
 * @brief Specialized board class for the Memory Tic-Tac-Toe variant.
 *
 * Responsibilities:
 *  - Maintain board state
 *  - Apply player moves
 *  - Detect winning, draw, and game-over conditions
 *
 * Inherits from `Board<char>` and overrides core board behaviors.
 */
class Memory_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Symbol representing empty cells

public:
    /**
     * @brief Initializes a 3×3 specialized board.
     */
    Memory_Board();

    /**
     * @brief Applies a player's move to the board.
     * @param move Pointer to the player's move
     * @return true if the move is valid and successfully applied
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the specified player has met the win condition.
     * @param player Player to evaluate
     * @return true if the player satisfies the winning pattern
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the player has lost.
     * @param player Player to evaluate
     * @return false (loss detection not used in this variant)
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if the game ended in a draw.
     * @param player Player to evaluate
     * @return true if the board is full and no winning pattern exists
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Determines if the game is over (win or draw).
     * @param player Player to evaluate
     * @return true if the game has reached a terminal state
     */
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class Memory_UI
 * @brief User interface for the specialized Memory Tic-Tac-Toe variant.
 *
 * Responsibilities:
 *  - Display the board with custom formatting
 *  - Obtain player moves (human or AI)
 *
 * Inherits:
 *  - `Custom_UI<char>` : Provides display and input utilities
 *  - `AI`             : Optional AI-based move generation
 */
class Memory_UI : public Custom_UI<char>, public AI {
public:
    /**
     * @brief Initializes the specialized UI.
     */
    Memory_UI();

    /**
     * @brief Destructor
     */
    ~Memory_UI() {}

    /**
     * @brief Obtains the next move from a player.
     * @param player Player whose move is requested
     * @return Pointer to a newly allocated Move<char> object
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Renders the board with custom display logic.
     * @param matrix Two-dimensional grid of board characters
     */
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif // MEMORY_TIC_TAC_TOE_H