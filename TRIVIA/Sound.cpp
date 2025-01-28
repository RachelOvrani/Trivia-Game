#include <stdio.h>
#include <windows.h> 
#include <iostream>
#pragma comment(lib, "winmm.lib")




void keboard_hit_beep()
{
	Beep(800, 70);
}


void play_timer_beep(int sound)
{
	Beep(sound, 100);
}
void play_correct_answer(int index)
{
	char sound_file[100];
	sprintf_s(sound_file, "sounds\\correct%d.wav", index);
	PlaySound(sound_file, NULL, SND_FILENAME); //SND_FILENAME or SND_LOOP
}
void play_wrong_answer(int index)
{
	char sound_file[100];
	sprintf_s(sound_file, "sounds\\incorrect%d.wav", index);
	PlaySound(sound_file, NULL, SND_FILENAME); //SND_FILENAME or SND_LOOP

}
void play_time_over()
{
	PlaySound("sounds\\time_over.wav", NULL, SND_FILENAME); //SND_FILENAME or SND_LOOP
}
void play_applause_for_winners()
{
	PlaySound("sounds\\applause.wav", NULL, SND_FILENAME); //SND_FILENAME or SND_LOOP
}

void all_rights_reserved()
{
	PlaySound("sounds\\all_rights_reserved.wav", NULL, SND_FILENAME); //SND_FILENAME or SND_LOOP
}

void app_startup()
{
	PlaySound("sounds\\app_startup.wav", NULL, SND_FILENAME); //SND_FILENAME or SND_LOOP
}
