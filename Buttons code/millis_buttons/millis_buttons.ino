/* 
 *  Mic Buttons
 */
const byte button1 = 2; //green
const byte button2 = 3; //blue
const byte greenled = 7; 
const byte blueled = 9;

int button1Counter = 0;
int button2Counter = 0;
int greenledState = LOW;
int blueledState = LOW;
int button1State = 0;
int button2State = 0;
int lastButton1State = 0;
int lastButton2State = 0;

// will store last time LED was updated
unsigned long previousMillis = 0;        
unsigned long currentMillis = 0;
unsigned long debouncingMillis = 0;

// constants won't change
// interval at which to blink (milliseconds)
const long interval1 = 500;           
const long interval2 = 200;

// setup routine runs once when you press reset (only runs once)
void setup() {
    Serial.begin(9600);
    pinMode(button1,INPUT);
    pinMode(button2,INPUT);
    pinMode(greenled,OUTPUT);
    pinMode(blueled,OUTPUT);
}

// put your main code here, to run repeatedly
void loop(){
    // read the pushbutton input pin:
    button1State = digitalRead(button1);
    button2State = digitalRead(button2);
    // start timer
    currentMillis = millis();

  //button1 debounce
  if (currentMillis - debouncingMillis >= 50) {
    //check to see if button has been pressed
    if (button1State != lastButton1State) {
      // if the state has changed (button has been pressed), increment the counter
      if (button1State == HIGH) {
        // increment counter to change the LED mode
        button1Counter++;
        Serial.print("1");
        Serial.println(button1Counter);
        button2Counter = 0;
      } 
      //reset the debounce timer
      debouncingMillis = currentMillis;
    }
  }
  //button2 debounce
  if (currentMillis - debouncingMillis >= 50) {
    //check to see if button has been pressed
    if (button2State != lastButton2State) {
      // if the state has changed (button has been pressed), increment the counter
      if (button2State == HIGH) {
        // increment counter to change the LED mode
        button2Counter++;
        Serial.print("2");
        Serial.println(button2Counter);
        button1Counter = 0;
      } 
      //reset the debounce timer
      debouncingMillis = currentMillis;
    }
  }
  if(button1Counter == 1){    
    digitalWrite(greenled,HIGH);
  }
  if(button1Counter % 2 == 0){
    digitalWrite(greenled,LOW);
    button1Counter = 0;
  }
  if(button2Counter == 1) {      
    digitalWrite(blueled,HIGH);
  }
  if(button2Counter % 2 == 0) {
    digitalWrite(blueled,LOW);
    button2Counter = 0;
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButton1State = button1State;
  lastButton2State = button2State;
 }
  



