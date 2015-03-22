#include <allegro.h>
#include <stdio.h> //changed: C++ Namespace version of stdio instead of C version
   // May prevent depriciation warnings and such with compilation on some compilers
 
//using namespace std; // Include default namespace into function search scope
 
// screen dimensions (single place to change)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
 
#define GRID_W 20
#define GRID_H 20
 
#define MAX_FPS 60
#define LUCKY_NUM 7 //;)
 
// game state definitions
#define GAME_STATE_INTRO 0
#define GAME_STATE_MAINGAME  1
#define GAME_STATE_ABOUT 2
#define GAME_STATE_GAMEOVER  3
#define GAME_STATE_FINISH 4
 
// backbuffer
BITMAP* buffer;
 
//speed counter
volatile int speed_counter;
 
void increment_speed_counter ()
{
 speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter)
 
//fps counter
int fnum, fps;
 
volatile int fps_counter;
 
void increment_fps_counter ()
{
 fps_counter++;
}
END_OF_FUNCTION(increment_fps_counter)
 
 
// tile information
char tile_grid[GRID_H][GRID_W] = { 0 };
 
// size of tiles (pixels) which is now 16x16
// SCREEN_TILE_NUM_* is the number of tiles that can fit on the screen (X and Y)
#define TILE_SIZE 16
#define SCREEN_TILE_NUM_X 20
#define SCREEN_TILE_NUM_Y 15
 
 
/* 
*  Character definitions:
*/
#define CHARACTER_MAX_SPEED 2
#define MAX_ITEMS 2
#define ITEM_SHIELD 0
#define ITEM_SWORD 1
 
// character definition.
typedef struct Character {
        //character position.
        int x, y;
        //character speeds.
        int speedX, speedY;
        //character health, alive.
        int health;
        //general flags.
        int flag;
        //items of character
        int items[MAX_ITEMS];
        //general score.
        int score;
        //image of character
        BITMAP* image;
} Character;
 
void draw_character(Character *character) { // changed, made code more modular
        // the offsets are for allowing to show the character in the middle of the screen
        // or by setting their minimum
        int offsetx, offsety;
 
        // TODO: Implement character moving between tiles
 
        // set the 'left' tile offset that is displayed
        offsetx = character->x;
        if (character->x < (SCREEN_TILE_NUM_X / 2)) {
                offsetx = 0;
        } else if (character->x > (GRID_W - SCREEN_TILE_NUM_X / 2)) {
                offsetx = GRID_W - SCREEN_TILE_NUM_X;
        }
 
        // set the 'top' tile offset that is displayed
        offsety = character->y;
        if (character->y < (SCREEN_TILE_NUM_Y / 2)) {
                offsety = 0;
        } else if (character->y > (GRID_H - SCREEN_TILE_NUM_Y / 2)) {
                offsety = GRID_H - SCREEN_TILE_NUM_Y;
        }
 
        // draw the character onto the backbuffer
        // TODO: create variables for the type of action and direction for the source_x and source_y
        masked_blit(character->image, buffer, 0, 0, character->x - offsetx, character->y - offsety, TILE_SIZE, TILE_SIZE);
        return;
}
 
//Creates a instance of a character with default values.
Character* new_Character(int x, int y) {  // changed - syntax error there was ; before {
        Character *character;
        character = (Character *) malloc(sizeof(Character));
        character->x = x;
        character->y = y;
        character->health = 100;
        character->score = 0;
        character->speedX = character->speedY = 0; // needs to be removed if game = zelda type game
        character->flag = 0;
        //TODO: get a "character.bmp! :D
        //character->image = load_bitmap("character.bmp", NULL);
        character->image = create_bitmap(10,10);
        clear_bitmap (character->image);
        return character; //added
}
 
void character_SpeedUp(Character *character, int x, int y) {
        if(character->speedX < CHARACTER_MAX_SPEED && character->speedX > -CHARACTER_MAX_SPEED) {
                character->speedX += x;
        }
        if(character->speedY < CHARACTER_MAX_SPEED && character->speedY > -CHARACTER_MAX_SPEED) {
                character->speedY += y;
        }
}
 
void character_LogicCycle(Character *character) {
        character->x += character->speedX;
        character->y += character->speedY;
        if(character->x < 0) {
                character->x = 0;
        } else if(character->x + character->image->w > SCREEN_WIDTH) {
                character->x = SCREEN_WIDTH - character->image->w;
        }
        if(character->y < 0) {
                character->y = 0;
        } else if(character->y + character->image->h > SCREEN_HEIGHT) {
                character->y = SCREEN_HEIGHT - character->image->h;
        }
        if(character->speedX > CHARACTER_MAX_SPEED) {
                character->speedX--;
        } else if(character->speedX < - CHARACTER_MAX_SPEED) {
                character->speedX++;
        }
        if(character->speedY > CHARACTER_MAX_SPEED) {
                character->speedY--;
        } else if(character->speedY < - CHARACTER_MAX_SPEED) {
                character->speedY++;
        }
}
/* 
*  End of character definitions.
*/
 
