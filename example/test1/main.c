#include "../tgws.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int colors[15] = {
    40, 40, 60,//fone
    30, 30, 50,//fone kletka
    0, 0, 250,//flag
    255, 50, 250,//mine
    0, 0, 0//border
};

typedef struct {
    int is_mine;
    int revealed;
    int flagged;
    int adjacent_mines;
} Cell;

typedef struct {
    Cell **field;
    int height;
    int width;
    int player_y;
    int player_x;
    int revealed_count;
    int flagged_count;
    int game_lost;
    int game_won;
    int start_time;
    int first_click;
    int total_mines;
} Game;

Game game;
int page = 0;
int restgame=1; 
static uint64_t last_space_time = 0;
static int space_press_count = 0;

static uint64_t last_mouse_click_time = 0;
static int last_mouse_y = -1;
static int last_mouse_x = -1;

int prew_mbtn = 0;

void check_win_condition() {
    if (game.game_lost || game.game_won) return;

    int unopened_cells = (game.height * game.width) - game.revealed_count;
    
    if (unopened_cells == game.total_mines) {
        game.game_won = 1;
        
        for(int y = 0; y < game.height; y++) {
            for(int x = 0; x < game.width; x++) {
                if(!game.field[y][x].revealed && !game.field[y][x].flagged) {
                    game.field[y][x].flagged = 1;
                }
            }
        }
        game.flagged_count = game.total_mines;
    }
}

void place_mines(int skip_y, int skip_x) {
    srand(time(NULL) + rand());
    int placed = 0;
    int max_mines = game.height * game.width - 1;
    if (game.total_mines > max_mines) game.total_mines = max_mines;

    while(placed < game.total_mines) {
        int y = rand() % game.height;
        int x = rand() % game.width;
        
        if((y == skip_y && x == skip_x) || game.field[y][x].is_mine) continue;
        
        game.field[y][x].is_mine = 1;
        placed++;
    }
    
    for(int y = 0; y < game.height; y++) {
        for(int x = 0; x < game.width; x++) {
            if(game.field[y][x].is_mine) continue;
            
            int count = 0;
            for(int dy = -1; dy <= 1; dy++) {
                for(int dx = -1; dx <= 1; dx++) {
                    int ny = y + dy;
                    int nx = x + dx;
                    if(ny >= 0 && ny < game.height && nx >= 0 && nx < game.width) {
                        if(game.field[ny][nx].is_mine) count++;
                    }
                }
            }
            game.field[y][x].adjacent_mines = count;
        }
    }
}

void flood_fill(int y, int x) {
    if(y < 0 || y >= game.height || x < 0 || x >= game.width) return;
    if(game.field[y][x].revealed || game.field[y][x].flagged) return;
    
    game.field[y][x].revealed = 1;
    game.revealed_count++;
    
    if(game.field[y][x].adjacent_mines == 0 && !game.field[y][x].is_mine) {
        for(int dy = -1; dy <= 1; dy++) {
            for(int dx = -1; dx <= 1; dx++) {
                flood_fill(y + dy, x + dx);
            }
        }
    }
}

void game_open_cell(int y, int x) {
    if(y < 0 || y >= game.height || x < 0 || x >= game.width) return;
    if(game.field[y][x].revealed || game.field[y][x].flagged) return;
    
    if(game.first_click) {
        game.first_click = 0;
        place_mines(y, x);
    }
    
    if(game.field[y][x].is_mine) {
        game.game_lost = 1;
        for(int i = 0; i < game.height; i++) {
            for(int j = 0; j < game.width; j++) {
                if(game.field[i][j].is_mine) {
                    game.field[i][j].revealed = 1;
                }
            }
        }
    } else {
        flood_fill(y, x);
        check_win_condition();
    }
}

void game_open_adjacent(int y, int x) {
    if(y < 0 || y >= game.height || x < 0 || x >= game.width) return;
    if(!game.field[y][x].revealed) return;
    if(game.field[y][x].adjacent_mines == 0) return;
    
    int flag_count = 0;
    for(int dy = -1; dy <= 1; dy++) {
        for(int dx = -1; dx <= 1; dx++) {
            int ny = y + dy;
            int nx = x + dx;
            if(ny >= 0 && ny < game.height && nx >= 0 && nx < game.width) {
                if(game.field[ny][nx].flagged) flag_count++;
            }
        }
    }
    
    if(flag_count == game.field[y][x].adjacent_mines) {
        for(int dy = -1; dy <= 1; dy++) {
            for(int dx = -1; dx <= 1; dx++) {
                int ny = y + dy;
                int nx = x + dx;
                if(ny >= 0 && ny < game.height && nx >= 0 && nx < game.width) {
                    if(!game.field[ny][nx].revealed && !game.field[ny][nx].flagged) {
                        game_open_cell(ny, nx);
                    }
                }
            }
        }
    }
}

