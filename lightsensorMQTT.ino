#include <PubSubClient.h>
#include <EthernetClient.h>
#include <Ethernet.h>

const int LDR = A0;
const int LED =  8;
int LEDswitch = LOW;
int voltage;
bool oldLedHigh = false;

byte mac[]= { 0x90,0xA2,0xDA,0x0D,0x01,0x8B};

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

const char *host = "37.97.180.203";
uint16_t port = 1883;
const char *topic = "meterkastTVM";

void mqtt_callback (char* topic, byte* payload, unsigned int length) {

   char pl[40];
   memset(pl, '\0', length); // Waarom dit?
   strncpy(pl, (char *)payload, length);
}

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac);
  delay(2000);
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
}

void mqttConnect() {
   
   // Connect to mqtt broker and set callback
   mqttClient.setClient(ethClient);
   mqttClient.setServer(host, port);
   
   if(mqttClient.connect("_12345_RANDOM_UNIQUE_54321")) {

      // Subscribe to .../char [kunnen er meerdere zijn]
      mqttClient.subscribe(topic);

      // Set callback
      mqttClient.setCallback(mqtt_callback);
      Serial.println("connected");
   } else {
      Serial.println("not connected");
   }
}

void pulse()
{
  voltage = analogRead(LDR);
  Serial.println(voltage);
  
  if (voltage >= 250)
  {
    LEDswitch = HIGH;
    if(!oldLedHigh){
      mqttClient.publish(topic,'1');  
    }
    oldLedHigh = true; 
  }
  else
  {
    oldLedHigh = false;
    LEDswitch = LOW;
  }

  digitalWrite(LED, LEDswitch);
}

void loop()
{
    if( !mqttClient.connected() ) {
      mqttConnect();
      delay(250);
    } else {
      mqttClient.loop();
      pulse();
    }
  
//    voltage = analogRead(LDR);
//    if(voltage >= 200)
//    {
//       mqttClient.publish(topic,'1');
//    }
 //}

}



