#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define PLAYER_SIZE   40.0f
#define SPEED         5.0f
#define MAP_WIDTH 25   // 800 / 32 = 25 colunas
#define MAP_HEIGHT 18  // 576 / 32 = 18 linhas
#define BATTLE_RUNNING_CHANCE 40 // 40% de chance de fuga em batalhas
#define TILED_MAP_WIDTH 20
#define TILED_MAP_HEIGHT 15
#define SCALED_TILED_SIZE 64.0f
#define WALKABLE_TILES_COUNT (sizeof(WALKABLE_TILES) / sizeof(WALKABLE_TILES[0]))
#define STEP_COOLDOWN_MS 150
#define HOTEL_SLEEPING_MSG_TIMEOUT_MS 2000
#define HOTEL_VPETCOIN_PRICE 16.0f
#define ALERT_DURATION 4000

static const char *MAP_FILES[] = {
    "assets/Map1.csv",
    "assets/Map2_Village.csv",
    "assets/Map3_Floresta_Mar.csv",
    "assets/Map4_Village_Inn_Map3.csv",
};

#define TOTAL_MAPS_COUNT (sizeof(MAP_FILES) / sizeof(MAP_FILES[0]))
typedef enum {
    MAP_TILE_WALL = 1,
    MAP_TILE_GROUND = 0
} MapTile;

typedef enum {
    STATE_MENU,
    STATE_MAP,
    STATE_PAUSE,
    STATE_BATTLE,
    STATE_STORE_MENU
} GameState;

typedef enum {
   STORE_TYPE_HOTEL
} StoreType;


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
    PET_RUGUASUMON = 3,
    PET_JAGUARETEMON = 5
} VPetType;

typedef struct { 
    char name[30]; 
    int level; 
    int hp;
    int hp_max; 
    int strength; 
    int defense;
    int total_exp;
    float vpetcoin_reward;
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
    VPet vpet_data;
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
    bool key_pressed;
    bool pause_key_pressed;
} PlayerGamePad;

//{X, Y}
typedef struct {
    int x;
    int y;
} Coordinates;
typedef struct {
    Coordinates player;
    Coordinates enemy;
} OponentsPositioning;

typedef enum {
    MESSAGE_TYPE_ERROR,
    MESSAGE_TYPE_INFO,
    MESSAGE_TYPE_WARN,
} MessageType;

typedef struct {
    char message[100];
    MessageType type;
    Uint64 start_time_ms;
    Uint64 duration;
} Alert;

typedef struct {
    GameState state;
    bool running;
    float app_time;
    float delta_time;
    Uint64 last_step_time;
    Uint64 sleep_start_time;
    Alert alert;
    char save_data_file[10]
} GameStateMachine;

typedef struct {
    BattleState current_state;
    Uint64 message_start_time; // Armazena o timestamp de início da mensagem
    Uint64 message_duration;   // Duração da mensagem em milissegundos (ex: 2000ms = 2s)
    char current_message[64];  // Texto da mensagem temporizada
    VPet player;
    float player_vpetcoin_balance;
    VPet enemy;    
} BattleSystem;

typedef struct {
    float x, y;
    float width, height;
} Camera;

typedef struct {
    Coordinates player_pos;
    int current_map_id;
    VPet vpt;
    // Adicione outras variáveis de progresso/inventário aqui
    float vpetcoin;
} SaveData;

typedef struct {
    int map_id;
    Coordinates pos;
    int tiles[16][3];
    int title_count;
} MapObject;

typedef struct {
    int origin_map_id;
    Coordinates origin_pos;
    int dest_map_id;
    Coordinates dest_pos;
} MapPortal;

typedef struct 
{
    SDL_Texture* tileset_texture;
    int (*world_map)[TILED_MAP_HEIGHT][TILED_MAP_WIDTH];
    Coordinates player_coordinates;
    Coordinates enemy_coordinates;
    int current_map[TILED_MAP_HEIGHT][TILED_MAP_WIDTH];
    int current_map_id;
    int width_tiles; 
    int height_tiles;
    Camera camera;
    Coordinates* object_tiles;
    int object_tiles_total;
    MapMob* spawned_mobs;
    int spawned_mobs_size;
    int spawn_enemies_limit;
} MapState;

typedef struct {
    StoreType store_type;
    int selected_option;
    bool hotel_free_on_early_game;
} MenuState;


static const Color COLOR_GREY_WALL = {60, 60, 75, 255};
static const Color COLOR_GREY_WALL_BORDER = { 40, 40, 50, 255};
static const Color COLOR_GREEN_GROUND = {25, 45, 30, 255};
static const Color COLOR_GREEN_GROUND_DETAILS = {20, 35, 25, 255};
static const Color COLOR_WHITE = { 255,255,255,255 };
static const Color COLOR_GREY = {150, 150, 150, 255};
static const Color COLOR_YELLOW = {255, 220, 0, 255};
static const Color COLOR_MENU_BUTTON_NOT_SELECTED = {0, 70, 130, 255};
static const Color COLOR_MENU_BUTTON_SELECTED = {0, 180, 255, 255};
static const Color COLOR_MENU_BUTTON_DISABLED = {50, 50, 50, 255};
static const Color COLOR_MENU_BUTTON_DISABLED_SELECTED = {120, 120, 120, 255};
static const Color COLOR_ENEMY_2 = {193, 114, 38, 255};
static const Color COLOR_ENEMY_3 = {139, 69, 19, 255};
static const Color COLOR_DIALOG_BACKGROUND = {20, 25, 40, 255};
static const Color COLOR_DIALOG_BORDER = {200, 200, 220, 255};
static const Color COLOR_MESSAGE_ERROR = {255, 75, 75, 255};
static const Color COLOR_MESSAGE_WARNING = {255, 200, 40, 255};

static const int WALKABLE_TILES[] = {
    300, 301, 302,  304, 306, 308, 309, 310, 
    348, //agua
    205, 375, //paredes hotel
    164, 165, 130, //balcão hotel
    160, 161, //livros hotel
};

// Define quantos inimigos serão gerados por mapa. podemos gerar zonas seguras como cidades e vilas
static const int SPAWN_ENEMIES_LIMIT_PER_MAP[] = {
    10, //mapa 1
    0,   // mapa 2
    16
};

