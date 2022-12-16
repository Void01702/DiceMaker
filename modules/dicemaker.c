#include "header.h"

int showMainMenu(){ //Manages the main menu of the game.
	//Regenerates the seed everytime the menu is loaded.
	int seed = rand();
	srand(seed);
	//Selection
	printf(" [1] Play\n [2] Credits\n [3] Exit\n\n\n");
	int selection = provideIntegerChoice(1, 3, "Please enter a value between 1 and 5\n", "Please enter a number\n");
	switch (selection){
		case 1:;
			char selection_2[100]={};
			do{
				while(getchar()!='\n'); //flush
				printf(" [1] Normal\n [2] Hard (WIP)\n [3] Custom (WIP)\n\n\n");
				scanf("%s", selection_2);
				if(selection_2[0]=='s' && selection_2[1]=='e' && selection_2[2]=='e' && selection_2[3]=='d'){
					printf("Please enter the seed\n");
					scanf("%d", &seed);
					srand(seed);
					printf("Seed : %d\n\n", seed);
				}
			}while(selection_2[0]<'1' || selection_2[0]>'4');
			int selection_3 = atoi(&selection_2[0]);
			float difficulty; //WIP unused for now
			float difficulty_scaling;
			int gamemode;
			switch (selection_3){
				case 1:;
					difficulty = 1;
					difficulty_scaling = 1.5;
					gamemode = 0;
					break;
				case 2:;
				difficulty = 2;
				difficulty_scaling = 2;
				gamemode = 1;
					break;
				case 3:;
					//WIP
					break;
				default:;
					colorPrintf("Error : This shouldn't happen. Going back to the menu.\n\n",RED);
					return showMainMenu();
					break;
			}
			Entity Player;
			init_player(&Player);
			//dice_print(Player); //test correct dice gen
			Encounter Level[11];
			init_level(Level, seed);
			//level_print(Level); //test correct level gen
			level_play(&Player, Level);
			//WIP
			return showMainMenu();
			break;
		case 2:;
			printf("\n\nI haven't done that yet, i'll do it later\n");
			//WIP
			return showMainMenu();
			break;
		case 3:;
			colorPrintf("\nBye bye !\n",GREEN);
			return 0;
			break;
		default:;
			colorPrintf("Error : This shouldn't happen. Going back to the menu.\n\n",RED);
			return showMainMenu();
			break;
	}
}

int main(){ //Initialize the game
	//Generates a random seed
	srand(time(NULL));
	for(int i=0; i<20; i++){//make rand more chaotic
		srand(rand());
	}
	//Windows compatibility
	char command[15];
	strcpy(command, "chcp 65001");
	system(command);
	//WIP
	printf("\n\nI haven't done the title yet, i'll do it later\n");

	return showMainMenu();
}

//todo: events; random item drop; fight
