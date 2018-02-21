// Henry's Bench
// 1st CAN Network - CAN TRANSMIT
#include <Canbus.h>
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>
#include <SPI.h>
#include <SD.h>      

// Chip Select pin is tied to pin 9 on the SparkFun CAN-Bus Shield
const int chipSelect = 9; 


void setup()
{

  Serial.begin(9600);
  
  Serial.println("CAN Write - Testing transmission of CAN Bus messages");
  delay(1000);
  
  if(Canbus.init(CANSPEED_125))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");
    
  delay(500);
    
    Serial.print("Initializing SD card...");
    // make sure that the default chip select pin is set to
    // output, even if you don't use it:
    pinMode(chipSelect, OUTPUT);
  
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      return;
    }
    Serial.println("card initialized.");
}

void loop()
{   

      tCAN message;

        message.id = 0x111; //formatted in HEX
        message.header.rtr = 0;
        message.header.length = 8; //formatted in DEC
        
        message.data[0] = 0x40;
        message.data[1] = 0x05;
        message.data[2] = 0x30;
        message.data[3] = 0x12; //formatted in HEX
        message.data[4] = 0x00;
        message.data[5] = 0x40;
        message.data[6] = 0x00;
        message.data[7] = 0x00;

        mcp2515_bit_modify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), 0);
        mcp2515_send_message(&message);

        Serial.println("pesan terkirim");
        //Serial.println(message.id);
        //Serial.println(message.header.rtr);
        //Serial.println(message.data[3]);
        

        delay(1000);

    File dataFile = SD.open("can.txt", FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile)   {  
               dataFile.print("ID: ");
               dataFile.print(message.id,HEX);
               dataFile.print(", ");
               dataFile.print("Data: ");
               dataFile.print(message.header.length,DEC);
               dataFile.print(" ");
               for(int i=0;i<message.header.length;i++) 
                {  
                  dataFile.print(message.data[i],HEX);
                  dataFile.print(" ");
                }
               dataFile.println("");
      dataFile.println(); //create a new row to read data more clearly
      dataFile.close();   //close file
      Serial.println();   //print to the serial port too:
  
    }  
    // if the file isn't open, pop up an error:
    else
    {
      Serial.println("error opening can.txt");
    } 
    delay(500);
}

