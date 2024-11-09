#define ERA_DEBUG
#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"
#define ERA_AUTH_TOKEN "d58ef8ad-326a-4889-844a-39a16fdcb75b"

#include <Arduino.h>
#include <ERa.hpp>
#include <ERa/ERaTimer.hpp>
#include <Wire.h>
#include <SPI.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

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
