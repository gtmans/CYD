//https://nl.aliexpress.com/item/1005010580680470.html 3.5inch
//https://ae01.alicdn.com/kf/S04556ab338724233b4bdbe7a012b868dp.jpg
#pragma once
// ---------------- SD ----------------
#include  <SD.h>
#include  <SPI.h>
#include <LovyanGFX.hpp>

#define SD_CS   5
#define SD_SCK  18 
#define SD_MISO 19 
#define SD_MOSI 23
SPIClass sdSPI(HSPI);
#define def_rot 3

class LGFX : public lgfx::LGFX_Device {

  lgfx::Bus_SPI       _bus_tft;
  lgfx::Panel_ST7796  _panel;
  lgfx::Light_PWM     _light;
  lgfx::Touch_XPT2046 _touch;

public:

LGFX()
{

  // ---------- TFT SPI ----------
  {
    auto cfg = _bus_tft.config();

    cfg.spi_host   = VSPI_HOST;
    cfg.spi_mode   = 0;

    cfg.freq_write = 40000000;
    cfg.freq_read  = 16000000;

    cfg.pin_sclk = 14;
    cfg.pin_mosi = 13;
    cfg.pin_miso = 12;
    cfg.pin_dc   = 2;

    _bus_tft.config(cfg);
    _panel.setBus(&_bus_tft);
  }


  // ---------- TFT ST7796 ----------
  {
    auto cfg = _panel.config();

    cfg.pin_cs  = 15;
    cfg.pin_rst = -1;

    cfg.panel_width  = 320;
    cfg.panel_height = 480;

    cfg.memory_width  = 320;
    cfg.memory_height = 480;

    cfg.rgb_order = false;
    cfg.invert    = false;

    _panel.config(cfg);
  }


  // ---------- Backlight ----------
  {
    auto cfg = _light.config();

    cfg.pin_bl = 27;
    cfg.invert = false;

    _light.config(cfg);
    _panel.setLight(&_light);
  }


  // ---------- Touch XPT2046 ----------
  {
    auto cfg = _touch.config();

    cfg.spi_host = VSPI_HOST;
    cfg.freq     = 2500000;

    cfg.pin_sclk = 14;
    cfg.pin_mosi = 13;
    cfg.pin_miso = 12;

    cfg.pin_cs  = 33;
    cfg.pin_int = 36;

    cfg.bus_shared = true;

    // fysieke touch oriëntatie
    cfg.x_min = 280;
    cfg.x_max = 3860;

  //cfg.y_min = 3860;
  //cfg.y_max = 340;
    cfg.y_min = 340;
    cfg.y_max = 3860;
    
    cfg.offset_rotation  =  6;  //fout 0,1,2,3,4,5,7 Offset of values in the direction of rotation 0 ~ 7 (4 ~ 7 are upside down)
    _touch.config(cfg);
    _panel.setTouch(&_touch);
  }


  setPanel(&_panel);
}

};

LGFX tft;
