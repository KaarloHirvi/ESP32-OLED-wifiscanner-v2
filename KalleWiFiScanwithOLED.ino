/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include "ESP8266WiFi.h"
#include <Arduino.h>
/* Added to support OLED */
#include <U8g2lib.h>
/* Added to support basic string ops */
#include <string.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

char msg2[100];

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  u8g2.begin();
  Serial.println("Setup done");
}

void loop() {
  Serial.println("scan start");
  
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose font
  
  if (n == 0)
  {
    strcpy(msg2, "Scanned. No wifi found");
    Serial.println(msg2);
    u8g2.drawStr(0,10, msg2);
    u8g2.sendBuffer();          // from memory to the display
    u8g2.clearBuffer();          // clear the internal memory
  } 
  else
  {
    char countStr[10];        
    Serial.print(n);
    Serial.println(" networks found");
    
    u8g2.drawStr(0,10, "Scanned.");
    u8g2.sendBuffer();
    delay(400);
    u8g2.drawStr(0,20, "Wifi was found");
    u8g2.sendBuffer();          // from memory to the display
    u8g2.clearBuffer();          // clear the internal memory
    delay(2000);
    int j = 0;
    for (int i = 0; i < n; ++i) {
      delay(200); 
      if (j==3) {
        delay(300);
        u8g2.clearBuffer();
        u8g2.sendBuffer();
        j=0;
      }
      u8g2.setCursor(0,(j+1)*10);
      u8g2.print(WiFi.SSID(i));
      u8g2.sendBuffer();          // from memory to the display
      delay(500);
      // u8g2.clearBuffer();          // clear the internal memory
      
      sprintf(countStr, "%d", i+1);
      strcpy(msg2, countStr);
      strcat(msg2, " :");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(100);
      j++;
    }
  }
  // u8g2.sendBuffer();          // transfer internal memory to the display
  Serial.println("");

  // Wait a bit before scanning again
  delay(1000);
}
