#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

using namespace std;

bool check_draw(int board_no, vector<vector<char>> board, char player);
bool check_win(int board_no, vector<vector<char>> board, char player);
void win_update_board(vector<vector<int>> &global, int board_no, char player);
void draw_update_board(vector<vector<int>> &global, int board_no);
int start_game();
void end_game();
void display_board(const vector<vector<char>> board, chrono::steady_clock::time_point start, int steps, int mode);
void board_animation(vector<vector<char>> board, chrono::steady_clock::time_point start, int steps, int mode);
void player_move(vector<vector<int>> global, vector<vector<char>> &board, int &board_no, int &temp_board_no, char player);
void bot_move(vector<vector<int>> global, vector<vector<char>> &board, int &board_no, int &temp_board_no);

#endif 
