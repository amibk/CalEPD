#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include <stdint.h>
#include <math.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include <string>
#include <epd.h>
#include <Adafruit_GFX.h>
#include <epdspi.h>
#include <gdew_colors.h>
#include <esp_timer.h>

// Controller: EK79652  Note: This is the display that the T5S from TTGO use
#define GDEW027C44_WIDTH 176
#define GDEW027C44_HEIGHT 264
#define GDEW027C44_BUFFER_SIZE (uint32_t(GDEW027C44_WIDTH) * uint32_t(GDEW027C44_HEIGHT) / 8)
// 1 byte of this color in the buffer
#define GDEW027C44_8PIX_BLACK 0xFF
#define GDEW027C44_8PIX_WHITE 0x00

class Gdew027c44 : public Epd
{
  public:
    Gdew027c44(EpdSpi& IO);
    
    uint8_t colors_supported = 1;
    void drawPixel(int16_t x, int16_t y, uint16_t color);  // Override GFX own drawPixel method
    
    // EPD tests 
    void init(bool debug = false);

    void fillScreen(uint16_t color);
    void update();
    // Partial update of rectangle from buffer to screen, does not power off
    void updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true);

  private:
    EpdSpi& IO;
    uint8_t _buffer[GDEW027C44_BUFFER_SIZE];
    uint8_t _rbuffer[GDEW027C44_BUFFER_SIZE];
    bool color = false;
    bool _initial = true;
    bool _debug_buffer = false;
    uint16_t _setPartialRamArea(uint16_t x, uint16_t y, uint16_t xe, uint16_t ye);
    void _partialRamArea(uint8_t command, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void _writeToWindow(uint8_t command, uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h);
    void _refreshWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void _wakeUp();
    void _sleep();
    void _waitBusy(const char* message);
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h);

    // Command & data structs
    static const epd_init_3 epd_soft_start;
    static const epd_init_1 epd_panel_setting;
    static const epd_init_1 epd_extra_setting;
    // LUT init tables 
    static const epd_init_44 lut_20_vcomDC;
    static const epd_init_42 lut_21;
    static const epd_init_42 lut_22_red;
    static const epd_init_42 lut_23_white;
    static const epd_init_42 lut_24_black;
};