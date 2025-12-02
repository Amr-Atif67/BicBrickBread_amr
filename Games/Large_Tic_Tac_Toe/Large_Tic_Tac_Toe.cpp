#include "Large_Tic_Tac_Toe.h"

#include <iostream>

// ============================================================================
// Activation Functions
// ============================================================================

/// ReLU activation: max(0, x)
auto relu = [](double x) { return x > 0 ? x : 0; };

/// ReLU derivative: 1 if x > 0, else 0
auto relu_derivative = [](double x) { return x > 0 ? 1.0 : 0.0; };

/// Linear activation: f(x) = x
auto linear = [](double x) { return x; };

/// Linear derivative: f'(x) = 1
auto linear_derivative = [](double x) { return 1.0; };

// ============================================================================
// Large_XO_Board Implementation
// ============================================================================

/**
 * @brief Pre-computed bitmasks for all possible 3-in-a-row combinations on a 5x5 board.
 * 
 * This array contains 48 patterns:
 * - 15 horizontal patterns (3 per row × 5 rows)
 * - 5 vertical patterns (1 per column)
 * - 12 diagonal "\" patterns
 * - 12 diagonal "/" patterns
 * 
 * Each mask represents a specific 3-cell combination using bit positions 0-24.
 */
uint32_t Large_XO_Board::win3Masks[48] {};

// --- 1. Constructors ---

Large_XO_Board::Large_XO_Board()
    : Board(1, 1), emptyCell('.')
{
    // Initialize the 2D vector board with empty cells
    board.assign(5, vector<char>(5, emptyCell));

    if(win3Masks[0] == 0) {
        size_t maskIdx = 0;
        // Horizontal
        for (int r = 0; r < 5; ++r) {
            for (int c = 0; c <= 2; ++c) {
                int idx = 5 * r + c;
                win3Masks[maskIdx] = (1u<<(idx)) | (1u<<(idx + 1)) | (1u<<(idx + 2));
                ++maskIdx;
            }
        }

        // Vertical
        for (int c = 0; c < 5; ++c) {
            for (int r = 0; r <= 2; ++r) {
                int idx = 5 * r + c;
                win3Masks[maskIdx] = (1u<<(idx)) | (1u<<(idx + 5)) | (1u<<(idx + 10));
                ++maskIdx;
            }
        }

        // Diagonal: top-left to bottom-right
        for (int r = 0; r <= 2; ++r) {
            for (int c = 0; c <= 2; ++c) {
                int idx = 5 * r + c;
                win3Masks[maskIdx] = (1u<<(idx)) | (1u<<(idx + 6)) | (1u<<(idx + 12));
                ++maskIdx;
            }
        }

        // Diagonal: top-right to bottom-left
        for (int r = 0; r <= 2; ++r) {
            for (int c = 2; c < 5; ++c) {
                int idx = 5 * r + c;
                win3Masks[maskIdx] = (1u<<(idx)) | (1u<<(idx + 4)) | (1u<<(idx + 8));
                ++maskIdx;
            }
        }
    }
}

// --- 2. Accessors (Read Operations) ---

char Large_XO_Board::getCell(size_t r, size_t c)
{
    // Convert 2D coordinates to 1D bit index
    size_t idx = 5 * r + c;
    
    // Check which player occupies this position
    if (boardX & (1u << idx)) return 'X';
    if (boardO & (1u << idx)) return 'O';
    
    return emptyCell;
}

char Large_XO_Board::getEmptyCell()
{
    return emptyCell;
}

int Large_XO_Board::getMoveCount()
{
    return nMoves;
}

// --- 3. Mutators (Write/Update Operations) ---

bool Large_XO_Board::updateCell(size_t r, size_t c, char s)
{
    // Convert 2D coordinates to 1D bit index
    size_t idx = r * 5 + c;
    
    // Check bounds and if cell is already occupied (unless undoing)
    if (idx > 25 || (boardXO & (1u << idx) && s != 0))
        return false;
    
    // Handle undo operation (s == 0 means remove piece)
    if (s == 0) {
        --nMoves;
        uint32_t mask = ~(1u << idx);  // Create mask with 0 at idx, 1s elsewhere
        
        boardX &= mask;                // Clear X's bit

        boardO &= mask;                // Clear O's bit
        
        boardXO &= mask;               // Clear occupied bit
        return true;
    }

    // Place piece on board using bitwise OR
    if (s == 'X')
        boardX |= (1u << idx);
    else
        boardO |= (1u << idx);

    boardXO |= (1u << idx);
    
    ++nMoves;  // Increment move counter
    
    return true;
}