void game_toggle_flag(int y, int x) {
    if(y < 0 || y >= game.height || x < 0 || x >= game.width) return;
    if(game.field[y][x].revealed) return;
    
    if(game.field[y][x].flagged) {
        game.field[y][x].flagged = 0;
        game.flagged_count--;
    } else {
        game.field[y][x].flagged = 1;
        game.flagged_count++;
    }
}

void game_free() {
    for(int i = 0; i < game.height; i++) {
        free(game.field[i]);
    }
    free(game.field);
  restgame=1;
}

void game_init(int start_y, int start_x, int height, int width, int bombs) {
    game.height = height;
    game.width = width;
    game.total_mines = bombs;
    game.player_y = start_y;
    game.player_x = start_x;
    
    game.field = (Cell**)malloc(height * sizeof(Cell*));
    for(int i = 0; i < height; i++) {
        game.field[i] = (Cell*)malloc(width * sizeof(Cell));
        for(int j = 0; j < width; j++) {
            game.field[i][j].is_mine = 0;
            game.field[i][j].revealed = 0;
            game.field[i][j].flagged = 0;
            game.field[i][j].adjacent_mines = 0;
        }
    }

    game.revealed_count = 0;
    game.flagged_count = 0;
    game.game_lost = 0;
    game.game_won = 0;
    game.first_click = 1;
    game.start_time = time(NULL);
    space_press_count = 0;
    last_space_time = 0;
    last_mouse_click_time = 0;
    last_mouse_y = -1;
    last_mouse_x = -1;
}

void draw_cell(int y, int x, int is_hover) {
    int screen_y = 2 + y;
    int screen_x = 2 + x * 2;
    
    Cell *cell = &game.field[y][x];
    int is_player = (y == game.player_y && x == game.player_x);
    
    if(!cell->revealed) {
        int r = is_player ? 150 : (is_hover ? 80 : colors[0]);
        int g = is_player ? 150 : (is_hover ? 80 : colors[1]);
        int b = is_player ? 180 : (is_hover ? 100 : colors[2]);
        
        tgws_draw_square(r, g, b, screen_y, screen_x, 1, 2);
        
        if(cell->flagged) {
            fwrdisplay2(screen_y, screen_x, "F", colors[6], colors[7], colors[8], r, g, b);
        }
    } else {
        int r = is_player ? 60 : colors[3];
        int g = is_player ? 60 : colors[4];
        int b = is_player ? 80 : colors[5];
        
        tgws_draw_square(r, g, b, screen_y, screen_x, 1, 2);
        
        if(cell->is_mine) {
            fwrdisplay2(screen_y, screen_x, "X", colors[9], colors[10], colors[11], r, g, b);
        } else if(cell->adjacent_mines > 0) {
            char num[2];
            sprintf(num, "%d", cell->adjacent_mines);
            fwrdisplay2(screen_y, screen_x, num, 100 + cell->adjacent_mines * 15, 180, 100, r, g, b);
        }
    }
}

void handle_space_input() {
    uint64_t current_time = get_time_us();
    uint64_t time_diff = current_time - last_space_time;
    
    if(time_diff > 400000) {
        space_press_count = 1;
    } else {
        space_press_count++;
    }
    
    if(space_press_count == 1) {
        game_open_cell(game.player_y, game.player_x);
    } else if(space_press_count == 2) {
        game_open_adjacent(game.player_y, game.player_x);
        space_press_count = 0;
    }
    
    last_space_time = current_time;
}

