
// the setup function runs once when you press reset or power the board
/*
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(10, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(10, HIGH);
  delay(1000);                       // wait for 1000 ms
  digitalWrite(10, LOW);
  delay(1000);      
}
*/


#include <SPI.h>

#define SCALE_FACTOR 0.000152587890625

// 10/(2^16) = 0.000152587890625

#define BUSY 3
#define RESET 4
#define START_CONVERSION 5
#define CHIP_SELECT 10
#define MISO 12
#define SCK 13
#define TOTAL_RAW_BYTES 16

int bytesToRead = TOTAL_RAW_BYTES;  
byte raw[TOTAL_RAW_BYTES];  
signed long parsed[8];

void setup() {  
  pinMode(BUSY, INPUT);
  pinMode(RESET, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(START_CONVERSION, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(CHIP_SELECT, OUTPUT);

  Serial.begin(115200);
  //SPI.begin();
  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
  
  digitalWrite(START_CONVERSION, HIGH);  
  digitalWrite(CHIP_SELECT, HIGH);
  digitalWrite(RESET, HIGH);
  delayMicroseconds(1);
  digitalWrite(RESET, LOW);
}

void loop() {  
  int i;

  digitalWrite(START_CONVERSION, LOW);
  delayMicroseconds(5);
  digitalWrite(START_CONVERSION, HIGH);

  while (digitalRead(BUSY) == HIGH) 
  {
    // wait for conversion to complete
  }
  
  digitalWrite(CHIP_SELECT, LOW);
  while (bytesToRead > 0) 
  {
    raw[TOTAL_RAW_BYTES - bytesToRead] = SPI.transfer(0x00);
    bytesToRead--;
  }
  digitalWrite(CHIP_SELECT, HIGH);
  bytesToRead = TOTAL_RAW_BYTES;

  parseRawBytes();

  //Serial.write(raw, 16);

  for(i=0; i<8; i++) 
  {
    Serial.print((float)parsed[i] * SCALE_FACTOR, 5);
    //Serial.print(raw[i], 5);
    //Serial.print(i, 5);
    Serial.print(",");
  }
  Serial.print("\r\n");
  delay(500);
}

void parseRawBytes() {
  parsed[0] = (raw[0] << 8) + raw[1];
  parsed[1] = (raw[2] << 8) + raw[3];
  parsed[2] = (raw[4] << 8) + raw[5];
  parsed[3] = (raw[6] << 8) + raw[7];
  parsed[4] = (raw[8] << 8) + raw[9];
  parsed[5] = (raw[10] << 8) + raw[11];
  parsed[6] = (raw[12] << 8) + raw[13];
  parsed[7] = (raw[14] << 8) + raw[15];
}
