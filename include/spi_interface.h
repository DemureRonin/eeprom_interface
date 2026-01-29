#pragma once
#include <cstdint>

class spi {
public:
    virtual ~spi() = default;

    virtual std::uint8_t transfer(uint8_t data) = 0;

    virtual void set_cs(bool level) = 0;
};

