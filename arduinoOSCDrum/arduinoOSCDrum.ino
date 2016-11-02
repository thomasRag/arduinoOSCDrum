#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>

EthernetUDP Udp;


/*
  CONFIGURATION ARDUINO (à adapter)
*/

// mode osc ou serial pour les test
const int isOSCmode = 1; // mode 0: serial, mode 1: osc

// Arduino IP et mac adress
IPAddress ip(192, 168, 0, 7);
byte mac[] = { 0xDE, 0xAD, 0xAE, 0xEF, 0xFE, 0xED };

// isadora IP et port
IPAddress outIp(10, 0, 1, 13);
const unsigned int outPort = 1234;

/*
  CONFIGURATION CAPTEUR (à adapter)
*/
// nombre de piezo devant être lu
int nbSensors = 2;

// threshold : valeurs seuils à laquel les capteurs doivent envoyer une valeur. (une par type de capteur)
const int threshold[] = {300,500};

int customDelay = 20;

/*
  CONFIGURATION PROGRAMME (à adapter)
*/
int triggerable[];

void  initializeOSC(){
  Serial.println("OSC test");
  if (Ethernet.begin(mac, ip) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
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
  OSCMessage msg("/switch/" + pin);
  msg.add((int32_t)sensorReading);
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message
}

void sendAnalog(int pin){
    Serial.print( "switch");
    Serial.print(pin);
    Serial.print( ":");
    Serial.println(sensorReading);
    if (isOSCmode) {
      sendOSC(sensorReading, pin);
    }
}

void setup() {
  Serial.begin(9600);
  if(isOSCmode){
      initializeOSC();
  }
  Serial.print("nbSensors:");
  Serial.println(nbSensors);
  for(int i=0; i<nbSensors; i++){
    triggerable[i]= 1;
  }
}


void loop(){
  for(int i=0; i<nbSensors; i++){
    int sensorReading = analogRead(pin);
    if ( sensorReading > threshold[pin]){
      if (triggerable[i] == 1){
        sendAnalog(i);
        triggerable[i] = 0;
      }
    }
    else {
      triggerable[i] = 1;
    }
  }
   delay(customDelay);
}




