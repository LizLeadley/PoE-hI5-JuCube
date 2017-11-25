//* text printer demo
//Works for both lines with delay
#include <LiquidCrystal_I2C.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x3E, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int startstring = 0;      // recognition of beginning of new string
int charcount = 0;        // keeps track of total chars on screen

void setup () {

  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  lcd.begin(16,2);        // initialize the lcd as 20x4 (16,2 for 16x2)
  lcd.home ();            // go home to character 0 on line 0 (1st line)
  lcd.print(" Text Printer   ");  
  lcd.setCursor(0,1);     // character 0 on line 1 (2nd line)
  lcd.print ("   Version 02 ");

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);  

  lcd.setCursor(0,0);     // Set cursor position to top left corner
  delay(3000);
  //  Serial.println("Hello, Hello");

}  

void loop() {
  char incomingByte = 0;                             // for incoming serial data

  if (Serial.available() )
  {                                                  // Check for incoming Serial Data
    digitalWrite(13, HIGH);
    incomingByte = Serial.read();
    if ((incomingByte == '~') && (startstring == 1)) // Check for closing '~' to end printing of serial data      
    {
      startstring = 0;                               // Set the printing to off
      delay(5000);                                   // Wait 5 seconds
      lcd.clear();                                   // Wipe the screen
      charcount = 0;                                 // reset the character count to 0
      lcd.setCursor(0,0);                            // reset the cursor to 0,0
    }

    if (startstring == 1){                           // check if the string has begun if first '~' has been read
      if (charcount <= 30){                          // check if charcount is under or equal to 30
        lcd.print(incomingByte);                     // Print the current byte in the serial
        charcount++;                                 // Increment charcount
      }
    }

    if (charcount == 16){                            // if the charcount is equal to 16 aka end first line
      lcd.setCursor(0,1);                            // set cursor to second line
    }

    if (charcount == 31){                            // if the charcount is equal to 31 aka the screen is full
      delay(500);
      lcd.clear();                                   // clear screen
      lcd.setCursor(0,0);                            // set cursor to 0,0
      lcd.print(incomingByte);                       // continue printing data
      charcount = 1;                                 // set charcount back to 1
    }

    if (incomingByte == '~'){                        // Check if byte is marker ~ to start the printing
      startstring = 1;                               // start printing
    }  
  }
  digitalWrite(13, LOW);
  delay(10);                                         // 10ms delay for stability
}
