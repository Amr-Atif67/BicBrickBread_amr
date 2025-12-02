/**
 * @file Memory_TicTacToe.h
 * @brief Defines the Memory Tic-Tac-Toe classes which extend the generic board game framework.
 *
 * Memory Tic-Tac-Toe is a variation of classic Tic-Tac-Toe where:
 *  - Players flip memory cards instead of directly placing X/O.
 *  - A move may reveal a hidden symbol.
 *  - Matching pairs or revealed patterns influence the game state.
 *
 * This file provides:
 *  - `Memory_Board`: A specialized board with hidden/revealed cell logic.
 *  - `Memory_UI`: User interface class for displaying the memory grid and receiving moves.
 */

#ifndef MEMORY_TIC_TAC_TOE_H
#define MEMORY_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"

using namespace std;

/**
 * @class Memory_Board
 * @brief Represents the board for the Memory Tic-Tac-Toe game.
 *
 * The Memory version of Tic-Tac-Toe behaves differently from classic X-O:
 *  - Each cell initially contains a **hidden symbol**.
 *  - A move consists of selecting a cell to **reveal**.
 *  - Game logic checks for matches, memory flips, and revealed sequences.
 *
 * Inherits from `Board<char>` and overrides all core board behaviors
 * (update, win condition, draw condition, and game termination).
 *
 * @see Board
 */
class Memory_Board : public Board<char> {

private:
    char hidden_symbol = '?';   ///< Symbol representing an unrevealed memory cell.
    char blank_symbol  = '.';   ///< Symbol for an empty revealed space (optional).
    int revealed_count = 0;     ///< Number of cells currently revealed.
    vector<vector<bool>> revealedMatrix; ///< Tracks which cells are revealed.

public:
    /**
     * @brief Constructs a standard Memory Tic-Tac-Toe board (3×3).
     *
     * Initializes:
     *  - Internal board matrix
     *  - Hidden cell states
     *  - Reveal tracking grid
     */
    Memory_Board();

    /**
     * @brief Reveals/matches a cell as a part of a player's move.
     *
     * Memory Tic-Tac-Toe uses flip-based logic:
     *  - If a cell is hidden → reveal it.
     *  - Matching patterns may cause special scoring or additional turns.
     *
     * @param move Pointer to a Move<char> object describing the player action.
     * @return true if the move is applied successfully, false otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks whether the given player has achieved a win condition.
     *
     * In Memory Tic-Tac-Toe, win conditions may include:
     *  - Revealing a specific pattern of symbols.
     *  - Forming a line of matching revealed symbols.
     *
     * @param player The player being evaluated.
     * @return true if the player has met a winning pattern.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the given player has lost the game.
     *
     * Typically unused in Memory Tic-Tac-Toe unless variant rules apply.
     * This implementation returns false unless custom loss rules are added.
     *
     * @param player Player being evaluated.
     * @return false (unless extended rules override this).
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if the game has ended in a draw.
     *
     * A draw occurs when:
     *  - All cells are revealed, AND
     *  - No winning pattern has formed.
     *
     * @param player Player being evaluated.
     * @return true if the board is filled without a winner.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks whether the game is over (win or draw).
     *
     * @param player Player whose status is being evaluated.
     * @return true if the game is in a terminal state.
     */
    bool game_is_over(Player<char>* player) override;
};


/**
 * @class Memory_UI
 * @brief User Interface for the Memory Tic-Tac-Toe game.
 *
 * Provides:
 *  - Display of the memory grid (hidden + revealed states)
 *  - Move input handling for both humans and AI
 *
 * Inherits from:
 *  - `Custom_UI<char>` for UI functions.
 *  - `AI` for optional AI-based memory matching logic.
 *
 * @see Custom_UI
 * @see AI
 */
class Memory_UI : public Custom_UI<char>, public AI {

public:

    /**
     * @brief Constructs the Memory Tic-Tac-Toe user interface.
     *
     * Initializes:
     *  - Welcome message
     *  - UI configuration for memory-style grids
     */
    Memory_UI();

    /**
     * @brief Destructor for Memory_UI.
     */
    ~Memory_UI() {}

    /**
     * @brief Obtains the player's move (reveal action).
     *
     * In Memory Tic-Tac-Toe, the player selects a hidden cell to flip.
     *
     * @param player The player whose move is requested.
     * @return Pointer to a newly allocated Move<char> object.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Displays the memory board, showing hidden and revealed cells.
     *
     * Hidden cells display `hidden_symbol` (default: '?')
     * Revealed cells display their actual character.
     *
     * @param matrix 2D grid of board characters.
     */
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif // MEMORY_TIC_TAC_TOE_H