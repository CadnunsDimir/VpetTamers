#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define PLAYER_SIZE   40.0f
#define SPEED         5.0f
#define TILE_SIZE 32
#define MAP_WIDTH 25   // 800 / 32 = 25 colunas
#define MAP_HEIGHT 18  // 576 / 32 = 18 linhas


typedef enum {
    STATE_MENU,
    STATE_MAP,
    STATE_PAUSE
} GameState;

typedef struct { 
    int x; 
    int y; 
} PlayerPosition;

typedef struct { 
    char name[30]; 
    int level; 
    int hp; 
    int hp_max; 
    int strength; 
    int defense; 
} VPet;

typedef struct {
    int r; 
    int g; 
    int b; 
    int a;
} Color;

typedef struct {
    int x;
    int y;
    bool triggered_pause;
    bool triggered_confirm;
} GamePadDelta;

static const Color COLOR_GREY_WALL = {60, 60, 75, 255};
static const Color COLOR_GREY_WALL_BORDER = { 40, 40, 50, 255};
static const Color COLOR_GREEN_GROUND = {25, 45, 30, 255};
static const Color COLOR_GREEN_GROUND_DETAILS = {20, 35, 25, 255};
static const Color COLOR_WHITE = { 255,255,255,255 };
static const Color COLOR_GREY = {150, 150, 150, 255};
static const Color COLOR_MENU_BUTTON_NOT_SELECTED = {0, 70, 130, 255};
static const Color COLOR_MENU_BUTTON_SELECTED = {0, 180, 255, 255};
static const Color COLOR_MENU_BUTTON_DISABLED = {50, 50, 50, 255};
static const Color COLOR_MENU_BUTTON_DISABLED_SELECTED = {120, 120, 120, 255};