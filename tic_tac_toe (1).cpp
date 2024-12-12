#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <tuple>
#include "tic_tac_toe.h"

using namespace std;

//This function is used for returning a tuple of variables 
//such that the minimum, maximum of the row and column in the game board corresponding to the board number will be computed.
//Inputs: board_no
//Output: tuple<int, int, int, int> -> min_row, min_col, max_row, max_col
tuple<int, int, int, int> min_max_row_col(int board_no){
    int factor1 = (board_no-1)/3;
    int factor2 = (board_no-1)%3;
    int min_col = (factor2)*3;
    int max_col = 2 + (factor2)*3;
    int min_row = (factor1)*3;
    int max_row = 2 + (factor1)*3;
    return make_tuple(min_row, min_col, max_row, max_col);
}

//This function is used for predicting the outcomes of the next move from the player
//such that any move that the player can win in next board will be avoided.
//Inputs: board, board_no
//Output: true or false
bool bot_Predict(vector<vector<char>> board, int board_no) {
    int min_row, min_col, max_row, max_col;
    tie(min_row, min_col, max_row, max_col) = min_max_row_col(board_no);
    
    // Check if the bot can win
    for (int i = 0; i < 2; ++i) {
        char player = (i == 0) ? 'X' : 'O';
        for (int row = min_row; row <= max_row; ++row) {
            // Horizontal check
            if (board[row][min_col] == '-' && board[row][min_col+1] == player && board[row][max_col] == player)
                return true;
            else if (board[row][min_col] == player && board[row][min_col+1] == '-' && board[row][max_col] == player)
                return true;
            else if (board[row][min_col] == player && board[row][min_col+1] == player && board[row][max_col] == '-')
                return true;
        }
        for (int col = min_col; col <= max_col; ++col) {
            // Vertical check
            if (board[min_row][col] == '-' && board[min_row+1][col] == player && board[max_row][col] == player)
                return true;
            else if (board[min_row][col] == player && board[min_row+1][col] == '-' && board[max_row][col] == player)
                return true;
            else if (board[min_row][col] == player && board[min_row+1][col] == player && board[max_row][col] == '-')
                return true;
        }
        // Diagonal check
        if (board[min_row][min_col] == '-' && board[min_row+1][min_col+1] == player && board[max_row][max_col] == player)
            return true;
        else if (board[min_row][max_col] == '-' && board[min_row+1][min_col+1] == player && board[max_row][min_col] == player)
            return true;
        else if ((board[min_row][min_col] == player && board[min_row+1][min_col+1] == '-' && board[max_row][max_col] == player) || (board[min_row][max_col] == player && board[min_row+1][min_col+1] == '-' && board[max_row][min_col] == player))
            return true;
        else if (board[min_row][max_col] == player && board[min_row+1][min_col+1] == player && board[max_row][min_col] == '-')
            return true;
        else if (board[min_row][min_col] == player && board[min_row+1][min_col+1] == player && board[max_row][max_col] == '-')
            return true;
    }
    return false;
}

//This function is used for checking the draw condition in the game board corresponding to the board number.
//Inputs: board, board_no, player
//Output: true or false
bool check_draw(int board_no, vector<vector<char>> board, char player){
    int x_sum = 0, o_sum = 0;
    int min_row, min_col, max_row, max_col;
    tie(min_row, min_col, max_row, max_col) = min_max_row_col(board_no);
    for (int i = min_row; i <= max_row; i++){
        for (int j = min_col; j <= max_col; j++){
            if (board[i][j] == 'X') x_sum++;
            else if (board[i][j] == 'O') o_sum++;
        }
    }
    if (x_sum + o_sum == 9) return true;
    return false;
}

