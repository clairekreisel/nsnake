#ifndef NSNAKE_DISPLAY_H
#define NSNAKE_DISPLAY_H
#include "settings.h"

void init_display(void);

void end_display(void);

void render_display_changes(char* field, unsigned char perspective);

void display_message(char* message); //Assumes null terminated char array

void display_error_char(char error);

void debug_show_posz(char* posz, char* field, int len);

/*
Valid input options:
"q" -- Quit
"m(N <= dim)" -- m followed by values from MIN_MOVE_DIR to MAX_MOVE_DIR for up to each dimension, fill missing with 0
"pN" -- p followed by perspective value
*/
char* take_input(int dimensions);

#endif
