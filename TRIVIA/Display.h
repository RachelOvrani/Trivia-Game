#pragma once

#include <iostream>
#include <windows.h>   // WinApi header
#include "Types.h"




enum COLOR
{
	DEFAULT = 7, // text color contains blue.
	
	FORE_BLUE = FOREGROUND_BLUE, // text color contains blue.
	FORE_GREEN = FOREGROUND_GREEN, // text color contains green.
	FORE_RED = FOREGROUND_RED, // text color contains red.
	FORE_INTENSITY = FOREGROUND_INTENSITY, // text color is intensified.
	
	BACK_BLUE = BACKGROUND_BLUE, // background color contains blue.
	BACK_GREEN = BACKGROUND_GREEN, // background color contains green.
	BACK_RED = BACKGROUND_RED, // background color contains red.
	BACK_INTENSITY = BACKGROUND_INTENSITY, // background color is intensified.
};


void set_cursor_position(int x, int y);
COORD get_cursor_position();
void set_cursor_visible(bool visible);
void ClearScreen();



void init_display();
void set_heb(char* str);
void reverseStr(char* str);
void str_fix_for_hebrew(char* str);

void show_all_colors();
void set_color(int color);
void set_color(COLOR color);

void draw_border(int x, int y, int wide, int high, int color);
void draw_borders(Border* borders, int size);

void clear_border(int x, int y, int wide, int high);
void clear_borders(Border* borders, int size);

void blink_borders(Border* borders, int size, int duration);

void draw_border_of_TRIVIA(COLOR color);
void draw_players_info_section(GameSettings* gameSettings);

void show_text(TextLineInfo tli);
void show_text(const char* message);
void show_text(const char* message, int color);
void show_text(const char* message, int x, int y);
void show_text(const char* message, int color, int x, int y);

void show_text_with_beep(const char* msg, int speed, int sound_req);

void clear_text(const char* message, int x, int y);
void clear_text(TextLineInfo tli);

void blink_texts(TextLineInfo* text, int size, int duration, int times);
void blink_texts(TextLineInfo* str, int size, int duration);
void blink_texts(const char* text, int pos_x, int pos_y, int color, int duration, int times);

void draw_spaces(int times);
void draw_spaces(int times, int x, int y);
void draw_spaces(int x, int y, int wide, int high);

// **********  TRIVIA Functions *********************

void draw_open_screen();
void draw_MAZAL_TOV_screen();

void show_subject_suggested(SubjectSuggestList subject_suggest);

void _draw_timer(int choise_time);
void _update_timer(PlayersList players, int times);

void _draw_symbole1(int x, int y);
void _clear_symbole1(int x, int y);

void _draw_symbole2(int x, int y);
void _clear_symbole2(int x, int y);

void _draw_symbole3(int x, int y);
void _clear_symbole3(int x, int y);

void _draw_symbole4(int x, int y);
void _clear_symbole4(int x, int y);

void _draw_symbole5(int x, int y);
void _clear_symbole5(int x, int y);


void write_the_victory(PlayersList* players);
void lose_message(PlayersList* players);


void clear_border_of_players_amount(int x, int y, int wide, int high);
void clear_bord_of_question();
void clear_timer_area();
void clear_timer_second();
void clear_border_whisout_TRIVIA_border();
