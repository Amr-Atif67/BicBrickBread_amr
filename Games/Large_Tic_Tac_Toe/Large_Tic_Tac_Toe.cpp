#include "Large_Tic_Tac_Toe.h"

#include <iostream>


Large_XO_Board::Large_XO_Board():Board(5,5), emptyCell('.')
{
    board.assign(5, vector<char>(5, emptyCell));
}

bool Large_XO_Board::update_board(Move<char>* move)
{
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();

    // Safety Check
    if (r < 0 || r >= board.size() || c < 0 || c >= board[0].size())
        return false;

    if (board[r][c] != emptyCell)
        return false;

    board[r][c] = s;
    cout << "( " << s << " ) Has played.\n";
    return true;
}

bool Large_XO_Board::is_win(Player<char>* player)
{
    char sym = player->get_symbol();
    int rows = board.size();
    int cols = board[0].size();

    // Horizontal
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c <= cols - 3; ++c) {
            if (board[r][c] == sym && board[r][c+1] == sym && board[r][c+2] == sym)
                return true;
        }
    }

    // Vertical
    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r <= rows - 3; ++r) {
            if (board[r][c] == sym && board[r+1][c] == sym && board[r+2][c] == sym)
                return true;
        }
    }

    // Diagonal: top-left to bottom-right
    for (int r = 0; r <= rows - 3; ++r) {
        for (int c = 0; c <= cols - 3; ++c) {
            if (board[r][c] == sym && board[r+1][c+1] == sym && board[r+2][c+2] == sym)
                return true;
        }
    }

    // Diagonal: top-right to bottom-left
    for (int r = 0; r <= rows - 3; ++r) {
        for (int c = 2; c < cols; ++c) {
            if (board[r][c] == sym && board[r+1][c-1] == sym && board[r+2][c-2] == sym)
                return true;
        }
    }

    return false;
}

bool Large_XO_Board::is_lose(Player<char>* player) 
{
    char sym = (player->get_symbol() == 'X'? 'O' : 'X');
    int rows = board.size();
    int cols = board[0].size();

    // Horizontal
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c <= cols - 3; ++c) {
            if (board[r][c] == sym && board[r][c+1] == sym && board[r][c+2] == sym)
                return true;
        }
    }

    // Vertical
    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r <= rows - 3; ++r) {
            if (board[r][c] == sym && board[r+1][c] == sym && board[r+2][c] == sym)
                return true;
        }
    }

    // Diagonal: top-left to bottom-right
    for (int r = 0; r <= rows - 3; ++r) {
        for (int c = 0; c <= cols - 3; ++c) {
            if (board[r][c] == sym && board[r+1][c+1] == sym && board[r+2][c+2] == sym)
                return true;
        }
    }

    // Diagonal: top-right to bottom-left
    for (int r = 0; r <= rows - 3; ++r) {
        for (int c = 2; c < cols; ++c) {
            if (board[r][c] == sym && board[r+1][c-1] == sym && board[r+2][c-2] == sym)
                return true;
        }
    }

    return false;
}

bool Large_XO_Board::is_draw(Player<char>* player)
{
    if (is_win(player)) 
        return false;

    for (int r = 0; r < board.size(); r++)
        for (int c = 0; c < board[0].size(); c++)
            if (board[r][c] == emptyCell)
                return false;

    return true;
}

bool Large_XO_Board::game_is_over(Player<char>* player)
{
    return is_win(player) || is_draw(player);
}


Large_XO_UI::Large_XO_UI() 
    : UI<char>("5x5_XO",3) {}

Player<char> **Large_XO_UI::setup_players()
{
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer", "AI", "Random"};

    string nameX = get_player_name("Player X");
    PlayerType typeX = get_player_type_choice("Player X", type_options);
    players[0] = create_player(nameX, static_cast<char>('X'), typeX);

    string nameO = get_player_name("Player O");
    PlayerType typeO = get_player_type_choice("Player O", type_options);
    players[1] = create_player(nameO, static_cast<char>('O'), typeO);

    return players;
}

Player<char>* Large_XO_UI::create_player(string& name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

Move<char> *Large_XO_UI::get_move(Player<char> *player)
{
    int r, c;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol()
        << ") enter your move (row col): ";
        cin >> r >> c;
    } else if (player->get_type() == PlayerType::COMPUTER) {
        r = std::rand()%5, c = std::rand()%5;
     } //else if (player->get_type() == PlayerType::AI) {
    //     std::pair move = AI::bestMove(player, true);
    //     r = move.first, c = move.second;
    // }
    
    return new Move<char>(r, c, player->get_symbol());
}