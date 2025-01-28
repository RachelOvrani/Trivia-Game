#include <string.h>
#include <xmemory>
#include "Data.h"
#include "Math.h"
#include "Memory.h"
#include "Strings.h"
#include "Display.h"
#include "Encryption.h"




SubjectList subjectsList;
QuestionsList questionsList;
SuccessMessageList successMessage;
FailMessageList failMessage;





FileContent get_encrypted_file_content(FILE* file_p)
{
	FileContent fileContent;
	char* buffer;
	int lSize;
	int result;


	// בדיקה - כמה בתים יש בקובץ המקורי
	fseek(file_p, 0, SEEK_END);
	lSize = ftell(file_p);

	// הוזזת המיקום להתחלה
	rewind(file_p);

	// הקצאת מקום בגודל המתאים למערך ההצפנה
	// ובדיקתו 
	// \0 הגודל של המערך הוא בגודל התווים שבקובץ פלוס 1 בשביל 
	buffer = (char*)malloc(sizeof(char) * (lSize));
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		exit(2);
	}

	// העתקת כל הערכים למערך ההצפנה
	// ובדיקתו
	result = fread(buffer, 1, lSize, file_p);
	//printf(buffer);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		exit(3);
	}
	buffer[lSize] = '\0';
	// שנוי הערכים במערך ההצפנה - לערכים מוצפנים
	decrypt_bytes(buffer, result);

	fileContent.lines = split(buffer, '\n', &fileContent.size);
	for (int i = 0; i < fileContent.size; i++)
	{
		int len = strlen(fileContent.lines[i]);
		// אם יש בסוף השורה 'r\' - ידרוס אותו
		if (fileContent.lines[i][len - 1] == '\r')
		{
			fileContent.lines[i][len - 1] = '\0';
		}
		else
		{
			int ii = 0;
			// שורה אחרונה - לא יעשה כלום
		}
	}

	return fileContent;

}

Subject parse_subject_from_file_line(char* subject_line)
{
	
	int len = strlen(subject_line);

	int count = 0;
	char** str = split(subject_line, '\t', &count);

	Subject subject;
	int id = atoi(str[0]);
	subject.id = id;

	subject.name = _strdup(str[1]);

	free_arr(str, count);


	return subject;
}
Question parse_questions_from_file_line(char* question_line)
{
	Question question;
	int len = strlen(question_line);

	// הסרת סימן 'שורה חדשה' מסיום השורה
	//question_line[len - 1] = '\0'; // replace '\n' with '\0'
	//len--;


	int count = 0;
	char** str = split(question_line, '\t', &count);

	int id_question = atoi(str[0]);
	question.question_id = id_question;

	int id_subject = atoi(str[1]);
	question.subject_id = id_subject;

	int level = atoi(str[2]);
	question.hard_level = level;
	questionsList.max_level = level;


	question.question = _strdup(str[3]);

	question.answer1 = _strdup(str[4]);
	question.answer2 = _strdup(str[5]);
	question.answer3 = _strdup(str[6]);
	if (count < 9)
	{
		int right_ans_number = atoi(str[7]);
		question.right_answer = right_ans_number;
		return question;
	}

	question.answer4 = _strdup(str[7]);
	int right_ans_number = atoi(str[8]);
	question.right_answer = right_ans_number;

	free_arr(str, count);


	return question;
}