//This function is used for checking the win condition in the game board corresponding to the board number.
//Inputs: board, board_no, player
//Output: true or false
bool check_win(int board_no, vector<vector<char>> board, char player){
    int min_row, min_col, max_row, max_col;
    tie(min_row, min_col, max_row, max_col) = min_max_row_col(board_no);
    //check horizontal line
    for (int i = min_row; i <= max_row; i++){
        if (board[i][min_col] == player && board[i][min_col+1] == player && board[i][max_col] == player)
            return true;
    }
    //check vertical line
    for (int i = min_col; i <= max_col; i++){
        if (board[min_row][i] == player && board[min_row+1][i] == player && board[max_row][i] == player)
            return true;
    }
    //check oblique line
    if ((board[min_row][min_col] == player && board[min_row+1][min_col+1] == player && board[max_row][max_col] == player) || 
    (board[min_row][max_col] == player && board[min_row+1][min_col+1] == player && board[max_row][min_col] == player)){
            return true;
    }
    return false;
}

//This function is used for resetting the board number once the next board for the player or the bot to move is finished
//by checking the global board,
//such that the player and bot can choose any board they want to except that board.
//Inputs: global, board_no
//Output: true or false
bool reset_board_no(vector<vector<int>> global, int board_no){
    int row = (board_no-1)/3;
    int col = (board_no-1)%3;
    if (global[row][col] != 0){
        return true;
    }
    return false;
}

//This function is used to update the global board
//once there is a player or the bot to win in the game board with corresponding board number.
//Inputs: global, board_no, player
//Output: nothing
void win_update_board(vector<vector<int>> &global, int board_no, char player){
    int row = (board_no-1)/3;
    int col = (board_no-1)%3;
    global[row][col] = (player == 'X')? 2 : 1;
}

//This function is used to update the global board 
//once there is a draw in the game board with corresponding board number.
//Inputs: global, board_no, player
//Output: nothing
void draw_update_board(vector<vector<int>> &global, int board_no){
    int row = (board_no-1)/3;
    int col = (board_no-1)%3;
    global[row][col] = 3;
}

//This function is used to update the board after the player has made its choice
//and check whether the board for the player to move is valid or not.
//Inputs: global, board, board_no, temp_board_no, player
//Output: nothing
void player_move(vector<vector<int>> global, vector<vector<char>> &board, int &board_no, int &temp_board_no, char player){
    if (reset_board_no(global, board_no)){
        int temp;
        cout << "Board[" << board_no << "] has already finished, player " << player << " can choose the another board now:" << endl;
        while (true){
            cin >> temp;
            int the_row = (temp-1)/3;
            int the_col = (temp-1)%3;
            if (global[the_row][the_col] == 0){
                break;
            }
            else cout << "Please select the board where the match is not over. " << endl;
        }
        board_no = temp;
    }
    char opponent;
    opponent = (player == 'O')?'X':'O';
    int n_row = 0, n_col = 0;
    int input;
    int min_row, min_col, max_row, max_col;
    tie(min_row, min_col, max_row, max_col) = min_max_row_col(board_no);
    while (true)
    {   cout << "In board number [" << board_no << "], player " << player << " please select one box(1-9):" << endl ;
        cin >> input;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Please input an integer, not other types of data." << endl;
            continue; 
        }
        if (input <= 9 && input >= 1){
            switch (input) {
                case 1: n_row = min_row; n_col = min_col; break;
                case 2: n_row = min_row; n_col = min_col+1; break;
                case 3: n_row = min_row; n_col = max_col; break;
                case 4: n_row = min_row+1; n_col = min_col; break;
                case 5: n_row = min_row+1; n_col = min_col+1; break;
                case 6: n_row = min_row+1; n_col = max_col; break;
                case 7: n_row = max_row; n_col = min_col; break;
                case 8: n_row = max_row; n_col = min_col+1; break;
                case 9: n_row = max_row; n_col = max_col; break;
                default: cout << "Wrong input, please try again" << '\n'; break;
            }
        }
        else {
            cout << "Box number out of range, please select it again." << endl;
            continue;
        }
        if (board[n_row][n_col] != opponent && board[n_row][n_col] == '-'){
            board[n_row][n_col] = player;
            temp_board_no = board_no;
            board_no = input;
            break;
        }
        else if (board[n_row][n_col] == opponent) cout << "Player " << opponent << " has already selected the box." << endl;
        else if (board[n_row][n_col] == player) cout << "You have chosen this box already." << endl;
    }
}

