#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include "../header/mqtt.h"
#include "../header/game.h"
#include "../header/globals.h"

int board[] = {0,0,0,0,0,0,0,0,0};
char valid_chars[] = {'1','2','3','4','5','6','7','8','9'};
int pos_taken = 0;
char user[] = "user";
char auto_user[] = "ai";
int turn = 0;

//If you want to test out ai feel free to pass it as an argument.

int main(int argc, char *argv[]) {
	int rc, id=12;
    if( argc == 2 ) 
	{
	  //Do nothing valid input. 
	}
   else if( argc > 2 ) {
      printf("Too many arguments supplied.\n");
	  return -1;
   }
   else {
      printf("One argument expected.\n");
      return -1;
   }
	mosquitto_lib_init();

	struct mosquitto *mosq;

	mosq = mosquitto_new("PlayerOne", true, &id);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);
	
	rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 10);
	if(rc) {
		printf("Could not connect to Broker with return code %d\n", rc);
		return -1;
	}

	mosquitto_loop_start(mosq);

    if(!strcmp(auto_user, argv[1]))
    {
        printf("Auto match beginning!\n");
        auto_play(mosq);
    }else if(!strcmp(user, argv[1]))
    {
        printf("User match beginning!\n");
        user_play(mosq);
    }else
    {
        printf("Invalid argument!\n");
    }
	mosquitto_loop_stop(mosq, true);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}