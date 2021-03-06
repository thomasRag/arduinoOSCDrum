#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>

EthernetUDP Udp;


/*
  CONFIGURATION ARDUINO (à adapter)
*/

// mode osc ou serial pour les test
const int isOSCmode = 0; // mode 0: serial, mode 1: osc

// Arduino IP et mac adress
IPAddress ip(192, 168, 0, 7);
byte mac[] = { 0xDE, 0xAD, 0xAE, 0xEF, 0xFE, 0xED };

// isadora IP et port
int isDHCP = 0;
IPAddress outIp(192, 168, 0, 13);
const unsigned int outPort = 1234;
/*
  CONFIGURATION CAPTEUR (à adapter)
*/
// nombre de piezo devant être lu
int nbSensors = 6;

// threshold : valeurs seuils à laquel les capteurs doivent envoyer une valeur. (un par type de capteur)
const int threshold[] = {200,200, 200, 200,200,200};

int customDelay = 20;

/*
  CONFIGURATION PROGRAMME (do not change)
*/
int triggerable[] = {};

void  initializeOSC(){
  Serial.println("OSC test");
  if (isDHCP == 1 && Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  else {
    Ethernet.begin(mac, ip);
  }
  // print your local IP address:
  Serial.print("Arduino IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Udp.begin(8888);
}

void sendOSC(int sensorReading, int pin){

 if (pin == 0){
   OSCMessage msg("/switch/0");
   msg.add((int32_t)sensorReading);
   Udp.beginPacket(outIp, outPort);
   msg.send(Udp); // send the bytes to the SLIP stream
   Udp.endPacket(); // mark the end of the OSC Packet
   msg.empty(); // free space occupied by message
 }
 if (pin == 1){
   OSCMessage msg("/switch/1");
   msg.add((int32_t)sensorReading);
   Udp.beginPacket(outIp, outPort);
   msg.send(Udp); // send the bytes to the SLIP stream
   Udp.endPacket(); // mark the end of the OSC Packet
   msg.empty(); // free space occupied by message
 }
 if (pin == 2){
   OSCMessage msg("/switch/2");
   msg.add((int32_t)sensorReading);
   Udp.beginPacket(outIp, outPort);
   msg.send(Udp); // send the bytes to the SLIP stream
   Udp.endPacket(); // mark the end of the OSC Packet
   msg.empty(); // free space occupied by message
 }
//  if (pin == 3){
//    msg.setAddress("/switch/3");
//  }
//  if (pin == 4){
//    msg.setAddress("/switch/4");
//  }
//  if (pin == 5){
//    msg.setAddress("/switch/5");
//  }
}

void sendAnalog(int pin){
  int sensorReading = analogRead(pin);
  if ( sensorReading > threshold[pin]){
    Serial.print( "switch");
    Serial.print(pin);
    Serial.print( ":");
    Serial.println(sensorReading);
    if (isOSCmode) {
      sendOSC(sensorReading, pin);
    }
  };
}

void setup() {
  Serial.begin(9600);
  if(isOSCmode){
      initializeOSC();
  }
  Serial.print("nbSensors:");
  Serial.println(nbSensors);
  for(int i=0; i<nbSensors; i++){
    triggerable[i]= 0;
  }
}


void loop(){
  for(int i=0; i<nbSensors; i++){
    sendAnalog(i);
  }
   delay(customDelay);
}
