#define reciever_throttle 3  // receiver for right
#define reciever_direction 6 // receiver for left

#define reciever_motor1 7  // receiver for claw1
#define reciever_motor2 8  // receiver for claw2

#define IN1 A1
#define IN2 A2
#define ENA 10
#define ENB 5
#define IN3 A3
#define IN4 A4

#define IN1c 2
#define IN2c 4
#define IN3c 9
#define IN4c 11

int lastThrottle = 0, lastDirection = 0, lastMotor1 = 0, lastMotor2 = 0;  // Store last valid values

void setup() {
    Serial.begin(9600);

    pinMode(reciever_throttle, INPUT);
    pinMode(reciever_direction, INPUT);
    pinMode(reciever_motor1, INPUT);
    pinMode(reciever_motor2, INPUT);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(IN1c, OUTPUT);
    pinMode(IN2c, OUTPUT);
    pinMode(IN3c, OUTPUT);
    pinMode(IN4c, OUTPUT);
    
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
}

void loop() {
    unsigned long throttlespeed = pulseIn(reciever_throttle, HIGH, 25000);  // Timeout = 25ms
    unsigned long directionspeed = pulseIn(reciever_direction, HIGH, 25000);
    unsigned long motor1speed = pulseIn(reciever_motor1, HIGH, 25000);
    unsigned long motor2speed = pulseIn(reciever_motor2, HIGH, 25000);

    // Set default values if signal is lost
    if (throttlespeed == 0 || throttlespeed < 900 || throttlespeed > 2100) throttlespeed = 1500;  // Neutral
    if (directionspeed == 0 || directionspeed < 900 || directionspeed > 2100) directionspeed = 1500;
    if (motor1speed == 0 || motor1speed < 900 || motor1speed > 2100) motor1speed = 1500;
    if (motor2speed == 0 || motor2speed < 900 || motor2speed > 2100) motor2speed = 1500;

    int throttle = map(throttlespeed, 1000, 2000, -255, 255);
    int direction = map(directionspeed, 1000, 2000, -255, 255);
    int motor1 = map(motor1speed, 1000, 2000, -255, 255);
    int motor2 = map(motor2speed, 1000, 2000, -255, 255);

    // Apply dead zone
    if (abs(throttle) < 10) throttle = 0;
    if (abs(direction) < 10) direction = 0;
    if (abs(motor1) < 10) motor1 = 0;
    if (abs(motor2) < 10) motor2 = 0;

    // Save last valid values
    lastThrottle = throttle;
    lastDirection = direction;
    lastMotor1 = motor1;
    lastMotor2 = motor2;

    int left_speed = throttle + direction;
    int right_speed = throttle - direction;

    left_speed = constrain(left_speed, -255, 255);
    right_speed = constrain(right_speed, -255, 255);
    motor1 = constrain(motor1, -255, 255);
    motor2 = constrain(motor2, -255, 255);

    // Left Motor Control
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

    // Right Motor Control
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

    // Claw Motor 1 Control
    if (motor1 > 0) {
        digitalWrite(IN1c, HIGH);
        digitalWrite(IN2c, LOW);
    } else if (motor1 < 0) {
        digitalWrite(IN1c, LOW);
        digitalWrite(IN2c, HIGH);
    } else {
        digitalWrite(IN1c, LOW);
        digitalWrite(IN2c, LOW);
    }

    // Claw Motor 2 Control
    if (motor2 > 0) {
        digitalWrite(IN3c, HIGH);
        digitalWrite(IN4c, LOW);
    } else if (motor2 < 0) {
        digitalWrite(IN3c, LOW);
        digitalWrite(IN4c, HIGH);
    } else {
        digitalWrite(IN3c, LOW);
        digitalWrite(IN4c, LOW);
    }

    delay(10);  //delay
}
