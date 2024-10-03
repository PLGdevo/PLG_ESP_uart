#include <setup.h>

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
 // ERa.setScanWiFi(true);
  ERa.begin(ssid, pass);

  /* Setup timer called function every second */
  timer.setInterval(1000L, timerEvent);
  DEBUG_PRINT("-----------------PLG_start----------\n\r");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    DEBUG_PRINT(F("---------SSD1306 allocation failed------"));
    for (;;)
      ;
  }

  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 5);             // Start at top-left corner
  display.println(F("PLG.Oled"));
  display.display();
}

void loop()
{

  ERa.run();
  oled();
  uartPIC();
  // timer.run();
}

// //   if (Serial2.available())
// //   {
// //     char c = Serial2.read();
// //     data += c;
// //     if (c == '\n')
// //     {
// //       DEBUG_PRINT(" du lieu tu pic:");
// //       DEBUG_PRINT(data);
// //       data = "";
// //     }
// //   } // end read uart2
// //  if(Serial.available())
// //   {
// //     char x = Serial.read();
// //     if (x == '1')
// //     {
// //       DEBUG_PRINTLN("write data 1");
// //       SENTLN("27.565./56");
// //       delay(100);

// //     }
// //     if (x == '2')
// //     {
// //       DEBUG_PRINTLN("write data 2");
// //       SENTLN("34/PLG");
// //       delay(100);

// //     }
// void handle_message(String message)
// {
//     lastCmdTime = millis();

//     /*-------restart esp-----------FINE-*/

//     if (message.startsWith("RST"))
//     {
//         DEBUG_PRINTLN("[CMD] Restart ESP");
//         delay(200);
//         ESP.restart();
//     }

//     /*-------heartbeat sender------FINE-*/

//     if (message.startsWith("LIV"))
//     { // heartbeat sender's ID
//         aliveReadTime = millis();
//     }
// //   }
//cha la kogit kkkkkkk