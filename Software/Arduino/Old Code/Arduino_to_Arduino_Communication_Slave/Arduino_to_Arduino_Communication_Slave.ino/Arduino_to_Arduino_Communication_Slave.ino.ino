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
const int min_pos = 20;
const int max_pos = 100;
float servo_step = 1; // step size for servo sweep
unsigned int servo_delay = 20;
unsigned int prev_millis = 0;
unsigned int cur_millis = millis();
bool go = false;


void setup() {
  Serial.begin(9600);           // start serial for output
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event

  needle_tilt_servo.attach(6);
  needle_tilt_servo.write(min_pos);

}


void loop() {
  delay(100);
  if (go) {
    Serial.println("Go!");
    new_song_tilt(needle_tilt_servo, servo_delay, min_pos, max_pos);
    go = false;
    Serial.println("go is false");
  }
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
  if (x % 20 == 2) {
    go = true;
    Serial.println("Go is true");

    // new_song_tilt(needle_tilt_servo, servo_delay, min_pos, max_pos);
  }
}

void new_song_tilt(Servo needle_tilt_servo, unsigned int servo_delay, const int min_pos, const int max_pos) {
  tilt_up(needle_tilt_servo, servo_delay, min_pos, max_pos);
  tilt_down(needle_tilt_servo, 100, min_pos, max_pos);
}

void tilt_up(Servo needle_tilt_servo, unsigned int servo_delay, const int min_pos, const int max_pos) {
  unsigned int prev_millis = 0;
  boolean servo_step_move_done = false;
  boolean running_correctly = true;
  boolean needle_move_up_done = false;
  int pos = min_pos;
  while(!needle_move_up_done){
    unsigned int cur_millis = millis(); 
    // if it has, check that the delay has passed
    if (cur_millis - prev_millis >= servo_delay) {
      // update prev_millis
      prev_millis += servo_delay;
      if (++pos > max_pos) {
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

void tilt_down(Servo needle_tilt_servo, unsigned int servo_delay, const int min_pos, const int max_pos) {
  unsigned int prev_millis = 0;
  boolean servo_step_move_done = false;
  boolean running_correctly = true;
  boolean needle_move_down_done = false;
  int pos = max_pos;
  while(!needle_move_down_done){
    unsigned int cur_millis = millis();
    // if it has, check that the delay has passed
    if (cur_millis - prev_millis >= servo_delay) {
      // update prev_millis
      prev_millis += servo_delay;
      if (--pos < min_pos) {
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