void game_loop() {
    tgws_draw_square(colors[12], colors[13], colors[14], 0, 0, 60, 120);
    
    int elapsed = (game.game_lost || game.game_won) ? game.start_time : (time(NULL) - game.start_time);
    char ui[128];
    sprintf(ui, "TIME:%d MINES:%d FLAGS:%d/%d FPS:%i BUT:%i", elapsed, game.total_mines, 
            game.flagged_count, game.total_mines, refp,mbtn);
    tgws_print_label(0, 0, ui);
    
    int mouse_y = my - 2;
    int mouse_x = (mx - 2) / 2;
    
    for(int y = 0; y < game.height; y++) {
        for(int x = 0; x < game.width; x++) {
            int is_hover = (y == mouse_y && x == mouse_x && mouse_y >= 0 && mouse_x >= 0 && 
                          mouse_y < game.height && mouse_x < game.width);
            draw_cell(y, x, is_hover);
        }
    }
    
    if(game.game_lost) {
        static int blink = 0;
        static int butu=0;
        static int vrm[3];
        blink++;
    
        
    int ypos = game.height / 2 + 2;
    int xpos = game.width + 7;
    tgws_draw_square(10, 10, 20, ypos - 1, xpos - 2, 8, 20);
    tgws_draw_border(1, 255, 0, 0, ypos - 2, xpos - 3, 9, 21);
    if((blink+15)%2==0)tgws_print_label_f(ypos, xpos, "GAME OVER",10,10,20);
    tgws_print_label(ypos+2,xpos,"restart");
            
    //{{ button main menu
    butu = tgws_set_button_classic(ypos+2,xpos,0,9);
    if(butu == 2||(kbp ==1&&kbb=='m')){
    game_free();
    page=0;}
    vrm[0]=200;vrm[1]=200;vrm[2]=200;
    if(butu == 1){
    vrm[0]=255;vrm[1]=255;vrm[2]=255;
    } 
    tgws_print_label_fc(ypos+2,xpos,"MAIN MENU",vrm[0],vrm[1],vrm[2],10,10,20);
    //}} button main menu
    

    //{{ button restart
    butu = tgws_set_button_classic(ypos+4,xpos,0,9);
    if(butu == 2||(kbp ==1&&kbb=='r')){
    game_free();
    page=1;}
    vrm[0]=200;vrm[1]=200;vrm[2]=200;
    if(butu == 1){
    vrm[0]=255;vrm[1]=255;vrm[2]=255;
    } 
    tgws_print_label_fc(ypos+4,xpos,"RESTART",vrm[0],vrm[1],vrm[2],10,10,20);
    //}} button restart

    }
    
    if(game.game_won) {
           static int blink = 0;
        static int butu=0;
        static int vrm[3];
        blink++;
    
        
    int ypos = game.height / 2 + 2;
    int xpos = game.width + 7;
    tgws_draw_square(10, 10, 20, ypos - 1, xpos - 2, 8, 20);
    tgws_draw_border(1, 0, 255, 0, ypos - 2, xpos - 3, 9, 21);
    if((blink+15)%2==0)tgws_print_label_f(ypos, xpos, "WIN",10,10,20);
    tgws_print_label(ypos+2,xpos,"restart");
            
    //{{ button main menu
    butu = tgws_set_button_classic(ypos+2,xpos,0,9);
    if(butu == 2||(kbp ==1&&kbb=='m')){
    game_free();
    page=0;}
    vrm[0]=200;vrm[1]=200;vrm[2]=200;
    if(butu == 1){
    vrm[0]=255;vrm[1]=255;vrm[2]=255;
    } 
    tgws_print_label_fc(ypos+2,xpos,"MAIN MENU",vrm[0],vrm[1],vrm[2],10,10,20);
    //}} button main menu
    

    //{{ button restart
    butu = tgws_set_button_classic(ypos+4,xpos,0,9);
    if(butu == 2||(kbp ==1&&kbb=='r')){
    game_free();
    page=1;}
    vrm[0]=200;vrm[1]=200;vrm[2]=200;
    if(butu == 1){
    vrm[0]=255;vrm[1]=255;vrm[2]=255;
    } 
    tgws_print_label_fc(ypos+4,xpos,"RESTART",vrm[0],vrm[1],vrm[2],10,10,20);
    //}} button restart

    }
    
    if (!(game.game_lost+game.game_won)) {
        if(kbp == 1) {
            kbp = 0;
            
            if(kbb == 'w' || kbb == 'k') {
                game.player_y--;
                if(game.player_y < 0) game.player_y = game.height - 1;
            }
            else if(kbb == 's' || kbb == 'j') {
                game.player_y++;
                if(game.player_y >= game.height) game.player_y = 0;
            }
            else if(kbb == 'a' || kbb == 'h') {
                game.player_x--;
                if(game.player_x < 0) game.player_x = game.width - 1;
            }
            else if(kbb == 'd' || kbb == 'l') {
                game.player_x++;
                if(game.player_x >= game.width) game.player_x = 0;
            }
            else if(kbb == 'f') {
                game_toggle_flag(game.player_y, game.player_x);
            }
            else if(kbb == ' ') {
                handle_space_input();
            }
            else if(kbb == '0') {
                game.player_x = 0;
            }
            else if(kbb == '$') {
                game.player_x = game.width - 1;
            }
            else if(kbb == 'G') {
                game.player_y = game.height - 1;
            }
            else if(kbb == 'g') {
                game.player_y = 0;
            }
            else if(kbb == 'm') {
                game.player_y = game.height / 2;
                game.player_x = game.width / 2;
            }
            else if(kbb=='b') {
                page=0;
                game_free();
            }
            else if(kbb=='q') {
                game_free();
                tgws_close_window();
                exit(0);
            }
            else if(kbb=='r') {
                game_free();
                page=1;
            }
        }
        
        if(mbtn == 0 && prew_mbtn != 0) { 
            int cell_y = my - 2;
            int cell_x = (mx - 2) / 2;
            
            if(cell_y >= 0 && cell_y < game.height && cell_x >= 0 && cell_x < game.width) {
                game.player_y = cell_y;
                game.player_x = cell_x;
                
                uint64_t current_time = get_time_us();
                uint64_t time_diff = current_time - last_mouse_click_time;
                
                if((time_diff < 350000 && last_mouse_y == cell_y && last_mouse_x == cell_x) || 
                   game.field[cell_y][cell_x].revealed) {
                    game_open_adjacent(cell_y, cell_x);
                    last_mouse_click_time = 0;
                } else {
                    game_open_cell(cell_y, cell_x);
                    last_mouse_click_time = current_time;
                    last_mouse_y = cell_y;
                    last_mouse_x = cell_x;
                }
            }
        }
        
        if(mbtn == 2 && prew_mbtn != 2) { 
            int cell_y = my - 2;
            int cell_x = (mx - 2) / 2;
            
            if(cell_y >= 0 && cell_y < game.height && cell_x >= 0 && cell_x < game.width) {
                game.player_y = cell_y;
                game.player_x = cell_x;
                game_toggle_flag(cell_y, cell_x);
            }
        }
    }

}

