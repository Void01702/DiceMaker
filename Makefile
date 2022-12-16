CC=gcc
EXECUTABLE_NAME=DiceMaker

# Optional (use root if empty)
BUILD_FOLDER=

# Release
release : all clean
run_release : release run

# Compilation
dicemaker.o :
	@$(CC) -c modules/dicemaker.c -o $(BUILD_FOLDER)dicemaker.o

utilities.o :
	@$(CC) -c modules/utilities.c -o $(BUILD_FOLDER)utilities.o

init.o :
	@$(CC) -c modules/init.c -o $(BUILD_FOLDER)init.o

game.o :
	@$(CC) -c modules/game.c -o $(BUILD_FOLDER)game.o

# Everything at once
all : dicemaker.o utilities.o init.o game.o
	@$(CC) $(BUILD_FOLDER)dicemaker.o $(BUILD_FOLDER)utilities.o $(BUILD_FOLDER)init.o $(BUILD_FOLDER)game.o -o $(EXECUTABLE_NAME)

# Cleaner
clean :
	@rm -f $(BUILD_FOLDER)dicemaker.o $(BUILD_FOLDER)utilities.o $(BUILD_FOLDER)init.o $(BUILD_FOLDER)game.o

# Runners
run : all
	@echo "Running $(EXECUTABLE_NAME)"
	@./$(EXECUTABLE_NAME)
