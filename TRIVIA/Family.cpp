#include <windows.h>   // WinApi header
#include <thread>
#include "Display.h"
#include "Types.h"
#include "Sound.h"
#include "Data.h"
#include "Strings.h"
#include "UserInput.h"
#include "Global.h"
#include "Hebrew.h"

using namespace std;

void get_info_players_of_family(GameSettings* gameSettings)
{
	PlayersList* players = &gameSettings->players;


	// הכנסת כמות השחקנים
	players->count = 4;

	
	// הכנסת שמותם וסוגם של השחקנים
	players->list[0].name = _strdup(FAMILY_PLAYER_1);
	players->list[0].age = 1;
	players->list[0].type = PLAYER_TYPE_GROUP;
	players->list[0].amount = 0;

	players->list[1].name = _strdup(FAMILY_PLAYER_2);
	players->list[1].age = 1;
	players->list[1].type = PLAYER_TYPE_GROUP;
	players->list[1].amount = 0;

	players->list[2].name = _strdup(FAMILY_PLAYER_3);
	players->list[2].age = 1;
	players->list[2].type = PLAYER_TYPE_GROUP;
	players->list[2].amount = 0;

	players->list[3].name = _strdup(FAMILY_PLAYER_4);
	players->list[3].age = 1;
	players->list[3].type = PLAYER_TYPE_GROUP;
	players->list[3].amount = 0;

}


void _show_light(PlayersList playersList)
{

	int x = 30;
	int y = 10;
	int wide = 21;
	int high = 6;

	char amount[100];

	draw_border(x, y, wide, high, 13);
	int len = strlen(TABLE_WINNERS_MESSAGE);
	show_text(TABLE_WINNERS_MESSAGE, 11, (wide / 2) - (len / 2) + x, y - 1);

	for (int i = 0; i < playersList.count; i++)
	{
		set_cursor_position(x + 2, y + 1 + i);
		show_text(playersList.list[i].name, 14);

		set_cursor_position(x + 2 + 13, y + 1 + i);
		sprintf_s(amount, "%d", playersList.list[i].amount);
		show_text(amount, 12);

		Sleep(700);
	}


	set_hebrew(false);
	for (int i = 0; i < 7; i++)
	{
		// symbole1
		set_color(13);
		_draw_symbole1(8, 5);
		Sleep(300);

		set_color(5);
		_draw_symbole1(8, 5);
		_clear_symbole4(15, 19);

		// symbole2
		set_color(15);
		_draw_symbole2(65, 22);
		Sleep(300);

		set_color(8);
		_draw_symbole2(65, 22);
		_clear_symbole5(72, 8);

		// symbole3
		set_color(12);
		_draw_symbole3(45, 3);
		Sleep(300);

		set_color(4);
		_draw_symbole3(45, 3);
		_clear_symbole1(8, 5);

		// symbole4
		set_color(14);
		_draw_symbole4(15, 19);
		Sleep(300);

		set_color(6);
		_draw_symbole4(15, 19);
		_clear_symbole2(65, 22);

		// symbole5
		set_color(11);
		_draw_symbole5(72, 8);
		Sleep(300);

		set_color(1);
		_draw_symbole5(72, 8);
		_clear_symbole3(45, 3);
	}
	_clear_symbole4(15, 19);
	_clear_symbole5(72, 8);


	set_hebrew(IS_HEB);
}
void show_table(PlayersList playersList)
{
	clear_border_whisout_TRIVIA_border();

	Sleep(500);



	thread t1(play_applause_for_winners);
	thread t2(_show_light, playersList);
	t1.join();
	t2.join();



	Sleep(500);


	char* any_key = _strdup(PRESS_ANY_KEY_TO_EXIT);

	blink_texts(any_key, 28, 26, 12, 250, 1);
	show_text(any_key, 12, 28, 26);
	get_any_key();

}


