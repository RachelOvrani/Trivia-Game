#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <fstream>

#include "UserInput.h"
#include "Display.h"
#include "Types.h"
#include "Sound.h"
#include "Data.h"
#include "Math.h"
#include "Strings.h"
#include "Hebrew.h"
#include "Family.h"
#include "Encryption.h"
#include <thread>


using namespace std;


GameSettings gameSettings;



void game_processof_regular_game(Question* question, bool contnue_with_change_players, int* counter)
{
	int current_player_index = gameSettings.players.current_player_index;
	Player* player = &gameSettings.players.list[current_player_index];

	// אם אחד מהשחקנים הגיע לסכום 0
	//בתורו עוקפים עליו ואין לו זכות לענות על שאלה
	if (player->amount == 0)
		return;



	// קבלת הימור הכסף של השחקן הנוכחי
	get_player_bet_amount(&gameSettings.players);
	clear_bord_of_question();

	// הצגת הנושאים לבחירת המשתמש
	SubjectSuggestList ssl = get_subject_suggested_list();
	show_subject_suggested(ssl);
	// קבלת בחירת המשתמש ע''פ הנואים שהוצגו
	int len = strlen(SUBJECT_SELECTION__PRESS_YOUR_CHOISE);
	show_text(SUBJECT_SELECTION__PRESS_YOUR_CHOISE, 9, MESSAGE_X_, MESSAGE_Y_);
	set_pos_for_languge(len);

	char ch = get_choise_from_user("1234", false);
	int choise_subject = atoi(&ch);
	clear_bord_of_question();



	// הדפסת השאלה ו- 4 תשובות
	// קבלת תשובה מהשחקן הנוכחי
	*question = GetQuestion(ssl.list[choise_subject - 1].subject_id, ssl.list[choise_subject - 1].level);

	// הודעה על תשובתו וסידור סכומו
	show_question_and_get_amswer(&gameSettings.players, *question, gameSettings.answer_time, gameSettings.game_type);
	clear_bord_of_question();

	if (gameSettings.players.count == 1 && player->amount == 0)
	{
		lose_message(&gameSettings.players);
	}

	// בדיקת שחקן מנצח
	chek_winner(&gameSettings.players);

	// אם יש מנצח - הדפסה חגיגית כבוד המנצח
	if (gameSettings.players.winner_index != -1)
	{
		write_the_victory(&gameSettings.players);
	}

	if ((gameSettings.players.winner_index != (-1)) ||
		(gameSettings.players.count == 1 && player->amount == 0))
	{
		// שאלת השחקן אם רוצה לשחק שוב
		char answer = stop_or_continue_question();
		// בדיקת תשובתו
		if (answer == YES_OPTION)
		{
			// אם רוצה להמשיך לשחק
			// שאלה לשחקן אם רוצה לשמור נתונים קודמים
			contnue_with_change_players = change_starting_question();
		}
		else
		{
			len = strlen(EXIT_MESSAGE);
			clear_border_whisout_TRIVIA_border();
			show_text(EXIT_MESSAGE, 10, QUESTION_X, QUESTION_Y);
			Sleep(1000);
			exit(0);
		}

	}
	else if (*counter == 0)
	{
		nice_questions(*player);
		*counter = random(10, 15);
	}

	*counter--;
}