int draw_main_menu(){

  int vrm[3];

  tgws_draw_border(2,120,120,170,6,10,12,30);
  tgws_print_label(8,14,"C M I N E S W E E P E R");

  int butu=0;

  //{{ button play
  butu = tgws_set_button_classic(10,14,2,8);
  if(butu == 2||(kbp==1 && kbb=='p'))page=1;
  vrm[0]=40;vrm[1]=40;vrm[2]=40;
  if(butu == 1){
  vrm[0]=0;vrm[1]=0;vrm[2]=0;
  }
  tgws_print_label_f(11,15,"P L A Y",vrm[0],vrm[1],vrm[2]);
  tgws_draw_border(1,255-vrm[0],255-vrm[1],255-vrm[2],10,14,2,8);
  //}} button play

  //{{ button quit
  butu = tgws_set_button_classic(10,27,2,8);
  if(butu == 2||(kbp==1&&kbb=='q')){
    //game_free();
    tgws_close_window();
    exit(0);
  };
  vrm[0]=40;vrm[1]=40;vrm[2]=40;
  if(butu == 1){
  vrm[0]=0;vrm[1]=0;vrm[2]=0;
  }
  tgws_print_label_f(11,28,"Q U I T",vrm[0],vrm[1],vrm[2]);
  tgws_draw_border(1,255-vrm[0],255-vrm[1],255-vrm[2],10,27,2,8);
  //}} button quit

  //{{ button sett
  butu = tgws_set_button_classic(15,14,2,8);
  if(butu == 2||(kbp==1&&kbb=='s'))page=3;
  vrm[0]=40;vrm[1]=40;vrm[2]=40;
  if(butu == 1){
  vrm[0]=0;vrm[1]=0;vrm[2]=0;
  }
  tgws_print_label_f(16,15,"S E T T",vrm[0],vrm[1],vrm[2]);
  tgws_draw_border(1,255-vrm[0],255-vrm[1],255-vrm[2],15,14,2,8);
  //}} button sett

  //{{ button abou
  butu = tgws_set_button_classic(15,27,2,8);
  if(butu == 2||(kbp==1&&kbb=='a'))page=2;
  vrm[0]=40;vrm[1]=40;vrm[2]=40;
  if(butu == 1){
  vrm[0]=0;vrm[1]=0;vrm[2]=0;
  }
  tgws_print_label_f(16,28,"A B O U",vrm[0],vrm[1],vrm[2]);
  tgws_draw_border(1,255-vrm[0],255-vrm[1],255-vrm[2],15,27,2,8);
  //}} button abou


return 0;}

