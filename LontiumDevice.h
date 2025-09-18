#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <thread>
#include <chrono>

#include "LontiumConfig.h"
#include "I2cDevice.h"

class LontiumDevice {
public:
    enum class Value {
        OFF = 0, ON = 1
    };

    explicit LontiumDevice(const std::string &device, LontiumConfig &config, uint8_t addr = 0x32);

    bool checkChipId();

    void setHPD(Value value);

    void setEDID(const std::vector<uint8_t> &values);

    void rxReset();

    void lvdsInit();

    void lockDetect();

    void lvdsSoftReset();

    void colorConfig();

private:
    I2cDevice _i2c;
    LontiumConfig _config;

    void selectBank(uint8_t bank);
};
