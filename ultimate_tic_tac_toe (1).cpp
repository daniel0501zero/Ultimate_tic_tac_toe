#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include "tic_tac_toe.h"
using namespace std;

//This function is used for checking the player or the bot has met the winning condition or not.
//Inputs: global board(3*3), character player
//Output: true or false
bool uttt_win_check(vector<vector<int>> global, char player){
    int _check = 0;
    if (player == 'O') _check = 1;
    else _check = 2;
    //check horizontal line
    for (int row = 0; row < global.size(); row++){
        if (global[row][0] == _check && global[row][1] == _check && global[row][2] == _check) 
            return true;
    }
    //check vertical line
    for (int col = 0; col < global[0].size(); col++){
        if (global[0][col] == _check && global[1][col] == _check && global[2][col] == _check)
            return true;
    }
    //check oblique line
    if ((global[0][0] == _check && global[1][1] == _check && global[2][2] == _check) ||(global[0][2] == _check && global[1][1] == _check && global[2][0] == _check)){
        return true;
    }
    return false;
}

//This function is used for checking the global board has met the draw condition or not.
//Inputs: global board(3*3)
//Output: true or false
bool uttt_draw_check(vector<vector<int>> global){
    int one_sum = 0, tw_sum = 0, thr_sum = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (global[i][j] == 2) tw_sum++;
            else if (global[i][j] == 1) one_sum++;
            else if (global[i][j] == 3) thr_sum++;
        }
    }
    if (one_sum + tw_sum + thr_sum == 9) return true;
    return false;
}

//This function is used for initializing the board (for display), and global board (for checking different conditions)
//Inputs: global board(3*3), board(9*9)
//Output: nothing
void initialize_board(vector<vector<char>> &board, vector<vector<int>> &global){
    board.assign(9, vector<char>(9, '-'));
    global.assign(3, vector<int>(3, 0));
}