bool Large_XO_Board::update_board(Move<char>* move)
{
    // Extract move data
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();
    
    // Delegate to updateCell for actual implementation
    return updateCell(r, c, s);
}

// --- 4. Game Logic Checks ---

bool Large_XO_Board::game_is_over(Player<char>* player)
{
    // Game ends when all 24 moves have been played (25 cells, last one unplayed)
    return nMoves == 24;
}

bool Large_XO_Board::is_win(Player<char>* player)
{
    // Player wins if they have more 3-in-a-rows than opponent and game is over
    return countWin(player->get_symbol()) > countWin(player->get_symbol() == 'X' ? 'O' : 'X')
           && nMoves == 24;
}

bool Large_XO_Board::is_lose(Player<char>* player)
{
    // Player loses if they have fewer 3-in-a-rows than opponent and game is over
    return countWin(player->get_symbol()) < countWin(player->get_symbol() == 'X' ? 'O' : 'X')
           && nMoves == 24;
}

bool Large_XO_Board::is_draw(Player<char>* player)
{
    // Draw if both players have equal 3-in-a-rows and game is over
    return countWin(player->get_symbol()) == countWin(player->get_symbol() == 'X' ? 'O' : 'X')
           && nMoves == 24;
}

// --- 5. Helper Functions ---

float Large_XO_Board::countWin(char sym)
{
    float score = 0;
    
    // Get the appropriate bitboard for this symbol
    uint32_t& tempBoard = (sym == 'X' ? boardX : boardO);
    
    // Check each winning pattern
    for (const auto& mask : win3Masks) {
        // If (board & mask) == mask, then all 3 bits in the pattern are set
        score += (tempBoard & mask) == mask;
    }
    
    return score;
}

void Large_XO_Board::encode(char ai, Matrix& input)
{
    // Resize input matrix to match neural network input size (25 cells)
    input.resize(25, 1);
    
    // Helper lambda to translate bit position to character
    auto getChar = [&](size_t idx) {
        if (boardX & (1u << idx)) return 'X';
        if (boardO & (1u << idx)) return 'O';
        return emptyCell;
    };

    // Determine opponent symbol
    char opp = (ai == 'X') ? 'O' : 'X';

    // Encode each cell: AI pieces = 1.0, opponent pieces = -1.0, empty = 0.0
    for (size_t idx = 0; idx < 25; ++idx) {
        char cell = getChar(idx);
        
        if (cell == ai)
            input(idx, 0) = 1.0;   // AI's piece
        else if (cell == opp)
            input(idx, 0) = -1.0;  // Opponent's piece
        else
            input(idx, 0) = 0.0;   // Empty cell
    }
}

// ============================================================================
// Large_XO_UI Implementation
// ============================================================================

// --- 1. Constructors ---

Large_XO_UI::Large_XO_UI()
    : Custom_UI<char>("5x5 XO"s, 5)
{
    // Define network architecture: Input(25) -> Hidden(128) -> Hidden(256) -> Hidden(128) -> Output(25)
    std::vector<int> layerSizes = {25, 512, 512, 512, 25};
    
    // Define activation functions for each layer
    // Hidden layers use ReLU, output layer uses linear
    std::vector<std::function<double(double)>> activations = {
        relu, relu, relu, linear
    };
    
    // Define derivatives (needed for network construction, not used in inference)
    std::vector<std::function<double(double)>> derivatives = {
        relu_derivative, relu_derivative, relu_derivative, linear_derivative
    };
    
    try {
        // Create and load Neural Network for Player X
        NNX = std::make_shared<NeuralNetwork>(layerSizes, activations, derivatives);
        NNX->load("netX.bin");
        
        // Create and load Neural Network for Player O
        NNO = std::make_shared<NeuralNetwork>(layerSizes, activations, derivatives);
        NNO->load("netO.bin");
        
    } catch (const std::exception& e) {
        std::cerr << "Error initializing Large_XO_UI: " << e.what() << "\n";
    }
}

