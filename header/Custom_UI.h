#ifndef CUSTOM_UI_H
#define CUSTOM_UI_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <stdexcept>

/**
 * @file Custom_UI.h
 * @brief Defines a customizable user interface for board games.
 */

using std::string;
using std::vector;
using std::cout;
using std::cin;

/**
 * @class Custom_UI
 * @brief Provides a customizable UI supporting multiple player types.
 *
 * @tparam T Symbol type used for the game.
 */
template <class T>
class Custom_UI : public UI<T> {
protected:

    /**
     * @brief Ask the user to choose a player type and validate input.
     *
     * @param player_label "Player X" or "Player O".
     * @param options The list of type names to display.
     */
    PlayerType get_player_type_choice(string player_label, const vector<string>& options) override {
        cout << "\n========== Player Type Selection ==========\n";
        cout << "Choose " << player_label << " type:\n\n";

        for (size_t i = 0; i < options.size(); ++i)
            cout << "  " << (i + 1) << ") " << options[i] << "\n";

        cout << "\nEnter choice (1-" << options.size() << "): ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            throw std::runtime_error("Error: Choice must be an integer.");
        }

        if (choice < 1 || choice > (int)options.size()) {
            throw std::runtime_error("Error: Choice must be between 1 and 4.");
        }

        if (choice == 1) return PlayerType::HUMAN;
        if (choice == 2) return PlayerType::COMPUTER;
        if (choice == 3) return PlayerType::AI;
        return PlayerType::RANDOM;
    }

public:

    Custom_UI(int cell_display_width = 3)
        : UI<T>(cell_display_width) {}

    Custom_UI(string message, int cell_display_width)
        : UI<T>(message, cell_display_width) {}

    /**
     * @brief Display any message and pause.
     */
    virtual void display_message(string message) override {
        std::cout << "\n>>> " << message << "\n\n";
        system("pause");
    }

    /**
     * @brief Create a player based on name, symbol, and type.
     */
    virtual Player<T>* create_player(std::string& name, T symbol, PlayerType type) override {

        cout << "\n-------------------------------------------\n";
        cout << "Creating player...\n";

        // Random type selection
        if (type == PlayerType::RANDOM) {
            int i = std::rand() % 3;
            if (i == 0)      type = PlayerType::HUMAN;
            else if (i == 1) type = PlayerType::COMPUTER;
            else             type = PlayerType::AI;
        }

        cout << "  Name   : " << name   << "\n";
        cout << "  Symbol : " << symbol << "\n";
        cout << "  Type   : ";

        if (type == PlayerType::HUMAN)        cout << "Human\n";
        else if (type == PlayerType::COMPUTER) cout << "Computer\n";
        else if (type == PlayerType::AI)        cout << "AI\n";

        cout << "-------------------------------------------\n";

        return new Player<T>(name, symbol, type);
    }

    /**
     * @brief Setup X and O players.
     */
    virtual Player<T>** setup_players() override {
        Player<T>** players = new Player<T>*[2];

        vector<string> type_options = { "Human", "Computer", "AI", "Random" };

        // Player X
        string nameX = this->get_player_name("Player X");
        PlayerType typeX = get_player_type_choice("Player X", type_options);
        players[0] = create_player(nameX, static_cast<T>('X'), typeX);

        // Player O
        string nameO = this->get_player_name("Player O");
        PlayerType typeO = get_player_type_choice("Player O", type_options);
        players[1] = create_player(nameO, static_cast<T>('O'), typeO);

        return players;
    }

    /**
     * @brief Displays a 2D board matrix in a formatted grid.
     *
     * This function clears the screen, prints column indices,
     * draws horizontal separators, and prints each row of the matrix.
     *
     * @tparam T  The type stored in each cell (e.g., char).
     * @param matrix  The 2D matrix representing the board.
     */
    void display_board_matrix(const vector<vector<T>>& matrix) const override {
        // system("cls");
        
        UI<T>::display_board_matrix(matrix);
    }
};

#endif // CUSTOM_UI_H