#include "header.h"

void init_creature(Entity* Creature, int hp, int dices, int dice_size, int rerolls, float power, char name[20]){ //Create an entity with a specific stats
  strcpy(Creature->name, "");
  //Creature->name = {'\0'};
  Creature->maxHP = hp;
  Creature->HP = Creature->maxHP;
  Creature->armor = 0;
  Creature->nbDices = dices;
  Creature->rerolls = rerolls;
  for(int i=0; i<MAX_DICES; i++){
    Creature->dice[i].size = dice_size;
    for(int j=0; j<20; j++){
      strcpy(Creature->dice[i].face[j].name, name);
      Creature->dice[i].face[j].type = ATTACK;
      Creature->dice[i].face[j].level = BASE_ATTACK * power;
    }
  }
}

void init_fight(Encounter* Fight){ //Create a fight
  Fight->nbMonsters = 1;
  char name_1[20]={};
  strcpy(name_1, "Slash");
  char name_2[20]={};
  strcpy(name_2, "Light Slash");
  int RandMob = rand()%3;
  switch(RandMob){
  case 0:;
    init_creature(&Fight->monster[0], /*30*/ 1, 1, 6, 0, 1.0, name_1);
    strcpy(Fight->monster[0].name, "Monster id1");
    break;
  case 1:;
    init_creature(&Fight->monster[0], /*40*/ 1, 1, 6, 0, 2.0/3.0, name_2);
    strcpy(Fight->monster[0].name, "Monster id2");
    for(int i=0; i<2; i++){
      strcpy(Fight->monster[0].dice[0].face[i].name, "Defend");
      Fight->monster[0].dice[0].face[i].type = DEFENSE;
      Fight->monster[0].dice[0].face[i].level = BASE_DEFENSE*2.0/3.0;
    }
    break;
  case 2:;
    Fight->nbMonsters = 2;
    init_creature(&Fight->monster[0], /*20*/ 1, 1, 4, 0, 0.5, name_2);
    strcpy(Fight->monster[0].name, "Monster id3");
    init_creature(&Fight->monster[1], /*15*/ 1, 1, 4, 0, 3.0/4.0, name_1);
    strcpy(Fight->monster[1].name, "Monster id4");
    break;
  }
  for(int i=0; i<3; i++){ //generate reward
    strcpy(Fight->reward[i].name, "Heavy Slash");
    Fight->reward[i].type = ATTACK;
    Fight->reward[i].level = BASE_ATTACK*2;
  }
}

void init_level(Encounter Level[11], int seed){ //Create a level
  srand(seed);
  for(int i=0; i<11; i++){
    Level[i].type = FIGHT;
    Level[i].nbMonsters = -1;
  }
  Level[5].type = CHEST;
  Level[rand()%4 + 1].type = EVENT; //Choose events
  Level[rand()%5 + 6].type = EVENT;
  for(int i=0; i<11; i++){
    if(Level[i].type == FIGHT){ //Generate fights
      init_fight(&Level[i]);
    }
  }
}

void init_player(Entity* Player){ //Create a player
  char name[20]={};
  strcpy(name, "Slash");
  init_creature(Player, 50, 2, 4, 2, 1, name);
  strcpy(Player->name, "Player");
  //Player->name = {'P', 'l', 'a', 'y', 'e', 'r', '\0'};
  for(int i=0; i<MAX_DICES; i++){ //Adds defence items
    for(int j=0; j<2; j++){
      strcpy(Player->dice[i].face[j].name, "Defend");
      Player->dice[i].face[j].type = DEFENSE;
      Player->dice[i].face[j].level = BASE_DEFENSE;
    }
  }
  //Adds strong items
  Player->dice[0].size = 6;
  strcpy(Player->dice[0].face[2].name, "Heavy Slash");
  Player->dice[0].face[2].level = BASE_ATTACK*2;
  Player->dice[1].size = 6;
  strcpy(Player->dice[1].face[2].name, "Heavy Slash");
  Player->dice[1].face[2].level = BASE_ATTACK*2;
}
