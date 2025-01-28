#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <windows.h>   // WinApi header
#include "Display.h"
#include "Types.h"
#include "Sound.h"
#include "Data.h"
#include "Strings.h"
#include "UserInput.h"
#include "Global.h"
#include "Hebrew.h"
using namespace std;    // std::cout, std::cin

// ****************  Inline Functions ****************

void putcharAt(char c, int x, int y)
{
	set_cursor_position(x, y);
	putchar_for_language(c);
}


// **********  General Functions *********************

void init_display()
{

	system("mode con:cols=80 lines=30");
	system("title Trivia 1.0.0.0");


	SetConsoleOutputCP(862); // hebrew
	SetConsoleCP(862); // hebrew

	// font hebrew
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 24;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;


	//wcscpy_s(cfi.FaceName, L"Lucida Console");
	//wcscpy_s(cfi.FaceName, L"Consolas");
	//wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE),FALSE, &cfi);

	set_hebrew(IS_HEB);

	set_cursor_visible(false);

	ClearScreen();
}


void _set_heb__swap(char* c1, char* c2)
{
	char tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

void reverseStr(char* str)
{
	int len = strlen(str);
	// Swap character starting from two 
	// corners 
	for (int i = 0; i < len / 2; i++)
		_set_heb__swap(&str[i], &str[len - i - 1]);
}



void _set_heb__set_heb_char(char* c)
{
	if (*c >= -32 && *c <= -6)
	{
		*c -= 96;
	}
}
void set_heb(char* str)
{
	reverseStr(str);

	while (*str)
	{
		_set_heb__set_heb_char(str);
		str++;
	}
}


void set_cursor_position(int x, int y)
{
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

	// FOR DEBUG
	//COORD pos1 = { 1, 1 };
	//SetConsoleCursorPosition(output, pos1);
	//printf("y=%d, x=%d", pos.Y, pos.X);

	SetConsoleCursorPosition(output, pos);
}

int wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
	))
		return -1;
	return csbi.dwCursorPosition.X;
}
int wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(
		GetStdHandle(STD_OUTPUT_HANDLE),
		&csbi
	))
		return -1;
	return csbi.dwCursorPosition.Y;
}
COORD get_cursor_position()
{
	//HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

	//CONSOLE_SCREEN_BUFFER_INFO cbsi;
	//if (GetConsoleScreenBufferInfo(output, &cbsi))
	//{
	//	return cbsi.dwCursorPosition;
	//}
	//else
	//{
	//	// The function failed. Call GetLastError() for details.
	//	COORD invalid = { 0, 0 };
	//	return invalid;
	//}

	COORD pos;
	pos.X = wherex();
	pos.Y = wherey();
	return pos;
}




void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

void set_cursor_visible(bool visible)
{
	//// FOR DEBUG
	//char str[100];
	//sprintf_s(str, "\ncursor visible = %d\n", visible);
	//OutputDebugStringA(str);


	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}


void show_all_colors()
{
	for (size_t i = 1; i <= 15; i++)
	{
		for (size_t serias = 0; serias < 16; serias++)
		{
			int value = i + (serias * 16);
			set_color(value);
			char tmp[50];
			sprintf_s(tmp, "%5d", value);
			show_text(tmp);


			set_color(DEFAULT);
			show_text(" ");
		}
		show_text("\n\n");
	}
}
void set_color(int color)
{
	// set color
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}
void set_color(COLOR color)
{
	set_color((int)color);
}


void draw_border_corners(int x_start, int y_start, int x_end, int y_end)
{
	//left top
	putcharAt(CORNER_LEFT_TOP, x_start, y_start);

	//right top
	putcharAt(CORNER_RIGHT_TOP, x_end, y_start);

	//right botton
	putcharAt(CORNER_RIGHT_BOTTON, x_end, y_end);

	//left botton
	putcharAt(CORNER_LEFT_BOTTON, x_start, y_end);
}
void draw_border(int x, int y, int wide, int high, int color)
{
	int x_start = x;
	int y_start = y;
	int x_end = x + wide - 1;
	int y_end = y + high - 1;


	set_color(color);

	// draw top line
	for (int x = x_start; x <= x_end; x++)
	{
		putcharAt(205, x, y_start);
	}

	// draw botton line
	for (int x = x_start; x <= x_end; x++)
	{
		putcharAt(205, x, y_end);
	}


	// draw left line 
	for (int y = y_start; y <= y_end; y++)
	{
		putcharAt(186, x_start, y);
	}

	// draw right line
	for (int y = y_start; y <= y_end; y++)
	{
		putcharAt(186, x_end, y);
	}


	draw_border_corners(x_start, y_start, x_end, y_end);
}
void draw_borders(Border* borders, int size)
{
	for (int i = 0; i < size; i++)
	{
		Border current_border = borders[i];

		draw_border(current_border.x,
			current_border.y,
			current_border.wide,
			current_border.high,
			current_border.color);
	}
}

