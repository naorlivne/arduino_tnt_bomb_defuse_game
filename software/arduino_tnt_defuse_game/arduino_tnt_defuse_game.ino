#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

const int buzzer_pin =  1;// the number of the buzzer pin
const int cuttable_wires_out =  10;// pin of fake-cuttable wire output
const int cuttable_wires_in[]={11, 12, 13}; // pins of fake-cuttable wires input

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Create an object of keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // initilize pin modes

  pinMode(buzzer_pin, OUTPUT);
  pinMode(cuttable_wires_out, OUTPUT);
  for (byte i = 0; i < 3; i = i + 1) {
    pinMode(cuttable_wires_in[i], INPUT);
  }
}

void ring_buzzer_on(){
  digitalWrite(buzzer_pin, HIGH);
}


void ring_buzzer_off(){
  digitalWrite(buzzer_pin, LOW);
}
  
void loop(){

  char key = keypad.getKey();// Read the key
  // Print if key pressed
  if (key){
    ring_buzzer_on();
    delay(1000);
    ring_buzzer_off();
    display.println(key);
    display.display();
    display.clearDisplay();
    display.setCursor(0, 0);
  }
}
