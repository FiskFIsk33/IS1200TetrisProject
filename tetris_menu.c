
/* tetris_menu.c
 * This file written 2023 by Jonathan Johnson
 */
#include <stdint.h>
#include <pic32mx.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tetris.h"
#include "tetris_state.h"




/*
        Read high scores from EEPROM and store them in th predefined global struct array */
void read_high_scores(){
        
        uint8_t memread[80]; //four bytes score, three initials and null byte
        i2c_read_eeprom(memread, 80, state.hs_address);
        for(int i = 0; i < 10; i++){
                memcpy(&high_scores[i].high_score, memread+8*i, 4),
                high_scores[i].initials[0] = memread[4+(8*i)];
                high_scores[i].initials[1] = memread[5+(8*i)];
                high_scores[i].initials[2] = memread[6+(8*i)];
                high_scores[i].initials[3] = '\0';

        }
}

/*
* write current list of high scores into memory
* divided into two, in order to write more than 64bytes and over more than one page
*/
void write_high_scores(){
        uint8_t memwrite[80]; //four bytes score, three initials and null byte
        for(int i = 0; i < 10; i++){
                memcpy(memwrite+i*8, &high_scores[i].high_score, 4); //copy the four score bytes
                memwrite[4+(8*i)] = high_scores[i].initials[0]; 
                memwrite[5+(8*i)] = high_scores[i].initials[1]; 
                memwrite[6+(8*i)] = high_scores[i].initials[2]; 
                memwrite[7+(8*i)] = '\0';                       //not really in use, but for the sake of it.
        }
        uint16_t writeaddr;
        writeaddr = state.hs_address;
        //write first 64 bytes to full page
        i2c_write_eeprom(memwrite, 64, writeaddr);
        writeaddr += 64;
        //write the 16 remaining bytes to next page
        i2c_write_eeprom((memwrite+64), 16, writeaddr);

}

/*
* helper function
* see below
*/
int sort_high_scores_compare_helper(const struct High_score *x, const struct High_score *y){
        return y->high_score - x->high_score;
}
/*      
 *  High scores implemented as an array of structs
 *       in order for the initials to tag along
 *       while sorting by highest score
**/
void sort_high_scores(){
        qsort(high_scores, 10 ,sizeof(struct High_score), (__comparefunc_t)sort_high_scores_compare_helper);
}

/*
* called once on startup, resets high score list
* if the button combination is held 
* at startup
*/
void reset_highscore(){
        get_buttons(state.buttons, state.buttons_changed);
        if(state.buttons[2] && state.buttons[3]){
                for(int i = 0; i < 10; i++){
                        high_scores[i].high_score = 0;
                        high_scores[i].initials[0] = 'X';
                        high_scores[i].initials[1] = ' ';
                        high_scores[i].initials[2] = ' ';
                        high_scores[i].initials[3] = '\0';
                }
                
                write_high_scores();
        }
}

/*
 *      Main Menu
 *      this handles the main menu for every frame
 *      called every frame while in it */
void menu_main(){

        //start level settable between 0 and 19
        if(state.buttons[2] && state.buttons_changed[2] && state.level < 19){
                state.level++;
        }else if(state.buttons[1] && state.buttons_changed[1] && state.level > 0){
                state.level--;
        }else if(state.buttons[3] && state.buttons_changed[3]){
                state.hs_menu_y = 0;
                state.state = MENU_HIGHSCORE;
        }else if(state.buttons[0] && state.buttons_changed[0]){
                /* since frame_counter is continuously counting every frame
                   using whatever value it is when the player presses this button
                   should be random enough for our purposes*/
                srand((unsigned int)frame_counter);
                //2 since the old non random tetr was loaded into "next"
                // we need to flush that.
                spawn_piece(&tetromino, &state.wait);
                spawn_piece(&tetromino, &state.wait);
                state.state = PLAYING;
                state.wait = 20;
                return;
        }

        memset(screen, 0, 128*4); //clear screen buffer
        render_line_xy(screen, 32-((frame_counter/3) % (37+5*18)), 0, "WELCOME TO TETRIS", 1);

        render_line(screen, 3, "SET   ");
        render_line(screen, 4, "LVL   ");
        char levelstring[6];
        tostring(levelstring, state.level);
        render_line(screen, 5, levelstring);

        render_line(screen, 7, "BOTTOM");
        render_line(screen, 8, "BTN TO");
        render_line(screen, 9, "PLAY  ");

        print_screen(screen);
}

