/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
 //this code has passed check50 2016.fifteen fifteen.c
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions and player location;
int d;
int player_row, player_column;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    //iterate through the rows and columns setting up the boards initial play state
    for(int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            board[i][j] = d * d - 1 - d * i - j;
        }
    }
    //if the board has d as an even number swap the initial position of 1 and 2
    if(d % 2 == 0) {
        board[d - 1][d - 2] = 2;
        board[d - 1][d - 3] = 1;
    }
    //set the initial location of the player
    player_row = d - 1;
    player_column = d - 1;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    //iterate through the rows and columns printing the boards current state
    for(int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            //use %2 to preserve nice spacing
            if(board[i][j] == 0) {
                printf(" %2c ", '_');
            }
            else {
                printf(" %2i ", board[i][j]);
            }
        }
        //newline at the end of every row
        printf("\n");
    }
    printf("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    //check to see if tile is a valid number for the game
    if(tile <= 0 || tile >= d * d) {
        return false;
    }
    //find where the tile is
    int move_row, move_column;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if(board[i][j] == tile) {
                move_row = i;
                move_column = j;
            }
        }
    }
    //check if move is legal by checking the abs distance and checking that tile is on the same row or column
    if ((abs((move_row + move_column) - (player_row + player_column)) == 1 ) && (move_row == player_row || move_column == player_column)) {
        //when if is satisfied swap positions of the board and save positon of player
        board[move_row][move_column] = 0;
        board[player_row][player_column] = tile;
        player_row = move_row;
        player_column = move_column;
        return true;
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    //iterate through the boards rows and columns. If one tile is found not to match then return false
    for(int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if(board[i][j] != 0 && board[i][j] != d * i + j + 1) {
                return false;
            }
        }
    }
    //if the for loop executes with returning false then the game must be won!
    return true;
}
