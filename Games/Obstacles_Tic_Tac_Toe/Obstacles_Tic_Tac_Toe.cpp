#include "Obstacles_Tic_Tac_Toe.h"

/// Initialize static array of winning 4-in-a-row masks.
uint64_t Obstacles_Board::win4Masks[54] {};


/* ============================================================
    Obstacles_Board — Constructor
   ============================================================ */
/**
 * @brief Creates an empty 6×6 board, initializes the empty cell
 *        symbol, and generates all 4-in-a-row win masks if they
 *        haven't been generated already.
 *
 * Board layout uses 36 bits (0..35), mapping:
 *    index = row * 6 + col
 *
 * Bitboards used:
 *  - boardX      bits for X moves
 *  - boardO      bits for O moves
 *  - boardTraps  bits for traps (blocked cells)
 */
Obstacles_Board::Obstacles_Board()
    : Board(6, 6), emptyCell('.')
{
    board.assign(6, vector<char>(6, emptyCell));

    // If win masks not initialized, build them now
    if (win4Masks[0] == 0)
    {
        size_t maskIdx = 0;

        /* ----------------------------
           1. Horizontal win masks
           ---------------------------- */
        for (int r = 0; r < 6; ++r)
        {
            for (int c = 0; c <= 2; ++c)
            {
                int idx = r * 6 + c;
                win4Masks[maskIdx++] = 
                    (1ULL << idx) | (1ULL << (idx + 1)) |
                    (1ULL << (idx + 2)) | (1ULL << (idx + 3));
            }
        }

        /* ----------------------------
           2. Vertical win masks
           ---------------------------- */
        for (int c = 0; c < 6; ++c)
        {
            for (int r = 0; r <= 2; ++r)
            {
                int idx = r * 6 + c;
                win4Masks[maskIdx++] =
                    (1ULL << idx) | (1ULL << (idx + 6)) |
                    (1ULL << (idx + 12)) | (1ULL << (idx + 18));
            }
        }

        /* ----------------------------
           3. Diagonal masks
           ---------------------------- */
        for (int r = 0; r <= 2; ++r)
        {
            for (int c = 0; c <= 2; ++c)
            {
                int idx = r * 6 + c;
                win4Masks[maskIdx++] =
                    (1ULL << idx) | (1ULL << (idx + 7)) |
                    (1ULL << (idx + 14)) | (1ULL << (idx + 21));
            }
        }

        /* ----------------------------
           4. Diagonal masks
           ---------------------------- */
        for (int r = 0; r <= 2; ++r)
        {
            for (int c = 3; c < 6; ++c)
            {
                int idx = r * 6 + c;
                win4Masks[maskIdx++] =
                    (1ULL << idx) | (1ULL << (idx + 5)) |
                    (1ULL << (idx + 10)) | (1ULL << (idx + 15));
            }
        }
    }
}


/* ============================================================
    getCell()
   ============================================================ */
/**
 * @brief Returns the symbol stored at (r,c).
 * 
 * Priority:
 *   1. X bitboard
 *   2. O bitboard
 *   3. Trap bitboard
 *   4. empty cell
 */
char Obstacles_Board::getCell(size_t r, size_t c)
{
    size_t idx = r * 6 + c;

    if (boardX     & (1ULL << idx)) return 'X';
    if (boardO     & (1ULL << idx)) return 'O';
    if (boardTraps & (1ULL << idx)) return '#';

    return emptyCell;
}


/* ============================================================
    getEmptyCell()
   ============================================================ */
char Obstacles_Board::getEmptyCell()
{
    return emptyCell;
}


/* ============================================================
    getAvailableMove()
   ============================================================ */
/**
 * @brief Computes a list of all free cells (not X, O, or trap).
 *
 * @return vector of available index positions.
 */
std::vector<size_t> Obstacles_Board::getAvailableMove()
{
    std::vector<size_t> avail;
    avail.reserve(36);

    uint64_t full = boardX | boardO | boardTraps;

    for (size_t idx = 0; idx < 36; ++idx)
    {
        if ((full & (1ULL << idx)) == 0)
            avail.push_back(idx);
    }

    return avail;
}


/* ============================================================
    getMoveCount()
   ============================================================ */
int Obstacles_Board::getMoveCount()
{
    return nMoves;
}


/* ============================================================
    updateCell()
   ============================================================ */
/**
 * @brief Modifies the board at (r,c) with symbol s.
 * 
 * @param r row
 * @param c column
 * @param s symbol ('X', 'O') or 0 to clear.
 * 
 * Special behavior:
 *  - After a successful move, two new random traps are added.
 */
