#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>

//----------------------- Game-Rules Related ---------------------------//
#define BOARD_MAX_SIZE 9
#define BOARD_MIN_SIZE 3
#define MIN_TOKENS 3
#define NUM_OF_INIT_INPUTS 4

//----------------------- Input Identifiers ----------------------------//
#define COLOR_INDEX 0
#define ROW_INDEX 1
#define COLUMN_INDEX 2
#define TOKEN_INDEX 3

//----------------------- Game Identifiers ----------------------------//
#define TIE -1

//----------------------- Symbol Identifiers ---------------------------//
#define RED_SLOT_SYMBOL ('R')
#define YELLOW_SLOT_SYMBOL ('Y')
#define EMPTY_SLOT_SYMBOL (' ')

//----------------------- Message Identifiers --------------------------//
#define MSG_GET_BOARD_ROWS   0
#define MSG_GET_BOARD_COLS   1
#define MSG_GET_NUMBER_TOKENS 2

//--------------------------- Board Edges ------------------------------//
#define BOARD_VERT_SEP  '|'
#define BOARD_LEFT_ANG '\\'
#define BOARD_RIGHT_ANG '/'
#define BOARD_BOTTOM  '-'
#define BOARD_BOTTOM_SEP '-'

//----------------------------------------------------------------------//
//Init Functions
int game_init(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS]);
char get_color();
int get_rows();
int get_columns(int rows);
int get_tokens(int rows, int columns);
void board_init(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns);

//Game Functions
int game_manager(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS]);
void game_input_manager(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS], int in, bool* pplayer, bool* shouldPrint);
int add_to_board(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int req_column, bool player, char player1);
int is_endOfGame(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens, char player1);
void is_row_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens, char* player1, char* player2, int* winner);
void is_column_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens, char* player1, char* player2, int* winner);
void is_up_diag_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens, char* player1, char* player2, int* winner);
void is_down_diag_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens, char* player1, char* player2, int* winner);
int is_full(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int* winner);

//Print Functions
void print_welcome_message();
void print_read_game_params_message(int param);
void print_choose_color_message();
void print_seq(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int r, int c, int player);
void print_choose_move_message(int player);
void print_enter_column_message();
void print_full_column_message();
void print_unavailable_undo_redo_message();
void print_board(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int r, int c);
int print_winner(int player_id, char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int r, int c);


//--------------------------- Main Program -------------------------------//
int main()
{
    char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE];
    char inputs[NUM_OF_INIT_INPUTS];
    if (game_init(board, inputs)) {
        return 1;   //if scanf failed
    }
    if (game_manager(board, inputs)) {
        return 1; //if scanf failed
    }
    return 0;
}

//--------------------------- Init Functions -----------------------------//

//getting the necessary inputs from the user in order to initialize the game, and then initializing the game board
//returns 1 if scanf failed and 0 if ok
int game_init(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS])
{   //function length: 11 rows
    print_welcome_message();
    for (int i = 0; i < NUM_OF_INIT_INPUTS; i++) {
        switch (i){
            case COLOR_INDEX: inputs[i] = get_color(); break;
            case ROW_INDEX: inputs[i] = get_rows(); break;
            case COLUMN_INDEX: inputs[i] = get_columns(inputs[ROW_INDEX]); break;
            case TOKEN_INDEX: inputs[i] = get_tokens(inputs[ROW_INDEX], inputs[COLUMN_INDEX]); break;
        }
        if (inputs[i] == 0) {
            return 1;   //if scanf failed
        }
    }
    board_init(board,inputs[ROW_INDEX],inputs[COLUMN_INDEX]);
    return 0;
}

//getting the starting color from the user
char get_color()
{   //function length: 6 rows
    char color;
    do {
        print_choose_color_message();
        if (scanf(" %c", &color) != 1) {
            return 0;
        }
    } while (color != YELLOW_SLOT_SYMBOL && color != RED_SLOT_SYMBOL);
    return color;
}

//getting number of rows from the user
int get_rows() 
{   //function length: 6 rows
    int rows;
    do {
        print_read_game_params_message(MSG_GET_BOARD_ROWS);
        if (scanf(" %d", &rows) != 1) {
            return 0;
        }
    } while (rows < BOARD_MIN_SIZE || rows > BOARD_MAX_SIZE);
        return rows;
}

//getting number of columns from the user
int get_columns(int rows)
{   //function length: 6 rows
    int columns;
    do {
        print_read_game_params_message(MSG_GET_BOARD_COLS);
        if (scanf(" %d", &columns) != 1) {
            return 0;
        }
    } while (columns < BOARD_MIN_SIZE || columns > BOARD_MAX_SIZE || columns < rows);
    return columns;
}

