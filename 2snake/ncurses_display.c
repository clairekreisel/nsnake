#include "display.h"
#include <ncurses.h>
#include "settings.h"
#include <stdlib.h>

#define BASE_X 10
#define BASE_Y 5

void init_display(void){
    initscr();
    move(BASE_X, BASE_Y);
    for(int i = 0; i < FIELD_SIZE + 2; i++){
        move(BASE_X+i, BASE_Y);
        for(int j = 0; j < FIELD_SIZE + 2; j++){
            addch(219);
        }
    }
    for(int i = 0; i < FIELD_SIZE; i++){
        move(BASE_X+1+i, BASE_Y+1);
        for(int j = 0; j < FIELD_SIZE; j++){
            addch(' ');
        }
    }
    addch('\n');
}

void end_display(void){
    endwin();
}

void render_display_changes(char* field, unsigned char perspective){
    for(int i = 0; i < FIELD_SIZE*FIELD_SIZE; i++){
        if(i%FIELD_SIZE == 0){
            move(BASE_X+i/FIELD_SIZE+1, BASE_Y+1);
        }
        switch(field[i]){
            case 0:
                addch((i/FIELD_SIZE)+48);
                break;
            case 1:
                addch('S');
                break;
            case 2:
                addch('O');
                break;
        }
    }
    refresh();
}

void display_message(char* message){ //Assumes null terminated char array
    mvprintw(BASE_X+FIELD_SIZE+3, BASE_Y, "%s", message);
    refresh();
}
/*
Valid return values:
{q} -- Quit
{m(N <= dim)} -- m followed by values from MIN_MOVE_DIR to MAX_MOVE_DIR for up to each dimension, fill missing with 0
{pN} -- p followed by perspective value
NULL
*/
char* take_input(int dimensions){
    move(BASE_X+FIELD_SIZE+4, BASE_Y);
    char* result = NULL;
    char raw_in[254];
    getstr(raw_in);
    move(BASE_X+FIELD_SIZE+2, BASE_Y);
    for(int i = 0; i<254; i++){addch(' ');}
    switch(raw_in[0]){
        case 'q':
            char result_q = 'q';
            result = &result_q;
            break;
        case 'm':
            char* result_m = malloc(sizeof(char) * (dimensions*2 + 1));
            result_m[0] = 'm';
            int is_negative = 0;
            for(int i = 1; i<dimensions+1; i++){
                char next = raw_in[i];
                if(next == '\0'){
                    break;
                }
                result_m[i] = next;
            }
            result = result_m;
            break;
        case 'p':
            char result_p[2] = {'p'};
            if(raw_in[1] != '\0'){
                result_p[1] = raw_in[1];
            }
            result = result_p;
            break;
    }
    return result;
}
