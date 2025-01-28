#pragma once
#include "Types.h"


void get_info_players_of_family(GameSettings* gameSettings);

void chek_winners_list(PlayersList playersList, int* arr_winners_list);

void show_winners(PlayersList playersList, int* arr_winners_list);
void chek_winner_in_the_end_game_and_get_them(PlayersList* players, int* arr_winners_index);

void show_winners_table(PlayersList* players, int* arr_winners_index);


void draw_ready_screan(PlayersList players);

void show_border_with_current_subject_name_and_level(Question current_question);

void draw_current_player(Player current_player);

void show_table(PlayersList playersList);