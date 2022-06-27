#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define photoPin A3
#define servoPin 10

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo servo;

int lightVal = 0, dodged = 0, maxLight = 0;

// this value is dependent on the photoresistor and the brightness of your TV
// you will probably need to test different values here
int threshold = 350;

// this sets the starting servo position and the distance traveled for a button press
// you may need to change these values depending on the servo motor you are using
int startDegs = 180;
int pressDegs = 15;

void updateLCD(){
	lcd.clear();

	lcd.home();
	lcd.print("Dodged: ");
	lcd.print(dodged);

	lcd.setCursor(0, 1);
	lcd.print("Light val.: ");
	lcd.print(lightVal);
	lcd.print("Max light: ");
	lcd.print(maxLight);
}

void setup(){
	Serial.begin(9600);
	Wire.begin();

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
	}

	if (lightVal > threshold){
		delay(50);
		servo.write(startDegs - pressDegs);
		delay(200);
		servo.write(startDegs);
		dodged++;
		updateLCD();
		delay(990); // "debouncing"
	}
	else{
		updateLCD();
	}

	delay(10);
}
