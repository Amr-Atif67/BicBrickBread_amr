#ifndef Large_Tic_Tac_Toe

#include "../../header/BoardGame_Classes.h"
#include "../../header/Custom_UI.h"
#include "../../header/AI.h"
#include "../../Neural_Network/Include/NeuralNetwork.h"


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

    int countWin(char sym);
    int countTotal();
    
private:
    char emptyCell; ///> Empty Cell value
    int nMoves = 0;

};


class Large_XO_UI : public Custom_UI<char>
{
public:
    /**
     * @brief Construct the 5x5 XO_UI
     */
    Large_XO_UI();
    ~Large_XO_UI() {}

    /**
     * @brief Play the move
     * @param player The player which we want to check
     * @return The move object to be checked
     */
    Move<char>* get_move(Player<char>* player) override;

    std::pair<int,int> bestMove(Player<char>* player);
private:
    NeuralNetwork nn;
};

#endif // Large_Tic_Tac_Toe