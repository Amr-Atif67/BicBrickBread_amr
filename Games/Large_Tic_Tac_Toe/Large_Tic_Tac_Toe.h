#ifndef Large_Tic_Tac_Toe_H
#define Large_Tic_Tac_Toe_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/Custom_UI.h"
#include "../../header/AI.h"
#include "../../Neural_Network/Include/NeuralNetwork.h"
#include <memory>
#include <cstdint> // Required for uint32_t

/**
 * @brief Represents the 5x5 Tic-Tac-Toe Board, optimized using a Bit Board approach.
 * * This board tracks player moves using three 32-bit integers (bitmasks)
 * for 'X', 'O', and occupied cells, significantly speeding up win checks.
 * Inherits from the generic Board class.
 */
class Large_XO_Board : public Board<char>
{
public:
    // --- 1. Constructors ---
    /**
     * @brief Construct a 5x5 board, initializing the vector board and bitboards.
     */
    Large_XO_Board();

    // --- 2. Accessors (Read Operations) ---
    
    /**
     * @brief Access the state of a cell using the bitboard representation.
     * @param r The cell's row (0-4).
     * @param c The cell's column (0-4).
     * @return The char symbol of the cell ('X', 'O', or '.').
     */
    char getCell(size_t r, size_t c);

    /**
     * @brief Return the value representing an empty cell.
     * @return The char of the empty cell (e.g., '.').
     */
    char getEmptyCell();

    /**
     * @brief Return the number of played moves
     * @return the int [0,24]
     */
    int getMoveCount();

    // --- 3. Mutators (Write/Update Operations) ---

    /**
     * @brief Update a cell using Bit-Board optimization. This is the core modification function.
     * * @param r The cell's row (0-4) to update.
     * @param c The cell's column (0-4) to update.
     * @param s The new value of the cell ('X', 'O', or emptyCell to clear).
     * @return Bool true if the update was successful (move was valid), otherwise false.
     */
    bool updateCell(size_t r, size_t c, char s);

    /**
     * @brief Update board when the player plays a move. Overrides the base class method.
     * * This method validates the move and calls updateCell internally.
     * @param move The Move object containing the row, column, and symbol of the player.
     * @return Bool to identify if it is a valid move or not.
     */
    bool update_board(Move<char>* move) override;

    // --- 4. Game Logic Checks ---

    /**
     * @brief Check if the game has ended (board is full in this variant). Overrides base method.
     * @param player The player which we want to check (can be ignored if state is terminal).
     * @return True if there is a draw, win, or lose condition, otherwise false.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Check if the player wins or not (based on final score). Overrides base method.
     * @param player The player which we want to check.
     * @return True if the player has more 3-in-a-row combos than the opponent at game end.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Check if the player loses or not (based on final score). Overrides base method.
     * @param player The player which we want to check.
     * @return True if the player has fewer 3-in-a-row combos than the opponent at game end.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Check if it is a draw or not (based on final score). Overrides base method.
     * @param player The player which we want to check.
     * @return True if both players have the same number of 3-in-a-row combos at game end.
     */
    bool is_draw(Player<char>* player) override;

    // --- 5. Helper Functions ---

    /**
     * @brief Count how many consequetive 3-in-a-row winning patterns the given symbol has.
     * * This uses the stored bitmasks for fast counting.
     * @param sym The symbol of the player ('X' or 'O').
     * @return Number of wins (float to be compatible with Neural Network value scales).
     */
    float countWin(char sym);
    
