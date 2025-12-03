#include "Word_Tic_Tac_Toe.h"
#include <iostream>
#include <random>

// ============================================================================
// Static Member Initialization
// ============================================================================

// Initialize the static dictionary sets outside the class definition.
// These sets hold all valid words and their reverses for the game.
std::set<std::string> Word_XO_Board::dict;
std::set<std::string> Word_XO_Board::revDict;

std::vector<std::pair<int, char>> Word_XO_UI::score[3][3];

// ============================================================================
// Word_XO_Board Implementation
// ============================================================================

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------

Word_XO_Board::Word_XO_Board() : Board(3, 3), emptyCell('.')
{ 
    // Check if the static dictionary has already been loaded.
    // This ensures the dictionary is only loaded once, even if multiple boards are created.
    if(dict.empty()) {
        // Attempt to open the dictionary file.
        std::ifstream in("dic.txt");
        
        // If the file fails to open, throw a runtime exception.
        if (in.fail()) throw std::runtime_error("Can't Find \"dic.txt\"");
        
        // Extract words from the file and insert them into the static set.
        std::string word;
        while(in >> word) {
            dict.insert(word);
            std::reverse(word.begin(), word.end());
            revDict.insert(word);
        }
    }

    // Initialize all board cells to the empty cell marker.
    for (auto &row : board)
        for (auto &cell : row)
            cell = emptyCell;
}

// ----------------------------------------------------------------------------
// Board State Queries
// ----------------------------------------------------------------------------

char Word_XO_Board::getEmptyCell()
{
    return emptyCell;
}

int Word_XO_Board::getMoveCount()
{
    return nMoves;
}

bool Word_XO_Board::wordExist()
{
    std::string word;
    bool isFound = false;
    
    // Check Horizontal Lines
    for(int r = 0; r < 3; ++r) {
        word.clear();
        word.push_back(board[r][0]);
        word.push_back(board[r][1]);
        word.push_back(board[r][2]);
        
        isFound |= (dict.find(word) != dict.end());
        isFound |= (revDict.find(word) != revDict.end());
    }
    
    // Check Vertical Lines
    for(int c = 0; c < 3; ++c) {
        word.clear();
        word.push_back(board[0][c]);
        word.push_back(board[1][c]);
        word.push_back(board[2][c]);
        
        isFound |= (dict.find(word) != dict.end());
        isFound |= (revDict.find(word) != revDict.end());
    }

    // Check Main Diagonal
    word.clear();
    for(int rc = 0; rc < 3; ++rc) {
        word.push_back(board[rc][rc]); 
    }
    isFound |= (dict.find(word) != dict.end());
    isFound |= (revDict.find(word) != revDict.end());

    // Check Anti-Diagonal
    word.clear();
    for(int rc = 0; rc < 3; ++rc) {
        word.push_back(board[rc][2 - rc]); 
    }
    isFound |= (dict.find(word) != dict.end());
    isFound |= (revDict.find(word) != revDict.end());
    
    // Return true if any word is found on the board.
    return isFound;
}

// ----------------------------------------------------------------------------
// Board Updates
// ----------------------------------------------------------------------------

bool Word_XO_Board::update_board(Move<char> *move)
{
    // Extract move data: row (r), column (c), and symbol (sym).
    size_t r = move->get_x();
    size_t c = move->get_y();
    char sym = move->get_symbol();
    
    // Check for invalid input conditions:
    // 1. Coordinates out of 3x3 bounds (0-2).
    // 2. Attempting to place a symbol (sym != 0) on an already occupied cell.
    if (r > 2 || c > 2 || (board[r][c] != emptyCell && sym != 0)) return false;

    // --- Undo Logic (sym == 0) ---
    if (sym == 0) {
        // Reset the cell back to the empty marker.
        board[r][c] = emptyCell;
        // Decrement the move counter.
        --nMoves;
        return true; 
    }

    // --- Apply Move Logic (sym != 0) ---
    // Place the symbol on the board.
    board[r][c] = sym;
    // Increment the move counter.
    ++nMoves;
    return true;
}

void Word_XO_Board::setLastPlayer(Player<char> *player)
{
    lastPlayer = player;
}

// ----------------------------------------------------------------------------
// Game Logic Checks
// ----------------------------------------------------------------------------

bool Word_XO_Board::game_is_over(Player<char> *player)
{
    return is_win(player) || is_lose(player) || is_draw(player);
}

bool Word_XO_Board::is_win(Player<char> *player)
{
    return wordExist() && player == lastPlayer;
}

