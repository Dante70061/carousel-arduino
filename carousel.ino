#include <Servo.h>

Servo myServo;

int buttonPin = 2;
int servoPin = 9;
int photoPin = A0;

int melody[] = {523, 587, 659, 698, 784, 880, 987, 784, 659, 587, 523}; 
int noteDurations[] = {400, 400, 400, 400, 600, 600, 600, 600, 400, 400, 800};
int melodyLength = 11;
int currentNote = 0;
unsigned long lastNoteTime = 0;

int piezoPin = 10;

int leds[6] = {3,4,5,6,7,8};

int state = 0;
int last;

void setup() {

  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(piezoPin, OUTPUT);

  last = digitalRead(buttonPin);

  myServo.attach(servoPin);
  myServo.write(90);

  for(int i = 0; i < 6; i++){
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {

  int buttonState = digitalRead(buttonPin);
  int lightValue = analogRead(photoPin);

  Serial.print("Light Value: ");
  Serial.println(lightValue);

  if (last == HIGH && buttonState == LOW) {

    if (state == 0) {
      state = 1;
      Serial.println("System ON");
    }
    else {
      state = 0;
      Serial.println("System OFF");
    }

    delay(200);
  }

  last = buttonState;


  if (state == 1 && lightValue < 70) {
    myServo.write(120);
  }
  else {
    myServo.write(90);
  }


  if (state == 1) {
    for(int i = 0; i < 6; i++){
      digitalWrite(leds[i], HIGH);
    }
  }
  else {
    for(int i = 0; i < 6; i++){
      digitalWrite(leds[i], LOW);
    }
  }


  if (state == 1) {

  unsigned long currentTime = millis();

  if (currentTime - lastNoteTime > noteDurations[currentNote]) {

    tone(piezoPin, melody[currentNote]);

    lastNoteTime = currentTime;

    currentNote++;
    if (currentNote >= melodyLength) {
      currentNote = 0;
    }
  }

  } else {
    noTone(piezoPin);
  }

}
