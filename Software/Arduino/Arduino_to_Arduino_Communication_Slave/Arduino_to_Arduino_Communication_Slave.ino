// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <Servo.h>

Servo needle_tilt_servo; // creates servo object

int pos = 0; // position, for servo sweep iterator
float servo_step = 1; // step size for servo sweep
unsigned int servo_delay = 20;
unsigned int prev_millis = 0;
unsigned int cur_millis = millis();


void setup() {
  Serial.begin(9600);           // start serial for output
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.println("Set-Up End");
}


void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

int new_song_tilt(Servo needle_tilt_servo, unsigned int servo_delay) {
  tilt_up(needle_tilt_servo, servo_delay);
  tilt_down(needle_tilt_servo, 100);
}

int tilt_up(Servo needle_tilt_servo, unsigned int servo_delay) {
  unsigned int prev_millis = 0;
  boolean servo_step_move_done = false;
  boolean running_correctly = true;
  boolean needle_move_up_done = false;
  int pos = 0;
  while(!needle_move_up_done){
    unsigned int cur_millis = millis(); 
    // if it has, check that the delay has passed
    if (cur_millis - prev_millis >= servo_delay) {
      // update prev_millis
      prev_millis += servo_delay;
      if (++pos > 90) {
        needle_move_up_done = true;
      }
      else {
        needle_tilt_servo.write(pos);
        servo_step_move_done = true;
        pos ++;
      }
    }
}
}

int tilt_down(Servo needle_tilt_servo, unsigned int servo_delay) {
  unsigned int prev_millis = 0;
  boolean servo_step_move_done = false;
  boolean running_correctly = true;
  boolean needle_move_down_done = false;
  int pos = 90;
  while(!needle_move_down_done){
    unsigned int cur_millis = millis();
    // if it has, check that the delay has passed
    if (cur_millis - prev_millis >= servo_delay) {
      // update prev_millis
      prev_millis += servo_delay;
      if (--pos < 5) {
        needle_move_down_done = true;
      }
      else {
        needle_tilt_servo.write(pos);
        servo_step_move_done = true;
        pos --;
      }
    }
}
}
