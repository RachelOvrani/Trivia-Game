#include "Display.h"
#pragma once

// **********  General Functions *********************

int get_number_from_user(const char* message, COLOR color);
int get_number_from_user(const char* message);
int get_number_from_user();

char* get_str_from_user(const char* message, int color, bool required);
char* get_str_from_user(const char* message, int color);
char* get_str_from_user(const char* message, bool required);
char* get_str_from_user(const char* message);
char* get_str_from_user();

char  get_choise_from_user(const char* message, int color, const char* option, bool show_input);
char get_choise_from_user(const char* option, bool show_input);
char get_any_key();




// **********  TRIVIA Functions *********************
void get_player_bet_amount(PlayersList* players);
void get_palyers_info(GameSettings* gameSettings);

int get_time_to_answer();

void show_question_and_get_amswer(PlayersList* players, Question question, int time_to_answer, GAME_TYPE gameType);

void chek_winner(PlayersList* players);

char stop_or_continue_question();
bool change_starting_question();

void nice_questions(Player player);


//char get_choise_1234();
//char get_choise_12();
///char get_yn_question();