//This function is used by the bot_move fucntion
//such that it can return a number for the bot to win or block the player to win, otherwise randomly generate an available move.
//Inputs: board, board_no 
//Output: an integer
int bot_AI(vector<vector<char>> board, int board_no) {
    srand(static_cast<unsigned int>(time(0)));
    int min_row, min_col, max_row, max_col, next_board_no;
    bool guess = true;
    tie(min_row, min_col, max_row, max_col) = min_max_row_col(board_no);

    int fac_1 = 0, fac_2 = 0;
    //bot to win
    //horizontal
    for (int i = 0; i < 2; ++i) {
        char player = (i == 0) ? 'X' : 'O';
        fac_1 = 0, fac_2 = 0;
        for (int row = min_row; row <= max_row; ++row) {
            // Horizontal check
            if (board[row][min_col] == '-' && board[row][min_col+1] == player && board[row][max_col] == player)
                return (fac_1*3 + 1);
            else if (board[row][min_col] == player && board[row][min_col+1] == '-' && board[row][max_col] == player)
                return (fac_1*3 + 2);
            else if (board[row][min_col] == player && board[row][min_col+1] == player && board[row][max_col] == '-')
                return (fac_1*3 + 3);
            fac_1++;
        }
        for (int col = min_col; col <= max_col; ++col) {
            // Vertical check
            if (board[min_row][col] == '-' && board[min_row+1][col] == player && board[max_row][col] == player)
                return (1 + fac_2%3);
            else if (board[min_row][col] == player && board[min_row+1][col] == '-' && board[max_row][col] == player)
                return (4 + fac_2%3);
            else if (board[min_row][col] == player && board[min_row+1][col] == player && board[max_row][col] == '-')
                return (7 + fac_2%3);
            fac_2++;
        }
        // Diagonal check
        if (board[min_row][min_col] == '-' && board[min_row+1][min_col+1] == player && board[max_row][max_col] == player)
            return 1;
        else if (board[min_row][max_col] == '-' && board[min_row+1][min_col+1] == player && board[max_row][min_col] == player)
            return 3;
        else if ((board[min_row][min_col] == player && board[min_row+1][min_col+1] == '-' && board[max_row][max_col] == player) || (board[min_row][max_col] == player && board[min_row+1][min_col+1] == '-' && board[max_row][min_col] == player))
            return 5;
        else if (board[min_row][max_col] == player && board[min_row+1][min_col+1] == player && board[max_row][min_col] == '-')
            return 7;
        else if (board[min_row][min_col] == player && board[min_row+1][min_col+1] == player && board[max_row][max_col] == '-')
            return 9;
    }

    //fork situation
    if ((board[min_row][min_col] == '-' && board[min_row+1][min_col+1] == 'X') && (board[min_row][max_col] == 'X'|| board[max_row][min_col] == 'X'))
        return 1;
    else if ((board[min_row][max_col] == '-' && board[min_row+1][min_col+1] == 'X') && (board[min_row][min_col] == 'X'|| board[max_row][max_col] == 'X'))
        return 3;
    else if ((board[max_row][min_col] == '-' && board[min_row+1][min_col+1] == 'X') && (board[min_row][min_col] == 'X'|| board[max_row][max_col] == 'X'))
        return 7;
    else if ((board[max_row][max_col] == '-' && board[min_row+1][min_col+1] == 'X') && (board[min_row][max_col] == 'X'|| board[max_row][min_col] == 'X'))
        return 9;

    //if these if-else statements do not meet, then randomly generate a box number
    vector<int> available_moves;
    for (int row = min_row; row <= max_row; row++){
        for (int col = min_col; col <= max_col; col++){
            if (board[row][col] == '-') {
                available_moves.push_back((row%3)*3 + col%3 + 1);
            }
        }
    }

    if (!available_moves.empty()){
        for (auto &move : available_moves){
            next_board_no = move;
            if (bot_Predict(board, next_board_no)){
                guess = false;
                continue;
            }
            else {
                guess = true;
                break;
            }
        }
        if (guess) return next_board_no;
        else {
            next_board_no = available_moves[rand() % available_moves.size()];
            return next_board_no;
        }
    }
    return 6;
}

