#include "EnergyMeter.h"
uint16_t dataI2[2];
// Define the register addresses
const uint16_t EnergyMeter::three_Phase_Inputregister[31] = {
    //30001, 30003, 30005, 30007, 30009, 30011, 30013, 30015, 30017, 30019,
    0x0,0x2,0x4,0x6,0x8,0xA,0xC,0xE,0x10,0x12,
    //30021, 30023, 30025, 30027, 30029, 30031, 30033, 30035, 30037, 30039,
    0x14,0x16,0x18,0x1A,0x1C,0x1E,0x20,0x22,0x24,0x26,
    //30041, 30043, 30045, 30047, 30049, 30051, 30053, 30055, 30057
    0x28,0x2A,0x2C,0x2E,0x30,0x32,0x34,0x36,0x38
};

//const uint16_t EnergyMeter::three_Phase_Holdingregister[8] = {
  const uint16_t EnergyMeter::three_Phase_Holdingregister[8] = {
    //42385, 42386, 42387, 42390, 42391, 42392, 42393, 42388
    0x950,0x951,0x952,0x953,0x954,0x955,0x956,0x957
};

const uint16_t EnergyMeter::single_Phase_Inputregister[21] = {
    //30001, 30007, 30013, 30015, 30017, 30023, 30025, 30027, 30029, 30031,
    0x0,0x6,0xC,0xE,0x10,0x16,0x18,0x1A,0x1C,0x1E,
    //30033, 30035, 30037, 30039, 30041, 30043, 30049, 30051, 30053, 30055,
    0x20,0x22,0x24,0x26,0x28,0x2A,0x30,0x32,0x34,0x36,
    //30057
    0x38
};

const uint16_t EnergyMeter::single_Phase_Holdingregister[4] = {
    //42385, 42386, 42387, 42388
    0x950,0x951,0x952,0x953
};

EnergyMeter::EnergyMeter(uint8_t slaveID, HardwareSerial& serial, int baudRate, bool isThreePhase)
    : _slaveID(slaveID), _serial(serial), _baudRate(baudRate), _isThreePhase(isThreePhase) {
    // Initialize arrays to default values
    memset(_inputRegisterValues, 0, sizeof(_inputRegisterValues));
    memset(_holdingRegisterValues, 0, sizeof(_holdingRegisterValues));
}

bool EnergyMeter::begin() {
    _serial.begin(_baudRate);
    _modbus.begin(_slaveID, _serial);
    return true;
}

void EnergyMeter::readAllInputRegisters() {
    const uint16_t* registers = _isThreePhase ? three_Phase_Inputregister : single_Phase_Inputregister;
    size_t numRegisters = _isThreePhase ? sizeof(three_Phase_Inputregister) / sizeof(three_Phase_Inputregister[0]) : sizeof(single_Phase_Inputregister) / sizeof(single_Phase_Inputregister[0]);

    for (size_t i = 0; i < numRegisters; ++i) {
        //uint16_t regAddress = registers[i] - (30001); // Convert to zero-based address
        uint16_t regAddress = registers[i]; // Convert to zero-based address
        uint8_t result = _modbus.readInputRegisters(regAddress, 2);
        delay(100);
        if (result == _modbus.ku8MBSuccess) {
          for (uint8_t j = 0; j < 2; j++) {
      dataI2[j] = _modbus.getResponseBuffer(j);
      Serial.print("Value in Register Part");
      Serial.print(j);
      Serial.print(": ");
      Serial.println(dataI2[j]);
    }
            _inputRegisterValues[i] = static_cast<float>(_modbus.getResponseBuffer(0));
        } else {
            Serial.print("Failed to read input register ");
            Serial.print(registers[i]);
            Serial.print(": ");
            Serial.println(result);
        }
    }
}

void EnergyMeter::readAllHoldingRegisters() {
    const uint16_t* registers = _isThreePhase ? three_Phase_Holdingregister : single_Phase_Holdingregister;
    size_t numRegisters = _isThreePhase ? sizeof(three_Phase_Holdingregister) / sizeof(three_Phase_Holdingregister[0]) : sizeof(single_Phase_Holdingregister) / sizeof(single_Phase_Holdingregister[0]);

    for (size_t i = 0; i < numRegisters; ++i) {
       // uint16_t regAddress = registers[i] - (42385); // Convert to zero-based address
        uint16_t regAddress = registers[i] ; // Convert to zero-based address
        uint8_t result = _modbus.readHoldingRegisters(regAddress, 1);
        delay(100);
        if (result == _modbus.ku8MBSuccess) {
          for (uint8_t j = 0; j < 1; j++) {
      dataI2[j] = _modbus.getResponseBuffer(j);
      Serial.print("Register Line frequency");
      Serial.print(j);
      Serial.print(": ");
      Serial.println(dataI2[j]);
    }
            _holdingRegisterValues[i] = static_cast<float>(_modbus.getResponseBuffer(0));
        } else {
            Serial.print("Failed to read holding register ");
            Serial.print(registers[i]);
            Serial.print(": ");
            Serial.println(result);
        }
    }
}

float EnergyMeter::getInputRegisterValue(uint16_t index) {
    const uint16_t* registers = _isThreePhase ? three_Phase_Inputregister : single_Phase_Inputregister;
    if (index < ( _isThreePhase ? sizeof(three_Phase_Inputregister) / sizeof(three_Phase_Inputregister[0]) : sizeof(single_Phase_Inputregister) / sizeof(single_Phase_Inputregister[0]))) {
        return _inputRegisterValues[index];
    } else {
        Serial.println("Input register index out of bounds");
        return 0;
    }
}

float EnergyMeter::getHoldingRegisterValue(uint16_t index) {
    const uint16_t* registers = _isThreePhase ? three_Phase_Holdingregister : single_Phase_Holdingregister;
    if (index < ( _isThreePhase ? sizeof(three_Phase_Holdingregister) / sizeof(three_Phase_Holdingregister[0]) : sizeof(single_Phase_Holdingregister) / sizeof(single_Phase_Holdingregister[0]))) {
        return _holdingRegisterValues[index];
    } else {
        Serial.println("Holding register index out of bounds");
        return 0;
    }
}
bool EnergyMeter::isThreePhase() const {
    return _isThreePhase;
}