int draw_about(){

static int minipage=0;
static char str[64];
  sprintf(str,"< left|> right|b back| page:%i ",minipage);
  tgws_print_label(6,10,str);
  tgws_draw_border(minipage/4+1,255,255,255,7,10,15,30);

  if(kbp==1){
    kbp=0;
     if(kbb=='<')minipage--;
else if(kbb=='>')minipage++;
else if(kbb=='b')page = 0;
  }

  if(minipage<0)minipage=10;
  if(minipage>10)minipage=0;

switch(minipage){

	case 0:
		tgws_print_txt(8,11,
					   "hello it's Cmainsweeper\n"
					   "the game written in C\n"
					   "using the tgws library.\n"
					   "It is a classic minesweeper\n"
					   "ported to the terminal.\n"
					   "Next pages will explain\n"
					   "game rules, controls,\n"
					   "and the GPLv2 license.\n"
					   "Use terminal mouse or\n"
					   "keys to flip pages.\n"
					   "Press 'b' to go back\n"
					   "to main menu, then 'p'\n"
					   "or click to start play.\n");
		break;
	case 1:
		tgws_print_txt(8,11,
					   "GAME OBJECTIVE:\n"
					   "Your main goal here is\n"
					   "to clear the entire\n"
					   "minefield by uncovering\n"
					   "all safe cells without\n"
					   "detonating any hidden\n"
					   "mines.\n"
					   "Uncovering a single mine\n"
					   "results in an immediate\n"
					   "game over.\n"
					   "You win successfully\n"
					   "once every safe cell has\n"
					   "been fully revealed.\n"
					   "Be careful and think.\n");
		break;
	case 2:
		tgws_print_txt(8,11,
					   "NUMBER HINTS:\n"
					   "Revealing an empty cell\n"
					   "often exposes a number.\n"
					   "This digit indicates how\n"
					   "many mines are hidden in\n"
					   "the 8 adjacent cells\n"
					   "(horizontally, vertically,\n"
					   "and diagonally).\n"
					   "Use logical deduction\n"
					   "to figure out exactly\n"
					   "where the dangerous\n"
					   "mines are located on\n"
					   "the board layout.\n");
		break;
	case 3:
		tgws_print_txt(8,11,
					   "MARKING FLAGS:\n"
					   "If you strongly suspect\n"
					   "that a specific cell\n"
					   "contains a hidden mine,\n"
					   "you can place a flag on\n"
					   "it using the 'f' key\n"
					   "or Right Mouse Button\n"
					   "(RMB) for protection.\n"
					   "This prevents accidental\n"
					   "clicks during intense\n"
					   "sweeping sessions.\n"
					   "Press 'f' or RMB again\n"
					   "to remove the flag.\n");
		break;
	case 4:
		tgws_print_txt(8,11,
					   "CONTROLS (BASIC):\n"
					   "Terminal mouse support:\n"
					   "Left Mouse Button (LKM)\n"
					   "opens selected cell.\n"
					   "Right Mouse Button\n"
					   "places or removes flag.\n"
					   "WASD:\n"
					   "Move cursor across board.\n"
					   "Space:\n"
					   "Open current cell.\n"
					   "F key:\n"
					   "Toggle flag.\n");
		break;
	case 5:
		tgws_print_txt(8,11,
					   "VIM MOTIONS:\n"
					   "The game features full\n"
					   "built-in Vim keybindings\n"
					   "for lightning-fast\n"
					   "navigation on the grid:\n"
					   "h j k l - move cursor\n"
					   "0 - jump to start of line\n"
					   "$ - jump to end of line\n"
					   "g - jump to board top\n"
					   "G - jump to board bottom\n"
					   "m - center camera view\n"
					   "on current position.\n");
		break;
	case 6:
		tgws_print_txt(8,11,
					   "NAVIGATION:\n"
					   "To start playing from\n"
					   "this manual screen, you\n"
					   "need to press 'b' to\n"
					   "return to the main menu,\n"
					   "and then press 'p' to\n"
					   "start the game.\n"
					   "Alternatively, you can\n"
					   "just use your terminal\n"
					   "mouse to click the\n"
					   "menu buttons directly.\n");
		break;
	case 7:
		tgws_print_txt(8,11,
					   "GPLv2 LICENSE (1/2):\n"
					   "This program is free\n"
					   "software: you can redis-\n"
					   "tribute it and/or modify\n"
					   "it under the terms of the\n"
					   "GNU General Public\n"
					   "License as published by\n"
					   "the Free Software Found-\n"
					   "ation, version 2 only.\n"
					   "You are free to study\n"
					   "and modify the source\n"
					   "code according to your\n"
					   "personal needs.\n");
		break;
	case 8:
		tgws_print_txt(8,11,
					   "GPLv2 LICENSE (2/2):\n"
					   "It is distributed in the\n"
					   "hope that it will be\n"
					   "useful, but WITHOUT ANY\n"
					   "WARRANTY; without even\n"
					   "the implied warranty of\n"
					   "MERCHANTABILITY or\n"
					   "FITNESS FOR A PARTICULAR\n"
					   "PURPOSE. See the GNU\n"
					   "General Public License\n"
					   "for more details about\n"
					   "redistribution rights\n"
					   "and conditions.\n");
		break;
	case 9:
		tgws_print_txt(8,11,
					   "READY TO PLAY?\n"
					   "All rules, mouse and\n"
					   "Vim controls, and license\n"
					   "terms are covered.\n"
					   "Press 'b' for menu and\n"
					   "'p' to start playing,\n"
					   "or click with mouse.\n"
					   "Time to test your logic\n"
					   "and sweep the field!\n"
					   "Good luck and enjoy\n"
					   "the clean C code!\n");
		break;

	}

return 0;}

