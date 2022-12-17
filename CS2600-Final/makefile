CC=gcc 
all: doc src header bin lib obj program

#entry point for compilation. 
#currently is basic since there are no dependencies.
program:  
	$(CC) -o bin/program src/main.c src/mqtt.c src/game.c -lmosquitto


#generates the directories:
doc:
	mkdir -p $@
src:
	mkdir -p $@
header:
	mkdir -p $@
bin:
	mkdir -p $@
lib:
	mkdir -p $@
obj:
	mkdir -p $@