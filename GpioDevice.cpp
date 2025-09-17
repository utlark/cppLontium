#include "GpioDevice.h"

GpioDevice::GpioDevice(int pin, Value value) : _pin(pin) {
    _basePath = "/sys/class/gpio/gpio" + std::to_string(_pin);

    exportGPIO();
    writeFile(_basePath + "/direction", "out");

    setValue(value);
}

GpioDevice::~GpioDevice() {
    try {
        unexportGPIO();
    } catch (...) {
    }
}

void GpioDevice::exportGPIO() {
    if (!std::filesystem::exists(_basePath)) {
        writeFile("/sys/class/gpio/export", std::to_string(_pin));
        for (int i = 0; i < 10; ++i) {
            if (std::filesystem::exists(_basePath))
                break;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void GpioDevice::unexportGPIO() const {
    if (_pin >= 0 && std::filesystem::exists("/sys/class/gpio/export"))
        writeFile("/sys/class/gpio/unexport", std::to_string(_pin));
}

void GpioDevice::setValue(Value value) {
    writeFile(_basePath + "/value", std::to_string(static_cast<int>(value)));
}

void GpioDevice::writeFile(const std::string &path, const std::string &value) {
    std::ofstream file(path);
    if (!file)
        throw std::system_error(errno, std::system_category(), "Failed to write to " + path);

    file << value;
    if (!file)
        throw std::system_error(errno, std::system_category(), "Write failed to " + path);
}