void _load_data_subjects(char* file_name)
{
	FILE* subjects_file;

	// פתיחת הקובץ ובדיקתו
	//fopen_s(&subjects_file, "Subjects_h.txt", "rt");
	fopen_s(&subjects_file, file_name, "rt");
	if (subjects_file == NULL)
	{
		show_text("Error opening file\n");
		exit(1);
	}

	// קריאת תוכן הקובץ
	//הפיכה ללא מוצפנת
	// החזרת מבנה של מערך השורות 
	FileContent fileContent = get_encrypted_file_content(subjects_file);

	// קריאת הנתונים מהקובץ
	int line_index = 0;
	while (true)
	{

		// פיענוח השורה ויצירת אוביקט נושא
		Subject subject = parse_subject_from_file_line(fileContent.lines[line_index]);


		// הגדלת רשימת הנושאים
		// והוספת הנושא החדש
		subjectsList.list = (Subject*)realloc(subjectsList.list, sizeof(Subject) * (++subjectsList.count));
		subjectsList.list[subjectsList.count - 1] = subject;

		// בדיקה - אם הגיעה לסוף מערך השאלות
		// אם כן - יצא
		if (line_index == (fileContent.size - 1))break;
		// ואם לא - יגדיל את אינדקס
		line_index++;
	}

	// סגירת  הקובץ
	fclose(subjects_file);
}
void _load_data_quesitons(char* file_name)
{
	FILE* questions_file;

	//fopen_s(&questions_file, "Questions_h.txt", "rt");
	fopen_s(&questions_file, file_name, "rt");
	if (questions_file == NULL)
	{
		show_text("Error opening file\n");
		exit(1);
	}

	// קריאת תוכן הקובץ
	// הפיכה ללא מוצפנת
	// החזרת מבנה של מערך השורות
	FileContent fileContent = get_encrypted_file_content(questions_file);


	int line_index = 0;
	while (true)
	{

		// פיענוח השורה ויצירת אוביקט שאלה
		Question questions = parse_questions_from_file_line(fileContent.lines[line_index]);

		// הגדלת רשימת השאלות
		// והוספת השאלה החדשה
		questionsList.list = (Question*)realloc(questionsList.list, sizeof(Question) * (++questionsList.count));
		questionsList.list[questionsList.count - 1] = questions;

		// בדיקה - אם הגיעה לסוף מערך השאלות
		// אם כן - יצא 
		if (line_index == (fileContent.size - 1))break;
		// ואם לא - יגדיל את אינדקס
		line_index++;
	}

	fclose(questions_file);
}

void load_data()
{
	char file_name_of_subject[] = "data/Subjects_h.txt";
	char file_name_of_question[] = "data/Questions_h.txt";
	_load_data_subjects(file_name_of_subject);
	_load_data_quesitons(file_name_of_question);
}



SubjectSuggestList get_subject_suggested_list()
{
	SubjectSuggestList ssl;



	for (int i = 0; i < 4; i++)
	{
		while (true)
		{
			int index = random(0, subjectsList.count - 1);

			if (index != ssl.list[0].index &&
				index != ssl.list[1].index &&
				index != ssl.list[2].index &&
				index != ssl.list[3].index)
			{
				ssl.list[i].subject_id = subjectsList.list[index].id;
				subjectsList.list[index].name;
				ssl.list[i].level = random(1, questionsList.max_level);
				ssl.list[i].index = index;
				break;
			}
		}
	}

	return ssl;
}

Question GetQuestion(int subject_id, int level)
{
	int idex_first = 0;
	int idex_end = 0;

	for (int i = 1; i < questionsList.count; i++)
	{
		if (questionsList.list[i].subject_id == subject_id && questionsList.list[i].hard_level == level)
		{
			if (idex_first == 0)
			{
				idex_first = i;
			}
			else
			{
				idex_end = i;
			}
		}
		else if (idex_end != 0)
		{
			break;
		}
	}


	int question_id = random(idex_first, idex_end);

	return questionsList.list[question_id];


}

Question get_next_question(int number_question)
{
	if (questionsList.count == number_question)
	{
		questionsList.list[number_question-1].last_question = true;
	}

	return questionsList.list[number_question-1];
}


Question get_question_example()
{
	Question question;
	//question.cutegory = _strdup("Tora                        (1\\2)");
	question.hard_level = 2;

	question.question = _strdup("Who take out Bniy Yisrael from Mizraim ???");
	question.answer1 = _strdup("1. Moshe Rabienu");
	question.answer2 = _strdup("2. Yehoshua");
	question.answer3 = _strdup("3. Avraham Avinu");
	question.answer4 = _strdup("4. Lanan");

	question.right_answer = 1;

	return question;
}