// game loop functions
void show_Intro();
void main_Loop();
void game_Over();
void show_About();
void input();
 
//added
Character* new_Character(int x, int y);
 
void draw_map();
void draw_character();
 
// changed: made it more obvious
int gamestate = GAME_STATE_INTRO;  // 0=intro, 1=in game, 2=gameover, 3=exit //changed
 
Character* Player;
 
int main(int argc, char *argv[])
{
        // test for failure
        if(0 != allegro_init()) { fprintf(stderr, "%s", allegro_error); exit(1); }
        install_timer();
        install_keyboard();
        install_timer ();
        speed_counter = 0;
        fnum = 0;
        fps = 0;
        fps_counter = 0;
        LOCK_VARIABLE(speed_counter);
        LOCK_FUNCTION(increment_speed_counter);
        LOCK_VARIABLE(fps_counter);
        LOCK_FUNCTION(increment_fps_counter);
 
        install_int_ex(increment_fps_counter, BPS_TO_TIMER(LUCKY_NUM));
 
 
        // check if can create screen
        if (0 != set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0)){allegro_message("%s", allegro_error); return(1);} //changed
 
        // create backbuffer
        buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
 
 
        // begin game loop
        while(gamestate != GAME_STATE_FINISH)
        {
                if(gamestate == GAME_STATE_INTRO) show_Intro();
                if(gamestate == GAME_STATE_MAINGAME) main_Loop();
                if(gamestate == GAME_STATE_GAMEOVER) game_Over();
                if(gamestate == GAME_STATE_ABOUT) show_About();
        } // end game loop
 
        // remove backbuffer from memory
        destroy_bitmap(buffer);
 
        // finish
        allegro_exit();
        return 0;
} END_OF_MAIN()
 
