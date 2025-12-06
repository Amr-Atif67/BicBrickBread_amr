/**
 * 
 * This file initializes the X-O game by creating the user interface, board, and players.
 * It then launches the game loop via the GameManager class.
 * All dynamically allocated objects are properly deleted at the end.
 */

#include <iostream>    // Required for input/output operations (cout, cin)
#include <string>      // Required for string
#include <vector>      // Required for vector
#include <memory>      // Required for unique_ptr
#include <limits>      // Required for input clearing
#include <stdexcept>   // Required for exceptions

#include <iostream>    // Required for input/output operations (cout, cin)
#include <string>      // Required for string
#include <vector>      // Required for vector
#include <memory>      // Required for unique_ptr
#include <limits>      // Required for input clearing
#include <stdexcept>   // Required for exceptions

#include "Games/SUS/SUS.h"
#include "Games/XO_inf/XO_inf.h" 
#include "Games/Four_in_a_row/four.h" 
#include "Games/XO_num/xo_num.h" 
#include "Games/Ultimate_Tic_Tac_Toe/Ultimate.h" 
#include "Games/anti_XO/Anti_XO.h" 
#include "Games/Large_Tic_Tac_Toe/Large_Tic_Tac_Toe.h" ///> Required for the game Board and UI
#include "Games/PyramidXO/PyramidXO.h" ///> Required for the game Board and UI
#include "header/BoardGame_Classes.h"
#include "header/XO_Classes.h"

#include "header/BoardGame_Classes.h"
#include "header/XO_Classes.h"

using namespace std;

/**
 * @brief Main function to run the X-O game.
 *
 * This function manage the game by:
 * This function manage the game by:
 * - Initializing the random number generator
 * - Creating the X-O specific UI and board
 * - Setting up players using the UI
 * - Running the main game loop through the GameManager
 * - Performing cleanup of dynamically allocated memory
 *
 * @return int Returns 0 on successful execution.
 */



 // Welcome to BicBrickBread!
/*
⠄⠄⠄⠄⢠⣿⣿⣿⣿⣿⢻⣿⣿⣿⣿⣿⣿⣿⣿⣯⢻⣿⣿⣿⣿⣆⠄⠄⠄
⠄⠄⣼⢀⣿⣿⣿⣿⣏⡏⠄⠹⣿⣿⣿⣿⣿⣿⣿⣿⣧⢻⣿⣿⣿⣿⡆⠄⠄
⠄⠄⡟⣼⣿⣿⣿⣿⣿⠄⠄⠄⠈⠻⣿⣿⣿⣿⣿⣿⣿⣇⢻⣿⣿⣿⣿⠄⠄
⠄⢰⠃⣿⣿⠿⣿⣿⣿⠄⠄⠄⠄⠄⠄⠙⠿⣿⣿⣿⣿⣿⠄⢿⣿⣿⣿⡄⠄
⠄⢸⢠⣿⣿⣧⡙⣿⣿⡆⠄⠄⠄⠄⠄⠄⠄⠈⠛⢿⣿⣿⡇⠸⣿⡿⣸⡇⠄
⠄⠈⡆⣿⣿⣿⣿⣦⡙⠳⠄⠄⠄⠄⠄⠄⢀⣠⣤⣀⣈⠙⠃⠄⠿⢇⣿⡇⠄
⠄⠄⡇⢿⣿⣿⣿⣿⡇⠄⠄⠄⠄⠄⣠⣶⣿⣿⣿⣿⣿⣿⣷⣆⡀⣼⣿⡇⠄
⠄⠄⢹⡘⣿⣿⣿⢿⣷⡀⠄⢀⣴⣾⣟⠉⠉⠉⠉⣽⣿⣿⣿⣿⠇⢹⣿⠃⠄
⠄⠄⠄⢷⡘⢿⣿⣎⢻⣷⠰⣿⣿⣿⣿⣦⣀⣀⣴⣿⣿⣿⠟⢫⡾⢸⡟⠄.
⠄⠄⠄⠄⠻⣦⡙⠿⣧⠙⢷⠙⠻⠿⢿⡿⠿⠿⠛⠋⠉⠄⠂⠘⠁⠞⠄⠄⠄
⠄⠄⠄⠄⠄⠈⠙⠑⣠⣤⣴⡖⠄⠿⣋⣉⣉⡁⠄⢾⣦⠄⠄⠄⠄⠄⠄⠄⠄
*/
int main() {

    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator

    // Temporary menu to test and run different games Aalaa, ALi Wael
    bool finish = false;

    while (!finish) {
        int choice = 0;

        // ======================= MENU =======================
        cout << "==================== X-O Game Menu ====================\n";
        cout << "1) Classic XO\n";
        cout << "2) Infinite XO (XO_inf)\n";
        cout << "3) Four in a Row\n";
        cout << "4) Anti XO\n";
        cout << "5) 5x5 XO (Large Tic-Tac-Toe)\n";
        cout << "6) Numerical Tic-Tac-Toe\n";
        cout << "7) Ultimate Tic-Tac-Toe\n";
        cout << "8) Pyramid XO\n";
        cout << "9) SUS \n";
        cout << "10) Exit\n";
        cout << "=======================================================\n";
        cout << "Enter your choice [1-10]: ";

        try {
            if (!(cin >> choice)) {
                throw runtime_error("Invalid input: must be an integer.");
            }

            if (choice < 1 || choice > 10) {
                throw out_of_range("Choice must be an integer between 1 and 6.");
            }

            if (choice == 10) {
                cout << "Exiting the game. Goodbye!\n";
                finish = true;
                continue;
            }

            // Pointers for the UI and Board
            UI<char>* game_ui = nullptr;
            Board<char>* game_board = nullptr;

            // ================= SELECT GAME =================
            switch (choice) {
                case 1:
                    game_ui = new XO_UI();
                    game_board = new X_O_Board();
                    break;
                case 2:
                    game_ui = new XO_inf_UI();
                    game_board = new XO_inf_Board();
                    break;
                case 3:
                    game_ui = new FOUR_UI();
                    game_board = new FOUR_Board();
                    break;
                case 4:
                    game_ui = new Anti_XO_UI();
                    game_board = new Anti_XO_Board();
                    break;
                case 5:
                   game_ui = new Large_XO_UI();
                   game_board = new Large_XO_Board();
                   break;
                case 6:
                    game_ui = new  XO_NUM_UI();
                    game_board = new XO_NUM_Board();
                    break;
                case 7:
                    game_ui = new Ultimate_UI();
                    game_board = new Ultimate_Board();
                    break;
                case 8:
                    game_ui = new  PyramidXO_UI();
                    game_board = new PyramidXO_Board();
                    break;
                case 9:
                    game_ui=new SUS_UI();
                    game_board= new SUS_Board();
                    break;
                default:
                    throw out_of_range("Unexpected choice value.");
            }

            // ================= SETUP PLAYERS =================
            cout << "\n--- Setting up players ---\n";
            Player<char>** players = game_ui->setup_players();

            // ================= START GAME =================
            cout << "\n--- Starting the game ---\n";
            GameManager<char> game(game_board, players, game_ui);
            game.run();
            cout << "\n--- Game finished ---\n";

           
            // ================= CLEANUP =================
            delete game_board;
            for (int i = 0; i < 2; ++i) delete players[i];
            delete[] players;
            delete game_ui;
            
        } catch (const exception& e) {
            cerr << "\n[Error] " << e.what() << "\nPlease try again.\n\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("pause");
            cout << "\n";
        }
    }

    return 0; // Exit successfully
}