#include <string.h>
#include <stdio.h>
#include "Display.h"

void encrypt_string(char* plan_text)
{
	set_cursor_position(5, 3);
	puts(plan_text);
	int len = strlen(plan_text);
	for (int i = 0; i < len; i++)
	{
		plan_text[i] += 1;
	}
	set_cursor_position(5, 4);
	puts(plan_text);
}
void decrypt_string(char* encrypted_text)
{
	set_cursor_position(5, 6);
	puts(encrypted_text);
	int len = strlen(encrypted_text);
	for (int i = 0; i < len; i++)
	{
		encrypted_text[i] -= 1;
	}
	set_cursor_position(5, 7);
	puts(encrypted_text);
}

void encrypt_bytes(char* bytes, int size)
{
	for (int i = 0; i < size; i++)
	{
		bytes[i] += 2;
	}
}
void decrypt_bytes(char* bytes_encrypted, int size)
{
	for (int i = 0; i < size; i++)
	{
		bytes_encrypted[i] -= 2;
	}
}


void encrypt_file(const char* source_file_name, const char* target_file_name)
{
	FILE* sours_file_p;
	FILE* target_file_p;

	long lSize;
	int result;
	char* buffer;

	// פתיחת הקובץ המקורי ובדיקתו
	fopen_s(&sours_file_p, source_file_name, "rb");
	if (sours_file_p == NULL)
	{
		show_text("Error opening file\n");
		exit(1);
	}
	// פתיחת הקובץ החדש ובדיקתו
	int err = fopen_s(&target_file_p, target_file_name, "w+b");
	if (target_file_p == NULL)
	{
		show_text("Error opening file\n");
		exit(1);
	}

	// בדיקה - כמה בתים יש בקובץ המקורי
	fseek(sours_file_p, 0,  SEEK_END);
	lSize = ftell(sours_file_p);

	// הוזזת המיקום להתחלה
	rewind(sours_file_p);

	// הקצאת מקום בגודל המתאים למערך ההצפנה
	// ובדיקתו 
	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL)
	{ 
		fputs("Memory error", stderr); 
		exit(2); 
	}

	// העתקת כל הערכים למערך ההצפנה
	// ובדיקתו
	result = fread(buffer, 1, lSize, sours_file_p);
	if (result != lSize)
	{
		fputs("Reading error", stderr); 
		exit(3);
	}

	
	// שנוי הערכים במערך ההצפנה - לערכים מוצפנים
	encrypt_bytes(buffer, result);

	// כתיבה לקובץ החדש ממערך ההצפנה
	fwrite(buffer, 1, result, target_file_p);

	// שיחרור הזיכרון של מערך ההצפנה שהוקצה דינמית
	free(buffer);
	// סגירת הקובץ
	fclose(target_file_p);
	fclose(sours_file_p);
}
void decrypt_file(const char* source_file_name, const char* target_file_name)
{
	FILE* sours_file_p;
	FILE* target_file_p;

	long lSize;
	int result;
	char* buffer;

	// פתיחת קובץ ההצפנה ובדיקתו
	fopen_s(&sours_file_p, source_file_name, "rb");
	if (sours_file_p == NULL)
	{
		show_text("Error opening file\n");
		exit(1);
	}

	// פתיחת הקובץ החדש ובדיקתו
	fopen_s(&target_file_p, target_file_name, "w+b");
	if (target_file_p == NULL)
	{
		show_text("Error opening file\n");
		exit(1);
	}
	// בדיקה - כמה בתים יש בקובץ המקורי
	fseek(sours_file_p, 0, SEEK_END);
	lSize = ftell(sours_file_p);

	// הוזזת המיקום להתחלה
	rewind(sours_file_p);

	// הקצאת מקום בגודל המתאים למערך ההצפנה
	// ובדיקתו 
	buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		exit(2);
	}

	// העתקת כל הערכים למערך ההצפנה
	// ובדיקתו
	result = fread(buffer, 1, lSize, sours_file_p);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		exit(3);
	}


	// שנוי הערכים במערך ההצפנה - לערכים רגילים
	decrypt_bytes(buffer, lSize);

	// כתיבה לקובץ החדש ממערך ההצפנה
	fwrite(buffer, 1, lSize, target_file_p);

	// שיחרור הזיכרון של מערך ההצפנה שהוקצה דינמית
	free(buffer);
	// סגירת הקובץ
	fclose(target_file_p);
	fclose(sours_file_p);
}


//{
//	FILE* test_for_decrypt;
//
//	long lSize;
//	int result;
//	char* buffer;
//
//	// פתיחת הקובץ ובדיקתו
//	fopen_s(&test_for_decrypt, source_file_name, "r+t");
//	if (test_for_decrypt == NULL)
//	{
//		show_text("Error opening file\n");
//		exit(1);
//	}
//
//	// בדיקה - כמה בתים יש
//	fseek(test_for_decrypt, 0, SEEK_END);
//	lSize = ftell(test_for_decrypt);
//	// הוזזת המיקום להתחלה
//	rewind(test_for_decrypt);
//
//	// הקצאת מקום בגודל המתאים למערך ההצפנה
//	// ובדיקתו 
//	buffer = (char*)malloc(sizeof(char) * lSize);
//	if (buffer == NULL)
//	{
//		fputs("Memory error", stderr);
//		exit(2);
//	}
//
//	// העתקת כל הערכים למערך ההצפנה
//	// ובדיקתו
//	result = fread(buffer, 1, lSize, test_for_decrypt);
//	if (result != lSize)
//	{
//		fputs("Reading error", stderr);
//		exit(3);
//	}
//
//	// שנוי הערכים במערך ההצפנה - לערכים מוצפנים
//	decrypt_bytes(buffer, lSize);
//
//	// שיחרור הזיכרון של מערך ההצפנה שהוקצה דינמית
//	free(buffer);
//	// סגירת הקובץ
//	fclose(test_for_decrypt);
//
//}


