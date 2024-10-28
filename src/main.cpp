#include <setup.h>


char data[4] = "";
char data1[4] = "";
char datapic[4] = "";
uint8_t value1;
uint8_t value;
String cmd = "";
void khung_oled()
{
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE); //__ hang 1
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE); //__ hang 2
  display.drawLine(0, 30, 128, 30, SSD1306_WHITE); //__ hang 3
  display.drawLine(0, 40, 128, 40, SSD1306_WHITE); //__ hang 4
  display.drawLine(34, 0, 34, 64, SSD1306_WHITE);  // |
}
void exeCmd(String message)
{
  if (message.startsWith("CH"))
  {
    // display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    delay(200);
    SENTLN("d");
    ERa.virtualWrite(V0, value);
  }
}
void uartPIC()
{
  if (Serial2.available())
  {
    char c = Serial2.read();
    cmd += c;
    if (c == '\n')
    {
      exeCmd(cmd);
      DEBUG_PRINTLN(" PIC_gui");
      DEBUG_PRINTF("%s", cmd);
      cmd = "";
    } // end read uart2
  }
}
void oled()
{
  strcpy(data, (value == HIGH) ? "on" : "off"); 
  strcpy(data1, (value1 == HIGH) ? "on" : "off");

  display.clearDisplay();
  display.setTextSize(1);
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE); // dao nguoc man hinh 180'
  display.setCursor(0, 0);
  display.println(F("PLG"));
  display.setCursor(35, 0);
  display.println("ERA_RUN");
  display.setCursor(0, 10);
  display.println("RN1");
  display.setCursor(0, 20);
  display.println("RN2");
  display.setCursor(35, 10);
  display.println(data);
  display.setCursor(35, 20);
  display.println(data1);

  khung_oled();
  display.display();
}
ERA_WRITE(V0)
{
  value = param.getInt();
  DEBUG_PRINTLN("--------------write data R1------------- ");
  DEBUG_PRINTLN(value);
  SENTLN((value == HIGH) ? "f" : "d");
}
ERA_WRITE(V1)
{
  value1 = param.getInt();
  DEBUG_PRINTLN("--------------write data R2------------- ");
  DEBUG_PRINTLN(value1);
  SENTLN((value1 == HIGH) ? "R2 1" : "R2 0");
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("PLG"));
  display.setCursor(35, 0);
  display.println("CONNECT_STA");
  display.setCursor(0, 10);
  display.println("ssid:");
  display.setCursor(0, 20);
  display.println("pass:");
  khung_oled();
  display.display();

  DEBUG_PRINT("-----------------PLG_start_oled----------\n\r");
  delay(1000);

#if defined(BUTTON_PIN)
  initButton();
  ERa.setPersistent(true);
#endif
  // ERa.setScanWiFi(false);
  ERa.begin(ssid, pass);
  timer.setInterval(1000L, timerEvent);
  DEBUG_PRINT("-----------------PLG_start----------\n\r");
  delay(1000);
  SENTLN("R2 0");
  SENTLN("d");
}

void loop()
{
  ERa.run();
  if (ERa.connected())
  {
    oled();
    uartPIC();
  }
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
