#define ENA 9
#define IN1 8
#define IN2 7
#define IN3 6
#define IN4 5
#define ENB 10
#define TRIG 3
#define ECHO 2
#define BTN_FWD 4
#define BTN_BWD 11
#define BTN_LEFT 12
#define BTN_RIGHT 13

int directionMapping[4] = {0, 1, 2, 3};
bool isShuffled = false;

void setup() {
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    
    pinMode(BTN_FWD, INPUT_PULLUP);
    pinMode(BTN_BWD, INPUT_PULLUP);
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    
    Serial.begin(9600);
    randomSeed(analogRead(A0));
}

int getDistance() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    return pulseIn(ECHO, HIGH) * 0.034 / 2;
}

void moveCar(int command) {
    switch (command) {
        case 0: // Forward
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
      		Serial.println("front");
            break;
        case 1: // Backward
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
      		Serial.println("back");
            break;
        case 2: // Left
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
      		Serial.println("right");
            break;
        case 3: // Right
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
      		Serial.println("left");
            break;
    }
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void shuffleDirections() {
    if (!isShuffled) {
        for (int i = 3; i > 0; i--) {
            int j = random(i + 1);
            int temp = directionMapping[i];
            directionMapping[i] = directionMapping[j];
            directionMapping[j] = temp;
        }
        isShuffled = true;
    } else {
        directionMapping[0] = 0;
        directionMapping[1] = 1;
        directionMapping[2] = 2;
        directionMapping[3] = 3;
        isShuffled = false;
    }
}

void loop() {
    int distance = getDistance();
    
    if (distance < 10) {
        Serial.println("Crash detected! Toggling control mode.");
        shuffleDirections();
        delay(500);
    }
    
    if (digitalRead(BTN_FWD) == LOW) moveCar(directionMapping[0]);
    else if (digitalRead(BTN_BWD) == LOW) moveCar(directionMapping[1]);
    else if (digitalRead(BTN_LEFT) == LOW) moveCar(directionMapping[2]);
    else if (digitalRead(BTN_RIGHT) == LOW) moveCar(directionMapping[3]);
    else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
    }
}
