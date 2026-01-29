#pragma once

#include "spi_interface.h"


#include <cstdint>

/// @brief Class for interfacing with 25LC040A SPI EEPROM using bit-banging SPI.
class eeprom {
public:
    /// @brief Constructor
    /// @param spi Reference to SPI interface
    explicit eeprom(spi &spi);

    /// @brief Read multiple bytes from EEPROM
    /// @param address Start address (0..MAX_ADDR)
    /// @param data Pointer to buffer to store read data
    /// @param length Number of bytes to read
    void read(uint16_t address, uint8_t *data, uint16_t length);

    /// @brief Read a single byte from EEPROM
    /// @param address Byte address
    /// @return Data byte at the address
    uint8_t read_byte(uint16_t address);

    /// @brief Read a single bit from EEPROM
    /// @param address Byte address
    /// @param bit Bit index (0..7)
    /// @return Value of the bit (0 or 1)
    uint8_t read_bit(uint16_t address, uint8_t bit);

    /// @brief Write multiple bytes to EEPROM
    /// @param address Start address
    /// @param data Pointer to data buffer
    /// @param length Number of bytes (max MAX_PAGES)
    void write(uint16_t address, uint8_t *data, uint8_t length);

    /// @brief Write a single byte to EEPROM
    /// @param address Byte address
    /// @param data Data byte to write
    void write_byte(uint16_t address, uint8_t data);

    /// @brief Write a single bit to EEPROM
    /// @param address Byte address
    /// @param bit Bit index (0..7)
    /// @param value Bit value (0 or 1)
    void write_bit(uint16_t address, uint8_t bit, uint8_t value);

    /// @brief Enable write operations (sets WEL bit)
    void enable_write();

    /// @brief Disable write operations (resets WEL bit)
    void disable_write();

    /// @brief Read the STATUS register
    /// @return Status byte (WIP, WEL, BP0/1, WPEN)
    uint8_t status_read();

    /// @brief Write the STATUS register
    /// @param status Byte to write into STATUS register
    void status_write(uint8_t status);

    /// @brief Wait until the EEPROM finishes a write cycle (poll WIP)
    void status_poll_write();

private:
    spi &spi_; /// Reference to SPI interface
};
