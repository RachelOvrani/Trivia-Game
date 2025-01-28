#include<stdio.h>
#include "Display.h"
#include <conio.h>
#include "Sound.h"
#include "Data.h"
#include "Math.h"
#include "Strings.h"
#include "Hebrew.h"


// ****************  Inline Functions ****************

inline bool search_char_within_str(const char* str, char char_to_search)
{
	return std::string(str).find(char_to_search) != std::string::npos;
}

inline bool my_atoi(char const* _String, int* number)
{
	char tmp2[100];

	*number = atoi(_String);

	if (*number == 0 && strcmp(_String, "0") != 0)
	{
		return false;
	}
	else
	{
		_itoa_s(*number, tmp2, 10);
		if (strcmp(_String, tmp2) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

	}

}




// **********  General Functions *********************
int get_number_from_user(const char* message, COLOR color)
{
	if (message != NULL)
	{
		show_text(message, color);
	}



	while (true)
	{
		char tmp1[100];

		// אבג
		set_cursor_visible(true);
		gets_s(tmp1);
		set_cursor_visible(false);

		int number;
		bool success = my_atoi(tmp1, &number);

		if (success)
		{
			return number;
		}
		else
		{
			show_text(ERROR_MESSAGE__INVALID_INPUT_NUMBER);
		}
	}

}
int get_number_from_user(const char* message)
{
	return get_number_from_user(message, DEFAULT);
}
int get_number_from_user()
{
	return get_number_from_user(NULL);
}


char* get_str_from_user(const char* message, int color, bool required)
{
	if (message != NULL)
	{
		show_text(message, color);
	}

	COORD input_pos = get_cursor_position();

	char str[1000];
	//scanf_s("%s", str, 100);
	set_cursor_visible(true);
	gets_s(str, 50);
	set_cursor_visible(false);

	while (required == true && str[0] == '\0')
	{
		set_cursor_position(input_pos.X, input_pos.Y);
		set_cursor_visible(true);
		gets_s(str, 50);
		set_cursor_visible(false);
	}

	int len = strlen(str);
	int max_len = 12;
	int destination_len = (len < max_len) ? len : max_len;

	char* str_p;
	str_p = (char*)malloc(sizeof(char) * (destination_len + 1));

	// copy part of string
	for (int i = 0; i < destination_len; i++)
	{
		str_p[i] = str[i];
	}
	// add null terminator
	str_p[destination_len] = '\0';


	return str_p;
}
char* get_str_from_user(const char* message, int color)
{
	return get_str_from_user(message, color, true);
}
char* get_str_from_user(const char* message, bool required)
{
	return get_str_from_user(message, DEFAULT, required);
}
char* get_str_from_user(const char* message)
{
	return get_str_from_user(message, true);
}
char* get_str_from_user()
{
	return get_str_from_user(NULL);
}


char get_choise_from_user(const char* message, int color, const char* option, bool show_input)
{
	// בדיקה האם הוא היכניס הודעה וצבע 
	// אם כן מציג את ההודעה בצבע המתאים
	if (message != NULL)
	{
		show_text(message, color);
	}

	// בודק האם הוא רוצה שיראו מה שהמשתמש מכניס או לא
	char c;

	set_cursor_visible(true);
	c = _getch();
	set_cursor_visible(false);

	// אם הוא נתן אפשרויות
	if (option == NULL)
	{
		putchar_for_language(c);
		return c;
	}

	while (search_char_within_str(option, c) == false)
	{
		set_cursor_visible(true);
		c = _getch();
		set_cursor_visible(false);
	}
	if (show_input == true)
	{
		putchar_for_language(c);
	}
	return c;
}
char get_choise_from_user(const char* option, bool show_input)
{
	return get_choise_from_user(NULL, DEFAULT, option, show_input);
}
char get_any_key()
{
	// הסיבה שלא השתמשנו בפונקציה שממתינה לקלט
	// מכיון שפונקציה זו מראה את הסמן בעת ההמתנה לקט
	// ובמקרה זה, זה פחות מתאים
	//return get_choise_from_user(NULL, DEFAULT, NULL, false);
	return _getch();
}






// **********  TRIVIA Functions *********************
void _get_player_bet_amount_show_error_message(const char* message)
{
	TextLineInfo error_message[2];
	error_message[0].color = FORE_RED;

	int len = strlen(message);
	error_message[0].pos_x = MESSAGE_X_;
	error_message[0].pos_y = MESSAGE_Y_;
	set_cursor_position(error_message[0].pos_x, error_message[0].pos_y);
	error_message[0].text = _strdup(message);

	error_message[1].color = FORE_RED;
	error_message[1].text = _strdup(BET_AMOUNT_QUESTION__INVALID_INPUT_TRY_AGAIN_MESSAGE);
	len = strlen(BET_AMOUNT_QUESTION__INVALID_INPUT_TRY_AGAIN_MESSAGE);
	error_message[1].pos_x = MESSAGE_X_;
	error_message[1].pos_y = MESSAGE_Y_ + 1;
	set_cursor_position(error_message[1].pos_x, error_message[1].pos_y);

	// blink messages
	blink_texts(error_message, 2, 1000);

}
void get_player_bet_amount(PlayersList* players)
{
	char message[100];

	int number = players->current_player_index;
	int inex_current_player = number;

	sprintf_s(message, BET_AMOUNT_QUESTION__INPUT_LABAL, players->list[number].name);
	int len = strlen(message);

	int bet_amount;
	bool is_number;
	do
	{
		char* choise;
		int* number;

		show_text(message, 10, QUESTION_X, QUESTION_Y);

		set_pos_for_languge(len);
		set_cursor_position((WIDE_SCREEN / 2) - 2, QUESTION_Y + 2);
		choise = get_str_from_user();


		is_number = my_atoi(choise, &bet_amount);
		free(choise);

		if (!is_number || (bet_amount > (players->list[inex_current_player].amount)) || (bet_amount < 1))
		{
			set_hebrew(false);
			set_cursor_position((WIDE_SCREEN / 2) - 2, QUESTION_Y + 2);
			draw_spaces(30);
			set_hebrew(IS_HEB);



			// if he entered signs exept of numbers
			if (!is_number)
			{
				_get_player_bet_amount_show_error_message(BET_AMOUNT_QUESTION__INVALID_INPUT_MESSAGE1);
			}
			// if the bet amount match large
			else if (is_number && bet_amount > players->list[inex_current_player].amount)
			{
				_get_player_bet_amount_show_error_message(BET_AMOUNT_QUESTION__INVALID_INPUT_MESSAGE2);
			}
			// if the bet amount match small
			else if (is_number && bet_amount < 1)
			{
				_get_player_bet_amount_show_error_message(BET_AMOUNT_QUESTION__INVALID_INPUT_MESSAGE3);
			}
		}

	} while (!is_number || bet_amount > players->list[inex_current_player].amount || bet_amount < 1);

	//players->list[inex_current_player].amount = bet_amount;
	players->current_player_bet_amount = bet_amount;
}


void get_palyers_info(GameSettings* gameSettings)
{
	PlayersList* players = &gameSettings->players;

	// הכנסת כמות השחקנים 

	int len = strlen(GAME_SETTING__PLAYERS_COUNT_QUESTION);
	show_text(GAME_SETTING__PLAYERS_COUNT_QUESTION, FORE_BLUE, QUESTION_X, QUESTION_Y);

	set_pos_for_languge(len);
	char options[MAX_OF_PLAYERS];
	for (int i = 0; i < MAX_OF_PLAYERS; i++)
	{
		options[i] = '0' + (i + 1); // מכניס את קוד האסקי של המספר
	}
	char count = get_choise_from_user(options, true);
	players->count = atoi(&count);


	//***********************************************//

	// הכנסת שמותם וסוגם של השחקנים
	char message[100];
	draw_border_of_TRIVIA(FORE_GREEN);
	for (int i = 0; i < players->count; i++)
	{
		// קבלת שמו של השחקן 
		if (players->count > 1)
		{
			// אם ישנם כמה שחקנים...
			sprintf_s(message, GAME_SETTING__PLAYER_NAME_QUESTION, i + 1);
			len = strlen(message);
			show_text(message, 5, QUESTION_X, QUESTION_Y + i);
			players->list[i].name = get_player_name_for_languae(len);

		}
		else
		{
			// אם יש שחקן יחיד...
			len = strlen(GAME_SETTING__PLAYER_NAME_QUESTION_SINGEL_PLAYER);
			show_text(GAME_SETTING__PLAYER_NAME_QUESTION_SINGEL_PLAYER, 5, QUESTION_X, QUESTION_Y);
			players->list[i].name = get_player_name_for_languae(len);
		}

	}

	//***********************************************//

	for (int i = 0; i < players->count; i++)
	{
		// קבלת סוג אדם - של השחקן 
		draw_border_of_TRIVIA(FORE_GREEN);
		sprintf_s(message, GAME_SETTING__PLAYER_KIND_QUESTION, players->list[i].name);
		len = strlen(message);
		show_text(message, 6, QUESTION_X, QUESTION_Y);

		set_pos_for_languge(len);
		char type = get_choise_from_user("12", false);
		players->list[i].type = (PLAYER_TYPE)atoi(&type);;
			

	}

	//***********************************************//

	// הדפסת המסגרת של הכסף של השחקנים
	draw_border_of_TRIVIA(FORE_GREEN);
	set_color(FORE_RED);
	draw_players_info_section(gameSettings);


}

int get_time_to_answer()
{
	// קבלת מספר שניות כדי לענות על השאלה
	int len = strlen(GAME_SETTING__TIME_TO_ANSWER_QUESTION_TITLE);
	show_text(GAME_SETTING__TIME_TO_ANSWER_QUESTION_TITLE, QUESTION_X, QUESTION_Y);


	len = strlen(GAME_SETTING__TIME_TO_ANSWER_QUESTION_OPTIONS);
	show_text(GAME_SETTING__TIME_TO_ANSWER_QUESTION_OPTIONS, QUESTION_X, QUESTION_Y + 1);

	set_pos_for_languge(len);
	char choise = get_choise_from_user("123", false);

	switch (choise)
	{
	case '1': return 5;

	case '2': return 10;

	case '3': return 15;
	}
	draw_spaces(QUESTION_X - 4, QUESTION_Y, QUESTION_WIDE, QUESTION_HIGH);
}

int _get_question_answer_with_timer(PlayersList players, int time_to_answer, int max_option)
{
	if (max_option == 4)
	{
		int len = strlen(QUESTION__ENTER_YOUR_ANSWER_MESSAGE_4);
		show_text(QUESTION__ENTER_YOUR_ANSWER_MESSAGE_4, 7, MESSAGE_X_, MESSAGE_Y_);
		set_pos_for_languge(len);
	}
	else 
	{
		int len = strlen(QUESTION__ENTER_YOUR_ANSWER_MESSAGE_3);
		show_text(QUESTION__ENTER_YOUR_ANSWER_MESSAGE_3, 7, MESSAGE_X_, MESSAGE_Y_);
		set_pos_for_languge(len);
	}

	while(_kbhit() > 0)
	{
		_getch();
	}

	int time_to_sleep = 100;
	int loop_times = (time_to_answer * 10)+1;
	char c = NULL;
	int time = time_to_answer;

	_draw_timer(time);
	for (int i = 1; i < loop_times; i++)
	{

		// עידכון את הטיימר
		if ((i % 10) == 0)
		{
			_update_timer(players, time);
			time--;
		}

		// קלט תשובה
		if (_kbhit() > 0)
		{
			c = _getch();

			switch (c)
			{
			case '1': return 1;
			case '2': return 2;
			case '3': return 3;
			
			default:
				break;
			}
			if (max_option == 4 && c == '4')
			{
				return 4;
			}
		}


		Sleep(time_to_sleep);
	}

	return -1;
}
void _chek_the_answer(PlayersList* players, Question question, GAME_TYPE gameType)
{
	int header_y = PLAYER_Y + 1;
	int header_x1 = PLAYER_X + 2;				// x1 = player name
	int header_x2 = header_x1 + 10 + 4;		// x2 = player amount

	int current_player_index = players->current_player_index;
	Player* current_player = &players->list[current_player_index];

	int current_bet_amount = players->current_player_bet_amount;

	int new_amount_of_right = (current_player->amount) + (question.hard_level * (current_bet_amount));
	int new_amount_of_wrong = (current_player->amount) - (current_bet_amount);

	int player_amswer = players->current_player_answer;
	int corrent_amswer = question.right_answer;


	if (player_amswer == corrent_amswer)
	{
		ResponseMessage* success_messege = get_response_success_message();

		char* success_message_text = NULL;
		switch (current_player->type)
		{
			case PLAYER_TYPE_SINGLE_MALE:
				success_message_text = success_messege->message_for_male;
				break;
			case PLAYER_TYPE_SINGLE_FEMALE:
				success_message_text = success_messege->message_for_female;
				break;
			case PLAYER_TYPE_GROUP:
				success_message_text = success_messege->message_for_group;
				break;
		}
		

		
		int len = strlen(success_message_text);

		show_text(success_message_text, 6, MESSAGE_X_, MESSAGE_Y_ + 2);
		play_correct_answer(random(1, 17));

		Sleep(500);


		if (gameType == GAME_TYPE_REGULAR)
		{
			show_text("        ", header_x2 - 1, header_y + current_player_index + 1);

			char tmp[50];
			sprintf_s(tmp, "%d", new_amount_of_right);
			show_text(tmp, 14, header_x2, header_y + current_player_index + 1);
			
			current_player->amount = new_amount_of_right;
		}
		else
		{
			show_text("        ", header_x2 - 1, header_y + current_player_index + 1);

			new_amount_of_right = current_player->amount + 300;
			char tmp[50];
			sprintf_s(tmp, "%d", new_amount_of_right);
			show_text(tmp, 14, header_x2, header_y + current_player_index + 1);
			
			current_player->amount = new_amount_of_right;
		}

	}
	else if (player_amswer != question.right_answer && player_amswer != -1)
	{
		ResponseMessage* fail_messege = get_response_fail_message();


		char* fail_message_text = NULL;
		switch (current_player->type)
		{
		case 1:
			fail_message_text = fail_messege->message_for_male;
			break;
		case 2:
			fail_message_text = fail_messege->message_for_female;
			break;
		case 3:
			fail_message_text = fail_messege->message_for_group;
			break;
		}


		

		int len = strlen(fail_message_text);
		show_text(fail_message_text, 4, MESSAGE_X_, MESSAGE_Y_ + 2);
		play_wrong_answer(random(1, 16));

		Sleep(500);

		if (gameType == GAME_TYPE_REGULAR)
		{
			show_text("        ", header_x2 - 1, header_y + current_player_index + 1);
			char tmp[50];
			sprintf_s(tmp, "%d", new_amount_of_wrong);
			show_text(tmp, 14, header_x2, header_y + current_player_index + 1);
				
			current_player->amount = new_amount_of_wrong;
		}
		else
		{
			// don't dicreas from the amount
		}


	}
	else if (player_amswer == -1)
	{
		
		if (current_player->type == PLAYER_TYPE_GROUP)
		{
			int len = strlen(TIMER_MESSAGE__TIME_OVER_2);
			show_text(TIMER_MESSAGE__TIME_OVER_2, 4, MESSAGE_X_, MESSAGE_Y_ + 2);
		}
		else
		{
			int len = strlen(TIMER_MESSAGE__TIME_OVER_1);
			show_text(TIMER_MESSAGE__TIME_OVER_1, 4, MESSAGE_X_, MESSAGE_Y_ + 2);
		}
		play_time_over();
		Sleep(500);

		if (gameType == GAME_TYPE_REGULAR)
		{
			show_text("        ", header_x2 - 1, header_y + current_player_index + 1);
			char tmp[50];
			sprintf_s(tmp, "%d", new_amount_of_wrong);
			show_text(tmp, 14, header_x2, header_y + current_player_index + 1);
			
			current_player->amount = new_amount_of_wrong;
		}
		else
		{
			// don't dicreas from amount
		}
	}

	clear_timer_area();
}
void show_question_and_get_amswer(PlayersList* players, Question question, int time_to_answer, GAME_TYPE gameType)
{
	
	int len = strlen(question.question);
	show_text(question.question, 6, QUESTION_X, QUESTION_Y);

	// תשובה ראשונה
	show_text("1.", 5, POS_ANSWER_X - 4, POS_ANSWER_Y);
	show_text(question.answer1, 3, POS_ANSWER_X, POS_ANSWER_Y);
	// תשובה שניה
	show_text("2.", 5, POS_ANSWER_X - 4, POS_ANSWER_Y + 1);
	show_text(question.answer2, 3, POS_ANSWER_X, POS_ANSWER_Y + 1);
	// תשובה שלישית
	show_text("3.", 5, POS_ANSWER_X - 4, POS_ANSWER_Y + 2);
	show_text(question.answer3, 3, POS_ANSWER_X, POS_ANSWER_Y + 2);

	int max_option;
	if (question.answer4 != NULL)
	{
		// תשובה רביעית
		show_text("4.", 5, POS_ANSWER_X - 4, POS_ANSWER_Y + 3);
		show_text(question.answer4, 3, POS_ANSWER_X, POS_ANSWER_Y + 3);
		max_option = 4;

		len = strlen(QUESTION__ENTER_YOUR_ANSWER_MESSAGE_4);
		set_cursor_position(MESSAGE_X_, MESSAGE_Y_);
	}
	else
	{
		max_option = 3;

		len = strlen(QUESTION__ENTER_YOUR_ANSWER_MESSAGE_3);
		set_cursor_position(MESSAGE_X_, MESSAGE_Y_);
	}

	players->current_player_answer = _get_question_answer_with_timer(*players, time_to_answer, max_option);

	_chek_the_answer(players, question, gameType);

	players->current_player_answer = -1;
	clear_bord_of_question();
}

void _chek_if_player_is_alone(PlayersList* players)
{
	int winner_player_index = -1;

	for (int i = 0; i < players->count; i++)
	{
		if (players->list[i].amount != 0)
		{
			if (winner_player_index == -1)
			{
				winner_player_index = i;
			}
			else
			{
				return;
			}

		}

	}

	players->winner_index = winner_player_index;
}
void _chek_if_player_is_max_amount(PlayersList* players)
{
	for (int i = 0; i < players->count; i++)
	{
		if (players->list[i].amount >= MAX_AMOUNT)
		{
			players->winner_index = i;
			Sleep(500);
		}
	}

}
void chek_winner(PlayersList* players)
{
	// בדיקה 1 - מנצח
	// רק אם יש יותר משחקן אחד
	if (players->count > 1)
	{
		_chek_if_player_is_alone(players);
	}

	// אם יש מנצח אפשר כבר לצאת 
	if (players->winner_index != -1)
		return;

	// בדיקה 2 - מנצח
	_chek_if_player_is_max_amount(players);

}


void nice_questions(Player player)
{
	clear_bord_of_question();
	char nice_question[10][50];
	char answers[5][20];

	if (player.type = (PLAYER_TYPE)1)//male
	{
		sprintf_s(nice_question[0], NICE_QUESTION1_1, player.name);
		sprintf_s(nice_question[4], NICE_QUESTION5_1, player.name);
	}
	else if (player.type = (PLAYER_TYPE)2)//female
	{
		sprintf_s(nice_question[0], NICE_QUESTION1_2, player.name);
		sprintf_s(nice_question[4], NICE_QUESTION5_2, player.name);
	}
	//general
	sprintf_s(nice_question[1], NICE_QUESTION2, player.name);
	sprintf_s(nice_question[2], NICE_QUESTION3, player.name);
	sprintf_s(nice_question[3], NICE_QUESTION4, player.name);

	//answers
	strcpy_s(answers[0], NICE_QUESTION1_ANS);
	strcpy_s(answers[1], NICE_QUESTION2_ANS);
	strcpy_s(answers[2], NICE_QUESTION3_ANS);
	strcpy_s(answers[3], NICE_QUESTION4_ANS);
	strcpy_s(answers[4], NICE_QUESTION5_ANS);


	int index = random(0, 4);

	int len = strlen(nice_question[index]);
	show_text(nice_question[index], 14, QUESTION_X, QUESTION_Y);

	set_cursor_visible(true);
	len = 5; // five spaces
	set_cursor_position(((WIDE_SCREEN / 2) - len), (QUESTION_Y + 2));
	get_nice_answre_for_languae(len);
	set_cursor_visible(false);

	len = strlen(answers[index]);
	show_text(answers[index], 14, QUESTION_X, QUESTION_Y + 5);
	Sleep(1500);
}

char stop_or_continue_question()
{
	clear_border_whisout_TRIVIA_border();

	int len = strlen(GAME_SETTING__PLAY_AGAIN_QUESTION);
	show_text(GAME_SETTING__PLAY_AGAIN_QUESTION, 11, QUESTION_X, QUESTION_Y);

	set_pos_for_languge(len);
	char choise = get_choise_from_user(YES_NO_OPTION, false);

	return choise;
}
bool change_starting_question()
{
	clear_border_whisout_TRIVIA_border();
	int len = strlen(GAME_SETTING__PLAY_AGAIN_WITH_NEW_PLAYERS_QUESTION);
	show_text(GAME_SETTING__PLAY_AGAIN_WITH_NEW_PLAYERS_QUESTION, 6, QUESTION_X, QUESTION_Y);

	set_pos_for_languge(len);
	char choise = get_choise_from_user(YES_NO_OPTION, false);

	return (choise == YES_OPTION);

}