/*
http://arduino.cc/en/Reference/analogWrite
*/

#define SPEAKER_PIN 13
#define WHITE_LED 12
#define RED_LED 11
#define BLUE_LED 10

// audio jack
#define AJ_TIP 7 // always powered
#define BLUE_BTN 6 
#define RED_BTN 5 
#define AJ_DO_NOT_USE 4 // its the "sleeve" pin from the audio-jack


void setup(){
    // DEBUG ONLY - communication with computer
    Serial.begin(9600);

    // sets LED pins for output
    pinMode(WHITE_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    // 
}


void loop(){
    delay(100);
    analogWrite(WHITE_LED, random(255));
    analogWrite(RED_LED, random(255));
    analogWrite(BLUE_LED, random(255));

    // button pressed
    if (false){
        Serial.print("button is being pressed");
    }
}
