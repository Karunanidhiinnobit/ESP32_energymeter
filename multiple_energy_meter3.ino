#include "EnergyMeter.h"

// Initialize Serial2 (TX2: GPIO 17, RX2: GPIO 16 on ESP32)
HardwareSerial& mySerial = Serial2;

uint8_t slaveID = 1;
int baudRate = 9600;
bool isThreePhase = true;

// Initialize the EnergyMeter object with parameters
EnergyMeter energymeter(mySerial, baudRate);

void setup() {
    // Start Serial2
    mySerial.begin(baudRate, SERIAL_8N1, 16, 17);  // (baud rate, config, RX pin, TX pin)
    Serial.begin(115200);
    
    // Initialize the energy meter
    energymeter.begin(slaveID);
}

void loop() {
     Serial.println("Three Phase Energy Meter Data");
     float valueInput;
     energymeter.readMeterData(1, true);
     for(uint8_t numRegister=0;numRegister<29;numRegister++)
     {
     valueInput=energymeter.valueInInputRegister(numRegister);
     Serial.println(valueInput);
     }
     
     Serial.println("Single Phase Energy Meter Data");
     energymeter.readMeterData(2, false);
     uint16_t valueHolding;
     for(uint8_t numRegister=0;numRegister<8;numRegister++)
     {
     valueHolding=energymeter.valueInHoldingRegister(numRegister);
     Serial.println(valueHolding);
     }
     
   
}
