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