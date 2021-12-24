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
#define RED 0
#define YELLOW 1
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
bool game_input_manager(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS], int in, bool* pplayer);
int add_to_board(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int req_column, bool player);
int is_endOfGame();

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
int print_winner(int player_id);


//--------------------------- Main Program -------------------------------//
int main()
{
    char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE];
    char inputs[NUM_OF_INIT_INPUTS];
    if (game_init(board, inputs)) {
        return 1;   //if scanf failed
    }
    //print_board(board, inputs[ROW_INDEX], inputs[COLUMN_INDEX]);
    if (game_manager(board, inputs)) {
        return 1; //if scanf failed
    }
    return 0;
}

//--------------------------- Init Functions -----------------------------//

//getting the necessary inputs from the user in order to initialize the game, and then initializing the game board
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
    } while (color != 'Y' && color != 'R');
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
            board[i][j] = ' ';
        }
    }
}

//------------------------ Game Functions ------------------------------//

int game_manager(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS])
{
    bool player = inputs[COLOR_INDEX] == 'R' ? RED : YELLOW, shouldPrint = true;
    int in;
    do {
        if (shouldPrint) {
            print_seq(board, inputs[ROW_INDEX], inputs[COLUMN_INDEX], player);
        }
        if (scanf(" %d", &in) != 1) {
            return 1;
        }
        shouldPrint = game_input_manager(board, inputs, in, &player);
    } while (!is_endOfGame(board, inputs[ROW_INDEX], inputs[COLUMN_INDEX], inputs[TOKEN_INDEX]));
    return 0;
}

bool game_input_manager(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS], int in, bool *pplayer) 
{
    if (in > 0 && in <= inputs[COLUMN_INDEX]) {
        if (add_to_board(board, inputs[ROW_INDEX], inputs[COLUMN_INDEX], in, *pplayer)) {
            print_full_column_message();
            print_enter_column_message();
            return false;
        }
        *pplayer = !*pplayer;
        return true;
    }
    switch (in) {
     //   case -1: undo(); break;
     //   case -2: redo(); break;
        default: print_enter_column_message(); break;
    }
    return false;
}


int add_to_board(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int req_column, bool player)
{
    char add;
    add = player ? 'Y' : 'R';
    for (int i = rows-1; i >= 0; i--) {
        if (board[i][req_column - 1] == ' ') {
            board[i][req_column - 1] = add;
            return 0;
        }
    }
    return 1;
}

int is_endOfGame(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens)
{
    /*
    int r_winner = is_row_winner(board, rows, columns, tokens); 
    int c_winner = is_column_winner(board, rows, columns, tokens);
    int d_winner = is_upward_diagonal_winner(board, rows, columns, tokens) + is_downward_diagonal_winner(board, rows, columns, tokens);
    if (r_winner) {
        print_board(board, rows, columns);
        return print_winner(r_winner);
    }
    if (c_winner) {
        print_board(board, rows, columns);
        return print_winner(c_winner);
    }
    if (d_winner) {
        print_board(board, rows, columns);
        return print_winner(d_winner);
    }
    */
    if (is_full(board, rows, columns)) {
        print_board(board, rows, columns);
        return print_winner(TIE);
    }
    return 0;
}

/*
int is_row_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens)
{
    int counter = 0;
    char last_char = 'Z';
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns && counter < tokens; j++) {
            if ((last_char == 'Z' || board[i][j] == last_char) && board[i][j] != ' ') {
                counter++;
            }
            else {
                counter = 0;
            }
            last_char = board[i][j];
        }
        if (counter == tokens) {
            if (last_char == 'R') {
                return RED;
            }
            else
                return YELLOW;
        }
        counter = 0;
        last_char = 'Z';
    }
    return 0;
}

int is_column_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens)
{
    int counter = 0;
    char last_char = 'Z';
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i < rows && counter < tokens; i++) {
            if ((last_char == 'Z' || board[i][j] == last_char) && board[i][j] != ' ') {
                counter++;
            }
            else {
                counter = 0;
            }
            last_char = board[i][j];
        }
        if (counter == tokens) {
            if (last_char == 'R') {
                return RED;
            }
            if (last_char == 'Y') {
                return YELLOW;
            }
        }
        counter = 0;
        last_char = 'Z';
    }
    return 0;
}
int is_upward_diagonal_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens)
{
    int counter = 0, r_ini = rows - 1, j = columns - tokens, i;
    char last_char = 'Z';
    do
    {
        i = r_ini;
        do {
            if(last_char=='Z')
        }while()
    } while ();
    
}

int is_downward_diagonal_winner(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns, int tokens)
{

}
*/
int is_full(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == ' ') {
                return 0;
            }
        }
    }
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

//Outputs winner or tie message
int print_winner(int player_id)
{   //function length: 3 rows
    if(player_id==0){
        return 0;
    }
    if (player_id > 0)
        printf("Player %d won! \n", player_id);
    else
        printf("That's a tie. \n");
    return 1;
}