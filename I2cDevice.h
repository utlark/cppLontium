#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

class I2cDevice {
public:
    I2cDevice(const std::string &device, int addr);

    ~I2cDevice();

    void writeReg(uint8_t reg, uint8_t value) const;

    [[nodiscard]] uint8_t readReg(uint8_t reg) const;

    void writeRegs(uint8_t reg, const std::vector<uint8_t> &values) const;

    [[nodiscard]] std::vector<uint8_t> readRegs(uint8_t reg, size_t len) const;

private:
    int fd;
    int addr;
};
