/* Record Needle Tilt
 *  One Servo with function that tilts from horizontal to vertical
 *  by Sophia Nielsen
 *  modified 6 Nov 2017
 */

#include <Servo.h>

Servo needle_tilt_servo; // creates servo object

int pos = 0;

void setup() {
  needle_tilt_servo.attach(5); // attaches servo to pin 5
}

void loop() {
  // put your main code here, to run repeatedly:
  new_song_tilt();
  delay(1000);
}

int new_song_tilt() {
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    needle_tilt_servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    needle_tilt_servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

