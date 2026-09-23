/*
  NM-CYD-C5
  ESP32-C5-WROOM-1
  2.8 inch 240x320

  TFT   : ST7789
  TOUCH : XPT2046
  SD    : MicroSD

  Shared SPI:
    SCK  = GPIO6
    MISO = GPIO2
    MOSI = GPIO7

  TFT CS   = GPIO23
  TOUCH CS = GPIO1
  SD CS    = GPIO10
  BL        = GPIO25

  TFT DC:
    GPIO5 below is a test value because the public
    RockBase pinout does not specify TFT DC.
*/

#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <LovyanGFX.hpp>

// ============================================================
// SPI
// ============================================================

#define SPI_SCK     6
#define SPI_MISO    2
#define SPI_MOSI    7

#define SD_SCK      SPI_SCK
#define SD_MISO     SPI_MISO
#define SD_MOSI     SPI_MOSI
#define SD_CS       10

#define TFT_CS      23
#define TFT_DC      24
#define TFT_BL      25

#define TOUCH_CS    1

// ============================================================
// Display rotation
// ============================================================
#define def_rot     3

// ============================================================
// SD SPI object
// ESP32-C5 gebruikt SPI2
// ============================================================

SPIClass sdSPI(FSPI);

// ============================================================
// LovyanGFX
// ============================================================
class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7789   _panel;
  lgfx::Bus_SPI        _bus;
  lgfx::Light_PWM      _light;
  lgfx::Touch_XPT2046  _touch;

public:
  LGFX()
  {
    // ========================================================
    // TFT SPI BUS
    // ========================================================

    {
      auto cfg = _bus.config();

      cfg.spi_host   = SPI2_HOST;

      cfg.spi_mode   = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read  = 16000000;

      cfg.spi_3wire  = false;
      cfg.use_lock   = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;

      cfg.pin_sclk   = SPI_SCK;
      cfg.pin_mosi   = SPI_MOSI;
      cfg.pin_miso   = SPI_MISO;
      cfg.pin_dc     = TFT_DC;

      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    // ========================================================
    // ST7789 PANEL
    // ========================================================

    {
      auto cfg = _panel.config();

      cfg.pin_cs        = TFT_CS;
      cfg.pin_rst       = -1;
      cfg.pin_busy      = -1;
      cfg.panel_width   = 240;
      cfg.panel_height  = 320;
      cfg.memory_width  = 240;
      cfg.memory_height = 320;
      cfg.offset_x      = 0;
      cfg.offset_y      = 0;
      cfg.offset_rotation = 0;
      cfg.readable      = false;
      cfg.invert        = false;
      cfg.rgb_order     = false;
      cfg.dlen_16bit    = false;
      cfg.bus_shared    = true;

      _panel.config(cfg);
    }

    // ========================================================
    // BACKLIGHT
    // GPIO25
    // ========================================================

    {
      auto cfg = _light.config();
      cfg.pin_bl      = TFT_BL;
      cfg.invert      = false;
      cfg.freq        = 44100;
      cfg.pwm_channel = 7;
      _light.config     (cfg);
      _panel.setLight   (&_light);
    }

    // ========================================================
    // XPT2046 TOUCH
    //
    // Shared SPI:
    // SCK  GPIO6
    // MISO GPIO2
    // MOSI GPIO7
    // CS   GPIO1
    //
    // Geen IRQ volgens gepubliceerde pinout
    // ========================================================

    {
      auto cfg = _touch.config();

      cfg.spi_host = SPI2_HOST;
      cfg.freq     = 2500000;
      cfg.pin_sclk = SPI_SCK;
      cfg.pin_mosi = SPI_MOSI;
      cfg.pin_miso = SPI_MISO;
      cfg.pin_cs   = TOUCH_CS;
      cfg.pin_int  = -1;

      // ------------------------------------------------------
      // Startwaarden XPT2046 calibratie.
      //
      // Deze kunnen later nauwkeurig worden gekalibreerd.
      // ------------------------------------------------------
/*
      cfg.x_min = 300;
      cfg.x_max = 3800;
      cfg.y_min = 300;
      cfg.y_max = 3800;
*/
      cfg.x_min = 348;
      cfg.x_max = 3794;
      cfg.y_min = 289;
      cfg.y_max = 3807;

      cfg.offset_rotation = 4;//bij def_rot 3 (USB on the left)
      cfg.bus_shared  = true;
      _touch.config     (cfg);
      _panel.setTouch   (&_touch);
    }

    // ========================================================
    // Attach panel
    // ========================================================

    setPanel(&_panel);
  }
};

// ============================================================
// Global TFT object
// ============================================================

LGFX tft;