Subject get_subject_by_id(int id)
{
	for (int i = 0; i < subjectsList.count; i++)
	{
		if (subjectsList.list[i].id == id)
		{
			return subjectsList.list[i];
		}
	}


	// לא אמור לקרות
	Subject subject;
	subject.id = -1;
	return subject;
}

void load_response_meesage()
{
	char message_for_male[9][50];
	char message_for_female[9][50];
	char message_for_group[9][50];

	// success message
	//**************************************//

	strcpy_s(message_for_male[0], ANS_RES_CORRECT_MESSAGE1_1);
	strcpy_s(message_for_male[1], ANS_RES_CORRECT_MESSAGE2_1);
	strcpy_s(message_for_male[2], ANS_RES_CORRECT_MESSAGE3_1);
	strcpy_s(message_for_male[3], ANS_RES_CORRECT_MESSAGE4_1);
	strcpy_s(message_for_male[4], ANS_RES_CORRECT_MESSAGE5_1);
	strcpy_s(message_for_male[5], ANS_RES_CORRECT_MESSAGE6_1);
	strcpy_s(message_for_male[6], ANS_RES_CORRECT_MESSAGE7_1);
	strcpy_s(message_for_male[7], ANS_RES_CORRECT_MESSAGE8_1);
	strcpy_s(message_for_male[8], ANS_RES_CORRECT_MESSAGE9_1);

	strcpy_s(message_for_female[0], ANS_RES_CORRECT_MESSAGE1_2);
	strcpy_s(message_for_female[1], ANS_RES_CORRECT_MESSAGE2_2);
	strcpy_s(message_for_female[2], ANS_RES_CORRECT_MESSAGE3_2);
	strcpy_s(message_for_female[3], ANS_RES_CORRECT_MESSAGE4_2);
	strcpy_s(message_for_female[4], ANS_RES_CORRECT_MESSAGE5_2);
	strcpy_s(message_for_female[5], ANS_RES_CORRECT_MESSAGE6_2);
	strcpy_s(message_for_female[6], ANS_RES_CORRECT_MESSAGE7_2);
	strcpy_s(message_for_female[7], ANS_RES_CORRECT_MESSAGE8_2);
	strcpy_s(message_for_female[8], ANS_RES_CORRECT_MESSAGE9_2);

	strcpy_s(message_for_group[0], ANS_RES_CORRECT_MESSAGE1_3);
	strcpy_s(message_for_group[1], ANS_RES_CORRECT_MESSAGE2_3);
	strcpy_s(message_for_group[2], ANS_RES_CORRECT_MESSAGE3_3);
	strcpy_s(message_for_group[3], ANS_RES_CORRECT_MESSAGE4_3);
	strcpy_s(message_for_group[4], ANS_RES_CORRECT_MESSAGE5_3);
	strcpy_s(message_for_group[5], ANS_RES_CORRECT_MESSAGE6_3);
	strcpy_s(message_for_group[6], ANS_RES_CORRECT_MESSAGE7_3);
	strcpy_s(message_for_group[7], ANS_RES_CORRECT_MESSAGE8_3);
	strcpy_s(message_for_group[8], ANS_RES_CORRECT_MESSAGE9_3);

	for (int i = 0; i < 9; i++)
	{
		successMessage.count = i + 1;
		successMessage.list = (ResponseMessage*)realloc(successMessage.list, successMessage.count * (sizeof(ResponseMessage)));
		successMessage.list[i].id = i + 1;
		successMessage.list[i].message_for_male = _strdup(message_for_male[i]);
		successMessage.list[i].message_for_female = _strdup(message_for_female[i]);
		successMessage.list[i].message_for_group = _strdup(message_for_group[i]);
	}

	// fail message
	//**************************************//
	char general_message[9][50];
	strcpy_s(general_message[0], ANS_RES_INCORRECT_MESSAGE1);
	strcpy_s(general_message[1], ANS_RES_INCORRECT_MESSAGE2);
	strcpy_s(general_message[2], ANS_RES_INCORRECT_MESSAGE3);
	strcpy_s(general_message[3], ANS_RES_INCORRECT_MESSAGE4);
	strcpy_s(general_message[4], ANS_RES_INCORRECT_MESSAGE5);
	strcpy_s(general_message[5], ANS_RES_INCORRECT_MESSAGE6);
	strcpy_s(general_message[6], ANS_RES_INCORRECT_MESSAGE7);
	strcpy_s(general_message[7], ANS_RES_INCORRECT_MESSAGE8);
	strcpy_s(general_message[8], ANS_RES_INCORRECT_MESSAGE9);

	

	for (int i = 0; i < 9; i++)
	{
		failMessage.count = i + 1;
		failMessage.list = (ResponseMessage*)realloc(failMessage.list, failMessage.count * (sizeof(ResponseMessage)));
		failMessage.list[i].id = i + 1;
		failMessage.list[i].message_for_male = _strdup(general_message[i]);
		failMessage.list[i].message_for_female = _strdup(general_message[i]);
		failMessage.list[i].message_for_group = _strdup(general_message[i]);
	}
}


