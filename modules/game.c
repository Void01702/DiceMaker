#include "header.h"

void item_free(Item Face){
  if(Face.child!=NULL){
    item_free(Face.child[1]);
    item_free(Face.child[2]);
    free(Face.child);
  }
}

void item_print(Item Face, int NbFaces, bool UseCaps){ //print an item
  int level = round_test(Face.level*NbFaces);
  switch(Face.type){
    case COMBINED_TARGET:;
      item_print(Face.child[0], NbFaces, UseCaps);
      printf(", and ");
      item_print(Face.child[1], NbFaces, false);
      break;
    case COMBINED_NO_TARGET:;
      item_print(Face.child[0], NbFaces, UseCaps);
      printf(", and ");
      item_print(Face.child[1], NbFaces, false);
      break;
    case ATTACK:;
      if(UseCaps) printf("Deal %d dammage", level);
      else printf("deal %d dammage", level);
      break;
    case ATTACK_ALL:;
      if(UseCaps) printf("Deal %d dammage to all ennemies", level);
      else printf("deal %d dammage to all ennemies", level);
      break;
    case DEFENSE:;
      if(UseCaps) printf("Gain %d armor", level);
      else printf("gain %d armor", level);
      break;
    default:;
      if(UseCaps) printf("Something?");
      else printf("do something?");
      break;
  }
}

void dice_print(Entity Player){ //print a dice
  for(int i=0; i<Player.nbDices; i++){
    printf("\nDice %d:\n", i+1);
    for(int j=0; j<Player.dice[i].size; j++){
      printf("%d: ", j+1);
      item_print(Player.dice[i].face[j], Player.dice[i].size, true);
      printf("\n");
    }
  }
}

void level_print(Encounter Level[11]){ //print a full level
  for(int i=0; i<11; i++){
    switch(Level[i].type){
      case FIGHT:;
        printf("Fight\nNb Monsters: %d\n", Level[i].nbMonsters);
        for(int j=0; j<Level[i].nbMonsters; j++){
          printf("Monster %d:\nHP: %d\n", j+1, Level[i].monster[j].HP);
          dice_print(Level[i].monster[j]);
        }
        printf("\n\n");
        break;
      case CHEST:;
        printf("Chest\n\n\n");
        break;
      case EVENT:;
        printf("Event\n\n\n");
        break;
      default:;
        printf("Error: room unknown\n\n\n");
        break;
    }
  }
}

void entity_print(Entity* Creature){
  printf("%s\nHP: %d/%d\n", Creature->name, Creature->HP, Creature->maxHP);
  if(Creature->armor > 0) printf("Armor: %d\n", Creature->armor);
}

void damage(Entity* Target, int power){
  if(Target->armor <= 0) Target->HP = Target->HP - power;
  else if(Target->armor > power) Target->armor = Target->armor - power;
  else{
    power = power - Target->armor;
    Target->armor = 0;
    Target->HP = Target->HP - power;
  }
}

void item_use(Item Face, int DiceLevel, Entity* Allies, int NbAllies, int NbUser, Entity* Enemies, int NbEnemies, int NbTarget){ //use an item
  int level = round_test(Face.level*DiceLevel);
  switch(Face.type){
    case COMBINED_TARGET:;
      item_use(Face.child[0], DiceLevel, Allies, NbAllies, NbUser, Enemies, NbEnemies, NbTarget);
      item_use(Face.child[1], DiceLevel, Allies, NbAllies, NbUser, Enemies, NbEnemies, NbTarget);
      break;
    case COMBINED_NO_TARGET:;
      item_use(Face.child[0], DiceLevel, Allies, NbAllies, NbUser, Enemies, NbEnemies, NbTarget);
      item_use(Face.child[1], DiceLevel, Allies, NbAllies, NbUser, Enemies, NbEnemies, NbTarget);
      break;
    case ATTACK:;
      damage(&Enemies[NbTarget], level);
      break;
    case ATTACK_ALL:;
      for(int i=0; i<NbEnemies; i++){
        damage(&Enemies[i], level);
      }
      break;
    case DEFENSE:;
      Allies[NbUser].armor = Allies[NbUser].armor + level;
      break;
    default:
      break;
  }
}