void game_processof_family_game(Question* question, bool contnue_with_change_players, int queue, int last_level_question)
{
	int current_player = gameSettings.players.current_player_index;
	gameSettings.players.current_player_bet_amount = 100;

	// הדפסת השאלה ו- 4 תשובות
	// קבלת תשובה מהשחקן הנוכחי
	*question = get_next_question(queue);
	
	


	// הדפסת שמו של השחקן הנוכחי על המסך
	if (question->hard_level != last_level_question)
	{
		clear_border_whisout_TRIVIA_border();
		show_border_with_current_subject_name_and_level(*question);
	}
	draw_current_player(gameSettings.players.list[current_player]);
	draw_ready_screan(gameSettings.players);


	// קביעת זמן הטיימר
	int crrent_player_age = gameSettings.players.list[gameSettings.players.current_player_index].age;

	if (crrent_player_age <= 12)
	{
		gameSettings.answer_time = 15;
	}
	else
	{
		gameSettings.answer_time = 10;
	}

	// הודעה על תשובתו וסידור סכומו
	show_question_and_get_amswer(&gameSettings.players, *question, gameSettings.answer_time, gameSettings.game_type);
	clear_bord_of_question();


	if (question->last_question == true)
	{
	
		show_table(gameSettings.players);
		
		int len = strlen(EXIT_MESSAGE);
		clear_border_whisout_TRIVIA_border();
		show_text(EXIT_MESSAGE, 10, QUESTION_X, QUESTION_Y-2);
		Sleep(1000);
		exit(0);
		
		return;
	}

	if ((gameSettings.players.winner_index != (-1)) ||
		(gameSettings.players.count == 1 && gameSettings.players.list[current_player].amount == 0))
	{
		// שאלת השחקן אם רוצה לשחק שוב
		char answer = stop_or_continue_question();
		// בדיקת תשובתו
		if (answer == YES_OPTION)
		{
			// אם רוצה להמשיך לשחק
			// שאלה לשחקן אם רוצה לשמור נתונים קודמים
			contnue_with_change_players = change_starting_question();
		}
		else
		{
			int len = strlen(EXIT_MESSAGE);
			clear_border_whisout_TRIVIA_border();
			show_text(EXIT_MESSAGE, 10, QUESTION_X, QUESTION_Y);
			Sleep(1000);
			exit(0);
		}
	}
}


void main()
{
	//encrypt_file("q_h_for_trivia.txt", "q_h_trivia.txt");

	// טעינת דף קוד לעברית
	init_display();
	
	// טעינת הנתונים (נושאים, שאלות) מהדיסק
	load_data();


	// איתחול למנגנון ליצירת מספרים אקראים
	init_random();
	// טעינת הודעות הצלחה ושגיאה
	load_response_meesage();

	/*
	thread t1 (draw_open_screen);
	thread t2 (app_startup);
	t1.join();
	t2.join();
	*/

	//clear_border_whisout_TRIVIA_border();
	//draw_MAZAL_TOV_screen();
	int queue = 0;
	int counter = 8;
	bool continue_with_change_players = true;
	int last_question_level = 0;


	while (true)
	{

		// הדפסת המיסגרת
		draw_border_of_TRIVIA(FORE_GREEN);

		if (continue_with_change_players)
		{
			reset_game_setings(&gameSettings);

			if (gameSettings.game_type == GAME_TYPE_FAMILY)
			{
				get_info_players_of_family(&gameSettings);
			}
			else if (gameSettings.game_type == GAME_TYPE_REGULAR)
			{
				get_palyers_info(&gameSettings);
				// קבלת השניות מהשחקנים לאורך עניית התשובה
				gameSettings.answer_time = get_time_to_answer();
			}
		}
		else
		{
			for (int i = 0; i < gameSettings.players.count; i++)
			{
				gameSettings.players.list[i].amount = INIT_AMOUNT;
			}
			gameSettings.players.winner_index = -1;
			draw_players_info_section(&gameSettings);
		}
		clear_bord_of_question();




		// תחילת המשחק
		for (int i = 0; i <= gameSettings.players.count; i++)
		{
			//// אם כל השחקנים נשאלו שאלה הסיבוב מתחיל שוב
			if (i == gameSettings.players.count)
			{
				i = 0;
			}

			// קביעת מס' השחקן הנוכחי
			gameSettings.players.current_player_index = i;


			queue++;
			Question question;
			
			if (gameSettings.game_type == GAME_TYPE_REGULAR)
			{
				game_processof_regular_game(&question, continue_with_change_players, &counter);
			}
			else
			{
				game_processof_family_game(&question, continue_with_change_players, queue, last_question_level);
			}

			last_question_level = question.hard_level;
		}


		
	}

	getchar();
	getchar();


}