//getting number of tokens from the user
int get_tokens(int rows, int columns)
{   //function length: 6 rows
    int tokens;
    do {
        print_read_game_params_message(MSG_GET_NUMBER_TOKENS);
        if (scanf(" %d", &tokens) != 1) {
            return 0;
        }
    } while (tokens < MIN_TOKENS || tokens > rows || tokens > columns);
    return tokens;
}

//initalizing the game board with empty spaces according to the amount of rows and columns inserted by the user
void board_init(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns)
{   //function length: 3 rows
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = EMPTY_SLOT_SYMBOL;
        }
    }
}

//------------------------ Game Functions ------------------------------//

//everything related to game management - takes control after init
//returns 1 if scanf failed and 0 if ok
int game_manager(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS])
{   //function length: 
    bool player = false, shouldPrint = true;
    int in;
    do {
        if (shouldPrint) {
            print_seq(board, inputs[ROW_INDEX], inputs[COLUMN_INDEX], player);
        }
        if (scanf(" %d", &in) != 1) {
            return 1;
        }
        shouldPrint = false;
        game_input_manager(board, inputs, in, &player, &shouldPrint);
    } while (!is_endOfGame(board, inputs[ROW_INDEX], inputs[COLUMN_INDEX], inputs[TOKEN_INDEX], inputs[COLOR_INDEX]));
    return 0;
}

//getting inputs from users and updating the game if needed
void game_input_manager(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS], int in, bool *pplayer, bool *shouldPrint) 
{   //function length:
    if (in > 0 && in <= inputs[COLUMN_INDEX]) {     //checking if the column input in an actual column
        if (add_to_board(board, inputs[ROW_INDEX], inputs[COLUMN_INDEX], in, *pplayer, inputs[COLOR_INDEX])) {
            print_full_column_message();
            print_enter_column_message();
            return;
        }
        *pplayer = !*pplayer;   //changing which player's turn it is
        *shouldPrint = true;
        return;
    }
    switch (in) {
        //   case -1: undo(); break;
        //   case -2: redo(); break;
    default: print_enter_column_message(); break;
    }
}

//adding to the game board as requested by user
//returns 1 if column is full, 0 if added
int add_to_board(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int req_column, bool player, char player1)
{//function length: 
    char player2 = player1 == RED_SLOT_SYMBOL ? YELLOW_SLOT_SYMBOL : RED_SLOT_SYMBOL;
    char add = player ? player2 : player1;
    for (int i = rows-1; i >= 0; i--) {
        if (board[i][req_column - 1] == EMPTY_SLOT_SYMBOL) {
            board[i][req_column - 1] = add;
            return 0;
        }
    }
    return 1;
}

//checking if the game has ended
//returns 1 if ended, 0 if not
int is_endOfGame(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens, char player1)
{//function length:
    char player2 = player1 == RED_SLOT_SYMBOL ? YELLOW_SLOT_SYMBOL : RED_SLOT_SYMBOL;
    int winner=0;
    is_row_winner(board, rows, columns, tokens, &player1, &player2, &winner);
    is_column_winner(board, rows, columns, tokens, &player1, &player2, &winner);
    is_up_diag_winner(board, rows, columns, tokens, &player1, &player2, &winner);
    is_down_diag_winner(board, rows, columns, tokens, &player1, &player2, &winner);
    if (winner != 0)
        return print_winner(winner, board, rows, columns);
    is_full(board, rows, columns, &winner);
    return print_winner(winner, board, rows, columns);
}

//checking if the game has ended due to a row-sequence
void is_row_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens, char* player1, char *player2, int* winner)
{//function length: 
    int count = 0;
    char prev = EMPTY_SLOT_SYMBOL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if ((prev == EMPTY_SLOT_SYMBOL || prev == board[i][j]) && (board[i][j] == *player1 || board[i][j] == *player2)) {
                count++;
            }
            else {
                count = 0;
            }
            prev = board[i][j];
            if (count == tokens) {
                *winner = board[i][j] == *player1 ? 1 : 2;
                return;
            }
        }
        count = 0;
        prev = EMPTY_SLOT_SYMBOL;
    }
}

//checking if the game has ended due to a column-sequence
void is_column_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens, char* player1, char* player2, int* winner)
{//function length: 
    int count = 0;
    char prev = EMPTY_SLOT_SYMBOL;
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i < rows; i++) {
            if ((prev == EMPTY_SLOT_SYMBOL || prev == board[i][j]) && (board[i][j] == *player1 || board[i][j] == *player2)) {
                count++;
            }
            else {
                count = 0;
            }
            prev = board[i][j];
            if (count == tokens) {
                *winner = board[i][j] == *player1 ? 1 : 2;
                return;
            }
        }
        count = 0;
        prev = EMPTY_SLOT_SYMBOL;
    }
}

