#include <filesystem>
#include <fstream>
#include <thread>

#include "GpioDevice.h"

GpioDevice::GpioDevice(int pin, Value value) : _pin(pin) {
    _basePath = "/sys/class/gpio/gpio" + std::to_string(_pin);

    ExportGpio();
    WriteFile(_basePath + "/direction", "out");

    SetValue(value);
}

GpioDevice::~GpioDevice() {
    UnexportGpio();
}

void GpioDevice::WriteFile(const std::string &path, const std::string &value) {
    std::ofstream file(path);
    if (!file)
        throw std::system_error(errno, std::system_category(), "Failed to open the file: " + path);

    file << value;
    if (!file)
        throw std::system_error(errno, std::system_category(), "Failed to write: " + path);
}

void GpioDevice::ExportGpio() {
    if (!std::filesystem::exists(_basePath)) {
        WriteFile("/sys/class/gpio/export", std::to_string(_pin));
        for (int i = 0; i < 10; ++i) {
            if (std::filesystem::exists(_basePath))
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void GpioDevice::UnexportGpio() const {
    if (_pin >= 0 && std::filesystem::exists("/sys/class/gpio/export"))
        WriteFile("/sys/class/gpio/unexport", std::to_string(_pin));
}

void GpioDevice::SetValue(Value value) {
    WriteFile(_basePath + "/value", std::to_string(static_cast<int>(value)));
}
