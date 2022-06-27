#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define photoPin A3
#define servoPin 10

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo servo;

int lightVal = 0, dodged = 0, maxLight = 0, minLight = 9999;

// this value is dependent on the photoresistor and the brightness of your TV
// you will probably need to test different values here
int threshold = 350;

// this sets the starting servo position and the distance traveled for a button press
// you may need to change these values depending on the servo motor you are using
int startDegs = 180;
int pressDegs = 15;

void updateLCD(){
    String spaces1 = "";
    for(int i = 0; i < 8 - String(dodged).length() - String(lightVal).length(); i++){
        spaces1 = spaces1 + " ";
    }

    String spaces2 = "";
    for(int i = 0; i < 4 - String(minLight).length(); i++){
        spaces2 = spaces2 + " ";
    }
    
    lcd.clear();

    lcd.home();
    lcd.print("LD: ");
    lcd.print(dodged);
    lcd.print(spaces1 + " L: ");
    lcd.print(lightVal);

    lcd.setCursor(0, 1);
    lcd.print("Lm: ");
    lcd.print(minLight);
    lcd.print(spaces2 + " LM: ");
    lcd.print(maxLight);
}

void setup(){
    Serial.begin(9600);

    pinMode(photoPin, INPUT);

    servo.attach(servoPin);
    servo.write(startDegs);

    lcd.begin(16, 2);
    updateLCD();
}

void loop(){
    lightVal = analogRead(photoPin);

    // print light value in serial
    Serial.println(lightVal);
  
    if(lightVal > maxLight){
        maxLight = lightVal;
        updateLCD();
    }

    if(lightVal < minLight){
        minLight = lightVal;
        updateLCD();
    }

    if (lightVal > threshold){
        minLight = 9999;
        delay(50);
        servo.write(startDegs - pressDegs);
        delay(200);
        servo.write(startDegs);
        dodged++;
        updateLCD();
        delay(990); // "debouncing"
    }

    delay(10);
}
