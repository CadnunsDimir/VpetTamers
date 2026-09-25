
#define TREE_DEF_TILES { \
    {260, 0, 0}, {261, 1, 0}, {262, 2, 0}, \
    {264, 0, 1}, {265, 1, 1}, {266, 2, 1}, \
    {268, 0, 2}, {269, 1, 2}, {270, 2, 2} \
} \

#define TREE_DEF_TILES_COUNT 9

static const MapObject map3_tree1 =  {
    .map_id = 3, 
    .pos = { 0, 5 },
    .tiles = TREE_DEF_TILES,
    .title_count = TREE_DEF_TILES_COUNT
};

static const MapObject map3_tree2 =  {
    .map_id = 3, 
    .pos = { 3, 3 },
    .tiles = TREE_DEF_TILES,
    .title_count = TREE_DEF_TILES_COUNT
};

static const MapObject map3_tree3 =  {
    .map_id = 3, 
    .pos = { 6, 12 },
    .tiles = TREE_DEF_TILES,
    .title_count = TREE_DEF_TILES_COUNT
};