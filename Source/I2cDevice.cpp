#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <system_error>
#include <unistd.h>

#include "I2cDevice.h"

I2cDevice::I2cDevice(const std::string &device, int addr) : _addr(addr) {
    _fd = open(device.c_str(), O_RDWR);
    if (_fd < 0)
        throw std::system_error(errno, std::system_category(), "Failed to open I2C device: " + device);

    if (ioctl(_fd, I2C_SLAVE, addr) < 0) {
        close(_fd);
        throw std::system_error(errno, std::system_category(), "Failed to set I2C slave address");
    }
}

I2cDevice::~I2cDevice() {
    if (_fd >= 0)
        close(_fd);
}

std::vector<uint8_t> I2cDevice::ReadRegs(uint8_t reg, size_t len) const {
    struct i2c_rdwr_ioctl_data packets{};
    struct i2c_msg messages[2];

    messages[0].addr = _addr;
    messages[0].flags = 0;
    messages[0].len = 1;
    messages[0].buf = &reg;

    std::vector<uint8_t> buf(len);
    messages[1].addr = _addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len = len;
    messages[1].buf = buf.data();

    packets.msgs = messages;
    packets.nmsgs = 2;

    if (ioctl(_fd, I2C_RDWR, &packets) < 0)
        throw std::system_error(errno, std::system_category(), "Failed to read register");

    return buf;
}

uint8_t I2cDevice::ReadReg(uint8_t reg) const {
    return ReadRegs(reg, 1)[0];
}

void I2cDevice::WriteRegs(uint8_t reg, const std::vector<uint8_t> &values) const {
    std::vector<uint8_t> buf;

    buf.reserve(values.size() + 1);
    buf.push_back(reg);
    buf.insert(buf.end(), values.begin(), values.end());

    struct i2c_rdwr_ioctl_data packets{};
    struct i2c_msg message{};

    message.addr = _addr;
    message.flags = 0;
    message.len = buf.size();
    message.buf = buf.data();

    packets.msgs = &message;
    packets.nmsgs = 1;

    if (ioctl(_fd, I2C_RDWR, &packets) < 0)
        throw std::system_error(errno, std::system_category(), "Failed to write register");
}


void I2cDevice::WriteReg(uint8_t reg, uint8_t value) const {
    WriteRegs(reg, {value});
}