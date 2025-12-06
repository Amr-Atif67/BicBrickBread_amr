#include <iostream>
#include <iomanip>
#include <cctype>
#include "xo_num.h"

XO_NUM_Board::XO_NUM_Board() : Board(3, 3)
{
    // Initialize all cells with blank_symbol
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol;
}

bool XO_NUM_Board::update_board(Move<char> *move)
{
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move and apply if valid
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
        return true;
    }
    return false;
}

bool XO_NUM_Board::is_win(Player<char> *player)
{

 

    // Check rows and columns
    for (int i = 0; i < rows; ++i)
    {
        if ((board[i][0]-'0'+board[i][1]-'0'+ board[i][2]-'0'==15) ||
            (board[0][i]-'0'+ board[1][i]-'0'+ board[2][i]-'0'==15 ))
            return true;
    }

    // Check diagonals
    if (((board[0][0]-'0'+ board[1][1]-'0'+board[2][2]-'0')== 15) ||((board[0][2]-'0'+ board[1][1]-'0'+ board[2][0]-'0')==15))
        return true;

    return false;
}

bool XO_NUM_Board::is_draw(Player<char> *player)
{
    return (n_moves == 9 && !is_win(player));
}

bool XO_NUM_Board::game_is_over(Player<char> *player)
{
    return is_win(player) || is_draw(player);
}



 XO_NUM_UI::XO_NUM_UI() : UI<char>("Numerical Tic-Tac-Toe", 3) {}
Player<char>**XO_NUM_UI::setup_players() {
    // changed player names from X,O to 1,2
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };
    string nameX = get_player_name("Player 1");
    PlayerType typeX = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(nameX, '1', typeX);
    string nameO = get_player_name("Player 2");
    PlayerType typeO = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(nameO, '2', typeO);

    return players;
}
Player<char> *XO_NUM_UI::create_player(string &name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

Move<char> *XO_NUM_UI::get_move(Player<char> *player)
{ 
    int x, y,index;

    if (player->get_type() == PlayerType::HUMAN)
    {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
        // ask user to enter his choice
        cout<<"\n Please enter your num\n";
        if (player->get_symbol()=='2'  && !even.empty()){
              for (char x : even){
               cout <<x << " ";
            }
        }
        else if (player->get_symbol()=='1'  && !odd.empty()){
              for (char x : odd)
               cout << x << " "; 
            }
        cin>>num;
        // this func checks user choice and removes it from vector if valid
      auto exist = [](vector<char>& v, char n) -> bool {
        for (auto it = v.begin(); it != v.end(); ) {
            if (*it == n){
                v.erase(it);
              return true;
            }
            ++it; 
        }
        return false;
    };
    // if player 1->odd numbers  player2 ->even

      if (player->get_symbol()=='2'){
        while ( !exist(even,num)){
            cout<<"Please choose from your numbers\n";
              for (char x : even){
               cout <<x << " ";
              }
             cin>>num;
        }
      }
       else if (player->get_symbol()=='1'){
       while (!exist(odd,num) ){
            cout<<"Please choose from your numbers\n";
              for (char x : odd){
               cout << x << " ";
              }
             cin>>num;
        }
      }
        
    }
    else if (player->get_type() == PlayerType::COMPUTER)
    {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
        if (player->get_symbol()=='2'  && !even.empty()){
            // random choice by index
              index =rand() % even.size();
              num=even[index];
              even.erase(even.begin() + index);
            
        }
       else if (player->get_symbol()=='1' && !odd.empty()){
              index =rand() % odd.size();
              num=odd[index];
              odd.erase(odd.begin() + index);
        }
       
           
   
    }
    return new Move<char>(x, y, num);
}