//This function is used to update the board after the bot has made its choice
//and check whether the board for the bot to move is valid or not.
//Inputs: global, board, board_no, temp_board_no, player
//Output: nothing
void bot_move(vector<vector<int>> global, vector<vector<char>> &board, int &board_no, int &temp_board_no){
    srand(static_cast<unsigned int>(time(0)));
    vector<int> board_choice; 
    if (reset_board_no(global, board_no)){
        cout << "Board [" << board_no << "] has already finished, player X can choose the another board now:" << endl;
        int temp;
        bool prediction = true;
        vector<int> available_board;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (global[i][j] == 0) available_board.push_back(i*3 + j + 1);
            }
        }
        for (auto &b : available_board){
            temp = b;
            int the_row = (temp-1)/3;
            int the_col = (temp-1)%3;
            if (global[the_row][the_col] == 0){
                if (bot_Predict(board, temp)){
                    prediction = true;
                    break;
                }
                else prediction = false;
            }
        }
        if (prediction == false){
            temp = available_board[rand() % available_board.size()]; 
        }
        board_no = temp;
        cout << "Player X chose board [" << board_no << "] " << endl;
        cout << "Press Enter to continue" << endl;
        while (true){
            cin.get();
            if (cin.get() == '\n') break;
                else cout << "Please press Enter to proceed";
        }
    }
    int n_row = 0, n_col = 0, box;
    int min_row, min_col, max_row, max_col;
    tie(min_row, min_col, max_row, max_col) = min_max_row_col(board_no);
    while (true){
        box = bot_AI(board, board_no);
        switch (box) {
          case 1: n_row = min_row; n_col = min_col; break;
          case 2: n_row = min_row; n_col = min_col+1; break;
          case 3: n_row = min_row; n_col = max_col; break;
          case 4: n_row = min_row+1; n_col = min_col; break;
          case 5: n_row = min_row+1; n_col = min_col+1; break;
          case 6: n_row = min_row+1; n_col = max_col; break;
          case 7: n_row = max_row; n_col = min_col; break;
          case 8: n_row = max_row; n_col = min_col+1; break;
          case 9: n_row = max_row; n_col = max_col; break;
          default: cout << "Wrong input, please try again" << '\n'; break;
        }
        if (board[n_row][n_col] != 'O' && board[n_row][n_col] == '-'){
            board[n_row][n_col] = 'X';
            temp_board_no = board_no;
            board_no = box;
            break;
        }
    }
}

//This function is for the display of the current state of the board
//and deliver necessary information in different modes.
//Inputs: board, start, steps, mode 
//Output: The game board with information (depends on the mode the user has chosen)
void display_board(const vector<vector<char>> board, chrono::steady_clock::time_point start, int steps, int mode) {
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_time = current_time - start;
    auto total_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count();
    
    int minutes = total_seconds / 60;
    int remainingSeconds = total_seconds % 60;
    system("clear");
    cout << "Current Board State:" << endl;
    cout << '\n';
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 'X') cout << "\033[31m" << board[i][j] << "\033[0m"; // Red for 'X'
            else if (board[i][j] == 'O') cout << "\033[34m" << board[i][j] << "\033[0m"; // Blue for 'O'
            else cout << board[i][j];
            if (j % 3 == 2 && j != 8) {
                cout << " | "; // Add vertical separators
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
        if (i % 3 == 2 && i != 8) cout << "---------------------" << endl; // Add horizontal separators
    }
    if (mode == 1){
        cout << endl;
        cout << "Game Time: " << minutes << " minutes " << remainingSeconds << " seconds" << endl;
        cout << "Number of Moves: " << steps << endl;
    }
    cout << endl;
}

