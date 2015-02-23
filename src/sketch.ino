#define TREADMILL_PIN 10

#define BAUD_RATE 9600
#define INCREMENT 1

#define STOP      255
#define MAX_SPEED 210

int currentSpeed = STOP;
int newSpeed = STOP;

void setup() {
    Serial.begin(BAUD_RATE);

    pinMode(TREADMILL_PIN, OUTPUT);
    analogWrite(TREADMILL_PIN, min(max(currentSpeed, 0), STOP));
}

boolean validSpeed(int speed) {
    return speed <= STOP && speed >= MAX_SPEED;
}

void loop() {
    if (currentSpeed < newSpeed) {
        currentSpeed = min(STOP, currentSpeed + INCREMENT);
        analogWrite(TREADMILL_PIN, currentSpeed);
    } else if (currentSpeed > newSpeed) {
        currentSpeed = max(MAX_SPEED, currentSpeed - INCREMENT);
        analogWrite(TREADMILL_PIN, currentSpeed);
    }
}

void serialEvent() {
    while (Serial.available()) {
        int input = Serial.parseInt();

        if (input == -1)
            newSpeed = currentSpeed + 1;
        else if (input == 1)
            newSpeed = currentSpeed - 1;
        else if (validSpeed(input))
            newSpeed = input;
    }
}