bool Obstacles_Board::updateCell(size_t r, size_t c, char s)
{
    size_t idx = r * 6 + c;

    if (idx >= 36) return false;

    uint64_t occupied = boardX | boardO | boardTraps;

    // If placing a move and it's already occupied fail.
    if (s != 0 && (occupied & (1ULL << idx)))
        return false;

    // Clearing a cell
    if (s == 0)
    {
        uint64_t mask = ~(1ULL << idx);
        boardX     &= mask;
        boardO     &= mask;
        boardTraps &= mask;

        --nMoves;
        return true;
    }

    // Placing X or O
    if (s == 'X') boardX |= (1ULL << idx);
    else          boardO |= (1ULL << idx);

    ++nMoves;

    // Add 2 new random traps
    auto avail = getAvailableMove();
    if (avail.size() >= 2)
    {
        size_t i1 = avail[rand() % avail.size()];
        boardTraps |= (1ULL << i1);

        avail.erase(std::find(avail.begin(), avail.end(), i1));

        size_t i2 = avail[rand() % avail.size()];
        boardTraps |= (1ULL << i2);
    }

    return true;
}


/* ============================================================
    update_board()
   ============================================================ */
/**
 * @brief Wrapper that applies a Move<char> to the board.
 */
bool Obstacles_Board::update_board(Move<char>* move)
{
    return updateCell(move->get_x(), move->get_y(), move->get_symbol());
}


/* ============================================================
    game_is_over()
   ============================================================ */
bool Obstacles_Board::game_is_over(Player<char>* player)
{
    return is_win(player) || is_lose(player) || is_draw(player);
}


/* ============================================================
    is_win()
   ============================================================ */
/**
 * @brief Checks if player's bitboard matches any win mask.
 */
bool Obstacles_Board::is_win(Player<char>* player)
{
    uint64_t boardTemp = (player->get_symbol() == 'X' ? boardX : boardO);

    for (uint64_t mask : win4Masks)
    {
        if ((boardTemp & mask) == mask)
            return true;
    }
    return false;
}


/* ============================================================
    is_lose()
   ============================================================ */
/**
 * @brief Opponent win check.
 */
bool Obstacles_Board::is_lose(Player<char>* player)
{
    uint64_t boardTemp = (player->get_symbol() == 'X' ? boardO : boardX);

    for (uint64_t mask : win4Masks)
    {
        if ((boardTemp & mask) == mask)
            return true;
    }
    return false;
}


/* ============================================================
    is_draw()
   ============================================================ */
/**
 * @brief Draw occurs if board is full and no one wins.
 */
bool Obstacles_Board::is_draw(Player<char>* player)
{
    return !is_win(player) &&
           !is_lose(player) &&
           nMoves == 36;
}


/* ============================================================
    Obstacles_UI — Constructor
   ============================================================ */
/**
 * @brief Initializes UI component with title and cell width.
 */
Obstacles_UI::Obstacles_UI()
    : Custom_UI<char>("Obstacles Tic Tac Toe"s, 3)
{}


/* ============================================================
    get_move()
   ============================================================ */
/**
 * @brief Retrieves move for human or computer player.
 *
 * Human:
 *   Prompts for input.
 *
 * Computer:
 *   Selects a random available move.
 */
Move<char>* Obstacles_UI::get_move(Player<char>* player)
{
    if (board == nullptr)
        board = static_cast<Obstacles_Board*>(player->get_board_ptr());

    int r, c;

    if (player->get_type() == PlayerType::HUMAN)
    {
        cout << player->get_name() << " (" << player->get_symbol()
             << ") enter your move (row col): ";
        cin >> r >> c;
    }
    else if(player->get_type() == PlayerType::COMPUTER)
    {
        auto avail = board->getAvailableMove();
        size_t idx = avail[rand() % avail.size()];
        r = idx / 6;
        c = idx % 6;
    }

    return new Move<char>(r, c, player->get_symbol());
}


/* ============================================================
    display_board_matrix()
   ============================================================ */
/**
 * @brief Prints a 6×6 board using bitboard lookup.
 */
void Obstacles_UI::display_board_matrix(const vector<vector<char>> &matrix) const
{
    // Clear the screen first
    system("cls");

    // Safety check: return early if matrix is empty
    if (matrix.empty() || matrix[0].empty()) return;

    // Board dimensions (fixed at 5x5)
    int rows = 6;
    int cols = 6;

    // Print column headers (0 1 2 3 4 5)
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
            cout << setw(cell_width) << (board == nullptr? '.' : board->getCell(i, j)) << " |";
        
        // Print horizontal separator line after each row
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
    }
    
    // Print final newline for spacing
    cout << endl;
}