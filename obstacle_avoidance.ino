#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 200
#define MAX_SPEED 220
#define TURN_SPEED 250  // Speed for turning
#define TURN_DURATION 1200  // Increased duration of turn in milliseconds

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;

boolean goesForward = false;
int distance = 100;

void setup() {
  myservo.attach(10);
  myservo.write(115);
  delay(2000);
  distance = readPing();
}

void loop() {
  delay(40);  // Delay for sensor reading stability
  distance = readPing();

  if (distance <= 15) {
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    delay(200);
   
    int distanceR = lookRight();
    int distanceL = lookLeft();

    if (distanceR >= distanceL) {
      turnRight();
    } else {
      turnLeft();
    }
  } else {
    moveForward();
  }
}

int lookRight() {
  myservo.write(50);
  delay(300);  // Allow time for servo to reach position
  int distance = readPing();
  myservo.write(115);  // Return to center position
  return distance;
}

int lookLeft() {
  myservo.write(170);
  delay(300);  // Allow time for servo to reach position
  int distance = readPing();
  myservo.write(115);  // Return to center position
  return distance;
}

int readPing() {
  delay(50);  // Allow time for the sensor to stabilize
  int cm = sonar.ping_cm();
  return (cm == 0) ? 250 : cm;  // Return 250 if ping fails
}

void moveStop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void moveForward() {
  if (!goesForward) {
    goesForward = true;
    motor1.setSpeed(MAX_SPEED);
    motor2.setSpeed(MAX_SPEED);
    motor3.setSpeed(MAX_SPEED);
    motor4.setSpeed(MAX_SPEED);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
  }
}

void moveBackward() {
  goesForward = false;
  motor1.setSpeed(MAX_SPEED);
  motor2.setSpeed(MAX_SPEED);
  motor3.setSpeed(MAX_SPEED);
  motor4.setSpeed(MAX_SPEED);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void turnRight() {
  motor1.setSpeed(TURN_SPEED);
  motor2.setSpeed(TURN_SPEED);
  motor3.setSpeed(TURN_SPEED);
  motor4.setSpeed(TURN_SPEED);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(TURN_DURATION);  // Adjusted duration for turning
  moveForward();  // Continue moving forward after turning
}

void turnLeft() {
  motor1.setSpeed(TURN_SPEED);
  motor2.setSpeed(TURN_SPEED);
  motor3.setSpeed(TURN_SPEED);
  motor4.setSpeed(TURN_SPEED);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(TURN_DURATION);  // Adjusted duration for turning
  moveForward();  // Continue moving forward after turning
}