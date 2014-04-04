#define TREADMILL_PIN 9

#define STOP      255
#define MAX_SPEED 210

int currentSpeed = STOP;
int incomingByte = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(TREADMILL_PIN, OUTPUT);

    Serial.print("currentSpeed: ");
    Serial.println(currentSpeed, DEC);
    analogWrite(TREADMILL_PIN, min(max(currentSpeed, 0), 255));
}

void loop() {
    // send data only when you receive data:
    if (Serial.available() > 0) {

        // read the incoming byte:
        incomingByte = Serial.read();
        Serial.print("char (dec): ");
        Serial.println(incomingByte, DEC);

        switch (incomingByte) {
            // s = STOP
            case 115:
                slowToStop();
                break;
            // j = increase speed
            case 106:
                increaseSpeed(1);
                break;
            // k = decrease speed
            case 107:
                decreaseSpeed(1);
                break;
            // xNNN = set speed to NNN
            case 120:
                int newSpeed = Serial.parseInt();
                changeSpeed(newSpeed, 1, 100);
                break;
        }
    }
}

void increaseSpeed(int increment) {
    currentSpeed = max(MAX_SPEED, currentSpeed - increment);
    setSpeed();
}

void decreaseSpeed(int decrement) {
    currentSpeed = min(STOP, currentSpeed + decrement);
    setSpeed();
}

void changeSpeed(int newSpeed, int increment, int msDelay) {
    // slow down!
    if (currentSpeed < newSpeed) {
        while (currentSpeed < newSpeed && currentSpeed < STOP) {
            decreaseSpeed(increment);
            delay(msDelay);
        }
    } else if (currentSpeed > newSpeed) {
        while (currentSpeed > newSpeed && currentSpeed > MAX_SPEED) {
            increaseSpeed(increment);
            delay(msDelay);
        }
    } 
}

void slowToStop() {
    Serial.println("stop");
    changeSpeed(STOP, 1, 100);
}

void setSpeed() {
    Serial.print("currentSpeed: ");
    Serial.println(currentSpeed, DEC);
    analogWrite(TREADMILL_PIN, currentSpeed);
}
