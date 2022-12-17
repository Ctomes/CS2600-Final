/**
Christopher Tomes CS2600
Final Project.
Play Tic Tac To over Mqtt

Phase 1. Display hashtag to LED matrix as constant. 
Completed. 
Currently Loops and Displays the Hash

Phase 2. Display X's and O's in cells. 
  "since the matrix is single colored and each cell is a 2 by 2. I can not display a tradition x and o so
  I will use slashes. \ = X, / = O" 2 columns at a time will be logical AND with the slashes to update the matrix.
  
Phase 3. Add input from numpad and use it to call functions. "keep it functional to reuse for networking"

Phase 4. Build publisher connection for outgoing messages.
Phase 5. Build listener connection for input and display. 

**/
#include <Keypad.h>
#include <WiFi.h>
#include <ArduinoMqttClient.h>

void displayBoard();

//replace with your own wifi credentials
const char *ssid_Router     =  "Phone";//Enter the router name
const char *password_Router =  "123123123";//Enter the router password



#define     REMOTE_IP          "********"  //input the remote server which is you want to connect
#define     REMOTE_PORT         8888       //input the remote port which is the remote provide


WiFiClient client;
MqttClient mqttClient(client);

const char broker[] = "test.mosquitto.org"; 
int        port     = 1883;
const char espOut[]  = "T3/EspOut";
const char espIn[]  = "T3/EspIn";


//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;


// define the symbols on the buttons of the keypad
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//Used Pins:
int latchPin = 2;          // Pin connected to ST_CP of 74HC595（Pin12）
int clockPin = 4;          // Pin connected to SH_CP of 74HC595（Pin11）
int dataPin = 15;          // Pin connected to DS of 74HC595（Pin14）

byte rowPins[4] = {14, 27, 26, 25}; // connect to the row pinouts of the keypad
byte colPins[4] = {13, 21, 22, 23};   // connect to the column pinouts of the keypad

// initialize an instance of class NewKeypad
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

