//--------------------------------------- IMPLEMENTATION
/**
 * @file Memory_TicTacToe.cpp
 * @brief Implementation of the Memory Tic-Tac-Toe board and UI classes.
 *
 * This file contains the logic for:
 *  - Revealing memory cells
 *  - Maintaining hidden/revealed states
 *  - Checking for end-game conditions (win/draw)
 *  - UI handling for memory-style board display
 */

#include <iostream>
#include <iomanip>
#include <cctype>
#include "Memory_Tic_Tac_Toe.h"

using namespace std;

//--------------------------------------- Memory_Board Implementation

/**
 * @brief Constructs a 3×3 Memory Tic-Tac-Toe board.
 *
 * - Uses '?' as hidden-cell marker  
 * - Initializes reveal tracking matrix  
 */
Memory_Board::Memory_Board() : Board(3, 3)
{
    revealedMatrix = vector<vector<bool>>(rows, vector<bool>(columns, false));

    // Fill entire board with "hidden" symbols
    for (auto &row : board)
        for (auto &cell : row)
            cell = hidden_symbol;
}

/**
 * @brief Reveals a memory cell for the given move.
 *
 * Memory game move logic:
 *  - If cell is hidden → reveal it (show its true content)
 *  - If already revealed → invalid move
 *  - If move->symbol == 0 → "undo reveal" (for game engines with backtracking)
 *
 * @return true if the action succeeds, false otherwise.
 */
bool Memory_Board::update_board(Move<char>* move)
{
    int x = move->get_x();
    int y = move->get_y();
    char action = move->get_symbol();  // Symbol is unused in Memory version

    // Bounds check
    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    // Undo move (used by AI or rollback)
    if (action == 0)
    {
        if (revealedMatrix[x][y])
        {
            revealedMatrix[x][y] = false;
            board[x][y] = hidden_symbol;
            revealed_count--;
        }
        return true;
    }

    // Reveal a hidden cell
    if (!revealedMatrix[x][y])
    {
        revealedMatrix[x][y] = true;

        // Example: revealed value could be based on internal data
        // For now, reveal cell content as uppercase letter or similar
        board[x][y] = 'A' + (x * columns + y);

        revealed_count++;
        return true;
    }

    // Cannot reveal an already revealed cell
    return false;
}

/**
 * @brief Checks if a player has won.
 *
 * Memory Tic-Tac-Toe sample win rule:
 *  - A player "wins" if any straight line (row/column/diagonal)
 *    contains **three revealed cells** (any symbols).
 *
 * Expandable for advanced rules:
 *  - Matching pairs
 *  - Symbol-specific lines
 *  - Two-turn memory flips
 */
bool Memory_Board::is_win(Player<char>* player)
{
    auto revealed = [&](int r, int c) { return revealedMatrix[r][c]; };

    // Check rows & columns
    for (int i = 0; i < rows; i++)
    {
        if (revealed(i,0) && revealed(i,1) && revealed(i,2))
            return true;

        if (revealed(0,i) && revealed(1,i) && revealed(2,i))
            return true;
    }

    // Diagonals
    if (revealed(0,0) && revealed(1,1) && revealed(2,2))
        return true;

    if (revealed(0,2) && revealed(1,1) && revealed(2,0))
        return true;

    return false;
}

/**
 * @brief Loss condition for Memory Tic-Tac-Toe.
 *
 * By default, Memory Tic-Tac-Toe has no "loss" rule.
 *
 * @return Always false.
 */
bool Memory_Board::is_lose(Player<char>*)
{
    return false;
}

/**
 * @brief Checks if the board is in a draw state.
 *
 * A draw occurs when:
 *  - All cells have been revealed
 *  - No winning pattern exists
 */
bool Memory_Board::is_draw(Player<char>* player)
{
    return (revealed_count == rows * columns && !is_win(player));
}

/**
 * @brief Game ends when win OR draw occurs.
 */
bool Memory_Board::game_is_over(Player<char>* player)
{
    return is_win(player) || is_draw(player);
}

//--------------------------------------- Memory_UI Implementation

/**
 * @brief Initializes Memory game UI with welcome message.
 */
Memory_UI::Memory_UI()
    : Custom_UI<char>("Welcome to Memory Tic-Tac-Toe", 3)
{
}

/**
 * @brief Gets a player's move.
 *
 * For Memory Tic-Tac-Toe:
 *  - Human: manually selects a hidden cell
 *  - Computer: random hidden cell
 *  - AI: uses memory-based reveal selection
 */
Move<char>* Memory_UI::get_move(Player<char>* player)
{
    int r, c;

    if (player->get_type() == PlayerType::HUMAN)
    {
        cout << player->get_name() << ", select a cell to reveal (row col): ";
        cin >> r >> c;
    }
    else if (player->get_type() == PlayerType::COMPUTER)
    {
        r = rand() % 3;
        c = rand() % 3;
    }
    else if (player->get_type() == PlayerType::AI)
    {
        // AI chooses best "memory" move (dummy implementation)
        auto best = bestMove(player, 9);
        r = best.first;
        c = best.second;
    }

    return new Move<char>(r, c, player->get_symbol());
}

/**
 * @brief Prints the board showing hidden and revealed cells.
 */
void Memory_UI::display_board_matrix(const vector<vector<char>>& matrix) const
{
    if (matrix.empty() || matrix[0].empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();

    cout << "\n    ";
    for (int j = 0; j < cols; ++j)
        cout << setw(cell_width + 1) << j;
    cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

    for (int i = 0; i < rows; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < cols; ++j)
            cout << setw(cell_width) << (matrix[i][j] == '.'? '.' : '#') << " |";
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
    }
    cout << endl;
}