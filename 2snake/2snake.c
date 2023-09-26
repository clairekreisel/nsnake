#include "display.h"
#include <stddef.h>
#include "settings.h"
#include <stdlib.h>

#define DIMENSIONS 2

unsigned char* pos_from_loc(char* field, unsigned char* loc){
    return field + loc[0]*FIELD_SIZE + loc[1];
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
        loc[i] = (unsigned char)(rand() % (FIELD_SIZE + 1));
    }
}

void spawn_random_fruit(char* field, int spawn_attempts){
    int success = 0;
    int attempts = 0;
    unsigned char loc[DIMENSIONS] = {0};
    while(success == 0 && attempts < spawn_attempts){
        rand_loc(loc);
        success = spawn_fruit(field, loc);
        attempts++;
    }
}

void move_head(unsigned char* locs, char* dir){
    for(int i = 0; i < DIMENSIONS; i++){
        unsigned char next_loc = locs[i] += dir[i];
        if(next_loc >= FIELD_SIZE){
            next_loc = 0;
        }
        else if(next_loc < 0){
            next_loc = FIELD_SIZE - 1;
        }
        locs[i] = next_loc;
    }
}

unsigned char move_snake(char* dir, char* field, unsigned char* locs, unsigned char* len){
    *pos_from_loc(field, locs+(*len-1)*DIMENSIONS) = 0;
    for(unsigned char i = DIMENSIONS * (*len-1); i > 1; i--){
        locs[i] = locs[i-2];
    }
    move_head(locs, dir);
    for(unsigned char i = 1; i < *len; i++){
        if(locs[0] == locs[i * DIMENSIONS] && locs[1] == locs[i * DIMENSIONS + 1]){
            return 0;
        }
    }
    unsigned char* head_pos = pos_from_loc(field, locs);
    switch(*head_pos){
        case 2:
            *len++;
            spawn_random_fruit(field, BASE_SPAWN_ATTEMPTS);
        default:
            *head_pos = 1;
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
            char is_neg = 0;
            for(int i = 0; i < DIMENSIONS*2; i++){
                switch(*(input+i+1)){
                    case '0':
                        move_dir[i/2] = 0;
                        if(i%2 == 0){
                            i++;
                        }
                        continue;
                    case '1':
                        move_dir[i/2] = 1;
                        if(is_neg == 1){
                            move_dir[i/2] *= -1;
                            is_neg = 0;
                        }
                        if(i%2 == 0){
                            i++;
                        }
                        continue;
                    case '-':
                        is_neg = 1;
                        break;
                }
            }
            free(input);
            return 1;
        case 'p':
            *perspective = *(input + 1);
            return 3;
        default:
            return 0;
    }
}

int main(){
    char field[FIELD_LW*FIELD_LW] = { 0 };
    char move_dir[2] = {0,1};
    unsigned char snake_len = DEFAULT_SNAKE_LEN;
    field[FIELD_LW/2 * FIELD_SIZE + FIELD_LW/2] = 1;
    unsigned char snake_locs[MAX_SNAKE_LEN*DIMENSIONS] = {FIELD_SIZE/2, FIELD_SIZE/2};
    unsigned char alive = 1;
    unsigned char perspective = 0;
    spawn_random_fruit(field, 8000);
    init_display();
    while(alive == 1){
        unsigned char input = 0;
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
                render_display_changes(NULL, perspective);
                break;
            case 1:
                alive = move_snake(move_dir, field, snake_locs, &snake_len);
            default: 
                render_display_changes(field, perspective);
                break;
        }
    }
    end_display();
    return 1;
}
