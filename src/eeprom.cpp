#include "eeprom.h"
#include "eeprom_constants.h"

#include <cassert>
#include <chrono>
#include <thread>

eeprom::eeprom(spi &spi) : spi_(spi) {
}

void eeprom::read(uint16_t address, uint8_t *data, uint16_t length) {
    assert(length > 0);
    assert(address <= MAX_ADDR);
    assert(address + length - 1 <= MAX_ADDR);
    assert(data != nullptr);

    uint8_t highest_bit = (address >> 8) & 1;
    uint8_t cmd = cmd::READ | (highest_bit << 3);
    uint8_t lower_bits = address & 0xFF;

    spi_.set_cs(cs_level::LOW);
    spi_.transfer(cmd);
    spi_.transfer(lower_bits);

    for (uint16_t i = 0; i < length; i++) {
        data[i] = spi_.transfer(DUMMY);
    }
    spi_.set_cs(cs_level::HIGH);
}

uint8_t eeprom::read_byte(uint16_t address) {
    uint8_t byte = 0;
    read(address, &byte, 1);
    return byte;
}

uint8_t eeprom::read_bit(uint16_t address, uint8_t bit) {
    assert(bit < 8);
    uint8_t byte = read_byte(address);
    return (byte >> bit) & 1;
}

void eeprom::write(uint16_t address, uint8_t *data, uint8_t length) {
    assert(address <= MAX_ADDR);
    assert(address + length - 1 <= MAX_ADDR);
    assert(length <= MAX_PAGES);
    assert(length > 0);
    assert((address & 0xF0) == ((address + length - 1) & 0xF0));
    assert(data != nullptr);

    uint8_t highest_bit = (address >> 8) & 1;
    uint8_t cmd = cmd::WRITE | (highest_bit << 3);
    uint8_t lower_bits = address & 0xFF;

    enable_write();
    spi_.set_cs(cs_level::LOW);
    spi_.transfer(cmd);
    spi_.transfer(lower_bits);

    for (uint8_t i = 0; i < length; i++) {
        spi_.transfer(data[i]);
    }

    spi_.set_cs(cs_level::HIGH);
    status_poll_write();
}

void eeprom::write_byte(uint16_t address, uint8_t data) {
    write(address, &data, 1);
}

void eeprom::write_bit(uint16_t address, uint8_t bit, uint8_t value) {
    assert(bit < 8);
    assert(value < 2);

    uint8_t byte = read_byte(address);
    byte = (byte & ~(1 << bit)) | (value << bit);
    write_byte(address, byte);
}

void eeprom::enable_write() {
    spi_.set_cs(cs_level::LOW);
    spi_.transfer(cmd::WREN);
    spi_.set_cs(cs_level::HIGH);
}

void eeprom::disable_write() {
    spi_.set_cs(cs_level::LOW);
    spi_.transfer(cmd::WRDI);
    spi_.set_cs(cs_level::HIGH);
}

uint8_t eeprom::status_read() {
    spi_.set_cs(cs_level::LOW);
    spi_.transfer(cmd::RDSR);
    uint8_t status = spi_.transfer(DUMMY);
    spi_.set_cs(cs_level::HIGH);
    return status;
}

void eeprom::status_write(uint8_t status) {
    enable_write();
    spi_.set_cs(cs_level::LOW);
    spi_.transfer(cmd::WRSR);
    spi_.transfer(status);
    spi_.set_cs(cs_level::HIGH);
    status_poll_write();
}

void eeprom::status_poll_write() {
    while (status_read() & status_bits::WIP) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