void clear_border_corners(int x_start, int y_start, int x_end, int y_end)
{
	//left top
	putcharAt(' ', x_start, y_start);
	//right top
	putcharAt(' ', x_end, y_start);

	//right botton
	putcharAt(' ', x_end, y_end);
	//left botton
	putcharAt(' ', x_start, y_end);
}
void clear_border(int x, int y, int wide, int high)
{
	int x_start = x;
	int y_start = y;
	int x_end = x + wide - 1;
	int y_end = y + high - 1;


	// draw top line
	for (int x = x_start; x <= x_end; x++)
	{
		putcharAt(' ', x, y_start);
	}

	// draw botton line
	for (int x = x_start; x <= x_end; x++)
	{
		putcharAt(' ', x, y_end);
	}


	// draw left line 
	for (int y = y_start; y <= y_end; y++)
	{
		putcharAt(' ', x_start, y);
	}

	// draw right line
	for (int y = y_start; y <= y_end; y++)
	{
		putcharAt(' ', x_end, y);
	}


	clear_border_corners(x_start, y_start, x_end, y_end);
}
void clear_borders(Border* borders, int size)
{
	for (int i = 0; i < size; i++)
	{
		Border current_border = borders[i];

		draw_border(current_border.x,
			current_border.y,
			current_border.wide,
			current_border.high,
			current_border.color);
	}
}

void blink_borders(Border* borders, int size, int duration)
{
	for (int i = 0; i < size; i++)
	{
		draw_border(
			borders[i].x,
			borders[i].y,
			borders[i].wide,
			borders[i].high,
			borders[i].color);
	}

	Sleep(duration);

	for (int i = 0; i < size; i++)
	{
		clear_border(
			borders[i].x,
			borders[i].y,
			borders[i].wide,
			borders[i].high);
	}
}

void show_text(TextLineInfo tli)
{
	// position
	if (tli.pos_x != -1 && tli.pos_y != -1)
	{
		set_cursor_position(tli.pos_x, tli.pos_y);
	}

	// color
	if (tli.color != -1)
	{
		set_color(tli.color);
	}

	show_text(tli.text);
}
void show_text(const char* message)
{

	printf_for_language(message);
	//printf("%s", message);
}
void show_text(const char* message, int color)
{
	set_color(color);
	show_text(message);
}
void show_text(const char* message, int x, int y)
{
	set_cursor_position(x, y);
	show_text(message);
}
void show_text(const char* message, int color, int x, int y)
{
	set_color(color);
	set_cursor_position(x, y);
	show_text(message);
}


void _show_text_with_beep__task_text(const char* msg, int delay)
{
	for (size_t i = 0; i < strlen(msg); i++)
	{
		putchar_for_language(msg[i]);
		Sleep(delay);
	}
}
void _show_text_with_beep__task_beep(int beeps, int delay, int sound)
{
	for (size_t i = 0; i < beeps; i++)
	{
		Beep(sound, delay);
	}
}
void show_text_with_beep(const char* msg, int speed, int sound_req)
{
	thread t1(_show_text_with_beep__task_text, msg, speed);
	thread t2(_show_text_with_beep__task_beep, strlen(msg), speed * 0.88, sound_req);
	t1.join();
	t2.join();

}



void clear_text(const char* message, int x, int y)
{
	int len = strlen(message);
	char* blank_text = _strdup(message);

	for (size_t i = 0; i < len; i++)
	{
		blank_text[i] = ' ';
	}

	show_text(blank_text, x, y);

	free(blank_text);
}
void clear_text(TextLineInfo tli)
{
	clear_text(tli.text, tli.pos_x, tli.pos_y);
}

