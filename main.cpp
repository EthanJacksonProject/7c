// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#define OceanDark 0x064273
#define WALL    0
#define PLANT   1
#define NPC     2
#define CAVE    3
#define CHEST    4
#define Good_Idol    5
#define Bad_Idol    6
#define No 0
#define Yes 1
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define GOD_BUTTON 7
char* line1;
char* Line2;


//void* KeyGiven = 0;
//void* IdolTaken = 0;
// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
int main ();

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    int has_idol;
    int game_state;
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
int get_action(GameInputs inputs) {
    double x = inputs.ax;
    double y = inputs.ay;
    //double z = inputs.az;
    int b1 = inputs.b1;
    int b2 = inputs.b2;
    int b3 = inputs.b3;
    
    if(!b1){
        MapItem* Eastern = get_east(Player.px, Player.py);
        MapItem* Southern = get_south(Player.px, Player.py);
        MapItem* Western = get_west(Player.px, Player.py);
        MapItem* Northern = get_north(Player.px, Player.py);
        if (Eastern -> data != (void*)NULL || Western -> data != (void*)NULL || Southern -> data != (void*)NULL || Northern -> data != (void*)NULL )
            return ACTION_BUTTON;
    }
    if(!b3)
        return MENU_BUTTON;
    if(!b2)
        return GOD_BUTTON;
    //if ( z > .2 )
      //  return NO_ACTION;
    //if ( z < -.2)
      //  return NO_ACTION;
    if ( x > .1 )
        return GO_UP;
    if ( x < -.1)
        return GO_DOWN;
    if ( y > .1 )
        return GO_RIGHT;
    if ( y < -.1)
        return GO_LEFT;
    
    return NO_ACTION;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define REDRAW 2
#define FULL_DRAW 3
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action){
        case GO_UP:{
            MapItem* Northern = get_north(Player.px, Player.py);
            if (Northern -> walkable > 0){
                Player.y = Player.py - 1;
            }
            return REDRAW;       
        }
        case GO_LEFT:{
            MapItem* Eastern = get_east(Player.px, Player.py);
            if (Eastern -> walkable > 0){
                Player.x = Player.px + 1;
            }
            return REDRAW; 
        }

        case GO_DOWN:{
            MapItem* Southern = get_south(Player.px, Player.py);
            if (Southern -> walkable > 0 ){
                Player.y = Player.py + 1;
            }  
            return REDRAW;      
        }
        case GO_RIGHT:{ 
            MapItem* Western = get_west(Player.px, Player.py);
            if (Western -> walkable > 0 ){
                Player.x = Player.px - 1;
            } 
            return REDRAW;  
        }
        case ACTION_BUTTON: {
            MapItem* Eastern = get_east(Player.px, Player.py);
            MapItem* Southern = get_south(Player.px, Player.py);
            MapItem* Western = get_west(Player.px, Player.py);
            MapItem* Northern = get_north(Player.px, Player.py);
            if (((Eastern -> type == CAVE) || (Western -> type == CAVE) || (Southern -> type == CAVE) || (Northern -> type == CAVE)) && Player.game_state > 1) {
                if(get_active_map() == (get_map(1))){
                    set_active_map(0);
                    Player.x = 8;
                    Player.y = 7;
                    return FULL_DRAW;
                }
                if(get_active_map() == (get_map(0))){
                    set_active_map(1);
                    Player.x = 3;
                    Player.y = 2;
                    return FULL_DRAW;
                }
            }
            if ((Eastern -> type) == NPC || Western -> type == NPC || Southern -> type == NPC || Northern -> type == NPC){
                if (Player.has_idol == Yes){
                    char* line1 = "Ahoy! You";
                    char* Line2 = "have got me...";
                    speech(line1, Line2);
                    line1 = "an important";
                    Line2 = "artifact!";
                    speech(line1, Line2);
                    line1 = "Hare be yer";
                    Line2 = "map! It will...";
                    speech(line1, Line2);
                    Player.has_key = Yes;
                    line1 = "guide ya to a";
                    Line2 = "landlocked...";
                    speech(line1, Line2);
                    Player.has_key = Yes;
                    line1 = "treasure!!!";
                    Line2 = "";
                    speech(line1, Line2);
                    Player.has_key = Yes;
                    return FULL_DRAW;
                } else {
                    line1 = "Ahoy, Matey!";
                    Line2 = "Thar be a fake...";
                    speech(line1, Line2);
                    line1 = "idol in that thar";
                    Line2 = "cave! Bring it...";
                    speech(line1, Line2);
                    line1 = "here & I'll give";
                    Line2 = "you a map to...";
                    speech(line1, Line2);
                    line1 = "a treasure";
                    Line2 = "beyond belief...";
                    speech(line1, Line2);
                    Player.game_state = 100;
                    return FULL_DRAW;
                }
            }
            if ((Eastern -> type) == Bad_Idol || Western -> type == Bad_Idol || Southern -> type == Bad_Idol || Northern -> type == Bad_Idol){
                Player.has_idol = Yes;
                line1 = "The False Idol!";
                Line2 = "an ancient script";
                speech(line1, Line2);
                line1 = "reads, 'True";
                Line2 = "value lies in";
                speech(line1, Line2);
                line1 = "difference...";
                Line2 = "This must be";
                speech(line1, Line2);
                line1 = "worth a lot";
                Line2 = "to that man!";
                speech(line1, Line2);
                add_no_idol(8,8);
                return FULL_DRAW;
            }
            if ((Eastern -> type == PLANT || Western -> type == PLANT || Southern -> type == PLANT || Northern -> type == PLANT) && (Player.x == 17 || Player.x == 18)&&(Player.has_key==Yes))     {
                map_erase(17,10);
                map_erase(18,10);
                return FULL_DRAW;
            } 
            if ((Eastern -> type == CHEST || Western -> type == CHEST || Southern -> type == CHEST || Northern -> type == CHEST) && Player.has_key == true){
                Player.game_state = GAME_OVER;
            }           
            break;
        }

        case GOD_BUTTON: {
            MapItem* Eastern = get_east(Player.px, Player.py);
            MapItem* Southern = get_south(Player.px, Player.py);
            MapItem* Western = get_west(Player.px, Player.py);
            MapItem* Northern = get_north(Player.px, Player.py);
            if (((Eastern -> type == CAVE) || (Western -> type == CAVE) || (Southern -> type == CAVE) || (Northern -> type == CAVE))) {
                if(get_active_map() == (get_map(1))){
                    set_active_map(0);
                    Player.x = 8;
                    Player.y = 7;
                    return FULL_DRAW;
                }
                if(get_active_map() == (get_map(0))){
                    set_active_map(1);
                    Player.x = 3;
                    Player.y = 2;
                    return FULL_DRAW;
                }
            }
            if ((Eastern -> type) == NPC || Western -> type == NPC || Southern -> type == NPC || Northern -> type == NPC){
                if (Player.game_state == 100){
                    char* line1 = "Ahoy! You";
                    char* Line2 = "have got me...";
                    speech(line1, Line2);
                    line1 = "an important";
                    Line2 = "artifact!";
                    speech(line1, Line2);
                    line1 = "Hare be yer";
                    Line2 = "map! It will...";
                    speech(line1, Line2);
                    Player.has_key = Yes;
                    line1 = "guide ya to a";
                    Line2 = "landlocked...";
                    speech(line1, Line2);
                    Player.has_key = Yes;
                    line1 = "treasure!!!";
                    Line2 = "";
                    speech(line1, Line2);
                    Player.has_key = Yes;
                    return FULL_DRAW;
                } else {
                    char* line1 = "Ahoy, Matey!";
                    char* Line2 = "Thar be a fake...";
                    speech(line1, Line2);
                    line1 = "idol in that thar";
                    Line2 = "cave! Bring it...";
                    speech(line1, Line2);
                    line1 = "here & I'll give";
                    Line2 = "you a map to...";
                    speech(line1, Line2);
                    line1 = "a treasure";
                    Line2 = "beyond belief...";
                    speech(line1, Line2);
                    Player.game_state = 100;
                    return FULL_DRAW;
                }
            }
            if ((Eastern -> type) == Bad_Idol || Western -> type == Bad_Idol || Southern -> type == Bad_Idol || Northern -> type == Bad_Idol){
                Player.has_idol = Yes;
                add_no_idol(8,8);
                return FULL_DRAW;
            }

            if ((Eastern -> type == PLANT || Western -> type == PLANT || Southern -> type == PLANT || Northern -> type == PLANT) ){
                if(Eastern -> type == PLANT){
                    Eastern -> walkable = true;
                    return FULL_DRAW;
                }
                if(Western -> type == PLANT){
                    Western -> walkable = true;
                    return FULL_DRAW;
                }
                if(Southern -> type == PLANT){
                    Southern -> walkable = true;
                    return FULL_DRAW;
                }
                if(Northern -> type == PLANT){
                    Northern -> walkable = true;
                    return FULL_DRAW;
                }
            }
            if ((Eastern -> type == WALL || Western -> type == WALL || Southern -> type == WALL || Northern -> type == WALL) ){
                if(Eastern -> type == WALL){
                    Eastern -> walkable = true;
                    return FULL_DRAW;
                }
                if(Western -> type == WALL){
                    Western -> walkable = true;
                    return FULL_DRAW;
                }
                if(Southern -> type == WALL){
                    Southern -> walkable = true;
                    return FULL_DRAW;
                }
                if(Northern -> type == WALL){
                    Northern -> walkable = true;
                    return FULL_DRAW;
                }
            }
            if ((Eastern -> type == CHEST || Western -> type == CHEST || Southern -> type == CHEST || Northern -> type == CHEST) && Player.has_key == true){
                Player.game_state = GAME_OVER;
            }           
            break;
        }
        case MENU_BUTTON: {
            uLCD.locate(0, 10);
            uLCD.printf("Menu...");
            break;
        }
    }
    return NO_RESULT;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (i == 0 && j == 0)
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");

    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    add_wall(15, 0, -VERTICAL, 10);
    add_wall(20, 0, -VERTICAL, 10);
    add_wall(15, 10, HORIZONTAL, 2);
    add_wall(19, 10, HORIZONTAL, 2);
    add_Chest(18, 8);
    pc.printf("Walls done!\r\n");
    add_plant(17,10);
    add_plant(18,10);
    add_npc(5,5);
    add_cave(7,7);
    print_map();
}
void init_quest_map()
{
    Map* map = set_active_map(1);

    for(int i = map_width() + 3; i < map_area(); i += 17){
        add_good_idol(i % map_width(), i / map_width());
    }
    
    add_bad_idol(8,8);

    
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
    add_cave(2,2);
    print_map();
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    // Initialize the maps
    maps_init();
    init_main_map();
    init_quest_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 3;

    uLCD.filled_rectangle(0,0,127,127, 0x064273);
    line1 = "7 C's";
    while(1){
        uLCD.text_string(line1, 1, 4, FONT_12X16 , WHITE);
        GameInputs inputs = read_inputs();       
        int action = get_action(inputs);
        if (action == ACTION_BUTTON)
            break;
    }
    // Initial drawing
    draw_game(true);

    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t; t.start();
        
        // Actuall do the game update:
        // 1. Read inputs
        GameInputs inputs = read_inputs();       
        // 2. Determine action (get_action) 
        int action = get_action(inputs);       
        // 3. Update game (update_game)
        int updates = update_game(action);
        uLCD.locate(0,0);
        uLCD.textbackground_color(BLACK);
        uLCD.printf("X: %2d, Y: %2d", Player.x, Player.y);
        uLCD.textbackground_color(OceanDark);
        if (Player.game_state == GAME_OVER)
            break;
        // 4. Draw frame (draw_game)
        switch (updates)
        {
            case REDRAW:
            draw_game(false);
            break;
            case FULL_DRAW:
            draw_game(true);
            break;
        }
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
    uLCD.locate(0,0);
    uLCD.textbackground_color(BLACK);
    uLCD.printf("Treasure Found");
}