int draw_setting(){

static int minipage=0;
static char str[64];
  sprintf(str,"< left|> right|b back| page:%i ",minipage);
  tgws_print_label(6,10,str);
  tgws_draw_border(minipage/4+1,255,255,255,7,10,15,30);

  if(kbp==1){
    kbp=0;
     if(kbb=='<')minipage--;
else if(kbb=='>')minipage++;
else if(kbb=='b')page = 0;
  }

  if(minipage<0)minipage=10;
  if(minipage>10)minipage=0;

switch(minipage){
    case 0:
    //color
      static int kss=0,kk=0;
      tgws_print_label(8,12,"fone -v");
      tgws_print_label(11,12,"fone game -v");
      tgws_print_label(14,12,"flag -v");
      tgws_print_label(17,12,"sweeper -v");
      tgws_print_label(20,12,"border -v");

      if(fcl_palitra!=1)for(int i=0;i<5;i++){
        int ot = i*3;
        fwrdisplay2(9+ot,11,"\u2502",255,255,255,0,0,0);
        fwrdisplay2(8+ot,11,"\u2502",255,255,255,0,0,0);
        fwrdisplay2(9+ot,24,"\u2502",255,255,255,0,0,0);
        tgws_draw_square(colors[ot],colors[ot+1],colors[ot+2],9+ot,12,1,12);
        kss = tgws_set_button_classic(8+ot,11,2,12);
        if(kss==2){kk=i;break;}
      }
 if(kss==2||fcl_palitra==1){
     int *scolora= tgws_call_palitra(10,10);
     if(scolora[0]!=-1){
         kss=0;
         colors[kk*3+1]=scolora[1];
         colors[kk*3+2]=scolora[2];
         colors[kk*3]=scolora[0];}
      //close color
    }
    break;
  }

return 0;}

int main(int argc, char *argv[]) {
    kbb = 20;
    tgws_init_window(argc, argv);
    page = 0;
    
    while(1) {
        switch(page) {
            case 1:
                if(restgame==1){game_init(6, 10, 40, 40, 20);restgame=0;}
                game_loop();
                break;
            case 0:
                draw_main_menu();break;
            case 2:
                draw_about();break;
            case 3:
                draw_setting();break;
            case 4:

                tgws_draw_square(colors[12], colors[13], colors[14], 0, 0, 60, 120);
                if(kbp == 1 && kbb == 'q') kbp = 0;
                break;
        }
        
        prew_mbtn = mbtn;
        tgws_draw_element();
    }

    game_free();
    tgws_close_window();
    return 0;
}
