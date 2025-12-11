#pragma once

#include <string>
#include <vector>

#include "I2cDevice.h"
#include "LontiumConfig.h"
#include "VideoInfo.h"

class LontiumDevice
{
public:
    enum class Value
    {
        OFF = 0,
        ON  = 1
    };

    explicit LontiumDevice (const std::string &device, const LontiumConfig &config, uint8_t addr = 0x32);

    [[nodiscard]] VideoInfo GetVideoInfo () const;

    [[nodiscard]] bool CheckChipId () const;

    void ColorConfig ();

    void LockDetect () const;

    void LvdsInit ();

    void LvdsSoftReset () const;

    void RxReset () const;

    void SetEDID (const std::vector<uint8_t> &values) const;

    void SetHPD (Value value) const;

private:
    I2cDevice     _i2c;
    LontiumConfig _config;

    void SelectBank (uint8_t bank) const;
};
