/*
  Two motors, motors OFF until a command is given (Arduino Uno)

  Serial Monitor: 9600 baud, line ending = Newline

  Send  1   -> motor 1 (pin 9)  goes to 90 degrees. Send 1 again: back to 0.
  Send  2   -> motor 2 (pin 10) goes to 90 degrees. Send 2 again: back to 0.
  Send  0   -> both motors switched off (no signal, they go limp).

  Until the first command, no signal is sent: both signal pins are held LOW.
  A motor is switched on only when its own command arrives.
*/

#include <Servo.h>

const byte MOTOR1_PIN = 9;
const byte MOTOR2_PIN = 10;

Servo motor1, motor2;
int  angle1 = 0, angle2 = 0;      // last angle of each motor
bool on1 = false, on2 = false;    // is the motor switched on?

void moveSlowly(Servo &m, int &current, int target) {
  while (current != target) {
    current += (target > current) ? 1 : -1;
    m.write(current);
    delay(15);
  }
}

void runMotor(Servo &m, byte pin, int &angle, bool &on) {
  if (!on) {
    m.write(angle);       // start at the stored angle, no jump to 90
    m.attach(pin);
    on = true;
    delay(300);
  }
  moveSlowly(m, angle, (angle == 0) ? 90 : 0);
}

void switchOff(Servo &m, byte pin, bool &on) {
  if (on) {
    m.detach();
    on = false;
  }
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);     // keep the signal line quiet
}

void setup() {
  Serial.begin(9600);
  switchOff(motor1, MOTOR1_PIN, on1);
  switchOff(motor2, MOTOR2_PIN, on2);
  Serial.println("Motors OFF. Send 1 or 2 to move a motor, 0 to switch off.");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();

    if (c == '1') {
      runMotor(motor1, MOTOR1_PIN, angle1, on1);
      Serial.print("Motor 1 at ");
      Serial.println(angle1);
    }
    else if (c == '2') {
      runMotor(motor2, MOTOR2_PIN, angle2, on2);
      Serial.print("Motor 2 at ");
      Serial.println(angle2);
    }
    else if (c == '0') {
      switchOff(motor1, MOTOR1_PIN, on1);
      switchOff(motor2, MOTOR2_PIN, on2);
      Serial.println("Both motors OFF.");
    }
  }
}
