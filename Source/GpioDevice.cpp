#include <filesystem>
#include <fstream>
#include <thread>

#include "GpioDevice.h"

GpioDevice::GpioDevice (std::string pin, const Direction direction, const Value value) : _pin(std::move(pin))
{
    Export();
    SetDirection(direction);
    SetValue(value);
}

GpioDevice::~GpioDevice ()
{
    Unexport();
}

void GpioDevice::SetValue (const Value value) const
{
    WriteFile("/sys/class/gpio/gpio" + _pin + "/value", value == Value::LOW ? "0" : "1");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void GpioDevice::WriteFile (const std::string &path, const std::string &data)
{
    std::ofstream file(path);
    if (!file)
        throw std::system_error(errno, std::system_category(), "Failed to open: " + path);

    file << data;
    if (!file)
        throw std::system_error(errno, std::system_category(), "Failed to write: " + path);
}

void GpioDevice::Export () const
{
    if (!std::filesystem::exists("/sys/class/gpio/gpio" + _pin))
        WriteFile("/sys/class/gpio/export", _pin);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void GpioDevice::SetDirection (const Direction direction) const
{
    WriteFile("/sys/class/gpio/gpio" + _pin + "/direction", direction == Direction::IN ? "in" : "out");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void GpioDevice::Unexport () const
{
    WriteFile("/sys/class/gpio/unexport", _pin);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