void blink_texts(TextLineInfo* text, int size, int duration, int times)
{
	for (int i_time = 0; i_time < times; i_time++)
	{
		for (int i = 0; i < size; i++)
		{
			show_text(text[i].text, text->color, text[i].pos_x, text[i].pos_y);
		}

		Sleep(duration);

		for (int i = 0; i < size; i++)
		{
			clear_text(text[i]);
		}

		Sleep(500);
	}
}
void blink_texts(TextLineInfo* text, int size, int duration)
{
	blink_texts(text, size, duration, 1);
}
void blink_texts(const char* text, int pos_x, int pos_y, int color, int duration, int times)
{
	TextLineInfo tli;
	tli.text = text;
	tli.pos_x = pos_x;
	tli.pos_y = pos_y;
	tli.color = color;

	blink_texts(&tli, 1, duration, times);
}

void draw_spaces(int times)
{
	char* spaces = (char*)malloc(times + 1);

	for (size_t i = 0; i < times; i++)
	{
		spaces[i] = ' ';
	}
	spaces[times] = '\0';
	show_text(spaces);

	free(spaces);
}
void draw_spaces(int times, int x, int y)
{
	set_cursor_position(x, y);
	draw_spaces(times);
}
void draw_spaces(int x, int y, int wide, int high)
{
	for (int i = 0; i < high; i++)
	{
		draw_spaces(wide, x, y + i);
	}
}


// **********  TRIVIA Functions *********************
void draw_open_screen()
{
	set_hebrew(false);

	int len = strlen(TRIVIA_OPEN_SCREEN_GAME_NAME_01);
	int x = QUESTION_X;
	int y = OPEN_SCREEN__LOGO_ANIMATION_ROW_END;
	int color = OPEN_SCREEN__LOGO_COLOR_ANIMATION_START;

	for (int i = 0; i < 7; i++)
	{
		//y = i;
		color++;

		Sleep(200);
		clear_text(TRIVIA_OPEN_SCREEN_GAME_NAME_01, x, y);

		show_text(TRIVIA_OPEN_SCREEN_GAME_NAME_01, color, x, y + 1);
		show_text(TRIVIA_OPEN_SCREEN_GAME_NAME_02, color, x, y + 2);
		show_text(TRIVIA_OPEN_SCREEN_GAME_NAME_03, color, x, y + 3);
		show_text(TRIVIA_OPEN_SCREEN_GAME_NAME_04, color, x, y + 4);
		show_text(TRIVIA_OPEN_SCREEN_GAME_NAME_05, color, x, y + 5);
		show_text(TRIVIA_OPEN_SCREEN_GAME_NAME_06, color, x, y + 6);
		show_text(TRIVIA_OPEN_SCREEN_GAME_NAME_07, color, x, y + 7);
		show_text(TRIVIA_OPEN_SCREEN_GAME_NAME_08, color, x, y + 8);
		show_text(TRIVIA_OPEN_SCREEN_GAME_NAME_09, color, x, y + 9);
		show_text(TRIVIA_OPEN_SCREEN_GAME_NAME_10, color, x, y + 10);
	}
	Sleep(2000);

	set_hebrew(IS_HEB);


	char* lines[5];
	lines[0] = _strdup(TRIVIA_OPEN_SCREEN_ALL_RHIGT_RESERVER_01);
	lines[1] = _strdup(TRIVIA_OPEN_SCREEN_ALL_RHIGT_RESERVER_02);
	lines[2] = _strdup(TRIVIA_OPEN_SCREEN_ALL_RHIGT_RESERVER_03);
	lines[3] = _strdup(TRIVIA_OPEN_SCREEN_ALL_RHIGT_RESERVER_04);
	lines[4] = _strdup(TRIVIA_OPEN_SCREEN_ALL_RHIGT_RESERVER_05);




	y = 20;
	len = strlen(lines[0]);
	for (size_t i = 0; i < 5; i++)
	{
		show_text(lines[i], color, QUESTION_X, OPEN_SCREEN__TEXT_DEVELOPER_NAME_ROW_START + i);
		Sleep(250);
	}

	Sleep(2000);
}
void draw_MAZAL_TOV_screen()
{
	set_hebrew(false);

	int len = strlen(MAZAL_TOV_SCREEN_GAME_NAME_01);
	int x = QUESTION_X;
	int y = OPEN_SCREEN__LOGO_ANIMATION_ROW_END-1;
	int color = OPEN_SCREEN__LOGO_COLOR_ANIMATION_START-6;

	for (int i = 0; i < 10; i++)
	{
		//y = i;
		color++;

		Sleep(300);
		clear_text(MAZAL_TOV_SCREEN_GAME_NAME_01, x, y);

		show_text(MAZAL_TOV_SCREEN_GAME_NAME_01, color, x, y + 1);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_02, color, x, y + 2);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_03, color, x, y + 3);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_04, color, x, y + 4);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_05, color, x, y + 5);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_06, color, x, y + 6);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_07, color, x, y + 7);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_08, color, x, y + 8);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_09, color, x, y + 9);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_10, color, x, y + 10);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_11, color, x, y + 11);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_12, color, x, y + 12);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_13, color, x, y + 13);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_14, color, x, y + 14);
		show_text(MAZAL_TOV_SCREEN_GAME_NAME_15, color, x, y + 15);
	}
	Sleep(1000);

	set_hebrew(IS_HEB);


	//char any_key[] = "הקש מקש כלשהוא להמשך...";
	char* any_key = _strdup(PRESS_ANY_KEY_TO_CONTINUE);
	//show_text(any_key);

	blink_texts(any_key, 25, 27, 12, 250, 1);
	show_text(any_key, 12, 25, 27);
	get_any_key();


}

