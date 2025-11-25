#ifndef Large_Tic_Tac_Toe

#include "../../header/BoardGame_Classes.h"
// #include "../../header/AI.h"


/**
 * @brief Board of 5x5 XO
 */
class Large_XO_Board : public Board<char>
{
public:
    /**
     * @brief Construct a 5x5 board
     */
    Large_XO_Board();

    /**
     * @brief Update board when the player play a move
     * @param move The move of the player
     * @return Bool to identify if it is a valid move or not
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Check if the player lose or not
     * @param player The player which we want to check
     * @return True if he lose, otherwise false
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Check if the player wins or not
     * @param player The player which we want to check
     * @return True if he wins, otherwise false
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Check if it is a draw or not
     * @param player The player which we want to check
     * @return True if there is a draw, otherwise false
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Check if the game has ended
     * @param player The player which we want to check
     * @return True if there is draw, win,or lose , otherwise false
     */
    bool game_is_over(Player<char>* player) override;

private:
    char emptyCell; ///> Empty Cell value
};


class Large_XO_UI : public UI<char>
{
public:
    /**
     * @brief Construct the 5x5 XO_UI
     */
    Large_XO_UI();
    ~Large_XO_UI() {}

    /**
     * @brief Setup Players
     */
    Player<char>** setup_players() override;

    /**
     * @brief Create new player
     * @param name The name of the player
     * @param symbol The Symbol the player has on the board
     * @param type Type of the player whether AI, Player, Computer
     * @return Pointer to the created Player
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Play the move
     * @param player The player which we want to check
     * @return The move object to be checked
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif // Large_Tic_Tac_Toe