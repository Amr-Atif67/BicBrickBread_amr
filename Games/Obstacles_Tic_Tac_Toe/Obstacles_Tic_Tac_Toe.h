#ifndef OBSTACLES_TIC_TAC_TOE_H
#define OBSTACLES_TIC_TAC_TOE_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/Custom_UI.h"
#include <cstdint>
#include <algorithm>

/**
 * @class Obstachles_Board
 * @brief A modified Tic-Tac-Toe board supporting traps and 64-bit bitboard logic.
 *
 * This board implementation extends the base Board<char> class and
 * stores game state using three 64-bit bitboards (X positions, O positions,
 * and trap positions). Bitboards allow extremely fast win-checking through
 * precomputed win masks.
 *
 * Features:
 * - Supports traps/blocked cells that neither player can occupy.
 * - Tracks occupied positions via bitboards rather than 2D arrays.
 * - Provides win/lose/draw logic based on 4-in-a-row masks.
 * - Provides list of legal moves as indices in range [0..63].
 */
class Obstacles_Board : public Board<char>
{
public:
    /**
     * @brief Constructs an empty board with no moves made.
     *
     * Initializes bitboards, empty cell character,
     * and loads the static win masks if needed.
     */
    Obstacles_Board();

    /**
     * @brief Returns the symbol at position (r, c).
     *
     * @param r Row index.
     * @param c Column index.
     * @return Character stored at the requested cell ('X', 'O', trap symbol, or empty).
     */
    char getCell(size_t r, size_t c);

    /**
     * @brief Returns the character used to denote an empty cell.
     *
     * @return The empty cell symbol (usually '.').
     */
    char getEmptyCell();

    /**
     * @brief Returns a list of all legal moves.
     *
     * A move is considered legal if:
     * - Cell is not occupied by X.
     * - Cell is not occupied by O.
     * - Cell is not a trap.
     *
     * @return Vector of valid cell indices (0–63).
     */
    std::vector<size_t> getAvailableMove();

    /**
     * @brief Returns the total number of moves made so far.
     *
     * @return Number of moves played since start.
     */
    int getMoveCount();

    /**
     * @brief Attempts to place symbol `s` at (r, c).
     *
     * This updates the correct bitboard (X or O), but only if:
     * - Position is empty.
     * - Position is not blocked by a trap.
     *
     * @param r Row index.
     * @param c Column index.
     * @param s Symbol ('X' or 'O').
     * @return True if update was successful; false otherwise.
     */
    bool updateCell(size_t r, size_t c, char s);

    /**
     * @brief Applies a Move<char> object to the board.
     *
     * @param move Pointer to a Move<char> containing position + symbol.
     * @return True if move was applied successfully.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Determines whether the game is over for the given player.
     *
     * Game ends if:
     * - Player wins.
     * - Player loses.
     * - Board is full → draw.
     *
     * @param player Player to evaluate.
     * @return True if game ended; false otherwise.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Checks if the given player has a winning configuration.
     *
     * Uses bitwise AND to compare player's bitboard against each 4-in-a-row mask.
     *
     * @param player Pointer to player object.
     * @return True if player has a winning mask.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the given player has lost (opponent has 4-in-a-row).
     *
     * @param player Pointer to player.
     * @return True if opponent has a winning mask.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks for draw condition (board full and no winner).
     *
     * @param player Player to evaluate.
     * @return True if draw.
     */
    bool is_draw(Player<char>* player) override;

private:
    
    uint64_t boardX = 0;                  ///> 64-bit bitboard representing all X positions. */

    uint64_t boardO = 0;                  ///> 64-bit bitboard representing all O positions. */

    uint64_t boardTraps = 0;              ///> Bitboard representing traps/obstacles.

    static uint64_t win4Masks[54];        ///> Precomputed list of all 4-in-a-row winning masks.

    char emptyCell;                       ///> Symbol used for empty cells.

    int nMoves = 0;                       ///> Counter of how many moves have been played.
};

/**
 * @class Obstachles_UI
 * @brief User interface handler for Obstacles Tic-Tac-Toe.
 *
 * Handles:
 * - Rendering the board.
 * - Asking user for a move.
 * - Connecting UI logic with Obstachles_Board.
 */
class Obstacles_UI : public Custom_UI<char>
{
public:
    /**
     * @brief Creates a new UI handler.
     *
     * Board pointer is assigned externally after construction.
     */
    Obstacles_UI();

    /**
     * @brief Destructor.
     */
    ~Obstacles_UI() {}

    /**
     * @brief Obtains a move from a human or AI player.
     *
     * Converts player input into Move<char> objects.
     *
     * @param player Player whose turn it is.
     * @return Newly allocated Move<char> describing the player's action.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Renders the game board using a 2D matrix format.
     *
     * Called whenever board needs refreshing.
     *
     * @param matrix 2D grid of characters from board->to_matrix().
     */
    void display_board_matrix(const vector<vector<char>>& matrix) const override;

private:
    Obstacles_Board* board = nullptr;    ///> Pointer to the associated Obstachles_Board.
};

#endif // OBSTACLES_TIC_TAC_TOE_H