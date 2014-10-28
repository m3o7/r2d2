#include <PCM.h>
#include "sounddata-r2d2-excited.h"
#include "sounddata-r2d2-veryexcited.h"

#define SPEAKER_PIN 3
#define LED_WHITE 8
#define LED_RED 7
#define LED_BLUE 6

// audio jack
#define AJ_BTN_BLUE 13
#define AJ_BTN_RED 12 

// GLOBAL VARIABLES
int BTN_BLUE = LOW;
int BTN_BLUE_PREV = LOW;
unsigned long BTN_BLUE_STARTED = 0; // time since a button press started
bool BTN_BLUE_SHORT_PRESS = false; // true only for one run
bool BTN_BLUE_MEDIUM_PRESS = false; // true only for one run
bool BTN_BLUE_LONG_PRESS = false; // true only for one run

int BTN_RED = LOW;
int BTN_RED_PREV = LOW;
unsigned long BTN_RED_STARTED = 0; // time since a button press started
bool BTN_RED_SHORT_PRESS = false;
bool BTN_RED_MEDIUM_PRESS = false;
bool BTN_RED_LONG_PRESS = false;

// pointer to current sound
int CURRENT_R2D2_SOUND = 0;

// define button press times
unsigned long BTN_MEDIUM_LONG = 500;
unsigned long BTN_LONG = 2000;

void setup(){
    // DEBUG ONLY - communication with computer
    Serial.begin(9600);

    // sets LED pins for output
    pinMode(LED_WHITE, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    // Audio-Jack controlled buttons
    pinMode(AJ_BTN_BLUE, INPUT_PULLUP);
    pinMode(AJ_BTN_RED, INPUT_PULLUP);
}

void loop(){
    delay(50);
    read_buttons();    

    //print out the value of the pushbutton
    if (BTN_BLUE_SHORT_PRESS || BTN_BLUE_MEDIUM_PRESS || BTN_BLUE_LONG_PRESS){
        play_next_sound();
    }
    if (BTN_RED_SHORT_PRESS || BTN_RED_MEDIUM_PRESS || BTN_RED_LONG_PRESS){
        flicker_leds();
    }
}

void play_sound(const unsigned char soundData[], int size, int delay_ms) {
    // Play a given sound
    startPlayback(soundData, size);
}

void play_next_sound(){
    // Play the next sound in the loop
    if (CURRENT_R2D2_SOUND == 0){
        play_sound(sounddata_veryexcited_data, sizeof(sounddata_veryexcited_data));
    } else {
        play_sound(sounddata_excited_data, sizeof(sounddata_excited_data));
    }

    // update sound pointer
    CURRENT_R2D2_SOUND = (CURRENT_R2D2_SOUND > 0) ? 0 : CURRENT_R2D2_SOUND+1; 
}

void flicker_leds(){
    // control lights
    analogWrite(LED_WHITE, random(255));
    analogWrite(LED_RED, random(255));
    analogWrite(LED_BLUE, random(255));
}

void read_buttons(){
    // clear old state
    BTN_BLUE_SHORT_PRESS = false;
    BTN_BLUE_MEDIUM_PRESS = false;
    BTN_BLUE_LONG_PRESS = false;
    BTN_RED_SHORT_PRESS = false;
    BTN_RED_MEDIUM_PRESS = false;
    BTN_RED_LONG_PRESS = false;

    // read sensor values - http://arduino.cc/en/Tutorial/InputPullupSerial
    BTN_BLUE = digitalRead(AJ_BTN_BLUE);
    BTN_RED = digitalRead(AJ_BTN_RED);
    // //ardprintf("%d - %d", BTN_BLUE, BTN_RED); // DEBUG ONLY
    unsigned long CURRENT_TIME = millis();

    // PRESS STARTED ===========================================================
    /*BLUE BTN press finished*/
    if (BTN_BLUE_PREV == HIGH && BTN_BLUE == LOW){ 
        BTN_BLUE_STARTED = CURRENT_TIME;
        //ardprintf("BLUE start %l", BTN_BLUE_STARTED); // DEBUG ONLY
    }

    /*RED BTN press finished*/
    if (BTN_RED_PREV == LOW && BTN_RED == HIGH){ 
        BTN_RED_STARTED = CURRENT_TIME;
        //ardprintf("RED start %l", BTN_RED_STARTED); // DEBUG ONLY
    }

    // PRESS FINISHED ==========================================================
    /*BLUE BTN press finished*/
    if (BTN_BLUE_PREV == LOW && BTN_BLUE == HIGH){
        unsigned long ELAPSED_TIME = CURRENT_TIME - BTN_BLUE_STARTED;
        if (ELAPSED_TIME < BTN_MEDIUM_LONG){
            BTN_BLUE_SHORT_PRESS = true;
        } else if (ELAPSED_TIME < BTN_LONG) {
            BTN_BLUE_MEDIUM_PRESS = true;
        } else {
            BTN_BLUE_LONG_PRESS = true;
        }
        //ardprintf("BLUE ended: %l; %l", CURRENT_TIME, ELAPSED_TIME);
    }

    /*RED BTN press finished*/
    if (BTN_RED_PREV == LOW && BTN_RED == HIGH){
        unsigned long ELAPSED_TIME = CURRENT_TIME - BTN_RED_STARTED;
        if (ELAPSED_TIME < BTN_MEDIUM_LONG){
            BTN_RED_SHORT_PRESS = true;
        } else if (ELAPSED_TIME < BTN_LONG) {
            BTN_RED_MEDIUM_PRESS = true;
        } else {
            BTN_RED_LONG_PRESS = true;
        }
        //ardprintf("RED ended: %l; %l", CURRENT_TIME, ELAPSED_TIME);
    }

    // assign values to old
    BTN_BLUE_PREV = BTN_BLUE;
    BTN_RED_PREV = BTN_RED;
}

