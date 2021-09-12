#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

const int buzzer_pin =  10;// the number of the buzzer pin
const int cuttable_wires_out =  1;// pin of fake-cuttable wire output
const int cuttable_wires_in[]={11, 12, 13}; // pins of fake-cuttable wires input

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

// change to true if you want buzzing on key presses
bool buzz_on_key_press = false;

// set bomb starting state to armed, as switch doesn't allow string it will be mapped by the following table
// 0 = arming
// 1 = counting
// 2 = exploded
int bomb_state = 0;

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


// ring buzzer
void ring_buzzer_on(){
  digitalWrite(buzzer_pin, HIGH);
}

// stop ringing buzzer
void ring_buzzer_off(){
  digitalWrite(buzzer_pin, LOW);
}
  
void loop(){
  

  switch (bomb_state) {
  case 0:
     char key = keypad.getKey();// Read the key
    // Print if key pressed
    if (key){
      if (buzz_on_key_press == true){
        ring_buzzer_on();
        delay(100);
        ring_buzzer_off();
      }
      display.println("enter defuse code:");
      display.display();
      display.setCursor(0, 16);
      display.println(key);
      display.display();
      display.clearDisplay();
      display.setCursor(0, 0);
  }
    break;
  case 1:
    //do something when var equals 2
    break;
  case 2:
    ring_buzzer_on();
    display.println("bomb exploded");
    display.display();
    display.clearDisplay();
    display.setCursor(0, 0);
    break;
  }
}
