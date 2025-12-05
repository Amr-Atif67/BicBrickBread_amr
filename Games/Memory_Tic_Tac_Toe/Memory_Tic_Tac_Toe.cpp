//--------------------------------------- IMPLEMENTATION
/**
 * @file Memory_TicTacToe.cpp
 * @brief Implementation of a specialized Tic-Tac-Toe variant using Memory class names.
 *
 * This file contains the concrete implementation of:
 *  - Board logic: applying moves, tracking game state, and detecting end conditions.
 *  - UI logic: collecting moves from different player types and rendering the board.
 *
 * Note:
 *  - The game is a variant and differs from standard Tic-Tac-Toe mainly in display formatting.
 *  - No hidden cells or memory-specific rules are enforced in this implementation.
 */

#include <iostream>
#include <iomanip>
#include <cctype>
#include "Memory_Tic_Tac_Toe.h"

using namespace std;

//--------------------------------------- Memory_Board Implementation

/**
 * @brief Constructs a 3×3 board for the variant.
 *
 * Each cell is initialized with the `blank_symbol`.
 * Move count starts at zero.
 */
Memory_Board::Memory_Board() : Board(3, 3)
{
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol;
}

/**
 * @brief Updates the board with the given move.
 *
 * Functionality:
 *  - Validates that the move coordinates are within bounds.
 *  - If `move->symbol` is zero or blank, clears the cell (undo move).
 *  - If the cell is empty, places the player's symbol (uppercase) and increments move count.
 *
 * This ensures that no occupied cell is overwritten.
 *
 * @param move Pointer to the player's Move object.
 * @return true if the move is valid and applied, false otherwise.
 */
bool Memory_Board::update_board(Move<char>* move)
{
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    // Undo move if requested
    if (mark == 0 || mark == blank_symbol)
    {
        if (board[x][y] != blank_symbol)
            n_moves--;
        board[x][y] = blank_symbol;
        return true;
    }

    // Apply move only if the cell is empty
    if (board[x][y] == blank_symbol)
    {
        board[x][y] = toupper(mark);
        n_moves++;
        return true;
    }

    // Move invalid: cell already occupied
    return false;
}

/**
 * @brief Checks whether the specified player has a winning line.
 *
 * Winning condition:
 *  - Any complete row, column, or diagonal contains only the player's symbol.
 *
 * Implementation uses a small lambda to check equality of three cells.
 *
 * @param player Player whose state is evaluated.
 * @return true if player has a winning line, false otherwise.
 */
bool Memory_Board::is_win(Player<char>* player)
{
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c)
    {
        return a == b && b == c && a != blank_symbol;
    };

    // Check all rows and columns
    for (int i = 0; i < rows; ++i)
    {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

/**
 * @brief Determines if the player has lost the game.
 *
 * In this implementation, a player is considered to have lost if the
 * opponent has formed a winning line (row, column, or diagonal).
 *
 * @param player Pointer to the player being evaluated
 * @return true if the opponent has a winning line, false otherwise
 */
bool Memory_Board::is_lose(Player<char>* player)
{
    const char sym = (player->get_symbol() == 'X' ? 'O' : 'X');

    auto all_equal = [&](char a, char b, char c)
    {
        return a == b && b == c && a != blank_symbol;
    };

    // Check all rows and columns
    for (int i = 0; i < rows; ++i)
    {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

/**
 * @brief Checks if the game ended in a draw.
 *
 * Draw condition:
 *  - All cells are filled.
 *  - No player has a winning line.
 *
 * @param player Player to evaluate (used to check winning state).
 * @return true if the game is a draw, false otherwise.
 */
bool Memory_Board::is_draw(Player<char>* player)
{
    return (n_moves == rows * columns && !is_win(player) && !is_lose(player));
}

/**
 * @brief Determines if the game is over.
 *
 * Game ends if there is a win or a draw.
 *
 * @param player Player to evaluate.
 * @return true if game has ended.
 */
bool Memory_Board::game_is_over(Player<char>* player)
{
    return is_win(player) || is_draw(player) || is_lose(player);
}

//--------------------------------------- Memory_UI Implementation

/**
 * @brief Constructs the UI for this variant.
 *
 * Initializes the board size and welcome message via Custom_UI.
 */
Memory_UI::Memory_UI()
    : Custom_UI<char>("Welcome to Memory Tic-Tac-Toe Variant", 3)
{
}

/**
 * @brief Obtains the next move for a player.
 *
 * Behavior:
 *  - Human: prompts for row and column input.
 *  - Computer: chooses random coordinates.
 *  - AI: uses `bestMove` function to determine optimal move.
 *
 * @param player Player whose move is requested.
 * @return Pointer to a new Move<char> object representing the player's action.
 */
Move<char>* Memory_UI::get_move(Player<char>* player)
{
    int r, c;

    if (player->get_type() == PlayerType::HUMAN)
    {
        cout << player->get_name() << " (" << player->get_symbol()
             << ") enter your move (row col): ";
        cin >> r >> c;
    }
    else if (player->get_type() == PlayerType::COMPUTER)
    {
        r = rand() % 3;
        c = rand() % 3;
    }
    else if (player->get_type() == PlayerType::AI)
    {
        auto move = bestMove(player, 9);
        r = move.first;
        c = move.second;
    }

    return new Move<char>(r, c, player->get_symbol());
}

/**
 * @brief Displays the board matrix in a formatted table.
 *
 * Display customization:
 *  - Uses '#' in place of non blank cells
 *
 * @param matrix 2D vector representing the board state.
 */
void Memory_UI::display_board_matrix(const vector<vector<char>>& matrix) const
{
    if (matrix.empty() || matrix[0].empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();

    // Column indices
    cout << "\n    ";
    for (int j = 0; j < cols; ++j)
        cout << setw(cell_width + 1) << j;
    cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

    // Rows
    for (int i = 0; i < rows; ++i)
    {
        cout << setw(2) << i << " |";
        for (int j = 0; j < cols; ++j)
            cout << setw(cell_width) << (matrix[i][j] != '.' ? '#' : matrix[i][j]) << " |";
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
    }
    cout << endl;
}