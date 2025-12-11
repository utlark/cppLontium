#pragma once

#include <cstdint>

/**
 * Contains detailed timing parameters of the currently detected video mode.
 *
 * hActive / vActive:
 *     Visible (active) resolution in pixels and lines.
 *
 * hTotal / vTotal:
 *     Full line/frame length including blanking intervals.
 *
 * hFrontPorch / vFrontPorch:
 *     Number of pixels/lines between the end of active video
 *     and the start of the sync pulse.
 *
 * hBackPorch / vBackPorch:
 *     Number of pixels/lines between the end of the sync pulse
 *     and the start of the next active region.
 *
 * hSyncWidth / vSyncWidth:
 *     Width of the horizontal/vertical sync pulses.
 *
 * clockFreq:
 *     Pixel clock frequency in kHz, extracted from the internal measurement registers.
 *     Useful for validating timing or selecting matching LVDS output configuration.
 */
struct VideoInfo
{
    uint16_t vActive;     // Vertical active video height
    uint16_t hActive;     // Horizontal active video width
    uint16_t vTotal;      // Total lines per frame
    uint16_t hTotal;      // Total horizontal pixels per line

    uint16_t vFrontPorch; // Vertical front-porch duration
    uint16_t vBackPorch;  // Vertical back-porch duration
    uint16_t vSyncWidth;  // Vertical sync pulse width

    uint16_t hFrontPorch; // Horizontal front-porch duration
    uint16_t hBackPorch;  // Horizontal back-porch duration
    uint16_t hSyncWidth;  // Horizontal sync pulse width

    uint32_t clockFreq;   // Measured pixel clock frequency (kHz)
};
