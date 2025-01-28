#pragma once
#include "Types.h"
#include <stdio.h>


void load_data();

void load_response_meesage();
void reset_game_setings(GameSettings* gameSettings);

char** split(const char* str, char splitter, int* size);


ResponseMessage* get_response_success_message();
ResponseMessage* get_response_fail_message();



//void get_subject_suggested_list(SubjectSuggestList* subject_list);

SubjectSuggestList get_subject_suggested_list();


Question GetQuestion(int subject_id, int level);
Question get_next_question(int number_question);

Subject get_subject_by_id(int id);

Question get_question_example();