void chek_winners_list(PlayersList playersList, int* arr_winners_list)
{
	for (int i_1 = 0; i_1 < playersList.count; i_1++)
	{
		for (int i_2 = (i_1 + 1); i_2 < playersList.count; i_2++)
		{
			if (playersList.list[i_1].amount < playersList.list[i_2].amount)
			{
				int tmp = arr_winners_list[i_1];

				arr_winners_list[i_1] = arr_winners_list[i_2];
				arr_winners_list[i_2] = tmp;
			}

		}
	}
}
void show_winners(PlayersList playersList, int* arr_winners_list)
{
	int keep_last_index_winner = 2;
	for (int i = 3; i < playersList.count; i++)
	{
		if (playersList.list[arr_winners_list[i]].amount == playersList.list[arr_winners_list[2]].amount)
		{
			keep_last_index_winner = i;
		}
	}
	clear_border_whisout_TRIVIA_border();

	int x = 30;
	int y = 8;
	int wide = 21;
	int high = (keep_last_index_winner * 2) + 2;

	int len = strlen(WINNER_MESSAGE__MAZAL_TOV_FAMILY);
	set_cursor_position(QUESTION_X, y - 1);
	show_text(WINNER_MESSAGE__MAZAL_TOV_FAMILY, 12);

	if (keep_last_index_winner >= 3)
	{
		draw_border(x, y, wide, high, 13);
		for (int i = 0; i <= keep_last_index_winner; i++)
		{
			set_cursor_position(x + 4, y + 2 + i);
			char message[30];
			show_text(playersList.list[arr_winners_list[i]].name, 11);
			set_cursor_position(x + 14, y + 2 + i);
			char amount[15];
			sprintf_s(amount, "%d", playersList.list[arr_winners_list[i]].amount);
			show_text(amount, 14);

		}

	}
	else
	{
		draw_border(x, y, wide, high, 13);
		for (int i = 0; i <= keep_last_index_winner; i++)
		{
			set_cursor_position(x + 2, y + 2 + i);
			show_text(playersList.list[arr_winners_list[i]].name, 11);

			set_cursor_position(x + 14, y + 2 + i);
			char amount[15];
			sprintf_s(amount, "%d", playersList.list[arr_winners_list[i]].amount);
			show_text(amount, 14);
		}
	}
}
void chek_winner_in_the_end_game_and_get_them(PlayersList* players, int* arr_winners_index)
{
	//char*** winners_index = &arr_winners_index;

	int higher_amount_place1 = 0;
	int higher_amount_place2 = 0;
	int higher_amount_place3 = 0;

	// chek first winner
	for (int i = 0; i < players->count; i++)
	{
		if (players->list[i].amount >= higher_amount_place1)
		{
			higher_amount_place1 = players->list[i].amount;
			//*winners_index[0][0] = '0' + i;
			//strcpy_s(arr_winners_index[0],1, ("0" + i));
			arr_winners_index[0] = i;
		}
	}
	players->list[arr_winners_index[0]].name;

	// chek second winner
	for (int i = 0; i < players->count; i++)
	{
		if (players->list[i].amount >= higher_amount_place2 &&
			i != arr_winners_index[0])
		{
			higher_amount_place2 = players->list[i].amount;
			arr_winners_index[1] = i;
		}
	}
	players->list[arr_winners_index[1]].name;

	// chek third winner
	for (int i = 0; i < players->count; i++)
	{
		if (players->list[i].amount >= higher_amount_place3 &&
			i != arr_winners_index[0] &&
			i != arr_winners_index[1])
		{
			higher_amount_place3 = players->list[i].amount;
			arr_winners_index[2] = i;
		}
	}
	players->list[arr_winners_index[2]].name;



	// chaek same amount of tow winners
	if (higher_amount_place1 == higher_amount_place2)
	{
		Player* first_winner = &players->list[arr_winners_index[0]];
		Player* second_winner = &players->list[arr_winners_index[1]];

		if (first_winner->age <= second_winner->age)
		{
			// stay like define above
			;
		}
		else
		{
			// swap winners position 
			int tmp = arr_winners_index[0];
			arr_winners_index[0] = arr_winners_index[1];
			arr_winners_index[1] = tmp;

		}
	}
	if (higher_amount_place1 == higher_amount_place3)
	{
		Player* first_winner = &players->list[arr_winners_index[0]];
		Player* third_winner = &players->list[arr_winners_index[2]];

		if (first_winner->age <= third_winner->age)
		{
			// stay like define above
			;
		}
		else
		{
			// swap winners position 
			int tmp = arr_winners_index[0];
			arr_winners_index[0] = arr_winners_index[2];
			arr_winners_index[2] = tmp;
		}

	}
	if (higher_amount_place2 == higher_amount_place3)
	{
		Player* second_winner = &players->list[arr_winners_index[1]];
		Player* third_winner = &players->list[arr_winners_index[2]];

		if (second_winner->age <= third_winner->age)
		{
			// stay like define above
			;
		}
		else
		{
			// swap winners position 
			int tmp = arr_winners_index[1];
			arr_winners_index[1] = arr_winners_index[2];
			arr_winners_index[2] = tmp;

		}

	}
}
void show_winners_table(PlayersList* players, int* arr_winners_index)
{

	Player* first_winner = &players->list[arr_winners_index[0]];
	Player* second_winner = &players->list[arr_winners_index[1]];
	Player* third_winner = &players->list[arr_winners_index[2]];
	char winnes[100];

	int x = 30;
	int y = 8;
	int wide = 21;
	int high = 13;

	draw_border(x, y, wide, high, 13);

	for (int i = 0; i < 11; i++)
	{
		set_cursor_position(x + 2, y + 2 + i);

		sprintf_s(winnes, "%s\t\t%d", arr_winners_index[i], players->list[arr_winners_index[0 + i]].name, players->list[arr_winners_index[0 + i]].amount);
		show_text(winnes, 14);
		Sleep(700);
		show_text(winnes, 12);
	}



	// print the first winner
	//draw_border((WIDE_SCREEN / 2) - 15, (HIGH_SCREEN / 2) - 5, 30, 10, 11);
	//sprintf_s(winnes, "%s\t\t%d", first_winner->name, first_winner->amount);
	//show_text(winnes, 14, (WIDE_SCREEN / 2) - 14, (HIGH_SCREEN / 2) - 3);
	//Sleep(700);
	//show_text(winnes, 12, (WIDE_SCREEN / 2) - 14, (HIGH_SCREEN / 2) - 3);


	//// print the second winner
	//sprintf_s(winnes, "%s\t\t%d", second_winner->name, second_winner->amount);
	//show_text(winnes, 14, (WIDE_SCREEN / 2) - 14, (HIGH_SCREEN / 2) - 1);
	//Sleep(700);
	//show_text(winnes, 12, (WIDE_SCREEN / 2) - 14, (HIGH_SCREEN / 2) - 1);


	//// print the third winner
	//sprintf_s(winnes, "%s\t\t%d", third_winner->name, third_winner->amount);
	//show_text(winnes, 14, (WIDE_SCREEN / 2) - 14, (HIGH_SCREEN / 2) + 1);
	//Sleep(700);
	//show_text(winnes, 12, (WIDE_SCREEN / 2) - 14, (HIGH_SCREEN / 2) + 1);
	//Sleep(2000);
}

