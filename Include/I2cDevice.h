#pragma once

#include <cstdint>
#include <string>
#include <vector>

class I2cDevice {
public:
    I2cDevice(const std::string &device, int addr);

    ~I2cDevice();

    [[nodiscard]] std::vector<uint8_t> ReadRegs(uint8_t reg, size_t len) const;

    [[nodiscard]] uint8_t ReadReg(uint8_t reg) const;

    void WriteReg(uint8_t reg, uint8_t value) const;

    void WriteRegs(uint8_t reg, const std::vector<uint8_t> &values) const;

private:
    int _addr = -1;
    int _fd = -1;
};
