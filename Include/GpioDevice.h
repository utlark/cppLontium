#pragma once

#include <string>

class GpioDevice
{
public:
    enum class Value
    {
        LOW  = 0,
        HIGH = 1
    };

    enum class Direction
    {
        IN  = 0,
        OUT = 1
    };

    explicit GpioDevice (std::string pin, Direction direction = Direction::OUT, Value value = Value::LOW);

    ~GpioDevice ();

    void SetValue (Value value) const;

private:
    std::string _pin;

    static void WriteFile (const std::string &path, const std::string &data);

    void Export () const;

    void SetDirection (Direction direction = Direction::IN) const;

    void Unexport () const;
};