/*
        High score menu
        this handles the high score menu
        called every frame while in it */
void menu_high_score(){
        int line_height = 16;
        //change y
        if(state.buttons[2] && state.hs_menu_y <= 0){
                state.hs_menu_y++;
        }if(state.buttons[1] && state.hs_menu_y >= -(HS_LIST_SIZE*line_height-128+5)){
                state.hs_menu_y--;
        }

        if(state.buttons[3] && state.buttons_changed[3]){
                state.state = MENU_MAIN;
        }
        memset(screen, 0, 128*4);
        render_line_xy(screen, 32-((frame_counter/3) % (37+5*10)), state.hs_menu_y, "HIGH SCORES", 1);
        //render the scores
        for(int i = 0; i < 10; i++){
                char str[10];
                tostring(str, high_scores[i].high_score);
                render_line_xy(screen, 3, i*line_height+13+state.hs_menu_y, str, 1);
        }
        //render the names
        for(int i = 0; i < 10; i++){
                int ypos = i*16+7+state.hs_menu_y;
                char rank[3];
                tostring(rank, i+1);
                render_line_xy(screen, 0, ypos, rank, 1);
                render_line_xy(screen, 15, i*line_height+7+state.hs_menu_y, high_scores[i].initials, 0);
        }

        print_screen(screen);
}

/*
*       checks for high score, chages state if so, otherwise
*       simply shows game over until the user presses the button
*/
void menu_game_over(){

        //if score is higher than lowest high score, we have  a new high score
        if(high_scores[9].high_score < state.score){

                memset(high_scores[9].initials, 'A', 3); //set name to AAA
                state.state=HIGH_SCORE;

        }else if(state.buttons[0] && state.buttons_changed[0]){
                init_game();
                return;
        }

        render_line(screen, 14, "");
        render_line(screen, 15, "GAME  ");
        render_line(screen, 16, "  OVER");
        render_line(screen, 17, "");
        print_screen(screen);
}

/*
*       new high score
*       this end game menu lets the user input
*       initials for the high score list
*/
void menu_new_high_score(){
        if(state.buttons[0] && state.buttons_changed[0] && high_scores[9].initials[state.initial_index] > 'A'){
                high_scores[9].initials[state.initial_index]--;
        }else if(state.buttons[3] && state.buttons_changed[3] && high_scores[9].initials[state.initial_index] < 'Z'){
                high_scores[9].initials[state.initial_index]++;
        }else

        if(state.buttons[1] && state.buttons_changed[1] && state.initial_index > 0){
                state.initial_index--;
        }else if(state.buttons[2] && state.buttons_changed[2] && state.initial_index < 2){
                state.initial_index++;
        }else if(state.buttons[2] && state.buttons_changed[2] && state.initial_index == 2){ //done
                high_scores[9].high_score = state.score;
                sort_high_scores();
                write_high_scores();
                init_game();
                return;
        }

        if(state.initial_index == 0){
                render_line(screen, 19, " ^    ");
        }else if(state.initial_index == 1){
                render_line(screen, 19, "  ^   ");
        }else if(state.initial_index == 2){
                render_line(screen, 19, "   ^  ");
        }

        char initial_line[7] = "      ";
        initial_line[1] = high_scores[9].initials[0];
        initial_line[2] = high_scores[9].initials[1];
        initial_line[3] = high_scores[9].initials[2];

        render_line(screen, 14, "");
        render_line(screen, 15, "HIGH  ");
        render_line(screen, 16, " SCORE");
        render_line(screen, 17, "");
        render_line(screen, 18, initial_line);
        print_screen(screen);
}