bool Word_XO_Board::is_lose(Player<char> *player)
{  
    return wordExist() && player != lastPlayer;
}

bool Word_XO_Board::is_draw(Player<char> *player)
{
    // Check for a draw: no win, no loss (word formed), and the board is full (9 moves).
    return !is_win(player) && !is_lose(player) && nMoves == 9;
}

// ============================================================================
// Word_XO_UI Implementation
// ============================================================================

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------

Word_XO_UI::Word_XO_UI() : Custom_UI<char>("Word Tic Tac Toe"s, 5)
{
    // Pre-compute scores for all board positions on first instantiation.
    if (score[0][0].empty()) {
        for(int r = 0; r < 3; ++r) {
            for(int c = 0; c < 3; ++c) {
                score[r][c] = evaluate(r, c);
            }
        }
    }
}

// ----------------------------------------------------------------------------
// Player Interaction and Visualization
// ----------------------------------------------------------------------------

Move<char> *Word_XO_UI::get_move(Player<char> *player)
{
    int r, c;  // Row and column coordinates
    char sym;
    
    // Handle Human player input
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol()
             << ") enter your move (row col): ";
        cin >> r >> c;
        cout << "Enter the charachter: ";
        cin >> sym;
        sym = toupper(sym);
    }
    // Handle Computer (random) player
    else if (player->get_type() == PlayerType::COMPUTER) {
        r = std::rand() % 5;
        c = std::rand() % 5;
        sym = std::rand() % 26 + 'A';
    }
    // Handle AI player
    else if (player->get_type() == PlayerType::AI) {
        auto move = bestMove(player);
        r = std::get<0>(move), c = std::get<1>(move), sym = std::get<2>(move);
        cout << "\n\n R: " << r << ", C: " << c << ", Sym: " << c << "\n\n";
    }

    // Set the current player as the last player to make a move.
    dynamic_cast<Word_XO_Board*>(player->get_board_ptr())->setLastPlayer(player);
    return new Move<char>(r, c, sym);
}

// ----------------------------------------------------------------------------
// AI Core Functions
// ----------------------------------------------------------------------------