//This function is for the display of the current state of the board
//and deliver necessary information in different modes and a smooth transition between the main menu and the display board.
//Inputs: board, start, steps, mode 
//Output: A loading screen and a game board with information (depends on the mode the user has chosen)
void board_animation(vector<vector<char>> board, chrono::steady_clock::time_point start, int steps, int mode){
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_time = current_time - start;
    auto total_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count();  
    int minutes = total_seconds / 60;
    int remainingSeconds = total_seconds % 60;
    system("clear");
    cout << "█████████████████████████████████████████████████████████████" << endl;
    cout << "█████████████████████████████████████████████████████████████" << endl;
    cout << "███                                                       ███" << endl;
    cout << "███                                                       ███" << endl;
    cout << "███                                                       ███" << endl;
    cout << "███                ||Ultimate Tic-Tac-Toe||               ███" << endl;
    cout << "███                   Game is starting...                 ███" << endl;
    cout << "███                                                       ███" << endl;
    cout << "███                                                       ███" << endl;
    cout << "███                                                       ███" << endl;
    cout << "█████████████████████████████████████████████████████████████" << endl;
    cout << "█████████████████████████████████████████████████████████████" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    system("clear");
    this_thread::sleep_for(chrono::milliseconds(4));
    cout << "Current Board State:" << endl;
    cout << '\n';
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 'X') {
                cout << "\033[31m" << board[i][j] << "\033[0m"; // Red for 'X'
                this_thread::sleep_for(chrono::milliseconds(4));
            } else if (board[i][j] == 'O') {
                cout << "\033[34m" << board[i][j] << "\033[0m"; // Blue for 'O'
                this_thread::sleep_for(chrono::milliseconds(4));
            } else {
                 cout << board[i][j];
                this_thread::sleep_for(chrono::milliseconds(4));
            }
            if (j % 3 == 2 && j != 8) {
                cout << " | "; // Add vertical separators
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
        if (i % 3 == 2 && i != 8) {
            cout << "---------------------" << endl; // Add horizontal separators
            this_thread::sleep_for(chrono::milliseconds(4));
        }
    }
    if (mode == 1){
        cout << endl;
        cout << "Game Time: " << minutes << " minutes " << remainingSeconds << " seconds" << endl;
        cout << "Number of Moves: " << steps << endl;
    }
    cout << endl;  
}

//This function is for the display of the main menu
//and it requires the input from the user
//such that the user can choose the mode inside the main menu.
//Inputs: an integer from 1 to 4
//Output: depends on the mode the user has chosen
int start_game(){
    int option;
    cout << "█████████████████████████████████████████████████████████████" << endl;
    cout << "█████████████████████████████████████████████████████████████" << endl;
    cout << "███                                                       ███" << endl;
    cout << "███           Welcome to Ultimate Tic-Tac-Toe!            ███" << endl;
    cout << "███                                                       ███" << endl;
    cout << "███                   1. Single-player                    ███" << endl;
    cout << "███                   2. Two-player                       ███" << endl;
    cout << "███                   3. Records                          ███" << endl;
    cout << "███                   4. Quit                             ███" << endl;
    cout << "███                                                       ███" << endl;
    cout << "█████████████████████████████████████████████████████████████" << endl;
    cout << "█████████████████████████████████████████████████████████████" << endl;
    cout << "Enter your option(1-4): ";
    cin >> option;
    system("clear");
    return option;
}

//This function is for the ending scene after the user input 4 (quit the game) in the main menu.
//Inputs: not required
//Output: an ending scene
void end_game(){
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "█████████████████████████████████████████████████████████████" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "█████████████████████████████████████████████████████████████" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "███                                                       ███" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "███                                                       ███" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "███                                                       ███" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "███                ||Ultimate Tic-Tac-Toe||               ███" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "███                      Thank You!                       ███" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "███                Hope you love this game!               ███" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "███                                                       ███" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "███                                                       ███" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "█████████████████████████████████████████████████████████████" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
    cout << "█████████████████████████████████████████████████████████████" << endl;
    this_thread::sleep_for(chrono::milliseconds(20));
}