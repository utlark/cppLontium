#include <stdexcept>
#include <thread>

#include "LontiumDevice.h"

LontiumDevice::LontiumDevice (const std::string &device, const LontiumConfig &config, const uint8_t addr) : _i2c(device, addr), _config(config) { }

void LontiumDevice::SelectBank (const uint8_t bank) const
{
    _i2c.WriteReg(0xFF, bank);
}

VideoInfo LontiumDevice::GetVideoInfo () const
{
    VideoInfo videoInfo = {};

    SelectBank(0x60);
    videoInfo.hActive = _i2c.ReadReg(0x20) << 8 | _i2c.ReadReg(0x21);
    videoInfo.vActive = _i2c.ReadReg(0x22) << 8 | _i2c.ReadReg(0x23);

    videoInfo.hTotal = _i2c.ReadReg(0x1C) << 8 | _i2c.ReadReg(0x1D);
    videoInfo.vTotal = _i2c.ReadReg(0x1E) << 8 | _i2c.ReadReg(0x1F);

    videoInfo.vFrontPorch = _i2c.ReadReg(0x17);
    videoInfo.vBackPorch  = _i2c.ReadReg(0x16);
    videoInfo.vSyncWidth  = _i2c.ReadReg(0x13);

    videoInfo.hFrontPorch = _i2c.ReadReg(0x1A) << 8 | _i2c.ReadReg(0x1B);
    videoInfo.hBackPorch  = _i2c.ReadReg(0x18) << 8 | _i2c.ReadReg(0x15);
    videoInfo.hSyncWidth  = _i2c.ReadReg(0x14) << 8 | _i2c.ReadReg(0x19);

    SelectBank(0x80);
    videoInfo.clockFreq = (_i2c.ReadReg(0x44) & 0x07) << 16 | _i2c.ReadReg(0x45) << 8 | _i2c.ReadReg(0x46);

    return videoInfo;
}

bool LontiumDevice::CheckChipId () const
{
    SelectBank(0x60);

    if (const std::vector<uint8_t> idBytes = _i2c.ReadRegs(0x00, 3); idBytes[0] == 0x16 && idBytes[1] == 0x04)
        return true;
    return false;
}

void LontiumDevice::ColorConfig ()
{
    SelectBank(0x80);

    if ((_i2c.ReadReg(0x71) & 0x60) != 0x00)
    {
        SelectBank(0x60);
        _i2c.WriteReg(0x07, 0x8c); // YCbCr to RGB ClK enable

        SelectBank(0x80);
        if ((_i2c.ReadReg(0x71) & 0x60) == 0x20)
        {
            SelectBank(0x60);
            _i2c.WriteReg(0x52, 0x01); // YUV422 to YUV444 enable
        }
        else
        {
            SelectBank(0x60);
            _i2c.WriteReg(0x52, 0x00); // YUV422 to YUV444 disable
        }

        _i2c.WriteReg(0x53, 0x40 | static_cast<uint8_t>(_config.cpConvertMode));
    }
    else
    {
        SelectBank(0x60);
        _i2c.WriteReg(0x07, 0x80);
        _i2c.WriteReg(0x53, 0x00);
    }
}

void LontiumDevice::LockDetect () const
{
    const auto timeout   = std::chrono::seconds(5);
    const auto startTime = std::chrono::steady_clock::now();

    SelectBank(0x80);
    while ((_i2c.ReadReg(0x87) & 0x20) == 0x00)
    {
        if (std::chrono::steady_clock::now() - startTime >= timeout)
            throw std::runtime_error("LVDS PLL lock det failed");

        SelectBank(0x60);

        _i2c.WriteReg(0x0e, 0xfd);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        _i2c.WriteReg(0x0e, 0xff);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        SelectBank(0x80);
    }
}

