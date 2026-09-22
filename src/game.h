#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define PLAYER_SIZE   40.0f
#define SPEED         5.0f
#define MAP_WIDTH 25   // 800 / 32 = 25 colunas
#define MAP_HEIGHT 18  // 576 / 32 = 18 linhas
#define BATTLE_RUNNING_CHANCE 40 // 40% de chance de fuga em batalhas
#define TILED_MAP_WIDTH 20
#define TILED_MAP_HEIGHT 15
#define TOTAL_MAPS 1
#define SCALED_TILED_SIZE 64.0f
#define WALKABLE_TILES_COUNT (sizeof(WALKABLE_TILES) / sizeof(WALKABLE_TILES[0]))

typedef enum {
    MAP_TILE_WALL = 1,
    MAP_TILE_GROUND = 0
} MapTile;

typedef enum {
    STATE_MENU,
    STATE_MAP,
    STATE_PAUSE,
    STATE_BATTLE
} GameState;

typedef enum {
    BATTLE_STATE_MENU_ATTACK,
    BATTLE_STATE_MENU_RUN,
    BATTLE_STATE_ATTACKING,
    BATTLE_STATE_RUNNING,
    BATTLE_STATE_WIN,
    BATTLE_STATE_WIN_CLOSE_BATTLE,
    BATTLE_STATE_LOST,
    BATTLE_STATE_ENEMY_ATACKING
} BattleState;


typedef enum {
    PET_BK_MBARACAJAMON = 4,
    PET_JAGUAPIRUMON = 2,
    PET_RUGUASUMON = 3
} VPetType;

typedef struct { 
    char name[30]; 
    int level; 
    int hp;
    int hp_max; 
    int strength; 
    int defense;
    int total_exp;
    VPetType type;
} VPet;

typedef struct {
    bool is_found;
    VPet enemy;
} VPetEncounter;

typedef struct {
    bool is_active;
    int x;
    int y;
    VPet vpet_data
} MapMob;

typedef struct {
    int r; 
    int g; 
    int b; 
    int a;
} Color;

typedef struct {
    int delta_x;
    int delta_y;
    bool triggered_pause;
    bool triggered_confirm;
    bool down;
    bool up;
    bool right;
    bool left;
} PlayerGamePad;

typedef struct {
    int x;
    int y;
} Coordinates;
typedef struct {
    Coordinates player;
    Coordinates enemy;
} OponentsPositioning;

typedef struct {
    BattleState current_state;
    Uint64 message_start_time; // Armazena o timestamp de início da mensagem
    Uint64 message_duration;   // Duração da mensagem em milissegundos (ex: 2000ms = 2s)
    char current_message[64];  // Texto da mensagem temporizada
    VPet player;
    VPet enemy;
} BattleSystem;

typedef struct {
    float x, y;
    float width, height;
} Camera;

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
static const Color COLOR_ENEMY_2 = {193, 114, 38, 255};
static const Color COLOR_ENEMY_3 = {139, 69, 19, 255};
static const int WALKABLE_TILES[] = {
    300, 301, 302,  304, 306, 308, 309, 310
};

static const VPet ENEMY_JAGUAPIRUMON = {
    .name = "Jaguapirumon", //cachoro feio
    .defense = 2,
    .strength = 3,
    .hp = 20,
    .hp_max = 20,
    .type = PET_JAGUAPIRUMON,
    .level = 2
};

static const VPet ENEMY_RYGUASUMON = {
    .name = "Ruguasumon", //galinha domestica
    .defense = 1,
    .strength = 1,
    .hp = 10,
    .hp_max = 10,
    .type = PET_RUGUASUMON,
    .level = 1
};

static const VPet ENEMY_LIST[2] = {
    ENEMY_JAGUAPIRUMON,
    ENEMY_RYGUASUMON
};

//TODO: criar vpets (*mon) inspirados nos nomes abaixo:
/*
Jaguarete: Onça-pintada (literalmente "cão verdadeiro" ou "fera verdadeira").
Aguará: Lobo / Lobo-guará.
Ka'i: Macaco.
Tapiti: Coelho ou lebre.
Angujá: Rato.Pira: Peixe.
Jagua: Cachorro.Maracadá (ou mbarakaja): Gato.Kavaju: Cavalo.Vaca: Vaca.Curé: Porco / Suíno.Ryguasu: Galinha.Ovechá: Ovelha.Cavará: Cabra / Bode.
*/