#include <Servo.h>

Servo needle_tilt_servo; // creates servo object

// Servo move variables
int pos = 0; // position, for servo sweep iterator
const int min_pos = 20;
const int max_pos = 90;
float servo_step = 1; // step size for servo sweep
unsigned int servo_delay = 20;

boolean servo_step_move_done = false;
boolean needle_move_down_done = false;
boolean needle_move_up_done = false;

bool go = false;
bool needle_moved = false;

// Button Variables
const byte button3 = 2; // pause/play button
int button3State = 0;
int button3Counter = 0;
int lastButton3State = 0;

// Millis Variables
//unsigned long prev_millis = 0;
//unsigned long cur_millis;
unsigned long currentMillis = 0;
unsigned long debouncingMillis = 0;

void setup() {
  Serial.begin(9600);           // start serial for output
  Serial.println("begin setup");

  pinMode(button3,INPUT);
  
  needle_tilt_servo.attach(6);
  needle_tilt_servo.write(max_pos);
  
  //new_song_tilt_simple();
  Serial.println("end setup");
}


void loop() {
  button3State = digitalRead(button3);
  currentMillis = millis();
  
  //button3 debounce (play/pause)
  if (currentMillis - debouncingMillis >= 50) {
    Serial.println("debounced");
    //check to see if button has been pressed
    if (button3State != lastButton3State) {
      Serial.println("state change");
      // if the state has changed (button has been pressed), increment the counter
      if (button3State == HIGH) {
        Serial.println("button pressed");
        button3Counter++;
        Serial.print("0");
        Serial.println(button3Counter);
      } 
      //reset the debounce timer
      debouncingMillis = currentMillis;
      if (button3Counter == 2) {
        button3Counter = 0;
      }
    }
  }
  
  if (button3Counter == 0) {
    needle_moved = false;
    }
  if (button3Counter == 1 && !needle_moved) {
    new_song_tilt_simple();
  }
}


void new_song_tilt_simple(){
  Serial.println("Servo Max");
  delay(500);
  needle_tilt_servo.write(min_pos);
  Serial.println("Servo Min");
  delay(500);
}

//void new_song_tilt(Servo needle_tilt_servo, unsigned int servo_delay, const int min_pos, const int max_pos) {
//  tilt_up(needle_tilt_servo, servo_delay, min_pos, max_pos);
//  tilt_down(needle_tilt_servo, 100, min_pos, max_pos);
//}
//
//void tilt_up(Servo needle_tilt_servo, unsigned int servo_delay, const int min_pos, const int max_pos) {
//  servo_step_move_done = false;
//  needle_move_up_done = false;
//  int pos = min_pos;
//  while(!needle_move_up_done){
//    cur_millis = millis(); 
//    // if it has, check that the delay has passed
//    if (cur_millis - prev_millis >= servo_delay) {
//      if (++pos > max_pos) {
//        needle_move_up_done = true;
//      }
//      else {
//        needle_tilt_servo.write(pos);
//        servo_step_move_done = true;
//        pos ++;
//      }
//    }
//    // update prev_millis
//    prev_millis == cur_millis;
//}
//}

//void tilt_down(Servo needle_tilt_servo, unsigned int servo_delay, const int min_pos, const int max_pos) {
//  servo_step_move_done = false;
//  needle_move_down_done = false;
//  int pos = max_pos;
//  while(!needle_move_down_done){
//    cur_millis = millis();
//    // if it has, check that the delay has passed
//    if (cur_millis - prev_millis >= servo_delay) {
//  
//      if (--pos < min_pos) {
//        needle_move_down_done = true;
//      }
//      else {
//        needle_tilt_servo.write(pos);
//        servo_step_move_done = true;
//        pos --;
//      }
//      // update prev_millis
//      prev_millis == cur_millis;
//    }
//}
//}
