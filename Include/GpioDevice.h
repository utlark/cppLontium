#pragma once

class GpioDevice {
public:
    enum class Value {
        LOW = 0, HIGH = 1
    };

    explicit GpioDevice(int pin, Value value = Value::LOW);

    ~GpioDevice();

    void SetValue(Value value);

private:
    int _pin;
    std::string _basePath;

    static void WriteFile(const std::string &path, const std::string &value);

    void ExportGpio();

    void UnexportGpio() const;
};