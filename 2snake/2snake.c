#include "display.h"
#include <stddef.h>
#include "settings.h"
#include <stdlib.h>
#include "helper.h"

#define DIMENSIONS 2

unsigned char* pos_from_loc(char* field, unsigned char* loc){
    return field + loc[0]*(FIELD_SIZE-1) + loc[1];
}

int spawn_fruit(char* field, unsigned char* loc){
    unsigned char* pos = pos_from_loc(field, loc);
    if(*pos == 0){
        *pos == 2;
        return 1;
    }
    return 0;
}

void rand_loc(unsigned char* loc){
    for(int i = 0; i < DIMENSIONS; i++){
        loc[i] = (unsigned char)(rand() % (FIELD_SIZE));
    }
}

void spawn_random_fruit(char* field, int spawn_attempts){
    int success = 0;
    int attempts = 0;
    unsigned char loc[DIMENSIONS] = {0};
    while(success == 0){
        rand_loc(loc);
        success = spawn_fruit(field, loc);
        attempts++;
        if(spawn_attempts > 0 && attempts>spawn_attempts){break;}
    }
}

void move_head(unsigned char* pos, char* dir){
    unsigned char delta_pos = 0;
    unsigned char x = *pos % FIELD_SIZE;
    unsigned char y = (*pos - *pos % FIELD_SIZE)/FIELD_SIZE;
    unsigned char xy[DIMENSIONS] = {x, y};
    for(int i = 0; i < DIMENSIONS; i++){
        xy[i] += *(dir+i) * power(FIELD_SIZE, i); 
        if(xy[i] > FIELD_SIZE){
            xy[i] %= FIELD_SIZE;
        }
    }
    *pos = y * FIELD_SIZE + x;
}

unsigned char move_snake(char* dir, char* field, unsigned char* posz, unsigned char* len){
    field[*(posz+*len-1-1)] = 0; 
    for (int i = 1; i < *len-1; i++){
        posz[i] = posz[i-1];
    }
    move_head(posz, dir);
    for(unsigned char i = 1; i < *len; i++){
        if(*posz == *(posz+i)){
            return 0;
        }
    }
    switch(*posz){
        case 2:
            *len++;
            spawn_random_fruit(field, BASE_SPAWN_ATTEMPTS);
        default:
            *posz = 1;
    }
    return 1;
}


unsigned char parse_input(char* input, char* move_dir, unsigned char* perspective){
    if(input == NULL){
        return 0;
    }
    switch(*input){
        case 'q':
            return 2;
        case 'm':
            if(move_dir == NULL){return 0;}

            for(int i = 0; i < DIMENSIONS; i++){
                switch(*(input+i+1)){
                    case '0':
                        move_dir[i] = 0;
                        break;
                    case '+':
                        move_dir[i] = 1;
                        break;
                    case '-':
                        move_dir[i] = -1;
                        break;
                    default:
                        display_error_char(*(input+i+1));
                        return 0;
                }
            }
            return 1;
        case 'p':
            if(perspective == NULL){return 0;}
            *perspective = *(input + 1);
            return 3;
        default:
            return 0;
    }
}

int main(){
    char field[FIELD_LW*FIELD_LW] = { 0 };
    char move_dir[DIMENSIONS] = { 0 };
    unsigned char snake_len = DEFAULT_SNAKE_LEN;
    unsigned char* snake_posz = malloc(sizeof(unsigned char)*snake_len);
    for(int i = 1; i < snake_len; i++){
        *(snake_posz+i) = 0;
    }
    *(snake_posz) = (FIELD_LW/2 * FIELD_SIZE + FIELD_SIZE/2);
    field[*snake_posz] = 1;
    unsigned char alive = 1;
    unsigned char perspective = 0;
    spawn_random_fruit(field, -1);
    init_display();
    render_display_changes(field, perspective);
    unsigned char input = 0;
    while(alive == 1){
        display_message("Please enter a command: ");
        while(input == 0){
            input = parse_input(take_input(DIMENSIONS), move_dir, &perspective);
            if(input == 0){
                display_message("Invalid command, please enter a command: ");
            }
        }
        switch(input){
            case 2:
                alive = 2;
                render_display_changes(field, perspective);
                break;
            case 1:
                alive = move_snake(move_dir, field, snake_posz, &snake_len);
            default: 
                render_display_changes(field, perspective);
                break;
        }
        debug_show_posz(snake_posz, field, snake_len);
    }
    input = 0;
    display_message("You lost, enter \'q\' to quit.");
    while(input != 2){input=parse_input(take_input(DIMENSIONS), NULL, NULL);}
    end_display();
    return 1;
}
