#include "SUS.h"
#include <iostream>

SUS_Board::SUS_Board() : Board(3, 3)
{
    // Initialize all cells with blank_symbol
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol;
}
void SUS_Board::score(int x, int y, char sym){
    // Check if there is S-U-S  in the row of the last move
    if (board[x][0]=='S'&&board[x][1]=='U'&&board[x][2]=='S'){
        sym == 'S' ? s_score++ : u_score++;
    }
     // Check if there is S-U-S in the column of the last move
    if(board[0][y]=='S'&&board[1][y]=='U'&&board[2][y]=='S'){
        sym == 'S' ? s_score++ : u_score++;
    }
     // Check if there is S-U-S in the diagonal + last move is in the diagonal
    if (board[0][0]=='S'&& board[1][1]=='U'&& board[2][2]=='S' &&(x==y) ){
        sym == 'S' ? s_score++ : u_score++;
    }
    if (board[0][2]=='S'&& board[1][1]=='U' &&board[2][0]=='S' &&(x+y==2)){
        sym == 'S' ? s_score++ : u_score++;
    }

       
 }
bool SUS_Board::update_board(Move<char> *move)
{
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();
     if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0))
    {

        if (mark == 0)
        { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else
        { // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }
       score(x,y,mark);
       cout<<"S score: "<<s_score<<endl <<"U score: "<< u_score;

        return true;
    }
    return false;
}
bool SUS_Board::is_win(Player<char> *player)
{
     const char sym = player->get_symbol();
     if (n_moves==9 && s_score>u_score && sym=='S' ){
        return true;

     }
   
    return false;
}
bool SUS_Board::is_lose(Player<char> *player)
{
     const char sym = player->get_symbol();
   
    if (n_moves==9 && s_score<u_score && sym=='S' ){
        return true;
     }
    return false;
}

bool SUS_Board::is_draw(Player<char> *player)
{
    return (n_moves == 9 &&  s_score == u_score );
}

bool SUS_Board::game_is_over(Player<char> *player)
{
    return is_win(player) || is_draw(player);
}

//===============================UI========================================
SUS_UI::SUS_UI() : UI<char>("SUS", 3) {}

Player<char> **SUS_UI::setup_players()
{
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer", "AI", "Random"};

    string nameX = get_player_name("Player S");
    PlayerType typeX = get_player_type_choice("Player S", type_options);
    players[0] = create_player(nameX, static_cast<char>('S'), typeX);

    string nameO = get_player_name("Player U");
    PlayerType typeO = get_player_type_choice("Player U", type_options);
    players[1] = create_player(nameO, static_cast<char>('U'), typeO);

    return players;
}

Player<char> *SUS_UI::create_player(string &name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

Move<char> *SUS_UI::get_move(Player<char> *player)
{ 
    int x, y;
    
    if (player->get_type() == PlayerType::HUMAN) {
       
        cout << player->get_name() << " (" << player->get_symbol()
         << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
        
    } 
    else if (player->get_type() == PlayerType::COMPUTER) {
       AI=player->get_symbol();
        if (AI=='X'){
             OOP='O';
        }
        else if (AI=='O'){
             OOP='X';
        }
       auto* b = dynamic_cast<SUS_Board*>(player->get_board_ptr());
        SUS_UI::moove mov = SUS_UI::findBestMove(player,b->get_board_matrix());
        x = mov.row, y = mov.col;
     } 
    return new Move<char>(x, y, player->get_symbol());
}
//=========================AI==========================

int SUS_UI::evaluate(vector<vector<char>>& board, int lastRow, int lastCol, char lastPlayer)
{
  
    int score = 0;
    if (board[lastRow][0] == 'S' &&  board[lastRow][1] == 'U' &&  board[lastRow][2] == 'S'){
            score += (lastPlayer == AI) ? 10 : -10;
    }
    
    if (board[0][lastCol] == 'S' &&  board[1][lastCol] == 'U' && board[2][lastCol] == 'S'){
            score += (lastPlayer == AI) ? 10 : -10;
    }
    
    if(board[0][0]=='S'&&board[1][1]=='U'&&board[2][2]=='S'  &&(lastCol==lastRow)){
        score += (lastPlayer == AI) ? 10 : -10;
    }
    
    
    if(board[0][2]=='S'&& board[1][1]=='U' &&board[2][0]=='S' &&(lastCol+lastRow==2))
        {
            score += (lastPlayer == AI) ? 10 : -10;
        }
    
    
    return score;
}

bool SUS_UI::isMovesLeft(vector<vector<char>>& board)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == '.')
                return true;
    return false;
}

int SUS_UI::minimax(vector<vector<char>>& board, int depth, int alpha, int beta, bool isMax, int lastRow, int lastCol, char lastPlayer)
{
    int score = evaluate(board, lastRow, lastCol, lastPlayer);
    
    if (score >= 10) {return score - depth;} 
    if (score <= -10) {return score + depth;}
    if (!isMovesLeft(board)) return 0;
    if (isMax)
    {
        int best = INT_MIN;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == '.')
                {
                    board[i][j] = AI;
                    best = max(best, minimax(board, depth+1, alpha, beta, false, i, j, AI));
                    board[i][j] = '.';
                    alpha = max(alpha, best);
                    if (beta <= alpha) break;
                }
            }
            if (beta <= alpha) break;
        }
        return best;
    }
    else
    {
        int best = INT_MAX;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == '.')
                {
                    board[i][j] = OOP;
                    best = min(best, minimax(board, depth+1, alpha, beta, true, i, j, OOP));
                    board[i][j] = '.';

                    beta = min(beta, best);
                    if (beta <= alpha) break;
                }
            }
            if (beta <= alpha) break;
        }
        return best;
    }
}

SUS_UI::moove SUS_UI::findBestMove(Player<char>* player, vector<vector<char>> board)
{
    int bestVal = INT_MIN;
    moove bestMove;
    bestMove.row = -1;
    bestMove.col = -1;

    int alpha = INT_MIN, beta = INT_MAX;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == '.')
            {
                board[i][j] = AI;
                int moveVal = minimax(board, 0, alpha, beta, false, i, j, AI);
                board[i][j] = '.';
                
                if (moveVal > bestVal)
                {
                    bestVal = moveVal;
                    bestMove.row = i;
                    bestMove.col = j;
                }
                
                alpha = max(alpha, bestVal);
            }
        }
    }

    return bestMove;
}