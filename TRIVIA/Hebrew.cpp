#include "Global.h"
#include "Display.h"
#include "Strings.h"
#include <conio.h>
#include "UserInput.h"

bool is_hebrew = false;

bool _str_fix_for_hebrew__is_digit(char c)
{
	return (c >= '0' && c <= '9')
		|| (c == '-');

}
void _str_fix_for_hebrew__reverse_chrcter(char* str, int first_index, int end_index)
{
	int character = end_index - first_index + 1;
	char* s = (char*)malloc(sizeof(char) * character + 1);

	int last_character;
	for (int i = 0; i < character; i++)
	{
		s[i] = str[first_index + i];
		last_character = i;
	}

	s[last_character + 1] = '\0';
	reverseStr(s);

	for (int i = 0; i < character; i++)
	{
		str[first_index + i] = s[i];
	}

	free(s);
}
void str_fix_for_hebrew(char* str)
{
	int len = strlen(str);
	int first_index = -1;
	int end_index = -1;

	for (int i = 0; i < len; i++)
	{
		if (str[i] == '(') str[i] = ')';
		else  if (str[i] == ')') str[i] = '(';

		//if (str[i] >= 0 && str[i] <= 127) // ascii character
		if (_str_fix_for_hebrew__is_digit(str[i]))
		{
			if (first_index == -1)
			{
				first_index = i;
			}
			else //(first_index != -1)
			{
				if ((i == (len - 1))			// last char
					|| (!_str_fix_for_hebrew__is_digit(str[i + 1]))) // next char isn't digit
				{
					end_index = i;
				}
			}
		}
		else
		{
			first_index = -1;
			end_index = -1;
		}

		if (end_index != -1)
		{
			_str_fix_for_hebrew__reverse_chrcter(str, first_index, end_index);
			first_index = -1;
			end_index = -1;
		}


	}
}



bool get_hebrew()
{
	return is_hebrew;
}
void set_hebrew(bool value)
{
	is_hebrew = value;
}

int _get_x_pos_before(int x_original, int text_len)
{
	// *******************************
	//       טיפול במיקום הסמן
	// *******************************

	// 1. מציאת מיקום הסמן
	// 2. חישוב ציר X
	// 2.1. לצד ימין במקום צד שמאל
	x_original = WIDE_SCREEN - 1 - x_original;

	// 2.2. חישוב אורך הטקסט כדי לחזור אחורה
	x_original -= (text_len - 1);

	return x_original;
}
int _get_x_pos_after(int x_original, int len)
{
	// *********************************************************************************************
	//       החזרת הסמן למיקום שהמתכנת תיכנן שהוא יהיה אחרי ביצוע הפעולה
	// *********************************************************************************************

	// 6. הזזת הסמן למיקום המצופה
	return x_original += len;
}

void fix_char_RTL(char* c)
{
	// Fix Corners RTL
	switch (*c)
	{
	case (char)CORNER_LEFT_TOP:
		*c = CORNER_RIGHT_TOP;
		break;

	case (char)CORNER_RIGHT_TOP:
		*c = CORNER_LEFT_TOP;
		break;

	case (char)CORNER_RIGHT_BOTTON:
		*c = (char)CORNER_LEFT_BOTTON;
		break;

	case (char)CORNER_LEFT_BOTTON:
		*c = CORNER_RIGHT_BOTTON;
		break;

	default:
		break;
	}

}

void putchar_for_language(char c)
{
	if (is_hebrew)
	{
		// *******************************
		//       טיפול במיקום הסמן
		// *******************************

		// 1. מציאת מיקום הסמן
		COORD coord_original = get_cursor_position();
		int len = 1;

		// 2. חישוב ציר X
		int x_befor = _get_x_pos_before(coord_original.X, len);

		// 3. קביעת המיקום החדש של הסמן
		set_cursor_position(x_befor, coord_original.Y);


		// תיקון תוים שהם מימין לשמאל
		// פינות של מסגר
		fix_char_RTL(&c);


		// 4. הצגת התו
		putchar(c);

		// ***************************************************************************
		//       החזרת הסמן למיקום שהמתכנת תיכנן שהוא יהיה אחרי ביצוע הפעולה
		// ***************************************************************************

		// 5. עריכת המקום המצופה
		int x_after = _get_x_pos_after(coord_original.X, len);

		// 6. הזזת הסמן למיקום המצופה	
		set_cursor_position(x_after, coord_original.Y);
	}
	else
	{
		putchar(c);
	}
}
void printf_for_language(const char* message)
{
	if (is_hebrew)
	{
		// *******************************
		//       טיפול במיקום הסמן
		// *******************************

		// 1. מציאת מיקום הסמן
		COORD coord_original = get_cursor_position();
		int len = strlen(message);

		// 2. עריכת המיקום החדש
		int x_before = _get_x_pos_before(coord_original.X, len);

		// 3. קביעת המיקום החדש של הסמן
		set_cursor_position(x_before, coord_original.Y);

		// *******************************
		//       טיפול בטקסט
		// *******************************

		// 4. הכנת הטקסט לעברית ע"י הפיכתו ותיקונו
		char* msg = _strdup(message);
		// הפיכתו
		reverseStr(msg);
		// ותיקונו
		str_fix_for_hebrew(msg);

		// 5. הצגת הטקסט
		printf("%s", msg);
		free(msg);

		// ***************************************************************************
		//       החזרת הסמן למיקום שהמתכנת תיכנן שהוא יהיה אחרי ביצוע הפעולה
		// ***************************************************************************

		// 6. עריכת המיקום המצופה
		int x_after = _get_x_pos_after(coord_original.X, len);
		// 7. הזזת הסמן למיקום המצופה
		set_cursor_position(x_after, coord_original.Y);
	}
	else
	{
		printf("%s", message);
	}
}