    /**
     * @brief Encode the current board state into a Matrix format suitable for the Neural Network.
     * * AI's pieces map to 1.0, Opponent's to -1.0, and empty cells to 0.0.
     * @param ai The AI's symbol.
     * @param input The Matrix reference to be updated with the encoded board state.
     */
    void encode(char ai, Matrix &input);

private:
    uint32_t boardX = 0;                                ///< Bit board mask for X's pieces.
    uint32_t boardO = 0;                                ///< Bit board mask for O's pieces.
    uint32_t boardXO = 0;                               ///< Bit board mask for all occupied cells (X | O).
    static uint32_t win3Masks[48];                ///< Store all the cobinations of 3 consequetive cells (winning lines).
    char emptyCell;                                     ///< Empty Cell value, typically '.'.
    int nMoves = 0;                                     ///< Number of Moves that has been made.
};


/**
 * @brief User Interface and AI logic for the 5x5 Large Tic-Tac-Toe Game.
 * * This class handles player input, displays the board, and implements the
 * hybrid AI using Minimax search combined with a Neural Network heuristic.
 */
class Large_XO_UI : public Custom_UI<char>
{
public:
    // --- 1. Constructors ---
    /**
     * @brief Construct the 5x5 XO_UI and load the trained Neural Networks.
     */
    Large_XO_UI();
    
    /**
     * @brief Destructor for the UI class.
     */
    ~Large_XO_UI() {}

    // --- 2. Player Interaction and Visualization ---
    /**
     * @brief Get the move from either a Human, Computer (random), or AI player.
     * * If the player type is AI, it calls the bestMove function.
     * @param player The player whose turn it is.
     * @return The move object to be checked by the game manager.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Display the 5x5 board in a formatted grid layout.
     * 
     * Renders the board with:
     * - Column numbers across the top (0-4)
     * - Row numbers down the left side (0-4)
     * - Grid lines separating cells
     * - Cell contents ('X', 'O', or empty '.')
     * 
     * @param matrix The board matrix (parameter exists for interface compatibility but is unused;
     *               actual board data is fetched from board->getCell())
     */
    void display_board_matrix(const vector<vector<char>>& matrix) const override;

    // --- 3. AI Core Functions ---
    /**
     * @brief Determines the best move using a hybrid approach of Neural Network pruning and Minimax search.
     * * @param player The AI player making the move.
     * @param NN The specific Neural Network (X or O) to use for heuristic evaluation.
     * @param depth The maximum depth to search in the minimax tree.
     * @return A pair of integers {row, col} representing the optimal move.
     */
    std::pair<int,int> bestMove(Player<char>* player, std::shared_ptr<NeuralNetwork>& NN, int depth);

    /**
     * @brief Implements the Minimax search algorithm with Alpha-Beta pruning.
     * * @param board The current state of the board.
     * @param NN The Neural Network to be used for evaluating non-terminal nodes (heuristic).
     * @param maximize True if it's the maximizing player's (AI's) turn, false otherwise.
     * @param depth The current remaining depth of the search.
     * @param alpha The alpha value for pruning.
     * @param beta The beta value for pruning.
     * @param ai The symbol of the AI player.
     * @param opp The symbol of the opponent player.
     * @return The calculated minimax value of the current board state.
     */
    float minimax(Large_XO_Board* board, std::shared_ptr<NeuralNetwork>& NN, bool maximize, int depth, float alpha, float beta,
                  char ai, char opp);

    /**
     * @brief Evaluates the current board state for the AI.
     * * Returns a large value for a win/loss state, or the Neural Network's Q-value for non-terminal states.
     * @param board The current state of the board.
     * @param NN The Neural Network used for prediction.
     * @param ai The symbol of the AI player.
     * @param opp The symbol of the opponent player.
     * @return The heuristic evaluation score (float).
     */
    float evaluate (Large_XO_Board* board, std::shared_ptr<NeuralNetwork>& NN, char ai, char opp);


private:
    std::shared_ptr<NeuralNetwork> NNX;                              ///< Neural Network trained for Player X.
    std::shared_ptr<NeuralNetwork> NNO;                              ///< Neural Network trained for Player O.
    Large_XO_Board * board = nullptr;                                ///< Saved Board for display.
};

#endif // Large_Tic_Tac_Toe_H