// --- 2. Player Interaction ---

Move<char>* Large_XO_UI::get_move(Player<char>* player)
{
    // Initialize board pointer on first call
    // If board hasn't been set yet (nullptr), cast the player's board to Large_XO_Board type
    // This allows access to Large_XO_Board-specific methods like getCell() and encode()
    if (board == nullptr) {
        board = dynamic_cast<Large_XO_Board*>(player->get_board_ptr());
    }

    int r, c;  // Row and column coordinates
    
    // Handle Human player input
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol()
             << ") enter your move (row col): ";
        cin >> r >> c;
    }
    // Handle Computer (random) player
    else if (player->get_type() == PlayerType::COMPUTER) {
        r = std::rand() % 5;
        c = std::rand() % 5;
    }
    // Handle AI player
    else if (player->get_type() == PlayerType::AI) {
        std::pair<int, int> move;
        
        // Cast board to get move count
        auto* board = dynamic_cast<Large_XO_Board*>(player->get_board_ptr());
        int movesMade = board->getMoveCount(); // You'll need to add this getter
        
        // Dynamic depth based on game phase
        int depth;
        
        if (movesMade <= 4) {
            // Opening (moves 0-4): Few pieces, many options
            depth = 3;
        }
        else if (movesMade <= 16) {
            // Midgame (moves 5-16): Critical tactical phase
            depth = 4;
        }
        else {
            // Endgame (moves 17-24): Fewer pieces, less branching
            depth = 5;
        }
        
        // Select appropriate network
        if (player->get_symbol() == 'X')
            move = bestMove(player, NNX, depth);
        else
            move = bestMove(player, NNO, depth);
        
        r = move.first;
        c = move.second;
    }
    
    // Return move object
    return new Move<char>(r, c, player->get_symbol());
}

void Large_XO_UI::display_board_matrix(const vector<vector<char>> &matrix) const
{
    // Clear the screen first
    system("cls");

    // Safety check: return early if matrix is empty
    if (matrix.empty() || matrix[0].empty()) return;

    // Board dimensions (fixed at 5x5)
    int rows = 5;
    int cols = 5;

    // Print column headers (0 1 2 3 4)
    cout << "\n    ";
    for (int j = 0; j < cols; ++j)
        cout << setw(cell_width + 1) << j;
    
    // Print top border line
    cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

    // Print each row of the board
    for (int i = 0; i < rows; ++i) {
        // Print row number on the left
        cout << setw(2) << i << " |";
        
        // Print each cell in the row (fetched from bitboard representation)
        for (int j = 0; j < cols; ++j)
            cout << setw(cell_width) << (board == nullptr? board->getEmptyCell() : board->getCell(i, j)) << " |";
        
        // Print horizontal separator line after each row
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
    }
    
    // Print final newline for spacing
    cout << endl;
}

// --- 3. AI Core Functions ---

/**
 * @brief Determines the best move using hybrid Neural Network pruning + Minimax search.
 * 
 * Strategy:
 * 1. Use Neural Network to rank all possible moves by predicted Q-values
 * 2. Only search the top 8 most promising moves (NN pruning)
 * 3. For each candidate, use minimax to calculate exact value
 * 4. Return the move with highest minimax value
 * 
 * @param player The AI player making the move
 * @param NN The Neural Network (X or O) for heuristic evaluation
 * @param depth Maximum depth for minimax search tree
 * @return Pair {row, col} of the optimal move
 */
