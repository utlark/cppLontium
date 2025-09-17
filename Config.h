#pragma once

/**
 * LVDS Physical Port Selection
 * Selects which physical LVDS output port to use
 */
#define LVDS_1_Port 0x00  // Use LVDS output port 1 (primary)
#define LVDS_2_Port 0x01  // Use LVDS output port 2 (secondary)

/**
 * Synchronization Mode Selection
 * Determines how sync signals are handled in the LVDS output
 */
#define DE_Mode    0x20   // Data Enable mode (separate DE signal)
#define Sync_Mode  0x00   // Sync-on-Green mode (composite sync signals)

/**
 * Color Depth Configuration
 * Sets the number of bits per color channel
 */
#define ColorDepth_8_bit 0x00  // 8 bits per color channel (24-bit total color)
#define ColorDepth_6_bit 0x10  // 6 bits per color channel (18-bit total color, FRC)

/**
 * Port Channel Swap Control
 * Swaps the data channels between LVDS ports
 */
#define C_D_Port_Swap_En  0x02  // Enable channel-port swapping
#define C_D_Port_Swap_Dis 0x00  // Disable channel-port swapping (normal operation)

/**
 * Red-Blue Color Swap Control
 * Swaps red and blue color channels
 */
#define R_B_Color_Swap_En  0x01  // Enable red-blue color swap
#define R_B_Color_Swap_Dis 0x00  // Disable color swap (normal RGB order)

/**
 * LVDS Output Enable/Disable
 * Master control for LVDS output stage
 */
#define LVDS_Output_En  0x40  // Enable LVDS output
#define LVDS_Output_Dis 0x00  // Disable LVDS output (power saving mode)

/**
 * LVDS Data Mapping Standard
 * Selects the data mapping format for LVDS output
 */
#define VESA  0x00  // VESA standard mapping (most common for PC displays)
#define JEIDA 0x80  // JEIDA standard mapping (common for Japanese displays)

// Standard Definition Television formats
// Typical analog TV and basic digital TV resolutions with 4:3 or 16:9 aspect ratios
#define SDTV  0x00 // 480i, 480P, 576i, 576P (720x480, 720x576)

// Standard Definition PC formats
// Traditional computer monitor resolutions with 4:3 aspect ratio
#define SDPC  0x10 // 640x480 (VGA), 800x600 (SVGA), 1024x768 (XGA), 1152x864 (XGA+)

// High Definition Television formats
// Modern broadcast and streaming HD formats with 16:9 aspect ratio
#define HDTV  0x20 // 720P (1280x720), 1080P (1920x1080), 1080i

// High Definition PC formats
// Common laptop and desktop monitor resolutions, often with 16:9 or 16:10 aspect ratios
#define HDPC  0x30 // 1366x768 (WXGA), 1600x900 (HD+), 1920x1200 (WUXGA), 2560x1440 (QHD)

/**
 * Default Configuration Settings
 * These define the default operating parameters for the LVDS interface
 */

#define LVDS_Map VESA                           // Use VESA data mapping standard
#define CP_Convert_Mode SDPC                    // Default to HD television format conversion
#define LVDS_Port LVDS_1_Port                   // Use primary LVDS output port
#define LVDS_SyncMode Sync_Mode                 // Use composite sync mode
#define LVDS_ColorDepth ColorDepth_8_bit        // 8-bit color depth (24-bit total)
#define LVDS_C_D_Port_Swap C_D_Port_Swap_Dis    // Disable channel-port swapping
#define LVDS_R_B_Color_Swap R_B_Color_Swap_Dis  // Disable red-blue color swap