void show_subject_suggested(SubjectSuggestList subject_suggest)
{
	int len = strlen(SUBJECT_SELECTION__TITLE);
	show_text(SUBJECT_SELECTION__TITLE, 2, QUESTION_X, QUESTION_Y - 1);

	for (int i = 0; i < 4; i++)
	{
		Subject subject = get_subject_by_id(subject_suggest.list[i].subject_id);

		char subject_message[100];
		char level[30];

		sprintf_s(subject_message, "%d.  ", i + 1);
		len = strlen(subject_message);
		show_text(subject_message, 5, SUBJECT_SUGGEST_POS_X, SUBJECT_SUGGEST_POS_Y + i);

		//sprintf_s(subject_message, "%-20s 1:%d", subject.name, subject_suggest.list[i].level);
		set_cursor_visible(true);
		sprintf_s(subject_message, "%-20s ", subject.name);
		show_text(subject_message, 11, SUBJECT_SUGGEST_POS_X + len, SUBJECT_SUGGEST_POS_Y + i);

		sprintf_s(level, "1:%d", subject_suggest.list[i].level);
		show_text(level, 6);

		set_cursor_visible(false);
	}
}

void clear_border_of_players_amount(int x, int y, int wide, int high)
{
	draw_spaces(x, y, wide, high);
}
void clear_bord_of_question()
{
	for (int i = 0; i < QUESTION_HIGH + 1; i++)
	{
		set_cursor_position(3, (QUESTION_Y - 2) + i);
		draw_spaces(75);
	}
}
void clear_timer_area()
{
	set_cursor_position(2, TIMER_Y);
	for (int i = 0; i <= 26; i++)
	{
		putcharAt(' ', 2 + i, TIMER_Y);
	}
}
void clear_timer_second()
{
	set_cursor_position(TIMER_X, TIMER_Y);
	for (int i = 0; i <= 15; i++)
	{
		putcharAt(' ', TIMER_X + i, TIMER_Y);
	}
}
void clear_border_whisout_TRIVIA_border()
{
	draw_spaces(1, 1, 77, 26);
	//clear_bord_of_question();
	//clear_border_of_players_amount(PLAYER_X, PLAYER_Y, PLAYER_WIDE, MAX_PLAYER_HIGH);
	//clear_timer_area();
}

void _draw_border_write_text(const char* str_header, const char* str_footer)
{
	int len_header = strlen(str_header);
	int len_footer = strlen(str_footer);

	int x_position_header = (WIDE_SCREEN / 2) - (len_header / 2);
	int x_position_footer = (WIDE_SCREEN / 2) - (len_footer / 2);


	// draw header	
	show_text(str_header, x_position_header, 0);

	// draw footer
	show_text(str_footer, x_position_footer, (HIGH_SCREEN - 1));

}

void draw_border_of_TRIVIA(COLOR color)
{
	set_color(DEFAULT);
	//system("cls");
	ClearScreen();


	//*********** draw border ***********
	draw_border(0, 0, WIDE_SCREEN, HIGH_SCREEN, color);

	//*********** header and footer ***********
	_draw_border_write_text(GAME_BORDER__HEADER, GAME_BORDER__FOOTER);

}


