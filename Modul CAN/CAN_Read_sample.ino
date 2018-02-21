
#include <Canbus.h>
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>

int hasil = 0;
int rpm_motor;

//********************************Setup Loop*********************************//

void setup() {
  Serial.begin(115200); // For debug use
  Serial.println("CAN Read - Testing receival of CAN Bus message");  
  delay(1000);
  
  if(Canbus.init(CANSPEED_500))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");
    
  delay(1000);
}

//********************************Main Loop*********************************//

void loop(){
  tCAN message;
  if (mcp2515_check_message()) 
  	{
      if (mcp2515_get_message(&message)) 
  	{           
               //Serial.print("ID: ");
               //Serial.print(message.id,HEX);
               //Serial.print(", ");
               //Serial.print("Data: ");
               //Serial.print(message.header.length,DEC);
               //Serial.print(" ");
              // for(int i=0;i<message.header.length;i++) 
                //{	
                  //Serial.print(message.data[i],DEC);
                  //Serial.print(" ");
                //}
                for(int i=message.header.length-1;i>=0;i--){
                  hasil = hasil + message.data[i];
                  hasil = hasil * 100;
                }
                rpm_motor = hasil / 100;
                Serial.print("rpm_motor : ");
                Serial.print(rpm_motor);
                Serial.println("");
             //}
           }}

}
