#ifndef ENERGYMETER_H
#define ENERGYMETER_H

#include <Arduino.h>
#include <ModbusMaster.h>

class EnergyMeter {
public:
    // Constructor for the energy meter
    EnergyMeter(uint8_t slaveID, HardwareSerial& serial, int baudRate, bool isThreePhase);

    bool begin();

    // Read all input registers
    void readAllInputRegisters();
    // Read all holding registers
    void readAllHoldingRegisters();

     // Method to check if the meter is three-phase
    bool isThreePhase() const;

    // Getters for register values
    float getInputRegisterValue(uint16_t index);
    float getHoldingRegisterValue(uint16_t index);
     // Arrays for register addresses
    static const uint16_t three_Phase_Inputregister[31];
    static const uint16_t three_Phase_Holdingregister[8];
    static const uint16_t single_Phase_Inputregister[21];
    static const uint16_t single_Phase_Holdingregister[4];

private:
    uint8_t _slaveID;
    ModbusMaster _modbus;
    HardwareSerial& _serial;
    int _baudRate;
    bool _isThreePhase;

   
    // Storage for register values
    float _inputRegisterValues[31];
    float _holdingRegisterValues[8];
};

#endif // ENERGYMETER_H
