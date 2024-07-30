
#include <Arduino.h>
#include "EnergyMeter.h"

#define NUM_METERS 4
bool energymeter_Status[NUM_METERS];

// Array to hold EnergyMeter instances
EnergyMeter meters[NUM_METERS] = {
    {1, Serial2, 9600, true},
    {2, Serial2, 9600, false},
    {3, Serial2, 9600, true},
    {4, Serial2, 9600, false},
   
};

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX on pin 16, TX on pin 17, adjust as needed

    bool initSuccess = true;
    for (int i = 0; i < NUM_METERS; i++) {
        if (!meters[i].begin()) {
            delay(100);
            Serial.print("Failed to initialize energy meter with slave ID ");
            Serial.println(i + 1);
            initSuccess = false;
            if(initSuccess == false)
            {
              energymeter_Status[i]=false;
            }
            else
            {
              energymeter_Status[i]=true;
            }
        }
    }

    if (!initSuccess) {
        Serial.println("Failed to initialize one or more energy meters");
    }
}

void loop() {
    for (int i = 0; i < NUM_METERS; i++) {
        meters[i].readAllInputRegisters();
        meters[i].readAllHoldingRegisters();

        // Check if the meter is three-phase or single-phase
        if (meters[i].isThreePhase()) {
            // Print values from three-phase meter
            for (size_t j = 0; j < sizeof(EnergyMeter::three_Phase_Inputregister) / sizeof(EnergyMeter::three_Phase_Inputregister[0]); ++j) {
                Serial.print("Meter ");
                Serial.print(i + 1);
                Serial.print(" Three-Phase Input Register ");
                Serial.print(EnergyMeter::three_Phase_Inputregister[j]);
                Serial.print(": ");
                Serial.println(meters[i].getInputRegisterValue(j));
                delay(100);
            }

            for (size_t j = 0; j < sizeof(EnergyMeter::three_Phase_Holdingregister) / sizeof(EnergyMeter::three_Phase_Holdingregister[0]); ++j) {
                Serial.print("Meter ");
                Serial.print(i + 1);
                Serial.print(" Three-Phase Holding Register ");
                Serial.print(EnergyMeter::three_Phase_Holdingregister[j]);
                Serial.print(": ");
                Serial.println(meters[i].getHoldingRegisterValue(j));
                delay(100);
            }
        } else {
            // Print values from single-phase meter
            for (size_t j = 0; j < sizeof(EnergyMeter::single_Phase_Inputregister) / sizeof(EnergyMeter::single_Phase_Inputregister[0]); ++j) {
                Serial.print("Meter ");
                Serial.print(i + 1);
                Serial.print(" Single-Phase Input Register ");
                Serial.print(EnergyMeter::single_Phase_Inputregister[j]);
                Serial.print(": ");
                Serial.println(meters[i].getInputRegisterValue(j));
                delay(100);
            }

            for (size_t j = 0; j < sizeof(EnergyMeter::single_Phase_Holdingregister) / sizeof(EnergyMeter::single_Phase_Holdingregister[0]); ++j) {
                Serial.print("Meter ");
                Serial.print(i + 1);
                Serial.print(" Single-Phase Holding Register ");
                Serial.print(EnergyMeter::single_Phase_Holdingregister[j]);
                Serial.print(": ");
                Serial.println(meters[i].getHoldingRegisterValue(j));
                delay(100);
            }
        }
    }

    delay(1000);
}