void show_Intro() {
 
        //added: missing 2nd parameter
        //BITMAP* title_screen_bitmap = load_bitmap("title.bmp", NULL); //The title screen bitmap.
        BITMAP *title_screen_bitmap = create_bitmap(SCREEN_W, SCREEN_H);
 
        #define MIN_MENU_SEL 0
        #define MAX_MENU_SEL 2  
 
        int menusel = MIN_MENU_SEL;
        while (gamestate == GAME_STATE_INTRO) {
                // INPUT ROUTINE
                //changed: fixed various syntax errors in the next few lines
                if (keypressed()) {
                        int k = readkey();
                        //changed: now uses KEY_* vars
                        switch (k >> 8) {
                                case KEY_ESC: gamestate = GAME_STATE_INTRO; break; //escape
                                case KEY_UP: menusel--; if (menusel < MIN_MENU_SEL) menusel = MIN_MENU_SEL; break;
                                case KEY_DOWN: menusel++; if (menusel > MAX_MENU_SEL) menusel = MAX_MENU_SEL; break;
                                case KEY_ENTER:
                                switch (menusel) {
                                        case 0: {gamestate = GAME_STATE_MAINGAME; install_int_ex(increment_speed_counter, BPS_TO_TIMER(MAX_FPS));} 
                                        break;
                                        case 1: gamestate = GAME_STATE_ABOUT; 
                                        break;
                                        case 2: gamestate = GAME_STATE_FINISH; 
                                        break;
                                } break;
                        }
                        clear_keybuf();
                }
 
                // DRAWING ROUTINE
                // draw background
                blit(title_screen_bitmap, buffer, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
 
                // draw menu
                if (menusel == 0) { // play
                        textprintf_ex(buffer, font, 20,  80, makecol(0,255,255), -1, "Play Game");
                } else {
                        textprintf_ex(buffer, font, 20,  80, makecol(255,255,255), -1, "Play Game");
                }
                if (menusel == 1) { // about
                        textprintf_ex(buffer, font, 20,  90, makecol(0,255,255), -1, "About Game");
                } else {
                        textprintf_ex(buffer, font, 20,  90, makecol(255,255,255), -1, "About Game");
                }
                if (menusel == 2) { // exit
                        textprintf_ex(buffer, font, 20,  100, makecol(0,255,255), -1, "Exit Game");
                } else {
                        textprintf_ex(buffer, font, 20,  100, makecol(255,255,255), -1, "Exit Game");
                } // added
 
                //draw game state (temporary, just till we get some real game action)
                rectfill (buffer, 0, 0, screen->w, 10, 0);
                switch (gamestate)
                {
                        case (GAME_STATE_INTRO): {textprintf_right_ex(buffer, font, screen->w,  0, makecol(255,255,255), -1, "Intro"); break;}
                        //no longer needed cause of the debug coords... :)
                        //case (GAME_STATE_MAINGAME): {textprintf_right_ex(buffer, font, screen->w,  0, makecol(255,255,255), -1, "Main game"); break;}
                        case (GAME_STATE_GAMEOVER): {textprintf_right_ex(buffer, font, screen->w,  0, makecol(255,255,255), -1, "Game Over"); break;}
                        case (GAME_STATE_ABOUT): {textprintf_right_ex(buffer, font, screen->w,  0, makecol(255,255,255), -1, "About");break;}
                }
 
                // draw to screen
                blit(buffer, screen, 0,0, 0,0, SCREEN_WIDTH,SCREEN_HEIGHT);
        }
 
        destroy_bitmap(title_screen_bitmap);
        return;
}
 
// main game screen
void main_Loop() {
        Player = new_Character(100,100);
        while (gamestate == GAME_STATE_MAINGAME) {
                if (key[KEY_ESC]) {gamestate = GAME_STATE_INTRO; remove_int (increment_speed_counter); speed_counter = 0;}
 
                while (speed_counter > 0)
                {
                 input();
 
                 //atualizes the player
                 character_LogicCycle(Player);
                 speed_counter--;
                }
 
                // draw the map
                draw_map();
 
                // draw the character
                draw_character(Player); //changed
 
                //character positions for debugging
                rectfill (buffer, 0, 0, screen->w, 20, 0);
                textprintf_right_ex(buffer, font, screen->w,  0, makecol(255,255,255), -1, "X: %d", Player->x);
                textprintf_right_ex(buffer, font, screen->w,  10, makecol(255,255,255), -1, "Y: %d", Player->y);
 
                //count fps :)
                if (fps_counter >= LUCKY_NUM)
                {
                 remove_int (increment_fps_counter);
                 fps_counter = 0;
                 fps = fnum;
                 fnum = 0;
                 install_int_ex(increment_fps_counter, BPS_TO_TIMER(LUCKY_NUM));
                }
 
                //draw fps count
                textprintf_ex(buffer, font, 0,  0, makecol(255,255,255), -1, "FPS: %d", fps);
 
                // draw to screen
                blit(buffer, screen, 0,0, 0,0, SCREEN_WIDTH,SCREEN_HEIGHT);
 
                //add fps :)
                fnum++;
        }
        delete Player;
}
 
// game over screen
void game_Over() {
        while (gamestate == GAME_STATE_GAMEOVER) {
                if (key[KEY_ESC]) gamestate = GAME_STATE_INTRO;
        }
}
 
// about screen
void show_About() {
        while (gamestate == GAME_STATE_ABOUT) {
                if (key[KEY_ESC]) gamestate = GAME_STATE_INTRO;
        }
}
 
// draw the map onto the screen
// int offsetx: the offset from the left of the map
// int offsety: the offset from the top of the map
void draw_map() { //this thing is messy, I won't mess it more
        // where the tiles begin
        int offsetx, offsety;
 
        // offset the left-side of the tiles
        offsetx = Player->x - SCREEN_TILE_NUM_X / 2;
        if (Player->x < (SCREEN_TILE_NUM_X / 2)) {
                offsetx = 0;
        } else if (Player->x > (GRID_W - SCREEN_TILE_NUM_X / 2)) {
                offsetx = GRID_W - SCREEN_TILE_NUM_X / 2;
        }
 
        // offset the top-side of the tiles
        offsety = Player->y - SCREEN_TILE_NUM_Y / 2; //I believe it should be like this... it was like this before, correct me if I'm wrong:
        //offsety = new_Character - SCREEN_TILE_NUM_Y / 2;
        if (Player->y < (SCREEN_TILE_NUM_Y / 2)) {
                offsety = 0;
        } else if (Player->y > (GRID_H - SCREEN_TILE_NUM_Y / 2)) {
                offsetx = GRID_H - SCREEN_TILE_NUM_Y / 2;
        }
 
        for (int x = 0; x < SCREEN_TILE_NUM_X; x++) {
                for (int y = 0; y < SCREEN_TILE_NUM_Y; y++) {
                        // TODO: Get a tilemap!
                        //blit(tilemap, buffer, tile_grid[x+offsetx][y+offsety], 0, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                }
        }
}
 
//Gets keyboard input from the user
void input() {
        //Maybe this is wrong, long time since I mess with allegro
        if (key[KEY_UP]) character_SpeedUp(Player, 0, 1); 
        if (key[KEY_DOWN]) character_SpeedUp(Player, 0, -1);
        if (key[KEY_LEFT]) character_SpeedUp(Player, -1, 0);
        if (key[KEY_RIGHT]) character_SpeedUp(Player, 1, 0);
}