std::pair<int, int> Large_XO_UI::bestMove(Player<char>* player,
                                          std::shared_ptr<NeuralNetwork>& NN, int depth)
{
    // Cast board to Large_XO_Board type to access bitboard methods
    auto* board = dynamic_cast<Large_XO_Board*>(player->get_board_ptr());
    
    // Determine AI and opponent symbols
    char ai = player->get_symbol();
    char opp = (ai == 'X') ? 'O' : 'X';

    // PHASE 1: Get neural network predictions for all positions
    Matrix input;
    board->encode(ai, input);          // Convert board to NN input format
    Matrix out = NN->predict(input);   // Get Q-values for all 25 positions

    // PHASE 2: Build candidate list with Q-values
    std::vector<std::pair<double, int>> candidates;
    candidates.reserve(25);  // Pre-allocate to avoid reallocations
    
    for (int i = 0; i < 25; ++i) {
        int r = i / 5;
        int c = i % 5;
        
        // Only add empty cells as candidates
        if (board->getCell(r, c) == board->getEmptyCell()) {
            candidates.push_back({out(i, 0), i});  // {Q-value, board_index}
        }
    }
    
    // PHASE 3: Sort by Q-value (descending) and select top 8
    int searchCount = std::min((int)candidates.size(), 8);
    sort(candidates.rbegin(), candidates.rend());  // Highest Q-values first
    
    std::pair<int, int> bestMove = {-1, -1};
    float bestVal = -1e9;

    // PHASE 4: Use minimax to evaluate each candidate precisely
    for (int i = 0; i < searchCount; ++i) {
        int idx = candidates[i].second;  // Get board index from candidate
        int r = idx / 5;
        int c = idx % 5;

        // Try this move
        board->updateCell(r, c, ai);
        
        // Calculate exact value using minimax (opponent's turn next)
        float val = minimax(board, NN, false, depth - 1, -1e9, 1e9, ai, opp);
        
        // Undo the move to restore board state
        board->updateCell(r, c, 0);

        // Track the best move found so far
        if (val > bestVal) {
            bestVal = val;
            bestMove = {r, c};
        }
    }

    // Output chosen move for debugging
    cout << "\n[R,c] = [ " << bestMove.first << ", " << bestMove.second << " ]\n";
    
    return bestMove;
}

/**
 * @brief Minimax search with Alpha-Beta pruning and Neural Network move ordering.
 * 
 * Two search strategies based on depth:
 * - depth >= 2: Use NN to order moves (better pruning, more NN calls)
 * - depth < 2: Brute force all moves (simpler, faster for shallow search)
 * 
 * @param board Current board state
 * @param NN Neural Network for position evaluation
 * @param maximize True if maximizing player's turn (AI), false for minimizer (opponent)
 * @param depth Remaining search depth
 * @param alpha Alpha value for pruning (best maximizer can guarantee)
 * @param beta Beta value for pruning (best minimizer can guarantee)
 * @param ai AI player symbol
 * @param opp Opponent player symbol
 * @return Minimax evaluation of current position
 */
