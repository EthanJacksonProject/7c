#include "map.h"
#include "globals.h"
#include "graphics.h"

void* KeyGiven = 0;
void* IdolTaken = 0;
void* Direction = 0;

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map maps[2];
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    
    unsigned int location = Y * map_width() + X;
    return location;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key) {
    unsigned int HashValue = key % 50; // 50 tiles, so map each col into its own bucket
    return HashValue;
}

void maps_init() {
    Map* thisMap;
    
    thisMap = &maps[0];
    thisMap -> w = 50;
    thisMap -> h = 50;
    thisMap -> items = createHashTable(map_hash, 50);
    
    thisMap = &maps[1];
    thisMap -> w = 30;
    thisMap -> h = 20;
    thisMap -> items = createHashTable(map_hash, 50);
}

Map* get_active_map() {
    return &maps[active_map];
}

Map* set_active_map(int m)
{
    active_map = m;
    return &maps[active_map];
}

Map* get_map(int m)
{
    return &maps[m];
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width() {
    Map* map = get_active_map();
    return map->w;
}

int map_height() {
    Map* map = get_active_map();
    return map->h;
}

int map_area() {
    Map* map = get_active_map();
    return (map->w * map->h);
}

MapItem* get_north(int x, int y) {
    MapItem* item = (MapItem*)getItem(get_active_map() -> items, XY_KEY(x, y-1));
    return item;
}

MapItem* get_south(int x, int y) {
    MapItem* item = (MapItem*)getItem(get_active_map() -> items, XY_KEY(x, y+1));
    return item;
}

MapItem* get_east(int x, int y) {
    MapItem* item = (MapItem*)getItem(get_active_map() -> items, XY_KEY(x+1, y));
    return item;
}

MapItem* get_west(int x, int y) {
    MapItem* item = (MapItem*)getItem(get_active_map() -> items, XY_KEY(x-1, y));
    return item;
}

MapItem* get_here(int x, int y) {
    MapItem* item = (MapItem*)getItem(get_active_map() -> items, XY_KEY(x, y));
    return item;
}


void map_erase(int x, int y) {
    deleteItem(get_active_map() -> items, XY_KEY(x, y));
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_npc(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC;
    w1->draw = draw_npc;
    w1->walkable = false;
    w1->data = KeyGiven;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
    }
void add_cave(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = CAVE;
    w1->draw = draw_cave;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
    }
    
void add_good_idol(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = Good_Idol;
    w1->draw = draw_good_idol;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
    }

void add_no_idol(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = Good_Idol;
    w1->draw = draw_no_idol;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
    }
    
void add_bad_idol(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = Bad_Idol;
    w1->draw = draw_bad_idol;
    w1->walkable = false;
    w1->data = IdolTaken;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
    }
    
void add_Chest(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = CHEST;
    w1->draw = draw_Chest;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
    }

void add_Rock(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = Rock;
    w1->draw = draw_Rock;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
    }

void add_Fish(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = Fish;
    w1->draw = draw_Fish;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
    }