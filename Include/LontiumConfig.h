#pragma once

#include <cstdint>

/**
 * Selects the number of LVDS channels to use (single or dual link)
 */
enum class ChannelWidth : uint8_t {
    Single = 0x00,  // Single LVDS link (one channel)
    Dual = 0x01     // Dual LVDS link (two channels)
};

/**
 * Determines how sync signals are handled in the LVDS output
 */
enum class SyncMode : uint8_t {
    DE_Mode = 0x20,     // Data Enable mode (separate DE signal)
    Sync_Mode = 0x00    // Sync-on-Green mode (composite sync signals)
};

/**
 * Sets the number of bits per color channel
 */
enum class ColorDepth : uint8_t {
    Bit_8 = 0x00,  // 8 bits per color channel (24-bit total color)
    Bit_6 = 0x10   // 6 bits per color channel (18-bit total color, FRC)
};

/**
 * Swaps the data channels between LVDS ports
 */
enum class C_D_PortSwap : uint8_t {
    Disabled = 0x00,    // Normal operation
    Enabled = 0x02      // Enable channel-port swapping
};

/**
 * Swaps red and blue color channels
 */
enum class R_B_ColorSwap : uint8_t {
    Disabled = 0x00,    // Normal RGB order
    Enabled = 0x01      // Enable red-blue color swap
};

/**
 * Master control for LVDS output stage
 */
enum class LVDS_Output : uint8_t {
    Disabled = 0x00,    // Disable LVDS output (power saving mode)
    Enabled = 0x40      // Enable LVDS output
};

/**
 * Selects the data mapping format for LVDS output
 */
enum class LVDS_Map : uint8_t {
    VESA = 0x00,    // VESA standard mapping (most common for PC displays)
    JEIDA = 0x80    // JEIDA standard mapping (common for Japanese displays)
};

/**
 * Conversion mode for video/color formats
 */
enum class CP_Convert_Mode : uint8_t {
    // Standard Definition Television formats (4:3 or 16:9)
    // Examples: 480i, 480p (720x480), 576i, 576p (720x576)
    SDTV = 0x00,

    // Standard Definition PC formats (4:3)
    // Examples: 640x480 (VGA), 800x600 (SVGA), 1024x768 (XGA), 1152x864 (XGA+)
    SDPC = 0x10,

    // High Definition Television formats (16:9)
    // Examples: 1280x720 (720p), 1920x1080 (1080p, 1080i)
    HDTV = 0x20,

    // High Definition PC formats (16:9 / 16:10)
    // Examples: 1366x768 (WXGA), 1600x900 (HD+), 1920x1200 (WUXGA), 2560x1440 (QHD)
    HDPC = 0x30
};

struct LontiumConfig {
    LVDS_Map lvdsMap = LVDS_Map::VESA;
    LVDS_Output lvdsOutput = LVDS_Output::Enabled;
    CP_Convert_Mode cpConvertMode = CP_Convert_Mode::HDTV;
    ChannelWidth channelWidth = ChannelWidth::Dual;
    SyncMode syncMode = SyncMode::Sync_Mode;
    ColorDepth colorDepth = ColorDepth::Bit_8;
    C_D_PortSwap cDPortSwap = C_D_PortSwap::Disabled;
    R_B_ColorSwap rBColorSwap = R_B_ColorSwap::Disabled;
};
