//created by https://github.com/gtmans july 2026
#include "DIYTZT_ESP32_024_ILI9342.h"
  String TFSCRSIZ = "2.4\"";
  String TFSCREEN = "ILI9342";
  String TFTOUCH  = "XPT2046";
  String TFSDBUS  = "HSPI";

// ---------- SD ----------
void initSD()
{
  sdSPI.begin     (SD_SCK,SD_MISO,SD_MOSI);
  pinMode         (SD_CS, OUTPUT);
  digitalWrite    (SD_CS, HIGH);
  delay           (50);

  if (!SD.begin   (SD_CS, sdSPI))
  {
    tft.println   ("SD FAIL, retry...");
  //Serial.println("SD FAIL, retry...");
    delay         (150);
    if (!SD.begin (SD_CS, sdSPI))
    {
      tft.println ("SD FAIL (final)");
      return;
    }
  }
  show_SDworks();
}

void show_SDworks()   {
  Serial.println      ("SD OK");
  tft.setTextColor    (TFT_GREEN);
  tft.println         ("SD OK");
  // --- map directory lezen ---
  File root = SD.open ("/");
  if (!root) {
    Serial.println    ("Cannot open /");
    tft.setTextColor  (TFT_RED);
    tft.println       ("Cannot open /");
    return;
  }
  File f;
  while ((f = root.openNextFile())) {
    Serial.println    (f.name());
    tft.println       (f.name());
    f.close           ();
  }
  root.close          ();
}

void setup() {
  Serial.begin        (115200);
  delay               (500);
  Serial.println      ();
  Serial.println      (__FILE__);
  //SCREEN
  tft.init            ();
  tft.setBrightness   (255);
  tft.setRotation     (def_rot); 
  tft.setTextSize     (2);
//tft.fillScreen      (TFT_RED);
//delay               (500);
//tft.fillScreen      (TFT_GREEN);
//delay               (500);
  tft.fillScreen      (TFT_BLUE);
  tft.setTextColor    (TFT_WHITE);
  tft.println         ("Init "+TFSCRSIZ+":");
  tft.setTextColor    (TFT_GREEN);
  tft.println         ("TFT_OK "+TFSCREEN);
  // SD
  tft.setTextColor    (TFT_WHITE);
  tft.println         ("Init SD"+TFSDBUS+":");
  initSD              ();
  // TOUCH
  tft.setTextColor    (TFT_WHITE);
  Serial.println      ("Testing touch "+TFTOUCH);
  tft.println         ("Testing touch "+TFTOUCH);
  tft.setTextColor    (TFT_BLUE);
  tft.setTextSize     (3);
  Serial.println      ("Setup finished!");
  //tft.drawJpgFile(SD, "/map/Apeldoorn.jpg", 0, 0);
}

void loop()
{
  uint16_t x,y;
  if (tft.getTouch(&x,&y))
  {
    Serial.printf   ("Touch: %d %d\n",x,y);
    tft.fillCircle  (x,y,5,TFT_RED);
    tft.setCursor   (86,50);
    tft.fillRect    (80, 45, 175, 35, TFT_YELLOW);
    tft.print       ("X"+String(x)+" Y"+String(y));
  }
}
