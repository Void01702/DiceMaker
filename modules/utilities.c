#include "header.h"

int max_(int a, int b){ //Takes the biggest of 2 numbers
	if(a>b){
		return a;
	}
	return b;
}

int min_(int a, int b){ //Takes the smallest of 2 numbers
	if(a<b){
		return a;
	}
	return b;
}

int round_test(float x){
	int y = x;
	if(x-y>=0.5) y++;
	return y;
}

int provideIntegerChoice(int minimum, int maximum, char RangeFailMessage[], char IntegerFailMessage[]){ //Manages standard selection
	int integer = minimum - 1;
	int type;
	while(integer<minimum || integer>maximum){
		type = scanf("%d", &integer);
		if(type!=1 && type!=2){ //Test if selection is an integer
			printf("%s", IntegerFailMessage);
			while(getchar()!='\n'); //This specific line prevents a glitch where scanf will be bypassed infinitely, softlocking the program.
		}
		else if(integer<minimum || integer>maximum){ //Test if selection is in the correct range
			printf("%s", RangeFailMessage);
		}
	}
	return integer;
}

void colorPrintf(char * Text, int Color){ //Colored Text
	printf("\033[%dm%s\033[0m", Color, Text);
}

char* timeToClock(int seconds){ //Convert seconds time to clock format
	int hours = seconds/3600;
	int minutes = (seconds%3600)/60;
	int seconds_ = seconds%60;

	char* clock = malloc(sizeof(char)*8);
	sprintf(clock, "%02d:%02d:%02d", hours, minutes, seconds_);

	return clock;
}
