#ifndef ENERGYMETER_H
#define ENERGYMETER_H

#include <Arduino.h>
#include <ModbusMaster.h>

ModbusMaster node; // Defining the ModbusMaster instance

class EnergyMeter {
public:
    // Constructor for the energy meter
    EnergyMeter(HardwareSerial& serial, int baudRate);
    bool begin(uint8_t slaveID);
    uint16_t readInputRegister(uint16_t startAddress, uint8_t numRegister);
    uint16_t readHoldingRegister(uint16_t startAddress, uint8_t numRegister);
    uint16_t dataStoreInRegister(uint8_t numRegister, bool isInputRegister);
    uint32_t threePhaseEnergyMeterInputRegisterData(uint8_t numAddressRegister);
    uint32_t threePhaseEnergyMeterHoldingRegisterData(uint8_t numAddressRegister);
    uint32_t singlePhaseEnergyMeterInputRegisterData(uint8_t numAddressRegister);
    uint32_t singlePhaseEnergyMeterHoldingRegisterData(uint8_t numAddressRegister);
    void readMeterData(uint8_t slaveID, bool isThreePhase);
    float valueInInputRegister(uint8_t numRegister);
    uint16_t valueInHoldingRegister(uint8_t numRegister);
    float valueInInputRegister(uint16_t valueInput, uint8_t numRegister, uint8_t j);
    uint16_t valueInHoldingRegister(uint16_t valueHolding, uint8_t numRegister,uint8_t j);
    static const uint16_t three_Phase_Inputregister[29];
    static const uint16_t three_Phase_Holdingregister[8];
    static const uint16_t single_Phase_Inputregister[21];
    static const uint16_t single_Phase_Holdingregister[4];
    float Inputregister_Get_Data[30];
    uint16_t Holdingregister_Get_Data[8];

private:
    uint8_t _slaveID;
    HardwareSerial& _serial;
    int _baudRate;
    bool _isThreePhase;
    bool _isInputRegister;
    uint8_t numInputReadRegister;
    uint8_t numHoldingReadRegister;
    uint8_t numRegister;
};

const uint16_t EnergyMeter::three_Phase_Inputregister[29] = {
    0x0000,0x0002,0x0004,0x0006,0x0008,0x000A,0x000C,0x000E,0x0010,0x0012,
    0x0014,0x0016,0x0018,0x001A,0x001C,0x001E,0x0020,0x0022,0x0024,0x0026,
    0x0028,0x002A,0x002C,0x002E,0x0030,0x0032,0x0034,0x0036,0x0038
};

const uint16_t EnergyMeter::three_Phase_Holdingregister[8] = {
    0x0950,0x0951,0x0952,0x0953,0x0954,0x0955,0x0956,0x0957
};

const uint16_t EnergyMeter::single_Phase_Inputregister[21] = {
    0x0000,0x0006,0x000C,0x000E,0x0010,0x0016,0x0018,0x001A,0x001C,0x001E,
    0x0020,0x0022,0x0024,0x0026,0x0028,0x002A,0x0030,0x0032,0x0034,0x0036,
    0x0038
};

const uint16_t EnergyMeter::single_Phase_Holdingregister[4] = {
    0x0950,0x0951,0x0952,0x0953
};

// Constructor implementation
EnergyMeter::EnergyMeter(HardwareSerial& serial, int baudRate)
    : _serial(serial), _baudRate(baudRate) {
    }
// start to read energy meter data     
void EnergyMeter::readMeterData(uint8_t slaveID, bool isThreePhase) {
    //Energy meter three phase
    if (isThreePhase) {
        begin(slaveID);
        delay(100);
        numInputReadRegister = 31;
        numHoldingReadRegister = 8;
        threePhaseEnergyMeterInputRegisterData(numInputReadRegister);
        threePhaseEnergyMeterHoldingRegisterData(numHoldingReadRegister);
    } else {
        //Energy meter single phase
        begin(slaveID);
        delay(100);
        numInputReadRegister = 21;
        numHoldingReadRegister = 4;
        singlePhaseEnergyMeterInputRegisterData(numInputReadRegister);
        singlePhaseEnergyMeterHoldingRegisterData(numHoldingReadRegister);
    }
}

