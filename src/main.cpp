#include <Arduino.h>
#include <SPI.h>

#define PIN_SEN (15)

#define SL900A_SPI_DIRECT_CMD_RESET (0b11000000)
#define SL900A_SPI_DIRECT_CMD_GET_TEMPERATURE (0b11000001)
#define SL900A_SPI_DIRECT_CMD_GET_BATTERY (0b11000010)
#define SL900A_SPI_DIRECT_CMD_GET_EXT_SENSOR_1 (0b11000011)
#define SL900A_SPI_DIRECT_CMD_GET_EXT_SENSOR_2 (0b11000100)
#define SL900A_SPI_DIRECT_CMD_GET_EXT_SENSOR_2 (0b11000100)
#define SL900A_SPI_DIRECT_CMD_START_LOGGING (0b11000101)
#define SL900A_SPI_DIRECT_CMD_STOP_LOGGING (0b11000110)
#define SL900A_SPI_DIRECT_CMD_READ_FIFO_STATUS (0b11000111)
#define SL900A_SPI_DIRECT_CMD_READ_REMAINING_SHELF_LIFE (0b11001000)
#define SL900A_SPI_DIRECT_CMD_READ_FIFO (0b11100000)
#define SL900A_SPI_DIRECT_CMD_WRITE_FIFO (0b11100001)

float GetTemperature() {
  int receivedVal = 0;
  int trash = 0;
  digitalWrite(PIN_SEN, HIGH);  // SEN (/cs) high to start coms
  delay(150);  // critical delay
  trash = SPI.transfer(SL900A_SPI_DIRECT_CMD_GET_TEMPERATURE);
  delay(10);  // critical delay
  receivedVal = SPI.transfer16(0x0000);  // receive data, 16 pulses
  digitalWrite(PIN_SEN, LOW);  // SEN (/cs) low after coms complete
  return receivedVal * .18 - 89.3; // temperature conversion
}

float GetBattery() {
  int receivedVal = 0;
  int trash = 0;
  digitalWrite(PIN_SEN, HIGH);  // SEN (/cs) high to start coms
  delay(150);  // critical delay
  trash = SPI.transfer(SL900A_SPI_DIRECT_CMD_GET_BATTERY);
  delay(10);  // critical delay
  receivedVal = SPI.transfer16(0x0000);  // receive data, 16 pulses
  digitalWrite(PIN_SEN, LOW);  // SEN (/cs) low after coms complete
  return receivedVal * .00165 + 1.69;  // battery conversion
}

float GetExtSensor1() {
  int receivedVal = 0;
  int trash = 0;
  digitalWrite(PIN_SEN, HIGH);  // SEN (/cs) high to start coms
  delay(150);  // critical delay
  trash = SPI.transfer(SL900A_SPI_DIRECT_CMD_GET_EXT_SENSOR_1);
  delay(10);  // critical delay
  receivedVal = SPI.transfer16(0x0000);  // receive data, 16 pulses
  digitalWrite(PIN_SEN, LOW);  // SEN (/cs) low after coms complete
  return receivedVal * .000310 + .310;  // voltage conversion
}

float GetExtSensor2() {
  int receivedVal = 0;
  int trash = 0;
  digitalWrite(PIN_SEN, HIGH);  // SEN (/cs) high to start coms
  delay(150);  // critical delay
  trash = SPI.transfer(SL900A_SPI_DIRECT_CMD_GET_EXT_SENSOR_2);
  delay(10);  // critical delay
  receivedVal = SPI.transfer16(0x0000);  // receive data, 16 pulses
  digitalWrite(PIN_SEN, LOW);  // SEN (/cs) low after coms complete
  return receivedVal * .000310 + .310;  // voltage conversion
}


void setup() 
{
  Serial.begin(115200);
  SPI.begin();
  while(!Serial.available()){}
  
  pinMode(PIN_SEN, OUTPUT);  //SEN pin
  digitalWrite(PIN_SEN, LOW);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
  // 1MHZ SPI, SPI mode1 (sample on falling edge)

  int receivedVal = 0;
  int trash = 0;
  /*
  digitalWrite(PIN_SEN, HIGH);              // SEN (/cs) high to start coms
  delay(150);                               // critical delay
  trash = SPI.transfer(SL900A_SPI_DIRECT_CMD_RESET);
  delay(10);                                // critical delay
  receivedVal = SPI.transfer16(0x0000);     // receive data, 16 pulses
  digitalWrite(PIN_SEN, LOW);               // SEN (/cs) low after coms complete
  */

  Serial.print("BATTV: ");
  Serial.print(GetBattery());
  Serial.print("\t");
  Serial.print("TEMPC: ");
  Serial.print(GetTemperature());
  Serial.print("\t");
  /*
  Serial.print("EXT1: ");
  Serial.print(GetExtSensor1());
  Serial.print("\t");
  Serial.print("EXT2: ");
  Serial.print(GetExtSensor2());
  Serial.print("\t");
  */
  Serial.println();

  receivedVal = 0;
  trash = 0;
  digitalWrite(PIN_SEN, HIGH);  // SEN (/cs) high to start coms
  delay(150);  // critical delay
  trash = SPI.transfer16(0b0100000001100100);  // 0b01 000   000 0110 0100
  // trash = SPI.transfer16(0b0100000010100000);  // 0b01 000   000 1010 0000
  // trash = SPI.transfer16(0b0100000001011010);  // 0b01 000   000 0101 1010
  delay(10);  // critical delay
  int n = 2;
  for (int i=0; i<n; i++) {
    receivedVal = SPI.transfer(0x0000);  // receive data
    for (int j=7; j>=0; j--) {
      Serial.print(bitRead(receivedVal, j));
    }
  }
  digitalWrite(PIN_SEN, LOW);  // SEN (/cs) low after coms complete
  Serial.println();


  receivedVal = 0;
  trash = 0;
  digitalWrite(PIN_SEN, HIGH);  // SEN (/cs) high to start coms
  delay(150);  // critical delay
  trash = SPI.transfer16(0b0100000001100100);  // 0b01 000   000 0110 0100
  // trash = SPI.transfer16(0b0100000010100000);  // 0b01 000   000 1010 0000
  // trash = SPI.transfer16(0b0100000001011010);  // 0b01 000   000 0101 1010
  delay(10);  // critical delay
  n = 2;
  for (int i=0; i<n; i++) {
    receivedVal = SPI.transfer(0x0000);  // receive data
    for (int j=7; j>=0; j--) {
      Serial.print(bitRead(receivedVal, j));
    }
  }
  digitalWrite(PIN_SEN, LOW);  // SEN (/cs) low after coms complete
  Serial.println();

  delay(1000);
}

void loop() { 
}