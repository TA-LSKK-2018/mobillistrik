#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial NodeSerial (D2, D3);

const char* ssid = "Tes";//put your wifi ssid here.
const char* password = "tes12345";//put your wifi password here.
const char* mqtt_server = "m11.cloudmqtt.com";
const char* clientname = "DataSensor";
const char* userName = "xmootvdc";
const char* passWord = "6eixWyYWOXua";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  Serial.begin(115200);
  NodeSerial.begin(4800);

  setup_wifi();
  client.setServer(mqtt_server, 14286);
  client.setCallback(callback);

}

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientname,userName,passWord))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("OsoyooCommand");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()



void loop() {
  // put your main code here, to run repeatedly:
 while (NodeSerial.available() > 0){
    float suhu_bat1 = NodeSerial.parseFloat();
    float suhu_bat2 = NodeSerial.parseFloat();
    float suhu_motor = NodeSerial.parseFloat();
    float batteryVoltage1 = NodeSerial.parseFloat();
    float Amps = NodeSerial.parseFloat();
    float total = NodeSerial.parseFloat();
    float motor_rpm = NodeSerial.parseFloat();

    
    if (NodeSerial.read() == '\n'){
      Serial.print("Suhu_bat1 = ");Serial.print(suhu_bat1);
      Serial.print("\t Suhu_bat2 = ");Serial.print(suhu_bat2);
      Serial.print("\t Suhu_motor = ");Serial.print(suhu_motor); 
      Serial.print("\t V_batt = ");Serial.print(batteryVoltage);
      Serial.print("\t Arus dc = ");Serial.print(Amps);
      Serial.print("\t Arus ac = ");Serial.print(total);
      Serial.print("\t RPM motor = ");Serial.print(motor_rpm);
      Serial.print("*");
      
      delay(100);

      if (NodeSerial.read() == '*'){
      
        String strsuhu_bat1 = String(suhu_bat1);
        String strsuhu_bat2 = String(suhu_bat2);
        String strsuhu_motor = String(suhu_motor);
        String strbattVoltage = String(batteryVoltage);
        String strAmps = String(Amps);
        String strtotal = String(total);
        String strmotor_rpm = String(motor_rpm);

        
        char attributesuhu_bat1[10];
        strsuhu_bat1.toCharArray( attributesuhu_bat1, 10 );
        char attributesuhu_bat2[10];
        strsuhu_bat2.toCharArray( attributesuhu_bat2, 10 );
        char attributesuhu_motor[10];
        strsuhu_motor.toCharArray( attributesuhu_motor, 10 );
        char attributebattVoltage[10];
        strbattVoltage.toCharArray( attributebattVoltage, 10 );
        char attributestrAmps[10];
        strAmps.toCharArray( attributeAmps, 10 );
        char attributestrtotal[10];
        strtotal.toCharArray( attributetotal, 10 );
        char attributemotor_rpm[10];
        strmotor_rpm.toCharArray( attributemotor_rpm, 10 );

        client.publish("Suhu_batt1", attributesuhu_bat1);
        client.publish("Suhu_batt2", attributesuhu_bat2);
        client.publish("Suhu_motor", attributesuhu_motor);
        client.publish("V_batt1", attributebattVoltage);
        client.publish("Arus dc", attributeAmps);
        client.publish("Arus ac", attributetotal);
        client.publish("RPM Motor", attributemotor_rpm);
        
        Serial.println(attributesuhu_bat1);
        Serial.println(attributesuhu_bat2);
        Serial.println(attributesuhu_motor);
        Serial.println(attributebattVoltage);
        Serial.println(attributeAmps);
        Serial.println(attributetotal);
        Serial.println(attributerpm);
        Serial.println("");
      
      }
      
      if (!client.connected()) {
          reconnect();
        }
      client.loop();
    }
 }
}