void LontiumDevice::LvdsInit ()
{
    SelectBank(0x80);
    _i2c.WriteReg(0x2c, _i2c.ReadReg(0x2C) | 0x30);

    SelectBank(0x60);
    _i2c.WriteReg(0x80, 0x08);
    _i2c.WriteReg(0x06, 0xe7);

    SelectBank(0x80);
    while ((_i2c.ReadReg(0x43) & 0x80) == 0x00) // Hdmi-Rx-Is-Stable
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    while ((_i2c.ReadReg(0x13) & 0x01) == 0x00) // Hsync-Is-Stable
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    SelectBank(0x60);
    _i2c.WriteReg(
        0x59, static_cast<uint8_t>(_config.lvdsMap) | static_cast<uint8_t>(_config.lvdsOutput) | static_cast<uint8_t>(_config.syncMode) | static_cast<uint8_t>(_config.colorDepth) |
                  static_cast<uint8_t>(_config.cDPortSwap) | static_cast<uint8_t>(_config.rBColorSwap)
    );
    if (_config.colorDepth == ColorDepth::Bit_8)
        _i2c.WriteReg(0x5f, 0x00);
    else
        _i2c.WriteReg(0x5f, 0x38);
    _i2c.WriteReg(0x8b, 0x90);
    _i2c.WriteReg(0xa8, 0x07);
    _i2c.WriteReg(0x04, 0xf2);
    _i2c.WriteReg(0xa0, 0x58);
    _i2c.WriteReg(0xa4, static_cast<uint8_t>(_config.channelWidth));
    _i2c.WriteReg(0xa8, 0x00);
    _i2c.WriteReg(0xb0, 0x66);
    _i2c.WriteReg(0xb1, 0x66);
    _i2c.WriteReg(0xb2, 0x66);
    _i2c.WriteReg(0xb3, 0x66);
    _i2c.WriteReg(0xb4, 0x66);
    _i2c.WriteReg(0xb5, 0x41);
    _i2c.WriteReg(0xb6, 0x41);
    _i2c.WriteReg(0xb7, 0x41);
    _i2c.WriteReg(0xb8, 0x4c);
    _i2c.WriteReg(0xb9, 0x41);
    _i2c.WriteReg(0xbb, 0x41);
    _i2c.WriteReg(0xbc, 0x41);
    _i2c.WriteReg(0xbd, 0x41);
    _i2c.WriteReg(0xbe, 0x4c);
    _i2c.WriteReg(0xbf, 0x41);
    _i2c.WriteReg(0xa1, 0xb0);
    _i2c.WriteReg(0xa2, 0x10);
    _i2c.WriteReg(0x5c, static_cast<uint8_t>(_config.channelWidth));
    _i2c.WriteReg(0xba, 0x18);
    _i2c.WriteReg(0xc0, 0x18);
    _i2c.WriteReg(0x04, 0xe7);
}

void LontiumDevice::LvdsSoftReset () const
{
    SelectBank(0x60);

    uint8_t reg = _i2c.ReadReg(0x0E);
    _i2c.WriteReg(0x0E, reg & ~0x02);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    _i2c.WriteReg(0x0E, reg | 0x02);

    reg = _i2c.ReadReg(0x0D);
    _i2c.WriteReg(0x0D, reg & ~0x04);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    _i2c.WriteReg(0x0D, reg | 0x04);
}

void LontiumDevice::RxReset () const
{
    SelectBank(0x60);

    _i2c.WriteReg(0x0E, 0xBF);
    _i2c.WriteReg(0x09, 0xFD);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    _i2c.WriteReg(0x0E, 0xFF);
    _i2c.WriteReg(0x09, 0xFF);
    _i2c.WriteReg(0x0E, 0xC7);
    _i2c.WriteReg(0x09, 0x0F);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    _i2c.WriteReg(0x0E, 0xFF);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    _i2c.WriteReg(0x09, 0x8F);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    _i2c.WriteReg(0x09, 0xFF);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void LontiumDevice::SetEDID (const std::vector<uint8_t> &values) const
{
    SelectBank(0x80);

    _i2c.WriteReg(0x8E, 0x07);

    _i2c.WriteReg(0x8F, 0x00);
    _i2c.WriteRegs(0x90, values);

    _i2c.WriteReg(0x8E, 0x02);
}

void LontiumDevice::SetHPD (const Value value) const
{
    SelectBank(0x80);

    if (Value::ON == value)
        _i2c.WriteReg(0x06, _i2c.ReadReg(0x06) | 0x08);
    else
        _i2c.WriteReg(0x06, _i2c.ReadReg(0x06) & 0xF7);
}