ResponseMessage* get_response_success_message()
{
	int id_message = random(0, successMessage.count - 1);

	return &successMessage.list[id_message];
}



ResponseMessage* get_response_fail_message()
{
	int id_message = random(0, failMessage.count - 1);

	return &failMessage.list[id_message];
}

void reset_game_setings(GameSettings* gameSettings)
{
	for (int i = 0; i < gameSettings->players.count; i++)
	{
		gameSettings->players.list[i].amount = INIT_AMOUNT;
	}
	//gameSettings->answer_time = -1;
	gameSettings->players.count = 0;
	gameSettings->players.winner_index = -1;
	//gameSettings->game_type = GAME_TYPE_REGULAR;

}


char* add_char_to_string(char* str, char c)
{
	if (str == NULL)
	{
		str = (char*)malloc(2);
		str[0] = c;
		str[1] = '\0';
	}
	else
	{
		int len = strlen(str);
		int new_len = len + 2; // 1 leter + 1 null terminator

		str = (char*)realloc(str, new_len);


		str[new_len - 2] = c;
		str[new_len - 1] = '\0';
	}

	return str;
}

char** split(const char* str, char splitter, int* size)
{
	// allocate forst part
	// (char**)malloc(1 * sizeof(char*));

	int len = strlen(str);

	char** parts = NULL;

	int parts_coount = 0;

	char* current_part;

	for (int i = 0; i < len; i++)
	{
		if (str[i] != splitter)
		{
			// אם לא קיימים "חלקים", נוסיף את החלק הראשון
			if (parts_coount == 0)
			{
				parts = (char**)malloc(++parts_coount * sizeof(char*));
				parts[parts_coount - 1] = NULL;
			}

			// נעתיק את האות אל החלק
			parts[parts_coount - 1] = add_char_to_string(parts[parts_coount - 1], str[i]);

			//// בדיקה אם התו הבא הוא ספליטר
			//// או שהוא סיום המחרוזת
			//// במקרה כזה נכניס תו סיום מחרוזת
			//if ((i == (len - 1))				// אני התו האחרון של המחרוזת
			//	|| (str[i + 1] == splitter))	// התו הבא של המחרוזת הוא ספליטר
			//{
			//	//add_char_to_string(parts[parts_coount - 1], '\0');
			//	free(parts[parts_coount - 1]);
			//}
		}
		else	// if the letter is splitter
		{
			// יתכן שהמחרוזת התחילה עם טאב ועדיין לא נוצר חלק ראשון
			if (parts_coount == 0)
			{
				parts = (char**)malloc((++parts_coount) * sizeof(char*));
			}
			else
			{
				parts = (char**)realloc(parts, (++parts_coount) * sizeof(char*));
			}
			parts[parts_coount - 1] = NULL;

			//if ((i == (len - 1)))				// אני התו האחרון של המחרוזת
			//{
			//	add_char_to_string(parts[parts_coount - 1], '\0');
			//}
		}
	}


	*size = parts_coount;

	return parts;
}