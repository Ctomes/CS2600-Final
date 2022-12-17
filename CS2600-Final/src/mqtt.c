#include <stdio.h>
#include <stdlib.h>

#include <mosquitto.h>
#include "../header/game.h"
#include "../header/globals.h"
extern int turn;
extern int board[9];
extern int pos_taken;


/*
Initialize the connection to the broker
*/
void on_connect(struct mosquitto *mosq, void *obj, int rc) 
{
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "T3/EspOut", 0);
}

/*
The PC program is responsible for the Entire state of the game. 
The program will verify all forms of input. If it is not the esp32 turn it will ignore.
If the input passed is invalid or does not make sense ie spot not available the program will ignore it
Upon succesful move registration the program will publish this result to the broker and the ESP32 will display itself.
*/
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) 
{
	if(turn %2 == 0)
	{
		printf("Other player attempted to played out of turn!");
		return;
	}
	if( ((char *)(msg->payload))[0] == '*')
	{
		printf("Other player requested game reset.\n");
		clear_board();
		mosquitto_publish(mosq, NULL, "T3/EspIn", 6, msg->payload, 0, false);
	}
	int intChar = 0;
	intChar= atoi((char *)msg->payload);
	if(intChar > 0)
	{
		if(valid_move(intChar-1))
		{
			board[intChar-1] = -2;
			printf("Other Player chose: %s\n", (char *)msg->payload);
			mosquitto_publish(mosq, NULL, "T3/EspIn", 6, msg->payload, 0, false);
			turn+=1;
			pos_taken+=1;
            print_board();
			printf("Input: %d\n", intChar);
		}else
		{
			printf("Other Player: invalid position!\n");
		}
	}else
	{
		//printf("%s: is not  a num.\n",(char *)msg->payload);
	}

	
}