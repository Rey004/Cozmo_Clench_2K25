#define reciever_throttle 3
#define reciever_direction 6

#define IN1 A1
#define IN2 A2
#define ENA 10
#define ENB 5
#define IN3 A3
#define IN4 A4

void setup() {
    Serial.begin(9600);

    pinMode(reciever_throttle, INPUT);
    pinMode(reciever_direction, INPUT);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
}

void loop() {
    unsigned long throttlespeed = pulseIn(reciever_throttle, HIGH);
    unsigned long directionspeed = pulseIn(reciever_direction, HIGH);

    int throttle = map(throttlespeed, 1000, 2000, -255, 255);
    int direction = map(directionspeed, 1000, 2000, -255, 255);

    Serial.print("Throttle: ");
    Serial.println(throttle);
    Serial.print("Direction: ");
    Serial.println(direction);

    if (abs(throttle) < 10) throttle = 0;
    if (abs(direction) < 10) direction = 0;

    int left_speed = throttle + direction;
    int right_speed = throttle - direction;

    // Constrain speed to valid PWM range (0 - 255)
    left_speed = constrain(left_speed, -255, 255);
    right_speed = constrain(right_speed, -255, 255);

    // Motor control
    if (left_speed > 0) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, left_speed);
    } else if (left_speed < 0) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(ENA, -left_speed);
    } else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        analogWrite(ENA, 0);
    }

    if (right_speed > 0) {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, right_speed);
    } else if (right_speed < 0) {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENB, -right_speed);
    } else {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        analogWrite(ENB, 0);
    }

    delay(20);
}