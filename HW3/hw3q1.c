#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>

#define BOARD_MAX_SIZE 9
#define BOARD_MIN_SIZE 3
#define MIN_TOKENS 3
#define NUM_OF_INIT_INPUTS 4

//----------------------- Input Identifiers --------------------------//
#define COLOR_INDEX 0
#define ROW_INDEX 1
#define COLUMN_INDEX 2
#define TOKEN_INDEX 3

//----------------------- Symbol Identifiers --------------------------//
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

void print_welcome_message();
void print_read_game_params_message(int param);
void print_choose_color_message();
void print_choose_move_message(int player);
void print_enter_column_message();
void print_full_column_message();
void print_unavailable_undo_redo_message();
int game_init(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS]);
char get_color();
int get_rows();
int get_columns(int rows);
int get_tokens(int rows, int columns);
void board_init(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int rows, int columns);


/*
 * Outputs game board.
 *
 * board - contains spaces for empty cells, 'Y' or 'R' for
 *         respective player cells.
 * board_side - contains board dimensions {rows, cols}
 */
void print_board(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int board_side[]);

/*
 * Outputs winner or tie message.
 *
 * player_id - the id to be output. Pass <0 for tie.
 */
void print_winner(int player_id);


//--------------------------- Main Program -----------------------------//
int main()
{
    char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE];
    char inputs[NUM_OF_INIT_INPUTS];
    if (game_init(board, inputs)) {
        return 1;
    }

    return 0;
}

//--------------------------- Init Functions -----------------------------//

//getting the necessary inputs from the user in order to initiate the game, and then initiating the game board
int game_init(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], char inputs[NUM_OF_INIT_INPUTS])
{
    print_welcome_message();
    for (int i = 0; i < NUM_OF_INIT_INPUTS; i++) {
        switch (i){
            case COLOR_INDEX: inputs[i] = get_color(); break;
            case ROW_INDEX: inputs[i] = get_rows(); break;
            case COLUMN_INDEX: inputs[i] = get_columns(inputs[ROW_INDEX]); break;
            case TOKEN_INDEX: inputs[i] = get_tokens(inputs[ROW_INDEX], inputs[COLUMN_INDEX]); break;
        }
        if (inputs[i] == 0) {
            return 1;
        }
    }
    board_init(board,inputs[ROW_INDEX],inputs[COLUMN_INDEX]);
    return 0;
}

//getting the starting color from the user
char get_color()
{
    char color;
    do {
        print_choose_color_message();
        if (scanf(" %c", &color) != 1) {
            return 0;
        }
    } while (color != 'Y' && color != 'R');
    return color;
}

int get_rows() 
{
    int rows;
    do {
        print_read_game_params_message(MSG_GET_BOARD_ROWS);
        if (scanf(" %d", &rows) != 1) {
            return 0;
        }
    } while (rows < BOARD_MIN_SIZE || rows > BOARD_MAX_SIZE);
        return rows;
}

int get_columns(int rows)
{
    int columns;
    do {
        print_read_game_params_message(MSG_GET_BOARD_COLS);
        if (scanf(" %d", &columns) != 1) {
            return 0;
        }
    } while (columns < BOARD_MIN_SIZE || columns > BOARD_MAX_SIZE || columns < rows);
    return columns;
}

int get_tokens(int rows, int columns)
{
    int tokens;
    do {
        print_read_game_params_message(MSG_GET_NUMBER_TOKENS);
        if (scanf(" %d", &tokens) != 1) {
            return 0;
        }
    } while (tokens < MIN_TOKENS || tokens > rows || tokens > columns);
    return tokens;
}

void board_init(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE],int rows,int columns)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = ' ';
        }
    }
}

//---------------------- Printing Functions ----------------------------//

// message number 0 = "Welcome to 4-in-a-row game! \n"
void print_welcome_message()
{
    printf("Welcome to 4-in-a-row game! \n");
}

// message number 1 = "Please enter number of rows:"
// message number 2 = "Please enter number of columns:"
// message number 3 = "Please enter number of tokens:"
void print_read_game_params_message(int param)
{
    char const* const possible_params[] = { "rows", "columns", "tokens" };

    printf("Please enter number of %s: ", possible_params[param]);
}

// message number 4 = "Please choose starting color (Y)ellow or (R)ed: "
void print_choose_color_message()
{
    printf("Please choose starting color (Y)ellow or (R)ed: ");
}

// message number 5 = "Your move, player <player>. "
void print_choose_move_message(int player)
{
    printf("Your move, player %d. ", player);
}

// message number 6 = "Please enter column: "
void print_enter_column_message()
{
    printf("Please enter column: ");
}

// message number 7 = "Column full. "
void print_full_column_message()
{
    printf("Column full. ");
}

//message number 8 = "No moves to undo/redo. "
void print_unavailable_undo_redo_message()
{
    printf("No moves to undo/redo. ");
}

//Outputs game board.
void print_board(char board[BOARD_MAX_SIZE][BOARD_MAX_SIZE], int board_side[])
{
    /*
    assert(IS_IN_RANGEI(board_side[0], 0, BOARD_MAX_SIZE)
        && IS_IN_RANGEI(board_side[1], 0, BOARD_MAX_SIZE));
    */
    for (int row = 0; row < board_side[0]; ++row)
    {
        printf("\n%c", BOARD_VERT_SEP);
        for (int col = 0; col < board_side[1]; ++col)
            printf("%c%c", board[row][col], BOARD_VERT_SEP);
    }
    printf("\n%c", BOARD_LEFT_ANG);
    for (int ii = 0; ii < board_side[1] - 1; ++ii)
        printf("%c%c", BOARD_BOTTOM, BOARD_BOTTOM_SEP);
    printf("%c%c\n", BOARD_BOTTOM, BOARD_RIGHT_ANG);
}

//Outputs winner or tie message.
void print_winner(int player_id)
{
    if (player_id >= 0)
        printf("Player %d won! \n", player_id);
    else
        printf("That's a tie. \n");
}