#define SIZE 12
#define RESULTS_X 34
#define RESULTS_Y 3
#define PLAYER_X 16
#define PLAYER_Y 7
#define BOT_X 46
#define BOT_Y 7
#define COMMENTS_X 31
#define COMMENTS_Y 19
#define LEGEND_X 60
#define LEGEND_Y 10
#define DIALOG_X 5
#define DIALOG_Y 12
#define MENU_X 38
#define MENU_Y 10
#define MAX_SHIP_SIZE 6
#define MIN_SHIP_SIZE 2

enum State
{
    MENU = 0,
    PLAYER = 1,
    BOT = 2,
    HELP = 3
};

enum FieldType
{
    EMPTY = 0,
    SHIP_INTACT = 1,
    SHIP_HIT = 2,
    SHIP_SUNK = 3,
    NEARBY = 4,
    FRAME = 5,
    MISSED = 6,
    TEMPORARILY_EXCLUDED = 7
};

enum Colors
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    TURQUOISE = 3,
    RED = 4,
    PURPLE = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_TURQUOISE = 11,
    LIGHT_RED = 12,
    LIGHT_PURPLE = 13,
    YELLOW = 14,
    WHITE = 15
};