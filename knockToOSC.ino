/*
   Make an OSC message and send it over UDP

   Adrian Freed
*/
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>

EthernetUDP Udp;

// define what will be the communication mode
const int isOSCmode = 0; // mode 0: serial, mode 1: osc

//the Arduino's IP
IPAddress ip(192, 168, 0, 7);
// isadora IP. todo: receive it by udp ?
IPAddress outIp(10, 0, 1, 13);
const unsigned int outPort = 1234;
byte mac[] = { 0xDE, 0xAD, 0xAE, 0xEF, 0xFE, 0xED }; // you can find this written on the board of some Arduino Ethernets or shields

//const int knockSensors[] = {0,1};
const int threshold[] = {300,500};  // threshold value to decide when the detected sound is a knock or not

int nbSensors = 2;

void  initializeOSC(){
  Serial.println("OSC test");
  // Ethernet.begin(mac, ip);
  if (Ethernet.begin(mac) == 0) {
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
  //this line needed 
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
 //this setup section of code from http://trippylighting.com/teensy-arduino-ect/touchosc-and-arduino-oscuino/
 //this to begin serial
  Serial.begin(9600); //9600 for a "normal" Arduino board (Uno for example). 115200 for a Teensy ++2
  if(isOSCmode){
      initializeOSC();
  }
  Serial.print("nbSensors:");
  Serial.println(nbSensors);
}


void loop(){
  for(int i=0; i<nbSensors; i++){
    sendAnalog(i);
  }
   delay(100);
}




