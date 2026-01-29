/// @file eeprom_constants.h
/// @brief Constants and commands for 25LC040A SPI EEPROM

#pragma once
#include <cstdint>

/// @brief Maximum memory address for 25LC040A (512 bytes memory: 0..511)
static constexpr uint16_t MAX_ADDR = 0x1FF;

/// @brief Dummy byte used during SPI transfers (for clocking out data)
static constexpr uint8_t DUMMY = 0xFF;

/// @brief Maximum number of bytes per page write
static constexpr uint8_t MAX_PAGES = 16;

/// @brief Status register bit masks
namespace status_bits {
    /// @brief Write-In-Progress (WIP) bit in STATUS register
    static constexpr uint8_t WIP = 0x01;
}

/// @brief SPI EEPROM commands for 25LC040A
namespace cmd {
    /// @brief Read data from memory array
    static constexpr uint8_t READ = 0x03;

    /// @brief Write data to memory array
    static constexpr uint8_t WRITE = 0x02;

    /// @brief Reset Write Enable Latch (disable writes)
    static constexpr uint8_t WRDI = 0x04;

    /// @brief Set Write Enable Latch (enable writes)
    static constexpr uint8_t WREN = 0x06;

    /// @brief Read STATUS register
    static constexpr uint8_t RDSR = 0x05;

    /// @brief Write STATUS register
    static constexpr uint8_t WRSR = 0x01;
}

/// @brief Logic levels for SPI Chip Select (CS) line
namespace cs_level {
    /// @brief CS low (select the device)
    static constexpr bool LOW = false;

    /// @brief CS high (deselect the device)
    static constexpr bool HIGH = true;
}
