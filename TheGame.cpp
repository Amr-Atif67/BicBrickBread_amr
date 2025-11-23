/**
 * 
 * This file initializes the X-O game by creating the user interface, board, and players.
 * It then launches the game loop via the GameManager class.
 * All dynamically allocated objects are properly deleted at the end.
 */

#include <iostream> // Required for input/output operations (cout, cin)
#include <string>   // Required for string
#include <vector>   // Required for vector
#include <memory>   // Required for unique_ptr
#include "Games/XO_inf/XO_inf.h" 
#include "Games/Four_in_a_row/four.h" 
#include "Games/anti_XO/Anti_XO.h" 
#include "Games/Large_Tic_Tac_Toe/Large_Tic_Tac_Toe.h" ///> Required for the game Board and UI
#include "header\BoardGame_Classes.h"
#include "header\XO_Classes.h"

 
using namespace std;

/**
 * @brief Main function to run the X-O game.
 *
 * This function orchestrates the game by:
 * - Initializing the random number generator
 * - Creating the X-O specific UI and board
 * - Setting up players using the UI
 * - Running the main game loop through the GameManager
 * - Performing cleanup of dynamically allocated memory
 *
 * @return int Returns 0 on successful execution.
 */
int main() {

    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator

   
    // Temporary menu to test and run different games --Aalaa

    bool finish = 0;
    while (!finish) {
        int choice;
        cout << "Choose a game:\n";
        cout << "1)XO \n2)xo_inf\n3)Four_in_a_row\n4)Anti_XO\n5)5x5_XO\n6)Exit\n";
        cin >> choice;
        if (choice == 1) {
            // Create an instance of the specific UI for X-O using a pointer
            UI<char>* game_ui = new XO_UI();

            // Create the game board. For X-O, this is an X_O_Board.
            Board<char>* xo_board = new X_O_Board();

            // Use the UI to set up the players for the game.
            // The UI returns a dynamically allocated array of Player pointers.
            Player<char>** players = game_ui->setup_players();

            // Create the game manager with the board and the array of players.
            GameManager<char> x_o_game(xo_board, players, game_ui);

            // Run the game loop.
            x_o_game.run();

            // --- Cleanup ---
            // Delete the dynamically allocated board object.
            delete xo_board;

            // Delete the individual player objects.
            for (int i = 0; i < 2; ++i)
            {
                delete players[i];
            }
            // Delete the dynamically allocated array of player pointers itself.
            delete[] players;

        }
        else if (choice == 2) {
            UI<char>* game_ui = new XO_inf_UI();
            Board<char>* inf_board = new XO_inf_Board();
            Player<char>** players = game_ui->setup_players();
            GameManager<char> XO_inf_game(inf_board, players, game_ui);
            XO_inf_game.run();
            delete inf_board;
            for (int i = 0; i < 2; ++i) {
                delete players[i];
            }
            delete[] players;
        }

        else if (choice==3){
            UI<char>* game_ui = new FOUR_UI();
            Board<char>* four_board = new FOUR_Board();
            Player<char>** players = game_ui->setup_players();
            GameManager<char> FOUR_game(four_board, players, game_ui);
            FOUR_game.run();
            delete four_board;
            for (int i = 0; i < 2; ++i) {
                delete players[i];
            }
            delete[] players;
        }

        else if (choice == 4) {
            UI<char>* game_ui = new Anti_XO_UI();
            Board<char>* anti_board = new Anti_XO_Board();
            Player<char>** players = game_ui->setup_players();
            GameManager<char> anti_game(anti_board, players, game_ui);
            anti_game.run();
            delete anti_board;
            for (int i = 0; i < 2; ++i) {
                delete players[i];
            }
            delete[] players;
        }

        else if (choice == 5) {
            UI<char>* game_ui = new Large_XO_UI();
            Board<char>* large_board = new Large_XO_Board();
            Player<char>** players = game_ui->setup_players();
            GameManager<char> large_game(large_board, players, game_ui);
            large_game.run();
            delete large_board;
            for (int i = 0; i < 2; ++i) {
                delete players[i];
            }
            delete[] players;
        }
        
        else if (choice == 6) {
            finish = 1;
        }
    }
    return 0; // Exit successfully
}

// =====================================================================
