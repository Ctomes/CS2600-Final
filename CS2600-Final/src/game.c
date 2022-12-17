#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>
#include <time.h>
#include <string.h>
#include "../header/globals.h"

void print_board()// simple print function to show board
{
    printf("Tic Tac Toe:\n");
    for(int i= 0; i < 9; i+=1)
    {
        if(board[i] == 0)
        {
            printf("[ ]");
        }else if(board[i] == -2)
        {
            printf("[x]");
        }else{
            printf("[o]");
        }
        if((i+1) % 3 == 0)
        {
            printf("\n");
        }
    }
}
int valid_input(char in) //verify the keyboard input is valid
{
	for(int i = 0; i < SIZE_OF_INPUT; i++)
	{
		if(valid_chars[i] == in)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int valid_move(int pos)//verify the board square is available 
{//0..8
	if(board[pos] != 0)
	{
		return FALSE;
	}
	return TRUE;
}
void clear_board() //wipe the board for new/quit game
{
	printf("Clearing Board:\n");
	for(int i = 0; i < 9; i++)
	{
		board[i] = 0;
	}
	turn = 0;
	pos_taken = 0;
}


/*
Gamestate detection: near wins, wins, losses, etc
Will return a valid move that makes sense
*/
int win(int val)
{
	int win_conditions[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,7}};
	for(int i =0; i < 9; i++)
	{
		for(int k = 0; k < 8; k+=1)
		{
			if(board[win_conditions[k][0]] + board[win_conditions[k][1]] + board[win_conditions[k][2]] == val)
			{//if pos 0 + 1 + 2 = 3 or -6 winner is found, 2 or -4 next move can result in win, -1 or 2 is potential next move, 0 is a clear row/col
				if(val == -6 || val == 3)//Detecting winner case. dnt return a free spot since it cnt be found.
                {
                    return TRUE;
                }
                
                int j = 2;
				while(j >= 0)
				{
					if (board[win_conditions[k][j]] == 0)
					{
						return win_conditions[k][j];
					}else
					{
						j-=1;
					}
				}
			}
		}
	}
	return -1;
}


int auto_pick_move()//This will pick the best move by first checking if it can win, prevent a loss, or choose an optimal move to set up a win.
{
	int valid = 0;
	valid = win(2);
	if(valid != -1)
	{
		//printf("Attempting to win the Game %d\n", valid);
		return valid;
	}
	valid = win(-4);
	if(valid != -1){
		//printf("Do not let other player win the Game %d\n", valid);
		return valid;
	}
	valid = win(1);
	if(valid != -1){
		//printf("Calculated best move %d\n",valid);
		return valid;
	}
	return -1;
}
int winner_found()//Check if a game has been won
{
    if(win(-6) == TRUE){
        return TRUE;
    }
    if(win(3) == TRUE){
        return TRUE;
    }
    return FALSE;
}
/*
This is the driver for the T3 AI, The agent will always select the first move at random to induce a bit of variance.
Afterwards the agent will attempt to play optimally. Some games are winable by the Esp32, others are not. 
*/
void auto_play(struct mosquitto *mosq)
{
	char star = '*';
	int r = rand()%9;//0..8
	char c = r + '1';

    mosquitto_publish(mosq, NULL, "T3/EspIn", 6, &star, 0, false);
    //reset esp display.
	while(pos_taken < 9 && !winner_found())
	{
		if((turn%2 == 0))
		{
			int temp_r = auto_pick_move();
			if(temp_r != -1)
			{
				r = temp_r;
				c = temp_r + '1';
			}
			if(valid_move(r))
			{
                
				mosquitto_publish(mosq, NULL, "T3/EspIn", 6, &c, 0, false);
				board[(r)] = 1;
				turn += 1;
				pos_taken += 1;

			}
		}
		r = rand()%9;
		c = r + '1';
	}
	sleep(1);
    printf("End of game!\n");
}

/*
This is the driver for the User version of the game, the script will use this function as will to play the game.
Input is checked when it is the players turn and the game can be quit whenever 'q' is pressed. '*' additionally will
reset the board to start a new game against the Esp32
*/
void user_play(struct mosquitto *mosq)
{
    char star = '*';
    printf("Press 'q' to quit game..\n");
    printf("Press '*' to reset game..\n");
    mosquitto_publish(mosq, NULL, "T3/EspIn", 6, &star, 0, false);
    print_board();
	printf("Input:\n");
		while(!winner_found())
		{	
		if(turn%2 == 0)
		{
			char in = getchar();
			if(in == '\n'){
				//consume newline
				continue;
			}
			if(in == 'q'){//quit game
				printf("Quitting Game:\n");
				mosquitto_publish(mosq, NULL, "T3/EspIn", 6, &star, 0, false);
				break;
			}
			if(in == '*'){//reset states
				clear_board();
				turn+=0;
				mosquitto_publish(mosq, NULL, "T3/EspIn", 6, &star, 0, false);
			}
			if(!valid_input(in))//1..9
			{
				printf("Not an available input -->%d<--\n", in);
				continue;
			}
			if(!valid_move(in-49))//spot already taken
			{
				printf("Not an available move -->%c<--\n", in);
				continue;
			}
			mosquitto_publish(mosq, NULL, "T3/EspIn", 6, &in, 0, false);
			board[(in-49)] = 1;
			turn+=1;
			pos_taken+=1;
            print_board();
		}else{
			//waiting on input from esp
		}
	}
    printf("End of Game!\n");
}