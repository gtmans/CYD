#pragma once
// ---------------- SD ----------------
#include  <SD.h>
#include  <SPI.h>
#include <LovyanGFX.hpp>

#define SD_CS   5
#define SD_SCK  18 
#define SD_MISO 19 
#define SD_MOSI 23
  
#define           def_rot 0
SPIClass          sdSPI(HSPI);
  
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9342  _panel;
  lgfx::Bus_SPI        _bus;
  lgfx::Light_PWM      _light;
  lgfx::Touch_XPT2046  _touch;

public:
  LGFX() {
    {
      auto cfg = _bus.config();
      cfg.spi_host   = VSPI_HOST;
      cfg.spi_mode  = 0;
      cfg.freq_write= 55000000;
      cfg.freq_read = 20000000;
      cfg.pin_sclk  = 14;
      cfg.pin_mosi  = 13;
      cfg.pin_miso  = 12;
      cfg.pin_dc    = 2;

      _bus.config     (cfg);
      _panel.setBus   (&_bus);
    }
    // --- TFT panel ---
    {
      auto cfg =        _panel.config();
      cfg.pin_cs        = 15;
      cfg.pin_rst       = 4;
      cfg.pin_busy      = -1; 
      cfg.panel_width   = 320;//240
      cfg.panel_height  = 240;//240
      cfg.dummy_read_pixel = 8;// 
      cfg.dummy_read_bits = 1;//
      cfg.memory_width  = 320;//240
      cfg.memory_height = 240;//240
      cfg.invert        = true;//false
      cfg.rgb_order     = false;//true
    //cfg.bus_shared    =  true;// If the bus is shared with the SD card, set to true
    //cfg.bus_shared    =  false;// If the bus is shared with the SD card, set to true

      _panel.config     (cfg);
    }
    // --- Backlight ---
    {
      auto cfg =        _light.config();
      cfg.pin_bl        = 27;//or 21?
      cfg.freq          = 44100;
      cfg.pwm_channel   = 7;
      _light.config     (cfg);
      _panel.setLight   (&_light);
    }
// --- Touch XPT2046 ---
    {
      auto cfg = _touch.config();
      cfg.spi_host = VSPI_HOST;
      cfg.freq     = 1000000;
      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = 12;
      cfg.pin_cs   = 33;
      cfg.pin_int  = 36;
/*      
      cfg.x_min    = 200;
      cfg.x_max    = 3600;
      cfg.y_min    = 300;
      cfg.y_max    = 3900;
*/    
      cfg.x_min    = 280;
      cfg.x_max    = 3860;
      cfg.y_min    = 340;
      cfg.y_max    = 3860;

      _touch.config(cfg);
      _panel.setTouch(&_touch);
    }
    setPanel(&_panel);
  }
};
LGFX tft;
