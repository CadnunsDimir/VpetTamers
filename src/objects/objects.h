#include "map1_objects.h"
#include "map2_objects.h"
#include "map3.h"

static const MapObject ALL_OBJECTS_ON_MAPS[] = {
   map1_tree1,   
   map2_hotel,
   map2_hotel_2,
   map3_tree1,
   map3_tree2,
   map3_tree3
};

#define ALL_OBJECTS_ON_MAPS_COUNT (sizeof(ALL_OBJECTS_ON_MAPS) / sizeof(ALL_OBJECTS_ON_MAPS[0]))