float Large_XO_UI::minimax(Large_XO_Board* board, std::shared_ptr<NeuralNetwork>& NN,
                           bool maximize, int depth, float alpha, float beta,
                           char ai, char opp)
{
    // BASE CASE: Terminal state or depth limit reached
    if (depth == 0 || board->game_is_over(nullptr)) {
        return evaluate(board, NN, ai, opp);
    }
    
    // DEEP SEARCH (depth >= 2): Use Neural Network to order moves
    // This dramatically improves alpha-beta pruning effectiveness
    if (depth >= 2) {
        // Get NN predictions for move ordering
        Matrix input;
        board->encode(maximize ? ai : opp, input);  // Encode for current player
        Matrix out = NN->predict(input);
        
        // Build ordered list of available moves with their Q-values
        std::vector<std::pair<double, int>> moves;
        moves.reserve(25);
        
        for (int idx = 0; idx < 25; ++idx) {
            int r = idx / 5;
            int c = idx % 5;
            if (board->getCell(r, c) == board->getEmptyCell()) {
                moves.push_back({out(idx, 0), idx});
            }
        }
        
        // Sort moves: best first for maximizer, worst first for minimizer
        // This ordering helps alpha-beta prune more branches early
        if (maximize)
            std::sort(moves.rbegin(), moves.rend()); // Descending (best first)
        else
            std::sort(moves.begin(), moves.end());    // Ascending (worst first)
        
        // MAXIMIZING PLAYER (AI's turn)
        if (maximize) {
            float maxEval = -1e9;
            for (const auto& [qval, idx] : moves) {
                int r = idx / 5;
                int c = idx % 5;
                
                // Try move
                board->updateCell(r, c, ai);
                float eval = minimax(board, NN, false, depth - 1, alpha, beta, ai, opp);
                board->updateCell(r, c, 0);  // Undo
                
                // Update max value and alpha
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                
                // Beta cutoff: minimizer can avoid this branch
                if (beta <= alpha)
                    break;
            }
            return maxEval;
        } 
        // MINIMIZING PLAYER (opponent's turn)
        else {
            float minEval = 1e9;
            for (const auto& [qval, idx] : moves) {
                int r = idx / 5;
                int c = idx % 5;
                
                // Try move
                board->updateCell(r, c, opp);
                float eval = minimax(board, NN, true, depth - 1, alpha, beta, ai, opp);
                board->updateCell(r, c, 0);  // Undo
                
                // Update min value and beta
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                
                // Alpha cutoff: maximizer can avoid this branch
                if (beta <= alpha)
                    break;
            }
            return minEval;
        }
    }
    
    // SHALLOW SEARCH (depth < 2): Simple brute-force without NN ordering
    // At shallow depths, the overhead of NN calls isn't worth the benefit
    
    // MAXIMIZING PLAYER
    if (maximize) {
        float maxEval = -1e9;
        
        // Try all possible moves
        for (int r = 0; r < 5; ++r) {
            for (int c = 0; c < 5; ++c) {
                if (board->getCell(r, c) != board->getEmptyCell())
                    continue;
                
                // Try move
                board->updateCell(r, c, ai);
                float eval = minimax(board, NN, false, depth - 1, alpha, beta, ai, opp);
                board->updateCell(r, c, 0);  // Undo
                
                // Update max and alpha
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                
                // Beta cutoff
                if (beta <= alpha)
                    return maxEval;
            }
        }
        return maxEval;
    } 
    // MINIMIZING PLAYER
    else {
        float minEval = 1e9;
        
        // Try all possible moves
        for (int r = 0; r < 5; ++r) {
            for (int c = 0; c < 5; ++c) {
                if (board->getCell(r, c) != board->getEmptyCell())
                    continue;
                
                // Try move
                board->updateCell(r, c, opp);
                float eval = minimax(board, NN, true, depth - 1, alpha, beta, ai, opp);
                board->updateCell(r, c, 0);  // Undo
                
                // Update min and beta
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                
                // Alpha cutoff
                if (beta <= alpha)
                    return minEval;
            }
        }
        return minEval;
    }
}

/**
 * @brief Evaluate a board position using exact scoring or Neural Network heuristic.
 * 
 * Evaluation strategy:
 * - Terminal state: Return exact score (±10000 + score difference)
 * - Non-terminal: Return highest Q-value from NN for available moves
 * 
 * @param board Current board state
 * @param NN Neural Network for heuristic evaluation
 * @param ai AI player symbol
 * @param opp Opponent player symbol
 * @return Evaluation score (higher is better for AI)
 */
float Large_XO_UI::evaluate(Large_XO_Board* board, std::shared_ptr<NeuralNetwork>& NN,
                            char ai, char opp)
{
    // TERMINAL STATE: Game has ended, return exact score
    if (board->game_is_over(nullptr)) {
        float aiScore = board->countWin(ai);     // AI's 3-in-a-row count
        float oppScore = board->countWin(opp);   // Opponent's 3-in-a-row count
        
        // AI wins: large positive value + margin
        if (aiScore > oppScore)
            return 10000.0f + (aiScore - oppScore);
        
        // AI loses: large negative value + margin
        if (oppScore > aiScore)
            return -10000.0f + (aiScore - oppScore);
        
        // Draw: neutral value
        return 0.0f;
    }
    
    // NON-TERMINAL STATE: Use Neural Network heuristic
    Matrix input;
    board->encode(ai, input);          // Encode board state
    Matrix out = NN->predict(input);   // Get Q-values for all positions
    
    double bestQ = -1e9;

    // Find the maximum Q-value among valid moves
    // This represents the NN's estimate of the position's value
    for (int idx = 0; idx < 25; ++idx) {
        if (board->getCell(idx / 5, idx % 5) == board->getEmptyCell()) {
            bestQ = std::max(bestQ, out(idx, 0));
        }
    }
    
    return static_cast<float>(bestQ);
}