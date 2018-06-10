#include <SPI.h>
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>

#include <require_cpp11.h>

#define RST_PIN 5
#define SS_PIN 53
#define BUTTON_PIN 8
MFRC522 mfrc(SS_PIN,RST_PIN);

#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}

MFRC522::MIFARE_Key key;
MFRC522::MIFARE_Key key2;
void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc.PCD_Init();
  mfrc.PCD_AntennaOn();
  Serial.println(F("Gonna rewrite UID!"));

  for (byte i=0;i<6;i++){
    key.keyByte[i] = 0xFF;
    key2.keyByte[i] = 0xFF;
  }
  Serial.print(F("Using Key:"));
  dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
}
bool writeNewUid = false;
void loop() {
  if (digitalRead(BUTTON_PIN) == LOW){
    if (writeNewUid == false){
      writeNewUid = true;
      Serial.println("Ready to Write");
    }
  }
  // put your main code here, to run repeatedly:
  if (!mfrc.PICC_IsNewCardPresent() || ! mfrc.PICC_ReadCardSerial() ){
    delay(50);
    return;
  }
  
  Serial.print(F("Card UID:"));
  for (byte i = 0; i<mfrc.uid.size;i++){
    Serial.print(mfrc.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc.uid.uidByte[i], HEX);
  }
  Serial.println();

    // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc.PICC_GetType(mfrc.uid.sak);
  Serial.print(F("PICC type: "));
  Serial.print(mfrc.PICC_GetTypeName(piccType));
  Serial.print(F(" (SAK "));
  Serial.print(mfrc.uid.sak);
  Serial.print(")\r\n");
  if (  piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
    &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
    &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("This sample only works with MIFARE Classic cards."));
    return;
  }
  /*
  if (writeNewUid){
    mfrc.MIFARE_UnbrickUidSector(true);
    
    byte newUid[] = NEW_UID;
    if (mfrc.MIFARE_SetUid(newUid, (byte)4, true)){
      Serial.println(F("Wrote New UID"));
    }
    writeNewUid = false;
    mfrc.PICC_HaltA();
    if ( ! mfrc.PICC_IsNewCardPresent() || ! mfrc.PICC_ReadCardSerial() ) {
      return;
    }
  }
  */
  byte sector = 1;
  
  byte blockAddrStart = 4;
  int blockSize = 16;
  String data = "You are bad lululul";
  byte dataBytes[blockSize];
  data.getBytes(dataBytes, blockSize);
  byte trailerBlock = 7;
  
  Serial.println(F("Authenticating"));
  MFRC522::StatusCode status;
  status = (MFRC522::StatusCode) mfrc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc.GetStatusCodeName(status));
    return;
  }

  Serial.println(F("Current data in sector:"));
  mfrc.PICC_DumpMifareClassicSectorToSerial(&(mfrc.uid), &key, sector);
  Serial.println();


  byte buffer[18];
  byte size = sizeof(buffer);
  Serial.print(F("Reading data from block ")); Serial.print(blockAddrStart);
  Serial.println(F(" ..."));
  status = (MFRC522::StatusCode) mfrc.MIFARE_Read(blockAddrStart, buffer, &size);
  if (status != MFRC522::STATUS_OK){
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc.GetStatusCodeName(status));
  }
  Serial.print(F("Data in block ")); Serial.print(blockAddrStart);  Serial.println(F(":"));

  dump_byte_array(buffer, 16); Serial.println(" : "+String((char *)buffer)); Serial.println();
  Serial.println();
  status = (MFRC522::StatusCode) mfrc.MIFARE_Read(7, buffer, &size);
  if (status != MFRC522::STATUS_OK){
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc.GetStatusCodeName(status));
  }
  Serial.print(F("Data in block ")); Serial.print(7);  Serial.println(F(":"));

  
  dump_byte_array(buffer, 16); Serial.println(" : "+String((char *)buffer)); Serial.println();
  Serial.println();

  Serial.println(F("Authenticating again using key B..."));
  status = (MFRC522::StatusCode) mfrc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key2, &(mfrc.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc.GetStatusCodeName(status));
    return;
  }

  Serial.print(F("Writing data into block ")); Serial.print(blockAddrStart);
  Serial.println(F(" ..."));
  dump_byte_array(dataBytes, 16); Serial.println();
  status = (MFRC522::StatusCode) mfrc.MIFARE_Write(blockAddrStart, dataBytes, 16);
  if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc.GetStatusCodeName(status));
  }
  Serial.println();
  //Access bits for A = Read, B = Read Write
  byte newKey[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x78, 0x77, 0x88, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
  if (writeNewUid){
    Serial.print(F("Writing New Trailer: ")); Serial.print(7);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc.MIFARE_Write(7, newKey, 16);
     if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc.GetStatusCodeName(status));
     }
    Serial.println(); 
    writeNewUid = false;
  }
  // Halt PICC
  mfrc.PICC_HaltA();
    // Stop encryption on PCD
  mfrc.PCD_StopCrypto1();
  delay(3000);
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }

}
