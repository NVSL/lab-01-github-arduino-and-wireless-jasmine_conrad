/*
  ATmega128RFA1 Dev Board Basic Chat
  by: Jim Lindblom
      SparkFun Electronics
  date: July 3, 2012
  License: Beerware. Feel free to use, reuse, and modify this code
  as you please. If you find it useful, you can buy me a beer.

  This code sets up the ATmega128RFA1's wireless transciever in
  the most basic way possible to serve as a serial gateway.
  Serial into the ATmega128RFA1's UART0 will go out the RF radio.
  Data into the RF radio will go out the MCU's UART0.
*/

#define FIVE_SECONDS 5000000
#define DATA 'c'
#include "RadioFunctions.h"

unsigned int counter;

void setup()
{
  Serial.begin(9600);  // Start up serial
  Serial1.begin(115200);
  rfBegin(11);  // Initialize ATmega128RFA1 radio on channel 11 (can be 11-26)
  
  // Send a message to other RF boards on this channel
  rfPrint("ATmega128RFA1 Dev Board Online!\r\n");
  
  counter = 0;
}

void loop()
{ 
  if (Serial.available())  // If serial comes in...
  {
    char tmp = Serial.read();
    
    // Check received bytes
    if(tmp == 'r'){
      Serial.print("The number of bytes received: ");
      Serial.println(counter);
      counter = 0;
    }
    
    // Transmit bytes for 5 seconds
    else if(tmp == 't'){
      long time_end = micros()+FIVE_SECONDS;
      long time_cur = micros();
      while(time_cur < time_end){
        rfWrite(DATA);
        counter++;
        time_cur = micros();
      }
      Serial.print("The number of bytes sent: ");
      Serial.println(counter);
      counter = 0;
    }
  }

  if (rfAvailable())  // If data receievd on radio...
  {
    // Counting successfully received bytes
    if(DATA == rfRead())
      counter++;
  }
}
