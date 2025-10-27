#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "I2cDevice.h"
#include "LontiumConfig.h"

class LontiumDevice {
public:
    enum class Value {
        OFF = 0, ON = 1
    };

    explicit LontiumDevice(const std::string &device, LontiumConfig &config, uint8_t addr = 0x32);

    bool CheckChipId();

    void ColorConfig();

    void LockDetect();

    void LvdsInit();

    void LvdsSoftReset();

    void RxReset();

    void SetEDID(const std::vector<uint8_t> &values);

    void SetHPD(Value value);

private:
    I2cDevice _i2c;
    LontiumConfig _config;

    void SelectBank(uint8_t bank);
};
