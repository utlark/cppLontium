#include "I2cDevice.h"

I2cDevice::I2cDevice(const std::string &device, int addr) : fd(-1), addr(addr) {
    fd = open(device.c_str(), O_RDWR);
    if (fd < 0)
        throw std::runtime_error("Failed to open I2C device: " + device);

    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        close(fd);
        throw std::runtime_error("Failed to set I2C slave address");
    }
}

I2cDevice::~I2cDevice() {
    try {
        if (fd >= 0)
            close(fd);
    } catch (...) {
    }
}

void I2cDevice::writeReg(uint8_t reg, uint8_t value) const {
    writeRegs(reg, {value});
}

uint8_t I2cDevice::readReg(uint8_t reg) const {
    return readRegs(reg, 1)[0];
}

void I2cDevice::writeRegs(uint8_t reg, const std::vector<uint8_t> &values) const {
    std::vector<uint8_t> buf;

    buf.reserve(values.size() + 1);
    buf.push_back(reg);
    buf.insert(buf.end(), values.begin(), values.end());

    struct i2c_rdwr_ioctl_data packets{};
    struct i2c_msg message{};

    message.addr = addr;
    message.flags = 0;
    message.len = buf.size();
    message.buf = buf.data();

    packets.msgs = &message;
    packets.nmsgs = 1;

    if (ioctl(fd, I2C_RDWR, &packets) < 0)
        throw std::runtime_error("Failed to write registers");
}

std::vector<uint8_t> I2cDevice::readRegs(uint8_t reg, size_t len) const {
    struct i2c_rdwr_ioctl_data packets{};
    struct i2c_msg messages[2];

    messages[0].addr = addr;
    messages[0].flags = 0;
    messages[0].len = 1;
    messages[0].buf = &reg;

    std::vector<uint8_t> buf(len);
    messages[1].addr = addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len = len;
    messages[1].buf = buf.data();

    packets.msgs = messages;
    packets.nmsgs = 2;

    if (ioctl(fd, I2C_RDWR, &packets) < 0)
        throw std::runtime_error("Failed to read multiple registers");

    return buf;
}
