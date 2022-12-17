#!/bin/bash

STRING="Beginning Daemon"
userRequest="n"
echo $STRING

#mosquitto_sub -h "test.mosquitto.org" -t "T3/EspOut"
# Send MQTT values to a progress bar

#The Daemon on initialization will subscribe to a topic
#On request the ESP32 will post to the topic requesting a match.
#At which point this script will either call the scripted version or user version of game. 


while :; do
  msg=$(mosquitto_sub -h "test.mosquitto.org" -C 1 -t "T3/EspOut") 
  	if (($msg == "b"))#b = begin game. 
		then
		echo "Someone wants to play!"
		echo "Press any key to play them!"
		read -t 10 userRequest
		if [ -n "${userRequest}" ]; then
			echo "Beginning User Game"
			sleep 1
			./bin/program user
		else
			echo "Beginning AI game in your steed"
			sleep 1
			./ttt_alt.sh
		fi
	
  	fi
done

echo "Done"

