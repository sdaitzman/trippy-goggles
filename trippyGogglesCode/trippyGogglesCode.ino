// This work by Sam Daitzman and Josh Brancazio is licensed under the Creative Commons
// Attribution-ShareAlike 4.0 International License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.

#include <Servo.h>

#define indicatorPin       13  // turns on when button pressed

#define leftUpButtonPin    5   // pins for buttons
#define leftDownButtonPin  6
#define rightUpButtonPin   7
#define rightDownButtonPin 8

#define leftServoPin       9   // pins for servos
#define rightServoPin      10

#define servoMinPos 40         // servo will not be allowed to go below this rotation val
#define servoMaxPos 170        // or above this one

// holds button values - false when button is pressed
bool leftUpButtonVal, leftDownButtonVal, rightUpButtonVal, rightDownButtonVal;

// le servos
Servo leftServo;
Servo rightServo;

// servo starting position
int leftPos, rightPos;

// detach non-used servos at this time
int detachServosAt;
bool servosAttached;

void setup() {
  delay(250); // so uploads don't fail as much

  Serial.begin(9600);

  delay(500); // wait for le serial to stabilize

  // say hey
  Serial.println("Hello world.");

  // initialize all the button input pins
  pinMode(leftUpButtonPin, INPUT);
  pinMode(leftDownButtonPin, INPUT);
  pinMode(rightUpButtonPin, INPUT);
  pinMode(rightDownButtonPin, INPUT);

  pinMode(indicatorPin, OUTPUT); // LED on 13 by default

  // attach servos to their respective pins
  leftServo.attach(leftServoPin);
  delay(100);
  rightServo.attach(rightServoPin);
  delay(100);

  // get current position
  leftPos = leftServo.read();
  rightPos = rightServo.read();
  Serial.println("leftPos: " + leftPos);
  Serial.println("rightPos: " + rightPos);
  delay(100);

  // go to initial position
  leftServo.write(leftPos);
  delay(100);
  rightServo.write(rightPos);
  delay(100);
  Serial.println("Init finished");
}

void loop() {

  leftUpButtonVal       = digitalRead(leftUpButtonPin);
  leftDownButtonVal     = digitalRead(leftDownButtonPin);
  rightUpButtonVal      = digitalRead(rightUpButtonPin);
  rightDownButtonVal    = digitalRead(rightDownButtonPin);

  if (leftUpButtonVal == 0 | leftDownButtonVal == 0 || rightUpButtonVal == 0 || rightDownButtonVal == 0) {
    
    // detach servos in 1s
    detachServosAt = millis() + 1000;
    
    // attach servos
    rightServo.attach(rightServoPin);
    leftServo.attach(leftServoPin);
    servosAttached = true;
    Serial.print("Servos attached because ");
    
    // turn on LED if buttons pressed
    digitalWrite(indicatorPin, HIGH);
    
    if (leftUpButtonVal == 0) {
      leftPos++;
      Serial.print("the left up button was pressed. ");
    }

    if (leftDownButtonVal == 0) {
      leftPos--;
      Serial.print("the left down button was pressed. ");
    }

    if (rightUpButtonVal == 0) {
      rightPos++;
      Serial.print("the right up button was pressed. ");
    }

    if (rightDownButtonVal == 0) {
      rightPos--;
      Serial.print("the right down button was pressed. ");
    }

    leftPos =  constrain(leftPos, servoMinPos, servoMaxPos);
    rightPos = constrain(rightPos, servoMinPos, servoMaxPos);

    Serial.println("After constraining, leftPos \=  " + String(leftPos) + " and rightPos \= " + String(rightPos));

    leftServo.write(leftPos);
    rightServo.write(rightPos);
    delay(100);

  } else {
    if (millis() >= detachServosAt & servosAttached == true) {
      Serial.println("Detaching servos so they don't twitch"); // comment this out if your servos behave differently
      leftPos = leftServo.read();
      rightPos = rightServo.read();
      rightServo.detach();
      leftServo.detach();
      servosAttached = false;
    }
    // turn LED off if buttons not pressed
    digitalWrite(indicatorPin, LOW);
  }
}