std::tuple<int, int, char> Word_XO_UI::bestMove(Player<char> *player)
{
    Word_XO_Board* board = dynamic_cast<Word_XO_Board*>(player->get_board_ptr());
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // First move: prioritize center with high-scoring character
    if(board->getMoveCount() == 0) {
        int topCount = std::max(1, (int)(score[1][1].size() * 0.3));
        std::uniform_int_distribution<int> dis(0, topCount - 1);
        return {1, 1, score[1][1][dis(gen)].second};
    }
    
    // Define all possible lines (rows, columns, diagonals)
    std::vector<std::vector<std::pair<int, int>>> lines = {
        // Rows
        {{0, 0}, {0, 1}, {0, 2}}, {{1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {2, 1}, {2, 2}},
        // Columns
        {{0, 0}, {1, 0}, {2, 0}}, {{0, 1}, {1, 1}, {2, 1}}, {{0, 2}, {1, 2}, {2, 2}},
        // Diagonals
        {{0, 0}, {1, 1}, {2, 2}}, {{0, 2}, {1, 1}, {2, 0}}
    };

    // Lambda function to check if a line can form a word with one more character
    auto check_line_for_word = [&](std::vector<std::pair<int, int>> line) -> tuple<int,int,char> {
        int emptyR = -1;
        int emptyC = -1;
        int filledCount = 0;

        // Count filled cells and locate the empty one
        for (const auto& coord : line) {
            if (board->get_cell(coord.first, coord.second) == board->getEmptyCell()) {
                emptyR = coord.first;
                emptyC = coord.second;
            } else {
                ++filledCount;
            }
        }

        // If exactly 2 cells are filled, check if any character completes a word
        if (filledCount == 2) {
            for (char sym = 'A'; sym <= 'Z'; ++sym) {
                std::string temp_word = "";
                
                // Build the potential word
                for (const auto& coord : line) {
                    if (coord.first == emptyR && coord.second == emptyC) {
                        temp_word.push_back(sym);
                    } else {
                        temp_word.push_back(board->get_cell(coord.first, coord.second));
                    }
                }
                
                // Check if the word exists in dictionary or reverse dictionary
                if (Word_XO_Board::dict.find(temp_word) != Word_XO_Board::dict.end()) {
                    return {emptyR, emptyC, sym};
                }

                if (Word_XO_Board::revDict.find(temp_word) != Word_XO_Board::revDict.end()) {
                    return {emptyR, emptyC, sym};
                }
            }
        }

        return {-1, -1, 0};
    };

    // 1. Check for an immediate win
    std::vector<std::tuple<int, int, char>> winningMoves;

    for (const auto& line : lines) {
        tuple<int,int,char> winningMove = check_line_for_word(line);
        if (std::get<0>(winningMove) != -1) {
            winningMoves.push_back(winningMove);
        }
    }

    if (!winningMoves.empty()) {
        std::uniform_int_distribution<int> dis(0, winningMoves.size() - 1);
        auto [r, c, ch] = winningMoves[dis(gen)];
        return {r, c, ch};
    }

    // 2. Get all valid empty cells
    std::vector<std::pair<int, int>> emptyCells;
    
    for(int r = 0; r < 3; ++r) {
        for(int c = 0; c < 3; ++c) {
            if (board->get_cell(r, c) == board->getEmptyCell()) {
                emptyCells.push_back({r, c});
            }
        }
    }

    // 3. Find safe moves - cells where NO character creates a winning line for opponent
    std::vector<std::pair<int, int>> safeMoves;
    
    for (const auto& cell : emptyCells) {
        int r = cell.first;
        int c = cell.second;
        
        bool isSafe = true;
        
        // Check if any character at this position would give opponent a win
        for (char ch = 'A'; ch <= 'Z' && isSafe; ++ch) {
            Move<char> tempMove(r, c, ch);
            board->update_board(&tempMove); 

            for (const auto& line : lines) {
                std::tuple<int,int,char> threatMove = check_line_for_word(line);
                if (std::get<0>(threatMove) != -1) {
                    isSafe = false;
                    break; 
                }
            }

            Move<char> undoMove(r, c, 0); 
            board->update_board(&undoMove);
        }
        
        if (isSafe) {
            safeMoves.push_back({r, c});
        }
    }
    
    // 4. Select the best safe move based on pre-computed scores
    if(!safeMoves.empty()) {
        int i = std::rand() % safeMoves.size();
        int r = safeMoves[i].first, c = safeMoves[i].second;
        int topCount = std::max(1, (int)(score[r][c].size() * 0.3));
        std::uniform_int_distribution<int> dis(0, topCount - 1);
        return {r, c, score[r][c][dis(gen)].second}; 
    }

    // 5. Block opponent - find characters that make lines invalid
    std::vector<std::tuple<int, int, char>> blockMoves;

    for (const auto& [r, c] : emptyCells) {
        for (char ch = 'A'; ch <= 'Z'; ++ch) {
            Move<char> tempMove(r, c, ch);
            board->update_board(&tempMove);
            
            bool blocksOpponent = true;
            
            // Check if this move prevents opponent from forming words
            for (const auto& line : lines) {
                std::tuple<int,int,char> opponentWin = check_line_for_word(line);
                if (std::get<0>(opponentWin) != -1) {
                    blocksOpponent = false;
                }
            }
            
            Move<char> undoMove(r, c, 0);
            board->update_board(&undoMove);
            
            if (blocksOpponent) {
                blockMoves.push_back({r, c, ch});
            }
        }
    }

    if(!blockMoves.empty()) {
        std::uniform_int_distribution<int> dis(0, blockMoves.size() - 1);
        return blockMoves[dis(gen)];
    }
    
    // 6. Fallback - random move if no strategic move is found
    std::uniform_int_distribution<int> dis('A', 'Z');
    return {emptyCells[0].first, emptyCells[0].second, (char)dis(gen)};
}

std::vector<std::pair<int, char>> Word_XO_UI::evaluate(size_t r, size_t c)
{
    std::vector<std::pair<int, char>> ret;

    // Determine position type: 0 = corner, 1 = center, 2 = edge
    int posType = ((r == 1 && c == 1) ? 1 : ((r == 0 || r == 2) && (c == 0 || c == 2)) ? 0 : 2);
    
    // Evaluate each character based on how many words it can participate in
    for(char ch = 'A'; ch <= 'Z'; ++ch) {
        std::set<char> s1;
        std::set<char> s2;

        // Check all words in dictionary
        for(auto& word : Word_XO_Board::dict) {
            if(word[posType] != ch) continue;
            
            int temp = 0;
            for(int k = 0; k < 3; ++k) {
                if(k == posType) continue;
                if(temp == 0) s1.insert(word[k]);
                else s2.insert(word[k]);
                ++temp;
            }
        }

        // Score is the sum of unique characters at other positions
        ret.push_back({s1.size() + s2.size(), ch});
    }

    // Sort in descending order by score
    std::sort(ret.rbegin(), ret.rend());

    return ret;
}