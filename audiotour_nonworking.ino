#include <SPI.h>
#include <MFRC522.h>
#include <SdFat.h>
#include <SdFatUtil.h> 
#include <SFEMP3Shield.h>
#include <Servo.h> 

class Leg {
public:
  float angle=0,speed=0;
  Servo servo;  
  void update() {
    servo.write(90+angle);
  } 
} front,back;

// Setup SPI-pins for RFID shield avoiding conflict with the MP3 shield, see https://learn.sparkfun.com/tutorials/mp3-player-shield-hookup
#define SS_PIN 10
#define RST_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);
SFEMP3Shield MP3player;
SdFat sd;
unsigned long lastSeen = 0;
bool playing = false;
bool critter = true;

void setup() {
  back.servo.attach(3);
  front.servo.attach(4);

  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  sd.begin(SD_SEL, SPI_HALF_SPEED);
  MP3player.begin();
  MP3player.setVolume(0x0000,0x0000); // Set volume at maximum
  Serial.begin(115200);
}

void loop() {
  
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    lastSeen = millis();
    
    if (!playing) {
      mfrc522.PICC_HaltA();
      startAudio();
      critter = false;
    }
  }
  
  if (millis()-lastSeen>5000) {
    if (playing) {
      stopAudio();
      critter = true;
    }
  }
  
  if (critter) {
    updateCritter();
  }
  
//  Serial.println(millis()-lastSeen);
  
//  
//    Serial.println("start track");
//  MP3player.pauseDataStream(); // disable interrupts to avoid collisions on the SPI bus between RFID reader and MP3player
//    mfrc522.PICC_HaltA();
//    
//    MP3player.playTrack(1);
//  }
//  

//  else {
//    Serial.println(MP3player.getState()); //currentPosition()); 
//  }

//  MP3player.resumeDataStream(); //enable interrupts
}


void startAudio() {
  Serial.println("startAudio");
  playing = true;
  MP3player.stopTrack();
  MP3player.playTrack(1);
  MP3player.resumeDataStream(); //enable interrupts
}


void stopAudio() {
  Serial.println("stopAudio");
  MP3player.stopTrack();
  playing = false;
}

void updateCritter() {
// Serial.println("not playing");
  back.angle = random(-40,40);
  back.update();

  front.angle = random(-40,40);
  front.update();
}

void dumpUID() {
  // Dump UID
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  } 
  Serial.println();
}