int fight(Entity* Player, Encounter* Fight, int turn){ //start a fight
  printf("\033[%dmStart of turn %d\033[%dm\n\n", RED, turn+1, RESET_COLOR);
  sleep(1);

  bool* is_used = malloc(sizeof(bool) * Player->nbDices); //no dice is locked
  for(int i=0; i<Player->nbDices; i++) is_used[i] = false;

  Item* monster_roll = malloc(sizeof(Item) * Fight->nbMonsters); //monster rolls
  for(int i=0; i<Fight->nbMonsters; i++) monster_roll[i] = Fight->monster[i].dice[0].face[rand()%Fight->monster[i].dice[0].size];

  Item* player_roll = malloc(sizeof(Item) * Player->nbDices); //player rolls
  for(int i=0; i<Player->nbDices; i++) player_roll[i] = Player->dice[i].face[rand()%Player->dice[i].size];

  Player->armor = 0; //Player's turn

  //print UI
  entity_print(Player); //print player UI
  printf("\n");
  for(int i=0; i<Fight->nbMonsters; i++){ //print monster UI
    if(Fight->monster[i].HP>0){
      entity_print(&Fight->monster[i]);
      printf("Roll: ");
      item_print(monster_roll[i], Fight->monster[i].dice[0].size, true); //print monster rolls
      printf("\n\n");
    }
  }
  printf("\n");

  for(int i=0; i<Player->rerolls; i++){ //doin rerolls
    int x=false;
    for(int j=0; j<Player->nbDices; j++) if(!is_used[j]) x=true;
    if(x){
      for(int j=0; j<Player->nbDices; j++){ //print player rolls
        printf("Dice %d: ", j+1);
        item_print(player_roll[j], Player->dice[j].size, true);
        printf("\n");
      }
      printf("\n");
      printf("   Choose the dices you don't want to reroll, then select continue.\n\n"); //select which to lock
    }
    while(x){
      for(int j=0; j<Player->nbDices; j++) if(!is_used[j]) printf(" [%d] Lock dice %d\n", j+1, j+1);
      printf(" [%d] Continue\n\n", Player->nbDices+1);
      int selection = provideIntegerChoice(1, Player->nbDices+1, "Please enter a valid choice\n", "Please enter a number\n");
      if(selection>0 && selection < Player->nbDices+1){
        is_used[selection-1]=true;
        x=false;
        for(int j=0; j<Player->nbDices; j++) if(!is_used[j]) x=true;
      }
      else if(selection == Player->nbDices+1) x=false;
      else{
        printf("Error: invalid input in dice locking\n");
        return -1;
      }
    }
    for(int j=0; j<Player->nbDices; j++) if(!is_used[j]) player_roll[j] = Player->dice[j].face[rand()%Player->dice[j].size]; //if not locked, reroll
  }

  printf("\033[%dm", RED);
  for(int i=0; i<Player->nbDices; i++){ //print final player rolls
    printf("Dice %d: ", i+1);
    item_print(player_roll[i], Player->dice[i].size, true);
    printf("\n");
  }
  printf("\n\033[%dm", RESET_COLOR);

  for(int i=0; i<Player->nbDices; i++) is_used[i] = false; //reusing is_used for using dices
  int x=true;
  printf("   Choose what dices to use\n\n");
  while(x){  //until end turn
    for(int i=0; i<Player->nbDices; i++) if(!is_used[i]) printf(" [%d] Use dice %d\n", i+1, i+1);
    printf(" [%d] End turn\n\n", Player->nbDices+1);
    int selection = provideIntegerChoice(1, Player->nbDices+1, "Please enter a valid choice\n", "Please enter a number\n"); //choose what dice to use
    if(selection>0 && selection < Player->nbDices+1 && !is_used[selection-1]){
      if(player_roll[selection-1].type == ATTACK || player_roll[selection-1].type == COMBINED_TARGET){ //if it needs a target select
        for(int j=0; j<Fight->nbMonsters; j++) if(Fight->monster[j].HP>0) printf(" [%d] select monster %d\n", j+1, j+1);
        printf(" [%d] Cancel\n\n", Fight->nbMonsters+1);
        int selection_2 = provideIntegerChoice(1, Fight->nbMonsters+1, "Please enter a valid choice\n", "Please enter a number\n");//choose selected target
        if(selection_2>0 && (selection_2<Fight->nbMonsters+1) && Fight->monster[selection_2-1].HP>0){
          item_use(player_roll[selection-1], Player->dice[selection-1].size, Player, 1, 0, Fight->monster, Fight->nbMonsters, selection_2-1);
          is_used[selection-1]=true;
          x=false;
          for(int j=0; j<Player->nbDices; j++) if(!is_used[j]) x=true;
        }
        else if(selection_2>0 && (selection_2<Fight->nbMonsters+1) && Fight->monster[selection_2-1].HP<0) printf("This monster is already dead\n");
      }
      else{
        item_use(player_roll[selection-1], Player->dice[selection-1].size, Player, 1, 0, Fight->monster, Fight->nbMonsters, 0);
        is_used[selection-1]=true;
        x=false;
        for(int j=0; j<Player->nbDices; j++) if(!is_used[j]) x=true;
      }
    }
    else if(selection == Player->nbDices+1) x=false;
    else if(is_used[selection-1]){
      printf("this dice is alread used\n");
    }
    else{
      printf("Error: invalid input during player's turn\n");
      return -1;
    }
  }

  for(int i = 0; i<Fight->nbMonsters; i++){ //monster turn
    Fight->monster[i].armor = 0;
  }
  for(int i = 0; i<Fight->nbMonsters; i++) if(Fight->monster[i].HP>0) item_use(monster_roll[i], Fight->monster[i].dice[0].size, Fight->monster, Fight->nbMonsters, i, Player, 1, 0);

  free(is_used);
  free(monster_roll);
  free(player_roll);

  x=false;
  for(int i = 0; i<Fight->nbMonsters; i++) if(Fight->monster[i].HP > 0){ //test if fight ended
    for(int j=0; j<3; j++) item_free(Fight->reward[j]);
    return fight(Player, Fight, turn+1);
  }

  if(Player->HP<1){
    printf("\033[%dmGAME OVER\033[%dm\n\n", RED, RESET_COLOR);
    for(int j=0; j<3; j++) item_free(Fight->reward[j]);
    return 1;
  }

  printf("\033[%dmVictory!\033[%dm\n\n", GREEN, RESET_COLOR);
  sleep(1);
  x=true;
  while(x){
    printf("   Choose which face to add to your dice\n");
    for(int i=0; i<3; i++){
      printf("\n [%d] ", i+1);
      item_print(Fight->reward[i], 6, true);
    }
    printf("\n [4] Check your dices\n [5] Take none\n\n");
    int selection = provideIntegerChoice(1, 5, "Please enter a valid choice\n", "Please enter a number\n"); //choose what dice to use
    if(selection<4){
      dice_print(*Player);
      printf("\n");
      sleep(1);
      printf("Choose on what dice to add this face\n\n");
      for(int i=0; i<Player->nbDices; i++) printf(" [%d] Dice %d\n", i+1, i+1);
      printf(" [%d] Cancel\n\n", Player->nbDices+1);
      int selection_2 = provideIntegerChoice(1, Player->nbDices+1, "Please enter a valid choice\n", "Please enter a number\n");
      if(selection_2<=Player->nbDices){
        printf("\n\nChoose what face to replace\n");
        for(int i=0; i<Player->dice[selection_2-1].size; i++){
          printf("\n [%d] ", i+1);
          item_print(Player->dice[selection_2-1].face[i], Player->dice[selection_2-1].size, true);
        }
        printf("\n [%d] Cancel\n\n", Player->dice[selection_2-1].size+1);
        int selection_3 = provideIntegerChoice(1, Player->dice[selection_2-1].size+1, "Please enter a valid choice\n", "Please enter a number\n");
        if(selection_3<=Player->dice[selection_2-1].size){
          item_free(Player->dice[selection_2-1].face[selection_3-1]);
          strcpy(Player->dice[selection_2-1].face[selection_3-1].name, Fight->reward[selection-1].name);
          Player->dice[selection_2-1].face[selection_3-1].type = Fight->reward[selection-1].type;
          Player->dice[selection_2-1].face[selection_3-1].level = Fight->reward[selection-1].level;
          Player->dice[selection_2-1].face[selection_3-1].child = Fight->reward[selection-1].child;
          Fight->reward[selection-1].child = NULL;

          sleep(1);
          dice_print(*Player);
          printf("\n");
        }
        x=false;
      }
    }
    else if(selection==4) dice_print(*Player);
    else x=false;
  }
  for(int j=0; j<3; j++) item_free(Fight->reward[j]);
  return 0;
}

int level_play(Entity* Player, Encounter Level[11]){ //start a level
  for(int i=0; i<11; i++){
    switch(Level[i].type){
      case FIGHT:;
        Player->armor = 0;
        fight(Player, &Level[i], 0);
        break;
      case CHEST:;
        printf("this is a chest\n\n");
        break;
      case EVENT:;
        printf("this is an event\n\n");
        break;
      case SHOP:;
        printf("this is a shop\n\n");
        break;
      default:;
        printf("Error: unknown level\n\n\n");
        break;
    }
  }
  return 0;
}
