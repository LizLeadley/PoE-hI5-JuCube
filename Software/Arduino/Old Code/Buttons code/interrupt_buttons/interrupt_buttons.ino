volatile int playPin = 7;
const int button1 = 2;
const int button2 = 3;
int button1counter = 0;
int button2counter = 0;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(button1, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(button1),interruption1,FALLING);
pinMode(button2, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(button2),interruption2,FALLING);
}

void loop() {
  digitalWrite(playPin, HIGH);
  delay(1000);
  digitalWrite(playPin, LOW);
  delay(1000);
}

void interruption1() {
  playPin = 7;
  pinMode(playPin, OUTPUT);
  button1counter++;
  Serial.println(button1counter);
  
}

void interruption2() {
  playPin = 9;
  pinMode(playPin, OUTPUT);
  Serial.println("button2counter");
  
}

