Christopher Tomes
Cal Poly Pomona 
Fall 2022
CS2600 Systems Programming

TicTacToe Game

This project is a tic tac toe game in which an Esp32 and a PC can play a game at any time wirelessly over the internet utilizing the mqtt protocal.
The Daemon, running on the parent system will start a game whether a player is present or not when prompted by the ESP32. In the case no one
is present the system will run a script to play the game randomly. 

Special Note; I developed a small AI that will play the game intelegently in C. The contents of the agent are in game.c towards the bottom. 
To test it out run ./program ai 

Requirements: 
This was designed and developped on Ubuntu and requires a mqtt installation

The following command can be used to install the c libraries necessary.
sudo apt install gcc mosquitto mosquitto-clients libmosquitto

test.mosquitto.org was used as a broker. 

The esp32 requires the instalation of the  ArduinoMqttClient by Arduino.
The board utilizes the led matrix project and adds the numpad to the project, so should be easy to emulate. 