char* gets_heb()
{
	COORD coord = get_cursor_position();
	int x = coord.X;
	int y = coord.Y;
	int i = 0;

	char c;
	char* str = (char*)malloc(sizeof(char) * 1);
	str[0] = '\0';

	int counter = 0;
	while (true)
	{
		set_cursor_visible(true);
		char c = _getch();
		set_cursor_visible(false);


		// אחריות על קביעת הסמן
		if (c == '\b')
		{
			if (counter == 0)
			{
				//x = 20;
				//set_cursor_position(x, y);
				continue;
			}
			x++;
			set_cursor_position(x, y);
			putchar(' ');
			set_cursor_position(x, y);

			counter--;
			str = (char*)realloc(str, sizeof(char) * (counter + 1));
			str[counter] = '\0';

		}
		else if (c == '\r')
		{
			// ההתניה הבאה מחייבת הזנת ערך
			if (counter > 0)
			{
				return str;
			}
		}
		else if (counter >= 20) // max length of input
		{
			continue;
		}
		else
		{
			putchar(c);
			x--;
			set_cursor_position(x, y);

			counter++;
			str = (char*)realloc(str, sizeof(char) * (counter + 1));
			str[counter - 1] = c;
			str[counter] = '\0';
		}



	}
}

void set_pos_for_languge(int len)
{
	if (is_hebrew)
	{
		COORD coord = get_cursor_position();
		//RTL	
		coord.X = WIDE_SCREEN - 1 - coord.X;
		set_cursor_position(coord.X, coord.Y);
	}
}

void set_pos_for_languge(const char* str)
{
	if (is_hebrew)
	{
		int len = strlen(str);
		set_pos_for_languge(len);
	}
}

bool is_digit(char c)
{
	return (c >= '0') && (c <= '9');
}
char _getch_for_languae()
{
	if (is_hebrew)
	{
		COORD coord_original = get_cursor_position();

		set_pos_for_languge(0);
		set_cursor_visible(true);
		char c = _getch();		
		set_cursor_visible(false);
		set_cursor_position(coord_original.X, coord_original.Y);

		return c;
	}
	else
	{
		return _getch();
	}
}
char* gets_heb_with_numbers_support()
{
	char c;
	char* str;
	int counter = 0;
	int counter_numbers = 0;
	str = (char*)malloc(sizeof(char) * counter + 1);
	str[counter] = '\0';

	//set_cursor_position(0, 0);
	while (true)
	{
		c = _getch_for_languae();

		if (c == '\b')
		{
			if (counter == 0)
			{
				continue;
			}
			else if (is_digit(str[counter - 1]) &&
				(is_digit(str[counter - 2])))
			{
				int end_index = counter - 1;
				int start_index = end_index - counter_numbers;

				//COORD coord = get_cursor_position();
				//set_cursor_position(start_index, coord.Y);

				for (int i = 0; i <= end_index; i++)
				{
					putchar_for_language(' ');
				}

				//set_cursor_position(start_index, coord.Y);
				for (int i = 0; i < counter_numbers; i++)
				{
					putchar_for_language(str[counter - 2 - i]);
				}

				counter--;
				counter_numbers--;
				str = (char*)realloc(str, sizeof(char) * (counter + 1));
				str[counter] = '\0';
				continue;

			}
			else
			{

				COORD coord = get_cursor_position();
				set_cursor_position(coord.X - 1, coord.Y);
				putchar_for_language(' ');
				set_cursor_position(coord.X - 1, coord.Y);

				counter--;
				str = (char*)realloc(str, sizeof(char) * (counter + 1));
				str[counter] = '\0';
				continue;
			}
		}
		else if (c == '\r')
		{
			return str;
		}
		else
		{
			str = (char*)realloc(str, sizeof(char) * (++counter));
			str[counter - 1] = c;
			str[counter] = '\0';

			char current = c;
			char prev = str[counter - 2];

			if (is_digit(prev) == true
				&& is_digit(current) == true)
			{
				if (counter >= 3 && !is_digit(str[counter - 3]))
				{
					counter_numbers = 0;
				}
				counter_numbers++;

				COORD coord = get_cursor_position();
				set_cursor_position((coord.X - counter_numbers), coord.Y);

				// בגלל שזה ספרות שאמורים להיות מודפסים הפוך
				// נתחיל באינדקס הגדול ונסיים באינדקס הקטן
				int startIndex = counter - 1;
				int endIndex = startIndex - counter_numbers;

				for (size_t i = startIndex; i >= endIndex; i--)
				{
					putchar_for_language(str[i]);
					if (i == 0)
					{
						break;
					}
				}
			}

			else
			{
				putchar_for_language(c);
			}
		}
	}
	getchar();
	getchar();
}



///    TRIVIA FANCTION      //

char* get_player_name_for_languae(int msg_len)
{
	if (is_hebrew)
	{
		set_pos_for_languge(msg_len);
		return gets_heb();
		//return gets_heb_with_numbers_support();
	}
	else
	{
		return get_str_from_user();
	}
}

char* get_nice_answre_for_languae(int msg_len)
{
	if (is_hebrew)
	{
		set_pos_for_languge(msg_len);
		return gets_heb();
		//return gets_heb_with_numbers_support();
	}
	else
	{
		return get_str_from_user();
	}
}

