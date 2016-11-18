#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>

SdFat sd;
SFEMP3Shield MP3player;
EthernetUDP Udp;

/*
  CONFIGURATION CAPTEUR (à adapter)
*/
// nombre de piezo devant être lu
int nbSensors = 6;

// threshold : valeurs seuils à laquel les capteurs doivent envoyer une valeur. (un par type de capteur)
const int threshold[] = {100,100, 200, 200,200,200};

int customDelay = 20;

/*
  CONFIGURATION PROGRAMME (do not change)
*/
int triggerable[] = {};

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
  Serial.print("thresholds:");
  for(int i=0; i<nbSensors; i++){
   Serial.println(threshold[i]);
  }

  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");
  
  //Initialize the MP3 Player Shield
  uint8_t result = MP3player.begin();
  //check result, see readme for error codes.
  if(result != 0) {
    Serial.print(F("Error code: "));
    Serial.print(result);
    Serial.println(F(" when trying to start MP3 player"));
    if( result == 6 ) {
      Serial.println(F("Warning: patch file not found, skipping.")); // can be removed for space, if needed.
      Serial.println(F("Use the \"d\" command to verify SdCard can be read")); // can be removed for space, if needed.
    }
  }
}

void loop(){
  for(int i=0; i<nbSensors; i++){
    int sensorReading = analogRead(i);
    if ( sensorReading > threshold[i]){
      if (triggerable[i] == 1){
        MP3player.playTrack(i+1);
        Serial.print("playing");
        triggerable[i] = 0;
      }
    }
    else {
      triggerable[i] = 1;
    }
  }
   delay(customDelay);
}