//data structure to display the T3 Board.
int hashtag[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int board[] = {0,0,0,
               0,0,0,
               0,0,0};
int turn = 0;
void setup() {
  Serial.begin(115200);
  while(!Serial) {
    ; // wait for port to connect.
  }
  delay(10);

  Serial.print("Attempting to connect to WPA SSID: ");
  //Serial.println(ssid);

  WiFi.begin(ssid_Router, password_Router);
  Serial.print("\nWaiting for WiFi... ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();


  // Initialize Pins:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Serial.begin(115200); // Initialize the serial port and set the baud rate to 115200
  Serial.println("ESP32 is ready!");  // Print the string "UNO is ready!"

}

//functionality to flip led matrix.
void grabInput(char keyPressed)
{
    switch(keyPressed) {
    case '1':
      if(turn % 2 == 0){
        hashtag[0] = hashtag[0] | 0x80;
        hashtag[1] = hashtag[1] | 0x40;
      }else{
        hashtag[0] = hashtag[0] | 0xC0;
        hashtag[1] = hashtag[1] | 0xC0;
      }
      turn+=1;
      break;
    case '2':
      if(turn % 2 == 0){
        hashtag[3] = hashtag[3] | 0x80;
        hashtag[4] = hashtag[4] | 0x40;
      }else{
        hashtag[3] = hashtag[3] | 0xC0;
        hashtag[4] = hashtag[4] | 0xC0;       
      }
      turn+=1;
      break;
    case '3':
      if(turn % 2 == 0){
        hashtag[6] = hashtag[6] | 0x80;
        hashtag[7] = hashtag[7] | 0x40;
      }else{
        hashtag[6] = hashtag[6] | 0xC0;
        hashtag[7] = hashtag[7] | 0xC0; 
      }
      turn+=1;
      break;
    case '4':
      if(turn % 2 == 0){
        hashtag[0] = hashtag[0] | 0x10;
        hashtag[1] = hashtag[1] | 0x08;
      }else{
        hashtag[0] = hashtag[0] | 0x18;
        hashtag[1] = hashtag[1] | 0x18;
      }
      turn+=1;
      break;
    case '5':
      if(turn % 2 == 0){
        hashtag[3] = hashtag[3] | 0x10;
        hashtag[4] = hashtag[4] | 0x08;
      }else{
        hashtag[3] = hashtag[3] | 0x18;
        hashtag[4] = hashtag[4] | 0x18;
      }
      turn+=1;
      break;
    case '6':
      if(turn % 2 == 0){
        hashtag[6] = hashtag[6] | 0x10;
        hashtag[7] = hashtag[7] | 0x08;
      }else{
        hashtag[6] = hashtag[6] | 0x18;
        hashtag[7] = hashtag[7] | 0x18;
      }
      turn+=1;
      break;
    case '7':
      if(turn % 2 == 0){
        hashtag[0] = hashtag[0] | 0x02;
        hashtag[1] = hashtag[1] | 0x01;
      }else{
        hashtag[0] = hashtag[0] | 0x03;
        hashtag[1] = hashtag[1] | 0x03;
      }
      turn+=1;
      break;
    case '8':
      if(turn % 2 == 0){
        hashtag[3] = hashtag[3] | 0x02;
        hashtag[4] = hashtag[4] | 0x01;
      }else{
        hashtag[3] = hashtag[3] | 0x03;
        hashtag[4] = hashtag[4] | 0x03;
      }
      turn+=1;
      break;
    case '9':
        if(turn % 2 == 0){
          hashtag[6] = hashtag[6] | 0x02;
          hashtag[7] = hashtag[7] | 0x01;
        }else{
          hashtag[6] = hashtag[6] | 0x03;
          hashtag[7] = hashtag[7] | 0x03;
        }
        turn+=1;
        break;
    case '*':
        clearBoard();
        break;
    default:
      Serial.println("NOT VALID BRUH");
    
  }


    
}

//wipe board for subsequent game. 
void clearBoard(){
        hashtag[0]= 0x00;
        hashtag[1]= 0x00;
        
        hashtag[3]= 0x00;
        hashtag[4]= 0x00;
        
        hashtag[6]= 0x00;
        hashtag[7]= 0x00;
}
//comunicate with PC
void sendOutPut(char keyPressed)
{
  switch(keyPressed){
    case 'A':
        Serial.print("Subscribing to topic: ");
        mqttClient.subscribe(espIn);
        Serial.println();
        Serial.println("Success!");
        break;
    case '*':
        //clearBoard();
        //break;
    default:
        Serial.print("Sending message to topic: ");
        Serial.print(espOut);
        Serial.println(keyPressed);
    
        mqttClient.beginMessage(espOut);
        mqttClient.print(keyPressed);
        mqttClient.endMessage();
  }
}


void loop() {

  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;
  }
  if(mqttClient.available()){
   Serial.println("Incoming");
   grabInput(mqttClient.read());
  }
  


  // Get the character input
  char keyPressed = myKeypad.getKey();
  // If there is a character input, sent it to the serial port
  if (keyPressed)
  {
      sendOutPut(keyPressed);
  }

  displayBoard();
  
}
void displayBoard()
{
  // Define a one-byte variable (8 bits) which is used to represent the selected state of 8 column.
  int cols;
  // Display the static smiling pattern
  for (int j = 0; j < 10; j++ ) {  // repeat 500 times
    cols = 0x01;
    for (int i = 0; i < 8; i++) {   // display 8 column data by scaning
      matrixRowsVal(hashtag[i]);// display the data in this column
      matrixColsVal(~cols);          // select this column
      delay(1);                     // display them for a period of time
      matrixRowsVal(0x00);          // clear the data of this column
      cols <<= 1;                   // shift"cols" 1 bit left to select the next column
    }
    //delay(1);
  }
}
void matrixRowsVal(int value) {
  // make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, LSBFIRST, value);
  // make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}

void matrixColsVal(int value) {
  // make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  // make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}
