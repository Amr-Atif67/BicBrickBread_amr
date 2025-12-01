#ifndef WORD_TIC_TAC_TOE_H
#define WORD_TIC_TAC_TOE_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/Custom_UI.h"
#include "../../header/AI.h"
#include "../../Neural_Network/Include/NeuralNetwork.h"
#include <cstdint>
#include <set>
#include <fstream>
#include <tuple>
#include <algorithm>

// ============================================================================
// Word_XO_Board Class
// ============================================================================

/**
 * @class Word_XO_Board
 * @brief Represents the game board for a Word-based Tic-Tac-Toe variant.
 * 
 * This class inherits from the generic Board class and implements specialized 
 * logic for move validation, word scoring, and game termination, which are 
 * unique to this variant
 */
class Word_XO_Board : public Board<char> {
public:
    // ------------------------------------------------------------------------
    // Constructors
    // ------------------------------------------------------------------------
    
    /**
     * @brief Constructor for the Word_XO_Board.
     * 
     * Initializes the board dimensions and internal state. It is typically 
     * responsible for loading the dictionary for move.
     */
    Word_XO_Board();

    // ------------------------------------------------------------------------
    // Board State Queries
    // ------------------------------------------------------------------------
    
    /**
     * @brief Returns the character used to represent an unoccupied position on the board.
     * @return The char representing the empty cell (e.g., '.').
     */
    char getEmptyCell();

    /**
     * @brief Return the number of played moves.
     * @return The int [0,9] representing move count.
     */
    int getMoveCount();

    /**
     * @brief Checks if there is a word in the board or not.
     * @return `true` if there is a completed word; `false` otherwise.
     */
    bool wordExist();

    // ------------------------------------------------------------------------
    // Board Updates
    // ------------------------------------------------------------------------
    
    /**
     * @brief Processes and validates a player's move, then updates the board state.
     * 
     * This method checks standard board constraints (empty cell, bounds) and 
     * include custom validation for word formation and allowed 
     * placements. Overrides the base class method.
     * 
     * @param move The Move object containing the row, column, and symbol (char) of the player.
     * @return `true` if the move was successfully validated and applied; `false` otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Sets a pointer to the player who made the most recent move.
     * 
     * Necessary for scoring systems where the player who makes the final move 
     * is relevant to the win/draw calculation.
     * 
     * @param player The Player object who just completed their turn.
     */
    void setLastPlayer(Player<char>* player);

    // ------------------------------------------------------------------------
    // Game Logic Checks
    // ------------------------------------------------------------------------

    /**
     * @brief Determines if the game has reached a terminal state (win, loss, or draw).
     * 
     * Checks for a full board or if a player has achieved a predefined winning 
     * score/condition.
     * 
     * @param player The player for whom the terminal state is being checked.
     * @return `true` if the game is over; `false` otherwise.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Checks if the specified player has achieved the winning condition.
     * 
     * The win condition is determined by word formulation and last player
     * 
     * @param player The player to check for a win.
     * @return `true` if the player has won; `false` otherwise.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the specified player has lost the game (i.e., the opponent has won).
     * @param player The player to check for a loss.
     * @return `true` if the player has lost; `false` otherwise.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if the game has ended in a draw.
     * 
     * This condition occurs when the board is full and both players 
     * have the same score.
     * 
     * @param player The player for the context of the draw check.
     * @return `true` if the game is a draw; `false` otherwise.
     */
    bool is_draw(Player<char>* player) override;

    // ------------------------------------------------------------------------
    // Static Members
    // ------------------------------------------------------------------------
    
    static std::set<std::string> dict;      ///< Static set of valid words used for scoring and move validation (if applicable).
    static std::set<std::string> revDict;   ///< Static set of valid reverse words used for scoring and move validation (if applicable).

private:
    // ------------------------------------------------------------------------
    // Private Members
    // ------------------------------------------------------------------------
    
    int nMoves = 0;                         ///< Counter for the number of moves played so far.
    char emptyCell;                         ///< The character representing an empty, available cell.
    Player<char>* lastPlayer;               ///< Pointer to the player who executed the last move.
};

// ============================================================================
// Word_XO_UI Class
// ============================================================================

/**
 * @class Word_XO_UI
 * @brief User Interface and AI logic for the Word-based Tic-Tac-Toe Game.
 * 
 * This class handles player input, displays the board, and implements AI 
 * decision-making for the game.
 */
class Word_XO_UI : public Custom_UI<char>
{
public:
    // ------------------------------------------------------------------------
    // Constructors
    // ------------------------------------------------------------------------
    
    Word_XO_UI();

    // ------------------------------------------------------------------------
    // Player Interaction and Visualization
    // ------------------------------------------------------------------------
    
    /**
     * @brief Get the move from either a Human, Computer (random), or AI player.
     * 
     * If the player type is AI, it calls the bestMove function.
     * 
     * @param player The player whose turn it is.
     * @return The move object to be checked by the game manager.
     */
    Move<char>* get_move(Player<char>* player) override;

    // ------------------------------------------------------------------------
    // AI Core Functions
    // ------------------------------------------------------------------------
    
    /**
     * @brief Determines the best move using algorithm based on the analysis of the game.
     * @param player The AI player making the move.
     * @return A tuple of integers, char {row, col, symbol} representing the optimal move.
     */
    std::tuple<int, int, char> bestMove(Player<char>* player);

    /**
     * @brief Evaluates possible moves and their scores for a given board position.
     * @param r Row index.
     * @param c Column index.
     * @return A vector of pairs containing scores and corresponding characters.
     */
    std::vector<std::pair<int, char>> evaluate(size_t r, size_t c);

private:
    // ------------------------------------------------------------------------
    // Private Static Members
    // ------------------------------------------------------------------------
    
    static std::vector<std::pair<int, char>> score[3][3];  ///< Pre-computed scores for board positions.
};

#endif // WORD_TIC_TAC_TOE_H