bool EnergyMeter::begin(uint8_t slaveID) {
    _slaveID = slaveID;
    node.begin(slaveID, _serial);
    delay(100);
    return true;
}

uint16_t EnergyMeter::readInputRegister(uint16_t startAddress, uint8_t numRegister) {
    uint16_t dataInRegister;
    uint16_t result_Input = node.readInputRegisters(startAddress, numRegister);
    delay(100);

    if (result_Input == node.ku8MBSuccess) {
        dataInRegister = dataStoreInRegister(numRegister, true);       
    }
    return dataInRegister;
}

uint16_t EnergyMeter::readHoldingRegister(uint16_t startAddress, uint8_t numRegister) {
    uint16_t dataInRegister;
    uint16_t result_Holding = node.readHoldingRegisters(startAddress, numRegister);
    delay(100);

    if (result_Holding == node.ku8MBSuccess) {
        dataInRegister = dataStoreInRegister(numRegister, false);
        
    }
    return dataInRegister;
}

uint16_t EnergyMeter::dataStoreInRegister(uint8_t numRegister, bool isInputRegister) {
    uint16_t data[2]; // Assuming max 2 registers for example
    for (int j = 0; j < numRegister; j++) {
        data[j] = node.getResponseBuffer(j);
        
    }
    if (isInputRegister) {
        return data[1];
    } else {
        return data[0];
    }
}

// Three phase energy meter input register data
uint32_t EnergyMeter::threePhaseEnergyMeterInputRegisterData(uint8_t numAddressRegister) {
    
    //Serial.println("Three Phase Input Register");
    for (int j = 0; j < numAddressRegister; j++) {
        Inputregister_Get_Data[j]=valueInInputRegister(three_Phase_Inputregister[j], 2, j);
    }
    return 0; // Example return value
}

// Three phase holding register data
uint32_t EnergyMeter::threePhaseEnergyMeterHoldingRegisterData(uint8_t numAddressRegister) {
    
    //Serial.println("Three Phase Holding Register");
    for (int j = 0; j < numAddressRegister; j++) {
        Holdingregister_Get_Data[j]=valueInHoldingRegister(three_Phase_Holdingregister[j], 1,j);
    }
    return 0; // Example return value
}

uint32_t EnergyMeter::singlePhaseEnergyMeterInputRegisterData(uint8_t numAddressRegister) {
    
    //Serial.println("Single Phase Input Register");
    for (int j = 0; j < numAddressRegister; j++) {
        Inputregister_Get_Data[j]=readInputRegister(single_Phase_Inputregister[j], 2);
    }
    return 0; // Example return value
}

uint32_t EnergyMeter::singlePhaseEnergyMeterHoldingRegisterData(uint8_t numAddressRegister) {
    //Serial.println("Single Phase Holding Register");
    for (int j = 0; j < numAddressRegister; j++) {
        Holdingregister_Get_Data[j]=readHoldingRegister(single_Phase_Holdingregister[j], 1);
    }
    return 0; // Example return value
}
// Print register values
float EnergyMeter::valueInInputRegister(uint8_t numRegister) {
    Serial.println("Input register data");
    float valueInEachInputRegister;
    Serial.println(numRegister);
        valueInEachInputRegister=Inputregister_Get_Data[numRegister];
        Serial.println(Inputregister_Get_Data[numRegister]);
        return valueInEachInputRegister;
}
uint16_t EnergyMeter::valueInHoldingRegister(uint8_t numRegister){
    uint16_t valueInEachHoldingRegister;
    Serial.print("Holding register data");
    Serial.println(numRegister);
        valueInEachHoldingRegister=Holdingregister_Get_Data[numRegister];
        Serial.println(Holdingregister_Get_Data[numRegister]);
        return valueInEachHoldingRegister;
    
}

