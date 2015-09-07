
#include <NewSoftSerial.h>

int inByte = 0;      // Data coming in from computer to ARduino

byte power = 0;    // Initializing power, steering, direction.
byte steer = 0;
byte direc = 0;

NewSoftSerial mySerial(3, 2);

void setup()  
{
  Serial.begin(9600);
  // set the data rate for the NewSoftSerial port
  mySerial.begin(9600);
}



void loop()                     // run over and over again
{
  if (Serial.available() > 0) {    // Take steering information in
    // get incoming byte:          
    // w - forward
    // a - left
    // d - right
    // z - reverse
    inByte = Serial.read();
    Serial.println((char)inByte);
    if (inByte == 'w') {
      power = 10;
      direc = 1;
    }
    if (inByte == 'a') steer = -70;  // Turn left
    if (inByte == 'd') steer = 70;   // Turn right
    if (inByte == 'z') {             // Reverse
      power = 10;
      direc = 0;
    }
    if (inByte == 's') power = 0;    // Stop
    if (inByte == 'q') {             // Go straigh
      steer = 0;
    }
  }
  //char data[] = {NULL, 100, 50, 30};
  //mySerial.print(data[0]);

  
  mySerial.print('/');
                          // RobotC now knows the next two transmissions will be power and steering respectively.

  mySerial.print(power);  // Send Power
  mySerial.print(steer);  // Send steering info.
  mySerial.print(direc);
  delay(270);
  

}
