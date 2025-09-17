#pragma once

#include <chrono>
#include <string>
#include <thread>
#include <fstream>
#include <filesystem>
#include <system_error>

class GpioDevice {
public:
    enum class Value {
        LOW = 0, HIGH = 1
    };

    explicit GpioDevice(int pin, Value value = Value::LOW);

    ~GpioDevice();

    void setValue(Value value);

private:
    int _pin;
    std::string _basePath;

    void exportGPIO();

    void unexportGPIO() const;

    static void writeFile(const std::string &path, const std::string &value);
};