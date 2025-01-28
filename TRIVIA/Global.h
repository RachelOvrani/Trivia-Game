#pragma once

enum GAME_TYPE
{
	GAME_TYPE_REGULAR = 1,
	GAME_TYPE_FAMILY = 2,
};


#define FAMILY_MODE


// הגדרות
#define IS_HEB true
#define MAX_LEVEL 4

// ערכי המסגרת של המסך
#define HIGH_SCREEN 29//-1
#define WIDE_SCREEN 80//-1

// הגדרות שחקנים
#define MAX_OF_PLAYERS 13
#define INIT_AMOUNT 1000
#define MAX_AMOUNT 1000000


// ******************************************************************
// **************************  מסך כניסה  **************************
// ******************************************************************

// לוגו - אנימציית מיקום
#define OPEN_SCREEN__LOGO_ANIMATION_ROW_START 0
#define OPEN_SCREEN__LOGO_ANIMATION_ROW_END 5
#define OPEN_SCREEN__LOGO_ANIMATION_DELAY = 250

// לוגו - אנימציית צבע
#define OPEN_SCREEN__LOGO_COLOR_ANIMATION_START 7
#define OPEN_SCREEN__LOGO_COLOR_ANIMATION_END 14
#define OPEN_SCREEN__LOGO_COLOR_ANIMATION_DELAY 200


// טקסט - שם המפתח וכל הזכויות שמורות
#define OPEN_SCREEN__TEXT_DEVELOPER_NAME_ROW_START 20
#define OPEN_SCREEN__TEXT_DEVELOPER_NAME_ROW_COLOR 20



// ******************************************************************
// ************************** אזורים במסך  *************************
// ******************************************************************

// ערכי המסגרת של הטימר
#define TIMER_X 13
#define TIMER_Y 5
#define TIMER_WIDE 30
#define MAX_TIMER_HIGH 2


// ערכי המסגרת של השחקנים
#define PLAYER_X (WIDE_SCREEN - 28)
#ifdef FAMILY_MODE
#define PLAYER_Y 1
#else
#define PLAYER_Y 2
#endif
#define PLAYER_WIDE 25
#define MAX_PLAYER_HIGH (MAX_OF_PLAYERS+3)


// ערכי תיבת השאלות
#define QUESTION_WIDE 55
#define QUESTION_HIGH 13
#define QUESTION_X ((WIDE_SCREEN / 2)-(len/2))
#ifdef FAMILY_MODE
#define QUESTION_Y (HIGH_SCREEN / 2)+2
#else
#define QUESTION_Y (HIGH_SCREEN / 2)+1
#endif

// ערכי תיבת נושאים לבחירה
#define SUBJECT_SUGGEST_POS_WIDE 32
#define SUBJECT_SUGGEST_POS_HEIGH 4
#define SUBJECT_SUGGEST_POS_X ((WIDE_SCREEN / 2) - (SUBJECT_SUGGEST_POS_WIDE / 2))
#define SUBJECT_SUGGEST_POS_Y QUESTION_Y+1

// ערכי תיבת התשובות לשאלות
#define POS_ANSWER_WIDE 22
#define POS_ANSWER_HEIGH 4
#define POS_ANSWER_X ((WIDE_SCREEN / 2) - (POS_ANSWER_WIDE/2))
#define POS_ANSWER_Y QUESTION_Y+2


// ערכי תיבת הערות בתחתית מסך 
#define MESSAGE_X_ ((WIDE_SCREEN / 2)-(len/2))
#define MESSAGE_Y_ (HIGH_SCREEN - 5)





