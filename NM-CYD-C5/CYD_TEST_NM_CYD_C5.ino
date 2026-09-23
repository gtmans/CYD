#include          "NM-CYD-C5_ST7789.h"
String TFSCRSIZ = "2.8\"";
String TFSCREEN = "ST7789";
String TFTOUCH  = "XPT2046";
String TFSDBUS  = "SPI2";

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
    Serial.println("SD FAIL, retry...");
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
  //TFT
  tft.init            ();
  tft.setBrightness   (255);
  tft.setRotation     (def_rot); 
  tft.setTextSize     (2);
  tft.fillScreen      (TFT_RED);
  delay               (500);
  tft.fillScreen      (TFT_GREEN);
  delay               (500);
  tft.fillScreen      (TFT_BLUE);
  tft.setTextColor    (TFT_WHITE);
  tft.println         ("Init "  + TFSCRSIZ+":");
  tft.setTextColor    (TFT_GREEN);
  tft.println         ("TFT_OK "+ TFSCREEN);
  // SD
  tft.setTextColor    (TFT_WHITE);
  tft.println         ("Init SD"+TFSDBUS+":");
  initSD              ();
  tft.setTextColor    (TFT_WHITE);
  testPSRAM           ();
  tft.setTextSize     (3);
  tft.setTextColor    (TFT_BLACK);
  Serial.println      ("Testing touch "+TFTOUCH);
  Serial.println      ("Setup finished!");
}

/*SDRAM & FLASH settings Arduino IDE 2.x
  arduino Tools menu set: Board: ESP32C5 Dev Module
  arduino Tools menu set: Flash  size: 16MB
  arduino Tools menu set: PSRAM: Enabled
*/

void testPSRAM(){
  bool PSR            = psramFound();
  if  (PSR){
    tft.setTextColor    (TFT_GREEN);
    int PSRSIZE       = ESP.getPsramSize()/1000000;
    int PSRFREE       = ESP.getFreePsram()/1000000;
    tft.println         ("PSRAM found!");
    tft.printf          ("PSRAM size: %u Kbytes\n",PSRSIZE);
    tft.printf          ("PSRAM free: %u Kbytes\n",PSRFREE);
    Serial.printf       ("PSRAM size: %u Kbytes\n",PSRSIZE);
    Serial.printf       ("PSRAM free: %u Kbytes\n",PSRFREE);
  } else {
    tft.setTextColor    (TFT_RED);
    String msg        = "PSRAM NOT found, change Tools/Settings?";
    tft.println         (msg);
    Serial.println      (msg);
  }
}

void loop()
{
  uint16_t x,y;
  if (tft.getTouch      (&x,&y))
  {
    Serial.printf       ("Touch: %d %d\n",x,y);
    tft.fillCircle      (x,y,5,TFT_RED);
    tft.setCursor       (86,50);
    tft.fillRect        (80, 45, 175, 35, TFT_YELLOW);
    tft.print           ("X"+String(x)+" Y"+String(y));
  }
}
