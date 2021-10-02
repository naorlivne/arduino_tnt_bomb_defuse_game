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

// set bomb starting state to armed
String bomb_state = "arming";

// initial countdown seconds
int countdown_seconds = 90;

// initial delay speed time, the lower it is the faster the countdown will be
int delay_speed = 25;

// starting delay
int current_delay = 0;

// location of key
int key_location = 0;

String defuse_key = "";
String defuse_try_key = "";

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
    pinMode(cuttable_wires_in[i], INPUT_PULLUP);
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

bool cuttable_wires_state(int pin_number){
  if (digitalRead(cuttable_wires_in[pin_number]) == HIGH){
    return true;
  }
  else {
    return false;
    }
  }
  
void loop(){
  digitalWrite(cuttable_wires_out, HIGH);
  if (bomb_state == "arming") {
     char key = keypad.getKey();// Read the key
    // Print if key pressed
    if (key){
      if (buzz_on_key_press == true){
        ring_buzzer_on();
        delay(100);
        ring_buzzer_off();
      }
      if (key_location < 4) {
      key_location = key_location + 1;
      defuse_key = defuse_key + key;
      display.println("enter defuse code:");
      display.display();
      display.setCursor(0, 16);
      display.println(defuse_key);
      display.display();
      display.clearDisplay();
      display.setCursor(0, 0);
      }
  }
    if (key_location == 4){
      bomb_state = "counting";  
      }
  }
  else if (bomb_state == "counting") {
      key_location = 0;
      while (countdown_seconds != 0){
        char key = keypad.getKey();
        if (key){
            if (buzz_on_key_press == true){
              ring_buzzer_on();
              delay(100);
              ring_buzzer_off();
            }
            if (key_location < 4) {
            key_location = key_location + 1;
            defuse_try_key = defuse_try_key + key;
            }
            if (key_location == 4) {
              if (defuse_try_key == defuse_key){
                bomb_state = "defused";
                break;
                }
              else {
              bomb_state = "exploding";
              break;
              }
            }
        }
        display.println("counting down: " + String(countdown_seconds));
        display.display();
        display.setCursor(0, 16);
        display.println("defuse code: " +  String(defuse_try_key));
        display.display();
        display.clearDisplay();
        display.setCursor(0, 0);
        delay(1);
        if (current_delay == delay_speed){
          countdown_seconds = countdown_seconds -1;
          current_delay = 0;
        }
        current_delay = current_delay + 1;
        if (cuttable_wires_state(0) == false){
          bomb_state = "exploding";
          break;
       }
        if (cuttable_wires_state(1) == false){
          delay_speed = 10;
        }
        if (cuttable_wires_state(2) == false){
          bomb_state = "defused";
          break;
        }
      }
      if (countdown_seconds == 0){
      bomb_state = "exploding";  
      }
  }
  else if (bomb_state == "exploding") {
    //ring_buzzer_on();
    display.println("bomb exploded");
    display.display();
    display.clearDisplay();
    display.setCursor(0, 0);
    delay(1000);
    ring_buzzer_off();
  }
    else if (bomb_state == "defused") {
    display.println("bomb defused");
    display.display();
    display.clearDisplay();
    display.setCursor(0, 0);
  }
}