//checking if the game has ended due to an upward-digonal-sequence
void is_up_diag_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens, char* player1, char* player2, int* winner)
{//function length: 
    int count = 0;
    char prev = EMPTY_SLOT_SYMBOL;
    for (int d = 0; d <= columns+rows-2; d++) {
        for (int i = 0; i <= d&&i<rows; i++) {
            if (i + columns <= d) {
                continue;
            }
            if ((prev == EMPTY_SLOT_SYMBOL || prev == board[i][d-i]) && (board[i][d-i] == *player1 || board[i][d-i] == *player2)) {
                count++;
            }
            else {
                count = 0;
            }
            prev = board[i][d-i];
            if (count == tokens) {
                *winner = board[i][d-i] == *player1 ? 1 : 2;
                return;
            }
        }
        count = 0;
        prev = EMPTY_SLOT_SYMBOL;
    }
}

//checking if the game has ended due to a downward-diagonal-sequence
void is_down_diag_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens, char* player1, char* player2, int* winner)
{//function length:
    int count = 0;
    char prev = EMPTY_SLOT_SYMBOL;
    for (int d = -columns+1; d < rows; d++) {
        for (int i = 0; i < d+columns && i < rows; i++) {
            if (i < d) {
                continue;
            }
            if ((prev == EMPTY_SLOT_SYMBOL || prev == board[i][i-d]) && (board[i][i-d] == *player1 || board[i][i-d] == *player2)) {
                count++;
            }
            else {
                count = 0;
            }
            prev = board[i][i-d];
            if (count == tokens) {
                *winner = board[i][i-d] == *player1 ? 1 : 2;
                return;
            }
        }
        count = 0;
        prev = EMPTY_SLOT_SYMBOL;
    }
}

//checking if the board is full
//returns 1 if full, 0 if not
int is_full(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int *winner)
{//function length:
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == EMPTY_SLOT_SYMBOL) {
                return 0;
            }
        }
    }
    *winner = TIE;
    return 1;
}

//---------------------- Printing Functions ----------------------------//

// message number 0 = "Welcome to 4-in-a-row game! \n"
void print_welcome_message()
{   //function length: 1 row
    printf("Welcome to 4-in-a-row game! \n");
}

// message number 1 = "Please enter number of rows:"
// message number 2 = "Please enter number of columns:"
// message number 3 = "Please enter number of tokens:"
void print_read_game_params_message(int param)
{   //function length: 2 rows
    char const* const possible_params[] = { "rows", "columns", "tokens" };
    printf("Please enter number of %s: ", possible_params[param]);
}

// message number 4 = "Please choose starting color (Y)ellow or (R)ed: "
void print_choose_color_message()
{   //function length: 1 row
    printf("Please choose starting color (Y)ellow or (R)ed: ");
}

void print_seq(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int r, int c, int player)
{
    print_board(board, r, c);
    print_choose_move_message(player);
    print_enter_column_message();
}

// message number 5 = "Your move, player <player>. "
void print_choose_move_message(int player)
{   //function length: 1 row
    printf("Your move, player %d. ", player+1);
}

// message number 6 = "Please enter column: "
void print_enter_column_message()
{   //function length: 1 row
    printf("Please enter column: ");
}

// message number 7 = "Column full. "
void print_full_column_message()
{   //function length: 1 row
    printf("Column full. ");
}

//message number 8 = "No moves to undo/redo. "
void print_unavailable_undo_redo_message()
{   //function length: 1 row
    printf("No moves to undo/redo. ");
}

//Outputs game board
void print_board(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int r, int c)
{   //function length: 10 rows
    /*
    assert(IS_IN_RANGEI(board_side[0], 0, BOARD_MAX_SIZE)
        && IS_IN_RANGEI(board_side[1], 0, BOARD_MAX_SIZE));
    */
    for (int row = 0; row < r; ++row)
    {
        printf("\n%c", BOARD_VERT_SEP);
        for (int col = 0; col < c; ++col)
            printf("%c%c", board[row][col], BOARD_VERT_SEP);
    }
    printf("\n%c", BOARD_LEFT_ANG);
    for (int ii = 0; ii < c - 1; ++ii)
        printf("%c%c", BOARD_BOTTOM, BOARD_BOTTOM_SEP);
    printf("%c%c\n", BOARD_BOTTOM, BOARD_RIGHT_ANG);
}

//Outputs winner or tie message, with game board
//returns 1 if the game has ended, 0 if not
int print_winner(int player_id, char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int r, int c)
{   //function length: 7 rows
    if(player_id==0){
        return 0;
    }
    print_board(board, r, c);
    if (player_id > 0)
        printf("Player %d won! \n", player_id);
    else
        printf("That's a tie. \n");
    return 1;
}