void draw_ready_screan(PlayersList players)
{
	Player current_player = players.list[players.current_player_index];
	char message[50];
	sprintf_s(message, CONTINUE_MESSAGE, current_player.name); 
	int len = strlen(message);
	show_text(message, 14, QUESTION_X, QUESTION_Y - 2);
	Sleep(1000);
	clear_text(message, QUESTION_X, QUESTION_Y - 2);
}
void show_border_with_current_subject_name_and_level(Question current_question)
{
	int x = 25;
	int y = 10;
	int wide = 31;
	int high = 7;

	draw_border(x, y, wide, high, 9);
	Subject current_subject = get_subject_by_id(current_question.subject_id);
	show_text(THE_SUBJECT, 13, x + 2, y + 2);
	show_text(current_subject.name, 11);


	/*
	show_text(THE_SUBJECT, 13, x + 2, y + 2);
	if (current_question.subject_id == 101) show_text(current_subject.name, 11);
	else if (current_question.subject_id == 102) show_text(SUBJECT_NAME_102, 11);
	else if (current_question.subject_id == 103) show_text(SUBJECT_NAME_103, 11);
	else if (current_question.subject_id == 104) show_text(SUBJECT_NAME_104, 11);
	*/

	show_text(HARD_LEVEL, 13, x + 2, y + 4);
	char tmp[30];
	_itoa_s(current_question.hard_level, tmp, 10);
	show_text(tmp, 11);
	Sleep(1700);

	draw_spaces(x, y, wide, high);
}
void draw_current_player(Player current_player)
{
	draw_spaces(PLAYER_X, 3, 22, 4);
	draw_border(PLAYER_X, 3, 21, 4, FORE_RED);
	//sprintf_s(message, PLAYERS_AREA__COLUMN_HEADER_PLAYER, current_player.name);
	set_cursor_visible(true);
	set_cursor_position(PLAYER_X + 4, 4);
	show_text(PLAYERS_AREA__COLUMN_HEADER_PLAYER, 11);
	set_cursor_position(PLAYER_X + 7, 5);
	show_text(current_player.name, 14);
	set_cursor_visible(false);

}