void draw_players_info_section(GameSettings* gameSettings)
{
	// draw border
	int players_count = gameSettings->players.count;
	Player* players = gameSettings->players.list;

	int high = players_count + 3;

	draw_border(PLAYER_X, PLAYER_Y, PLAYER_WIDE, high, FORE_RED);

	// draw players info
	int header_y = PLAYER_Y + 1;

	int header_x1 = PLAYER_X + 2;				// x1 = player name
	int header_x2 = header_x1 + 10 + 4;		// x2 = player amount

	// draw players name
	show_text(PLAYERS_AREA__COLUMN_HEADER_PLAYER_NAME, 5, header_x1, header_y);
	for (int i = 0; i < players_count; i++)
	{
		show_text(players[i].name, 3, header_x1, header_y + i + 1);
	}


	// draw players amount
	show_text(PLAYERS_AREA__COLUMN_HEADER_PLAYER_AMOUNT, 12, header_x2, header_y);
	if (gameSettings->game_type == GAME_TYPE_REGULAR)
	{
		for (int i = 0; i < players_count; i++)
		{
			show_text("        ", header_x2 - 1, header_y + i + 1);

			char tmp[50];
			sprintf_s(tmp, "%d", players[i].amount);
			show_text(tmp, 14, header_x2, header_y + i + 1);
		}
	}
	else
	{
		for (int i = 0; i < players_count; i++)
		{
			show_text("        ", header_x2 - 1, header_y + i + 1);
			show_text(" ??? ", 14, header_x2, header_y + i + 1);
		}

	}

		

}

void _draw_lines_of_timer(int times)
{
	for (int i = times; i > 0; i--)
	{
		set_color(12);
		putcharAt('|', TIMER_X + i, TIMER_Y);
	}

}
void _clear_lines_of_timer(int times)
{
	for (int i = times; i > 0; i--)
	{
		putcharAt(' ', TIMER_X + i, TIMER_Y);
	}
}

void _draw_timer(int choise_time)
{
	show_text(TIMER_AREA__LEFT_TIME_LABEL, 2, TIMER_Y);

	for (int i = 0; i < choise_time; i++)
	{
		putcharAt('|', TIMER_X + i, TIMER_Y);
	}
}
void _update_timer(PlayersList players, int times)
{

	clear_timer_second();

	for (int i = times; i > 1; i--)
	{
		putcharAt('|', TIMER_X + i-2 ,TIMER_Y);
	}
	if (times != 1)
	{
		play_timer_beep(1500);
	}
}


void _draw_symbole1(int x, int y)
{
	/*
	   .
	 _\(/_
	 ./)\
	   '
	*/

	int rows = 4;

	char symbole[4][10] = { "  .", "_\\(/_","./)\\","  '" };

	for (size_t i = 0; i < rows; i++)
	{
		set_cursor_position(x, y + i);

		show_text(symbole[i]);
	}

}
void _clear_symbole1(int x, int y)
{
	/*
	   .
	 _\(/_
	 ./)\
	   '
	*/

	int rows = 4;

	char spases[] = "       ";

	for (size_t i = 0; i < rows; i++)
	{
		set_cursor_position(x, y + i);

		show_text(spases);
	}
}

void _draw_symbole2(int x, int y)
{
	/*
		 *''*
		*_\/_*
		* /\ *
		  * '
	*/

	int rows = 4;

	char symbole[4][10] = { " *''*","*_\\/_*","* /\\ *","  * '" };

	for (size_t i = 0; i < rows; i++)
	{
		set_cursor_position(x, y + i);

		show_text(symbole[i]);
	}

}
void _clear_symbole2(int x, int y)
{

	/*
		 *''*
		*_\/_*
		* /\ *
		  * '
	*/

	int rows = 4;

	char spases[] = "       ";

	for (size_t i = 0; i < rows; i++)
	{
		set_cursor_position(x, y + i);

		show_text(spases);
	}

}

void _draw_symbole3(int x, int y)
{
	/*
		   :
		'.\'/.'
		-= o =-
		.'/.\'.
		   :
	*/

	int rows = 5;

	char symbole[5][10] = { "   :","'.\\'/.'","-= o =-",".'/.\\'.","   :" };

	for (size_t i = 0; i < rows; i++)
	{
		set_cursor_position(x, y + i);

		show_text(symbole[i]);
	}

}
void _clear_symbole3(int x, int y)
{
	/*
		   :
		'.\'/.'
		-= o =-
		.'/.\'.
		   :
	*/

	int rows = 5;

	char spases[] = "       ";

	for (size_t i = 0; i < rows; i++)
	{
		set_cursor_position(x, y + i);

		show_text(spases);
	}

}