//This function is the main function to run the game logic
//Inputs: mode
//Output: Depending on the mode selected. 
//        1 is for the single player mode, player can compete with the bot in this mode.
//        2 is for the two-player mode, players can compete with each other in this mode.
//        3 is for checking winning records of competing with the bot, numbers of steps(steps) and time spent(duration) in winning the game will be displayed.
//        4 is for player to quit when they are in the main menu.
int main(){
    vector<vector<char>> board;
    vector<vector<int>> global;
    board.resize(9, vector<char>(9, '-'));
    global.resize(3, vector<int>(3, 0));
    int board_no, temp_board_no, steps = 0, mode;
    char player = 'O', opponent = 'X';
    bool playing = true, in_lobby = true;
    //
    system("clear");
    while (in_lobby){
        mode = start_game();
        if (mode == 1 || mode == 2){ 
            steps = 0;
            initialize_board(board, global);
            auto start = chrono::steady_clock::now();
            board_animation(board, start, steps, mode);
            cout << "The game begins with player " << player << endl;
            cout << "Please choose the starting board[1-9]:" << endl;
            while (true){
                cin >> board_no;
                if (cin.fail()) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cout << "Please input an integer, not other types of data." << endl;
                    continue; 
                }

                if (board_no >= 1 && board_no <= 9) {
                    break; 
                } 
                else {
                    cout << "The board number you have chosen is out of range [1-9], please input again." << endl;
                }
            }
            start = chrono::steady_clock::now();
            player = 'O';
            opponent = 'X';
            while (playing){
                if (mode == 1) player = 'O';
                else if (mode == 2) player = (opponent == 'X')?'O':'X';
                player_move(global, board, board_no, temp_board_no, player);
                display_board(board, start, steps, mode);
                if (check_win(temp_board_no, board, player)){
                    cout << "You(Player " << player << ") win in board [" << temp_board_no << "]! Keep it going!"<< endl;
                    cout << '\n';
                    win_update_board(global, temp_board_no, player);
                    if (uttt_win_check(global, player)){
                        auto end = chrono::steady_clock::now();
                        auto duration = chrono::duration_cast<chrono::seconds>(end-start).count();
                        cout << "Congratulations! You(Player " << player << ") win the game! " << endl; 
                        cout << "Press Enter to continue" << endl; 
                        while (true){
                            cin.get();
                            if (cin.get() == '\n') break;
                            else cout << "Please press enter to proceed";
                        }
                        if (mode == 1){
                            ofstream output("loadgame.txt", ios::app);
                            if (output.fail()){
                                cout << "error in adding records" << endl;
                                exit(1);
                            }
                            output << "Time to finish the game: " << duration/60 << " minutes " << duration%60 << " seconds" << endl;
                            output << "Number of steps: " << steps << endl;
                            output.close();
                        }
                        system("clear");
                        initialize_board(board, global);
                        break;
                    }
                    else {
                        if (uttt_draw_check(global)){
                            cout << "Next Round maybe... (draw)" << endl;
                            cout << "Press Enter to continue" << endl; 
                            while (true){
                                cin.get();
                                if (cin.get() == '\n') break;
                                else cout << "Please press enter to proceed";
                            }
                            system("clear");
                            initialize_board(board, global);
                            break;
                        }
                    }
                }
                else {
                    if (check_draw(temp_board_no, board, player)){
                        cout << "Board [" << temp_board_no << "] is draw. " << endl;
                        cout << '\n';
                        draw_update_board(global, temp_board_no);
                        if (uttt_draw_check(global)){
                            cout << "Draw!" << endl;
                            cout << "Press Enter to continue" << endl; 
                            while (true){ 
                                cin.get();
                                if (cin.get() == '\n') break;
                                else cout << "Please press enter to proceed";
                            }
                            system("clear");
                            initialize_board(board, global);
                            break;
                        }
                    }         
                }
                if (mode == 2){
                    opponent = (opponent == 'O')?'X':'O';
                    continue;
                }
                steps++;
                if (mode == 1){
                    player = 'X';
                    this_thread::sleep_for(chrono::seconds(1));
                    bot_move(global, board, board_no, temp_board_no);
                    display_board(board, start, steps, mode);
                    if (check_win(temp_board_no, board, player)){
                        cout << "the player " << player << " wins in board [" << temp_board_no << "]!" << endl;
                        cout << '\n';
                        win_update_board(global, temp_board_no, player);
                        if (uttt_win_check(global, player)){
                            cout << "You lost... I thought you were different..." << endl;
                            cout << "Press Enter to continue" << endl; 
                            while (true){    
                                cin.get();
                                if (cin.get() == '\n') break;
                                else cout << "Please press enter to proceed";
                            }
                            system("clear");
                            initialize_board(board, global);
                            break;
                        }
                        else {
                            if (uttt_draw_check(global)){
                                cout << "You can't beat it? (draw)" << endl;
                                cout << "Press Enter to continue" << endl; 
                                while (true){ 
                                    cin.get();
                                    if (cin.get() == '\n') break;
                                    else cout << "Please press enter to proceed";
                                }
                                system("clear");
                                initialize_board(board, global);
                                break;
                            }
                        }
                    }
                    else {
                        if (check_draw(temp_board_no, board, player)){
                            cout << "Board [" << temp_board_no << "] is draw. " << endl;
                            cout << '\n';
                            draw_update_board(global, temp_board_no);
                            if (uttt_draw_check(global)){
                                cout << "Draw!" << endl;
                                cout << "Press Enter to continue" << endl;
                                while (true){
                                    cin.get();
                                    if (cin.get() == '\n') break;
                                    else cout << "Please press enter to proceed";
                                }
                                system("clear");
                                initialize_board(board, global);
                                break;
                            }
                        } 
                    }
                    this_thread::sleep_for(chrono::seconds(1));
                }
            }
        }
        else if (mode == 3){
            ifstream file("loadgame.txt");
            if (file.fail()){
                cout << "error in opening the records!" << endl;
                exit(1);
            }
            string line;
            cout << "Recent Game Records:\n";
            while (getline(file, line)) {
                cout << line << endl; // Display each record
            }
            file.close();
        }
        else if (mode == 4) {
            system("clear");
            end_game();
            break;
        }
    }
    //
    return 0;
}