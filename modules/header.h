#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<unistd.h>

#define RELEASE_VERSION "alpha 1.0"

//#define RESET_COLOR "\033[0m"
#define RESET_COLOR 0
#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define PURPLE 35
#define CYAN 36
#define WHITE 37

//dice value basic
#define BASE_ATTACK 2.0
#define BASE_DEFENSE 1.0

typedef enum {
	COMBINED_TARGET = 1,
	COMBINED_NO_TARGET = 2,
  ATTACK = 3,
	ATTACK_ALL = 4,
	DEFENSE = 5,
} ItemEffect;

typedef struct ItemS{
  char name[20];
  ItemEffect type;
  float level;
  struct ItemS* child;
} Item;

typedef struct{
	int size;
	Item face[20];
} Dice;

#define MAX_DICES 5

#define ARMOR 0
#define MAX_TEMP_STATUS 1

typedef struct{
	char name[20];
	int HP;
	int maxHP;
	int status[1];
	int nbDices;
	Dice dice[MAX_DICES];
	int rerolls;
} Entity;

typedef enum {
	FIGHT = 0,
	CHEST = 1,
  EVENT = 2,
	SHOP = 3,
	REST = 4,
} EncounterType;

typedef struct{
	EncounterType type;
	int nbMonsters;
	Entity monster[4];
	Item reward[3];
} Encounter;

int round_test(float x);
int provideIntegerChoice(int minimum, int maximum, char RangeFailMessage[], char IntegerFailMessage[]);
void colorPrintf(char * Text, int Color);
char* timeToClock(int seconds);
int min(int a, int b);
void init_player(Entity* Player);
void dice_print(Entity Player);
void init_level(Encounter Level[11], int seed);
void level_print(Encounter Level[11]);
int fight(Entity* Player, Encounter* Fight, int turn);
int level_play(Entity* Player, Encounter Level[11]);

void init_fight(Encounter* Fight);
