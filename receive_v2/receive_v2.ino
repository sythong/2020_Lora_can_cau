
// Include Arduino FreeRTOS library
#include <SoftwareSerial.h>
#include "Receive.h"
#include <Wire.h>
#include <TimerOne.h>



String lora_data;
int statusConnection = 0;
int countDownConnection = 40;
int is_low_battery = 0;

bool stringComplete = false;
String inputString = "";

long last_time = 0;
void setup() {
  lora_init();
  Timer1.initialize(1000000);
  Timer1.attachInterrupt( updateTimer1s );
  last_time = millis();
}

void loop() {
  updateLoraReceive();
  if(millis()-last_time >100)
  {
    last_time = millis();
    updateStateLcd();
  }
}

//void TaskReceiveLora(void *pvParameters)  // This is a task.
//{
//  (void) pvParameters;
//  // initialize the LCD

//  while (true)
//  {
//    
//
//    if (is_low_battery == 1)
//    {
//      lcd.setCursor(2, 1);
//      lcd.print("  PIN YEU  ");
//    }
//    else
//    {
//      lcd.setCursor(2, 1);
//      lcd.print("   PIN  OK   ");
//    }
//
//    vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for one second
//  }
//}

void updateTimer1s()
{
  if (countDownConnection > 0)
      countDownConnection = countDownConnection - 1;
  else
    statusConnection = 0;
//    Serial.println(" update 1s");
}
