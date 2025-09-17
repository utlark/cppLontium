#include "LontiumDevice.h"

LontiumDevice::LontiumDevice(const std::string &device, uint8_t addr) : _i2c(device, addr) {}

void LontiumDevice::selectBank(uint8_t bank) {
    _i2c.writeReg(0xFF, bank);
}

bool LontiumDevice::checkChipId() {
    selectBank(0x60);

    std::vector<uint8_t> idBytes = _i2c.readRegs(0x00, 3);
    if ((idBytes[0] == 0x16) && idBytes[1] == 0x04)
        return true;
    else
        return false;
}

void LontiumDevice::setHPD(Value value) {
    selectBank(0x80);

    if (Value::ON == value)
        _i2c.writeReg(0x06, (_i2c.readReg(0x06) | 0x08));
    else
        _i2c.writeReg(0x06, (_i2c.readReg(0x06) & 0xF7));
}

void LontiumDevice::setEDID(const std::vector<uint8_t> &values) {
    selectBank(0x80);

    _i2c.writeReg(0x8E, 0x07);

    _i2c.writeReg(0x8F, 0x00);
    _i2c.writeRegs(0x90, values);

    _i2c.writeReg(0x8E, 0x02);
}

void LontiumDevice::rxReset() {
    selectBank(0x60);

    _i2c.writeReg(0x0E, 0xBF);
    _i2c.writeReg(0x09, 0xFD);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    _i2c.writeReg(0x0E, 0xFF);
    _i2c.writeReg(0x09, 0xFF);
    _i2c.writeReg(0x0E, 0xC7);
    _i2c.writeReg(0x09, 0x0F);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    _i2c.writeReg(0x0E, 0xFF);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    _i2c.writeReg(0x09, 0x8F);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    _i2c.writeReg(0x09, 0xFF);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void LontiumDevice::lvdsInit() {
    selectBank(0x80);
    _i2c.writeReg(0x2c, (_i2c.readReg(0x2C) | 0x30));

    selectBank(0x60);
    _i2c.writeReg(0x80, 0x08);
    _i2c.writeReg(0x06, 0xe7);

    selectBank(0x80);
    while ((_i2c.readReg(0x43) & 0x80) == 0x00) // Hdmi-Rx-Is-Stable
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    while ((_i2c.readReg(0x13) & 0x01) == 0x00) // Hsync-Is-Stable
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    selectBank(0x60);
    _i2c.writeReg(0x59, LVDS_Map + LVDS_Output_En + LVDS_SyncMode + LVDS_ColorDepth + LVDS_C_D_Port_Swap + LVDS_R_B_Color_Swap);
    if (LVDS_ColorDepth == ColorDepth_8_bit)
        _i2c.writeReg(0x5f, 0x00);
    else
        _i2c.writeReg(0x5f, 0x38);
    _i2c.writeReg(0x8b, 0x90);
    _i2c.writeReg(0xa8, 0x07);
    _i2c.writeReg(0x04, 0xf2);
    _i2c.writeReg(0xa0, 0x58);
    _i2c.writeReg(0xa4, LVDS_Port);
    _i2c.writeReg(0xa8, 0x00);
    _i2c.writeReg(0xb0, 0x66);
    _i2c.writeReg(0xb1, 0x66);
    _i2c.writeReg(0xb2, 0x66);
    _i2c.writeReg(0xb3, 0x66);
    _i2c.writeReg(0xb4, 0x66);
    _i2c.writeReg(0xb5, 0x41);
    _i2c.writeReg(0xb6, 0x41);
    _i2c.writeReg(0xb7, 0x41);
    _i2c.writeReg(0xb8, 0x4c);
    _i2c.writeReg(0xb9, 0x41);
    _i2c.writeReg(0xbb, 0x41);
    _i2c.writeReg(0xbc, 0x41);
    _i2c.writeReg(0xbd, 0x41);
    _i2c.writeReg(0xbe, 0x4c);
    _i2c.writeReg(0xbf, 0x41);
    _i2c.writeReg(0xa1, 0xb0);
    _i2c.writeReg(0xa2, 0x10);
    _i2c.writeReg(0x5c, LVDS_Port);
    _i2c.writeReg(0xba, 0x18);
    _i2c.writeReg(0xc0, 0x18);
    _i2c.writeReg(0x04, 0xe7);
}

void LontiumDevice::lockDetect() {
    const auto timeout = std::chrono::seconds(5);
    const auto startTime = std::chrono::steady_clock::now();

    selectBank(0x80);
    while ((_i2c.readReg(0x87) & 0x20) == 0x00) {
        if (std::chrono::steady_clock::now() - startTime >= timeout)
            throw std::runtime_error("Lvds pll lock det failed");

        selectBank(0x60);

        _i2c.writeReg(0x0e, 0xfd);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        _i2c.writeReg(0x0e, 0xff);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        selectBank(0x80);
    }
}

void LontiumDevice::lvdsSoftReset() {
    selectBank(0x60);

    uint8_t reg = _i2c.readReg(0x0E);
    _i2c.writeReg(0x0E, reg & (~0x02));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    _i2c.writeReg(0x0E, reg | 0x02);

    reg = _i2c.readReg(0x0D);
    _i2c.writeReg(0x0D, reg & (~0x04));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    _i2c.writeReg(0x0D, reg | 0x04);
}

void LontiumDevice::colorConfig() {
    selectBank(0x80);

    if (((_i2c.readReg(0x71)) & 0x60) != 0x00) {
        selectBank(0x60);
        _i2c.writeReg(0x07, 0x8c); // YCbCr to RGB ClK enable

        selectBank(0x80);
        if ((_i2c.readReg(0x71) & 0x60) == 0x20) {
            selectBank(0x60);
            _i2c.writeReg(0x52, 0x01); // YUV422 to YUV444 enable
        } else {
            selectBank(0x60);
            _i2c.writeReg(0x52, 0x00); // YUV422 to YUV444 disable
        }

        _i2c.writeReg(0x53, 0x40 + CP_Convert_Mode);
    } else {
        selectBank(0x60);
        _i2c.writeReg(0x07, 0x80);
        _i2c.writeReg(0x53, 0x00);
    }
}