// Define as member functions
float EnergyMeter::valueInInputRegister(uint16_t valueInput, uint8_t numRegister, uint8_t j) {
          float value;
    switch (valueInput) {
        case 0x0000:
            Serial.print("R Phase Voltage");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            return value;
        case 0x0002:
            Serial.print("Y Phase Voltage");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0004:
            Serial.print("B Phase Voltage");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0006:
            Serial.print("R Phase Current");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.001));
            Serial.println(value);
            return value;
            break;
        case 0x0008:
            Serial.print("Y Phase Current");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.001));
            Serial.println(value);
            return value;
            break;
        case 0x000A:
            Serial.print("B Phase Current");
            value= ((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.001));
            Serial.println(value);
            return value;
            break;
        case 0x000C:
            Serial.print("Average Voltage");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x000E:
            Serial.print("Average Current");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.001));
            Serial.println(value);
            return value;
            break;
        case 0x0010:
            Serial.print("Total Active Power");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.0001));
            Serial.println(value);
            return value;
            break;
        case 0x0012:
            Serial.print("Total Reactive Power");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.0001));
            Serial.println(value);
            return value;
            break;
        case 0x0014:
            Serial.print("Total Apparent Power");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.0001));
            Serial.println(value);
            return value;
            break;
        case 0x0016:
            Serial.print("Total Apparent Power");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.0001));
            Serial.println(value);
            return value;
            break;
        case 0x0018:
            Serial.print("Total Power Factor");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.001));
            Serial.println(value);
            return value;
            break;
        case 0x001A:
            Serial.print("Line Frequency");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x001C:
            Serial.print("Cumulative energy – kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x001E:
            Serial.print("Previous Day Consumption-kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0020:
            Serial.print("Present Day Consumption-kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0022:
            Serial.print("Previous Week Consumption-kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0024:
            Serial.print("Present Week Consumption-kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0026:
            Serial.print("Previous Month Consumption-kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0028:
            Serial.print("Present Month Consumption-kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x002A:
            Serial.print("Current Push to Push Consumption-kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x002C:
            Serial.print("Present Month MD-kW");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.0001));
            Serial.println(value);
            return value;
            break;
        case 0x002E:
            Serial.print("Previous Month MD-kW");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.0001));
            Serial.println(value);
            return value;
            break;
        case 0x0030:
            Serial.print("1st Integration period -kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0032:
            Serial.print("2nd Integration period (Previous of 1st integration period )-kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0034:
            Serial.print("3rd Integration period (Previous of 2nd integration period )-kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0036:
            Serial.print("4th Integration period (Previous of 3rd integration period)_kWh");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (0.01));
            Serial.println(value);
            return value;
            break;
        case 0x0038:
            Serial.print("Meter Serial Number");
            value=((readInputRegister(three_Phase_Inputregister[j], 2)) * (1));
            Serial.println(value);
            return value;
            break;
        /*default:
            Serial.println("Invalid Input Register Address");
            break;*/
    }
    //return 0; // Example return value
}

 uint16_t EnergyMeter::valueInHoldingRegister(uint16_t valueHolding, uint8_t numRegister, uint8_t j) {
          uint16_t value;
    switch (valueHolding) {
        case 0x0950:
            Serial.print("Active Energy Import");
            value= readHoldingRegister(three_Phase_Holdingregister[j], 1);
            return value;
            break;
        case 0x0951:
            Serial.print("Active Energy Export");
            value= readHoldingRegister(three_Phase_Holdingregister[j], 1);
            return value;
            break;
        case 0x0952:
            Serial.print("Reactive Energy Import");
            value= readHoldingRegister(three_Phase_Holdingregister[j], 1);
            return value;
            break;
        case 0x0953:
            Serial.print("Reactive Energy Export");
            value= readHoldingRegister(three_Phase_Holdingregister[j], 1);
            return value;
            break;
        case 0x0954:
            Serial.print("Apparent Energy Import");
            value= readHoldingRegister(three_Phase_Holdingregister[j], 1);
            return value;
            break;
        case 0x0955:
            Serial.print("Apparent Energy Export");
            value= readHoldingRegister(three_Phase_Holdingregister[j], 1);
            return value;
            break;
        case 0x0956:
            Serial.print("Active Energy");
            value= readHoldingRegister(three_Phase_Holdingregister[j], 1);
            return value;
            break;
        case 0x0957:
            Serial.print("Reactive Energy");
            value= readHoldingRegister(three_Phase_Holdingregister[j], 1);
            return value;
            break;
       /* default:
            Serial.println("Invalid Holding Register Address");
            break;*/
    }
    //return 0; // Example return value
}

#endif // ENERGYMETER_H