void _draw_symbole4(int x, int y)
{
	/*
		  .''.
		 :_\/_:
		 : /\ :
		  '..'
	*/

	int rows = 4;

	char symbole[4][10] = { " .''.",":_\\/_:",": /\\ :"," '..'" };

	for (size_t i = 0; i < rows; i++)
	{
		set_cursor_position(x, y + i);

		show_text(symbole[i]);
	}

}
void _clear_symbole4(int x, int y)
{
	/*
		  .''.
		 :_\/_:
		 : /\ :
		  '..'
	*/

	int rows = 4;

	char spases[] = "       ";

	for (size_t i = 0; i < rows; i++)
	{
		set_cursor_position(x, y + i);

		show_text(spases);
	}

}

void _draw_symbole5(int x, int y)
{
	/*
		   .
		.'.:.'.
		-=:o:=-
		'.':'.'
		   '
	*/

	int rows = 5;

	char symbole[5][10] = { "   .",".'.:.'.","-=:o:=-","'.':'.'","   '" };

	for (size_t i = 0; i < rows; i++)
	{
		set_cursor_position(x, y + i);

		show_text(symbole[i]);
	}

}
void _clear_symbole5(int x, int y)
{
	/*
		   .
		.'.:.'.
		-=:o:=-
		'.':'.'
		   '
	*/

	int rows = 5;

	char spases[] = "       ";

	for (size_t i = 0; i < rows; i++)
	{
		set_cursor_position(x, y + i);

		show_text(spases);
	}

}

void write_the_victory(PlayersList* players)
{
	// מנקה את המסך
	clear_border_whisout_TRIVIA_border();

	char message[100];

	// מציג את שם השחקן המנצח
	sprintf_s(message, WINNER_MESSAGE__PLAYER_NAME, players->list[players->winner_index].name);
	int len = strlen(message);
	show_text(message, 4, QUESTION_X, QUESTION_Y);

	// הודעת מזל טוב
	sprintf_s(message, WINNER_MESSAGE__MAZAL_TOV);
	len = strlen(message);
	show_text(message, 12, QUESTION_X, QUESTION_Y + 2);

	Sleep(500);
	set_hebrew(false);

	for (int i = 0; i < 4; i++)
	{
		// symbole1
		set_color(13);
		_draw_symbole1(8, 5);
		Sleep(300);

		set_color(5);
		_draw_symbole1(8, 5);
		_clear_symbole4(23, 19);

		// symbole2
		set_color(15);
		_draw_symbole2(60, 22);
		Sleep(300);

		set_color(8);
		_draw_symbole2(60, 22);
		_clear_symbole5(72, 8);

		// symbole3
		set_color(12);
		_draw_symbole3(48, 3);
		Sleep(300);

		set_color(4);
		_draw_symbole3(48, 3);
		_clear_symbole1(8, 5);

		// symbole4
		set_color(14);
		_draw_symbole4(23, 19);
		Sleep(300);

		set_color(6);
		_draw_symbole4(23, 19);
		_clear_symbole2(60, 22);

		// symbole5
		set_color(11);
		_draw_symbole5(72, 8);
		Sleep(300);
		
		set_color(1);
		_draw_symbole5(72, 8);
		_clear_symbole3(48, 3);
	

	}

	set_hebrew(IS_HEB);
	clear_border_whisout_TRIVIA_border();
	Sleep(150);
}
void lose_message(PlayersList* players)
{
	// מנקה את המסך
	clear_border_whisout_TRIVIA_border();

	char message[100];

	// מציג את שם השחקן המנצח
	sprintf_s(message, LOSSING_MESSAGE__PLAYER_NAME, players->list[0].name);
	int len = strlen(message);
	show_text(message, 4, QUESTION_X, QUESTION_Y);

	// הודעת מזל טוב
	len = strlen(LOSSING_MESSAGE__SINGEL_PLAYER);
	show_text(LOSSING_MESSAGE__SINGEL_PLAYER, 12, QUESTION_X, QUESTION_Y + 2);
	Sleep(1000);
}
