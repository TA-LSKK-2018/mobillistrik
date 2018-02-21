/*
This is for use with Sparkfun's CAN Bus Shield: https://www.sparkfun.com/products/10039
*/

#include <Canbus.h>
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>


void setup() {
  Serial.begin(9600);
  Serial.println("CAN Write - Testing transmission of CAN Bus messages");
  delay(1000);
  
  if(Canbus.init(CANSPEED_125))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");
    
  delay(1000);
}

void loop() 
{
  long int x = 1234;
  tCAN message;
        message.id = 0x111; //formatted in HEX
        message.header.rtr = 0;
        message.header.length = 8; //formatted in DEC
        for (int i = 0 ; i < message.header.length; i++){
          message.data[i] = x%100;
          x = x/100;
      	//message.data[1] = 00;
      	//message.data[2] = 56;
      	//message.data[3] = 78; //formatted in HEX
      	//message.data[4] = 0x00;
      	//message.data[5] = 0x40;
      	//message.data[6] = 0x00;
      	//message.data[7] = 0x00;
        }
        mcp2515_bit_modify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), 0);
        mcp2515_send_message(&message);
        for (int i = 0 ; i < message.header.length; i++){
        Serial.println(message.data[i],DEC);

        }
        Serial.println(x/100,DEC);
        Serial.println(x%100,DEC);
       // Serial.print("  ");
        //Serial.println(x/100);
        //Serial.println(message.id);
        //Serial.println(message.header.rtr);
        //Serial.println(message.data[3]);
        

        delay(1000);
}
