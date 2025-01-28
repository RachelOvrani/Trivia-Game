#pragma once

#include<stdio.h>
#include "Global.h"

enum PLAYER_TYPE
{
	PLAYER_TYPE_SINGLE_MALE = 1,
	PLAYER_TYPE_SINGLE_FEMALE = 2,
	PLAYER_TYPE_GROUP = 3,
};



// מבנים
struct Player
{
	char* name;
	PLAYER_TYPE type;
	int age = 0;
	int amount = INIT_AMOUNT;
};

struct PlayersList
{
	//Player* list;
	Player list[MAX_OF_PLAYERS];
	int count;

	int current_player_index;
	int current_player_bet_amount;
	int current_player_answer = -1;
	int winner_index = -1;
};

struct GameSettings
{
	PlayersList players;
	int answer_time = -1;

#ifdef FAMILY_MODE
	GAME_TYPE game_type = GAME_TYPE_REGULAR;
#else
	GAME_TYPE game_type = GAME_TYPE_REGULAR;
#endif 

};

//struct MessageInfo
//{
//	int blink;
//	int blink_delay;
//};

struct TextLineInfo
{
	const char* text;
	int pos_x = -1;
	int pos_y = -1;
	int color = -1;
};

struct Border
{
	int x;
	int y;
	int wide;
	int high;
	int color;
};

struct Message
{
	char* str;
};

struct FileContent
{
	char** lines = NULL;
	int size = 0;
};
struct Question
{
	int question_id;
	int subject_id;
	int hard_level;

	char* question;
	char* answer1 = NULL;
	char* answer2 = NULL;
	char* answer3 = NULL;
	char* answer4 = NULL;
	int right_answer;
	bool last_question = false;

};

struct QuestionsList
{
	Question* list = NULL;
	int max_level = 4;
	int count = 0;
};

struct Subject
{
	int id;
	char* name;
};



struct SubjectList
{
	Subject* list = NULL;
	int count = 0;
};



struct SubjectSuggest
{
	int subject_id = -1;
	int level;
	int index = -1;
};

struct SubjectSuggestList
{
	SubjectSuggest list[4];
	int max_level = 0;
};


struct ResponseMessage
{
	int id;
	char* message_for_male;
	char* message_for_female;
	char* message_for_group;
};

struct SuccessMessageList
{
	ResponseMessage* list = NULL;
	int count = 0;
};

struct FailMessageList
{
	ResponseMessage* list = NULL;
	int count = 0;
};
