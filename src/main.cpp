#include <setup.h>
#define ERA_DEBUG
#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"
#define ERA_AUTH_TOKEN "d58ef8ad-326a-4889-844a-39a16fdcb75b"

#include <Arduino.h>
#include <ERa.hpp>
#include <ERa/ERaTimer.hpp>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//_____________wifi congig_______________//
#define BUTTON_PIN 0
#if defined(BUTTON_PIN)
// Active low (false), Active high (true)
#define BUTTON_INVERT false
#define BUTTON_HOLD_TIMEOUT 3000UL

// This directive is used to specify whether the configuration should be erased.
// If it's set to true, the configuration will be erased.
#define ERA_ERASE_CONFIG false
#endif
#if defined(BUTTON_PIN)
#include <pthread.h>
#include <ERa/ERaButton.hpp>
#endif
const char ssid[] = "PLG.mcu";
const char pass[] = "ok11111111";
// const char ssid[] = "MakerSpaceLab";
// const char pass[] = "Maker2017";

WiFiClient mbTcpClient;
#if defined(BUTTON_PIN)
ERaButton button;
pthread_t pthreadButton;
static void *handlerButton(void *args)
{
  for (;;)
  {
    button.run();
    ERaDelay(10);
  }
  pthread_exit(NULL);
}
#if ERA_VERSION_NUMBER >= ERA_VERSION_VAL(1, 2, 0)
static void eventButton(uint8_t pin, ButtonEventT event)
{
  if (event != ButtonEventT::BUTTON_ON_HOLD)
  {
    return;
  }
  ERa.switchToConfig(ERA_ERASE_CONFIG);
  (void)pin;
}
#else
static void eventButton(ButtonEventT event)
{
  if (event != ButtonEventT::BUTTON_ON_HOLD)
  {
    return;
  }
  ERa.switchToConfig(ERA_ERASE_CONFIG);
}
#endif

void initButton()
{
  pinMode(BUTTON_PIN, INPUT);
  button.setButton(BUTTON_PIN, digitalRead, eventButton,
                   BUTTON_INVERT)
      .onHold(BUTTON_HOLD_TIMEOUT);
  pthread_create(&pthreadButton, NULL, handlerButton, NULL);
}
#endif

ERA_CONNECTED()
{
  ERA_LOG("ERa", "ERa connected!");
}
ERA_DISCONNECTED()
{
  ERA_LOG("ERa", "ERa disconnected!");
}
//_______________wifi-confic-end________________________//

ERaTimer timer;
// /****************************OLED**********************************
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//   **************************************************************/

void timerEvent()
{
  ERA_LOG("Timer", "Uptime: %d", ERaMillis() / 1000L);
}

// // #include <Preferences.h>
// /**************************************************************
//  *                UART1                                       *
//  **************************************************************/

#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)     // Định nghĩa hàm in ra thông tin debug
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__) // Định nghĩa hàm in ra thông tin debug kèm xuống dòng
#define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)   // Định nghĩa hàm in ra thông tin debug theo định dạng
// /**************************************************************
//  *                UART_2                                      *
//  **************************************************************/
#define SENT(...) Serial2.print(__VA_ARGS__)     // Định nghĩa hàm in ra thông tin debug
#define SENTLN(...) Serial2.println(__VA_ARGS__) // Định nghĩa hàm in ra thông tin debug kèm xuống dòng
#define SENTF(...) Serial2.printf(__VA_ARGS__)   // Định nghĩa hàm in ra thông tin debug theo định dạng

String data = "";
string datapic = "";
String data1 = "";
uint8_t value1;
uint8_t value;
String cmd = "";
void khung_oled()
{
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE); //__
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE); //__
  display.drawLine(0, 30, 128, 30, SSD1306_WHITE); // __
  display.drawLine(0, 40, 128, 40, SSD1306_WHITE); // __
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
  if (value == HIGH)
  {
    data = "on";
  }
  else // if(value == LOW)
  {
    data = "off";
  }
  if (value1 == HIGH)
  {
    data1 = "on";
  }
  else
  {
    data1 = "off";
  }

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
  /* Get value from Virtual Pin 0 and write LED */
  value = param.getInt();
  // digitalWrite(LED_PIN, value ? HIGH : LOW);
  if (value == HIGH)
  {
    DEBUG_PRINTLN("--------------write data R1------------- ");
    DEBUG_PRINTLN(value);
    SENTLN("f");
  }
  else
  {
    DEBUG_PRINTLN("--------------write data R1------------- ");
    DEBUG_PRINTLN(value);
    SENTLN("d");
  }
}
ERA_WRITE(V1)
{
  /* Get value from Virtual Pin 0 and write LED */
  value1 = param.getInt();
  // digitalWrite(LED_PIN, value ? HIGH : LOW);
  if (value1 == HIGH)
  {
    DEBUG_PRINTLN("--------------write data R2------------- ");
    DEBUG_PRINTLN(value1);
    SENTLN("R2 1");
  }
  else
  {
    DEBUG_PRINTLN("--------------write data R2------------- ");
    DEBUG_PRINTLN(value1);
    SENTLN("R2 0");
  }
}
////////len chuagit

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(3000);
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
