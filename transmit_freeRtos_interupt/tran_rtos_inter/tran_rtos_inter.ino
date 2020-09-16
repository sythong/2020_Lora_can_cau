
// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
#include <SoftwareSerial.h>
#include <JC_Button.h>
#include "State.h"

#define SW 8
#define PIN A0
#define LED 11
#define pinLow 200
SoftwareSerial loraSerial(2, 3);

StateType state;
long period_count_off = 30;


Button myBtn(SW);

void TaskUpdateStatus( void *pvParameters );
void TaskUpdateButton( void *pvParameters );
void ThreadTimer100ms(void *pvParameters );

void setup() {
  // Configure pin 2 as an input and enable the internal pull-up resistor.
//  Serial.begin(9600);
  loraSerial.begin(9600);
  state =  IDLE_STATE;
//  Serial.println("transmit ready");
  myBtn.begin();
  pinMode(LED, OUTPUT);
  // Now set up two tasks to run independently.
  xTaskCreate(TaskUpdateStatus,  "updateConnectStatus",  128,  NULL,  2,  NULL );
  xTaskCreate( TaskUpdateButton, "Update Event switch",  128,  NULL,  1,  NULL );
  xTaskCreate( ThreadTimer100ms, "timer 1s",  128,  NULL,  2,  NULL );

}

void loop() {
}

void TaskUpdateStatus(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  while (true)
  {
    int pin_value = analogRead(PIN);
//    Serial.println("pin value");
//    Serial.println(pin_value);
    loraSerial.print(str_bat);
    loraSerial.print(pin_value);
    loraSerial.println(",#");

    if (myBtn.isPressed())
    {
      loraSerial.println(str_off);
    }
    else
      loraSerial.println(str_on);
    vTaskDelay( 4000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskUpdateButton(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  while (true)
  {
    myBtn.read();
    switch (state)
    {
      case IDLE_STATE:
        if (myBtn.isPressed())
        {
          state = CHECKING_STATE;
//          Serial.println("CHECKING_STATE");     
        }
        vTaskDelay( 300 / portTICK_PERIOD_MS );
        break;
      case CHECKING_STATE:
        if (!myBtn.isPressed()) {
          state = TRANSMIT_STATE;
//          Serial.println("TRANSMIT STATE");
          period_count_off = TIME_TO_IDLE;
        }
        break;
      case TRANSMIT_STATE:
         loraSerial.println(str_on);
        if (period_count_off == 0)
        {
          state = IDLE_STATE;
          loraSerial.println("$1,IDLE");
//          Serial.println("IDLE STATE");
        }
        // Dieu kien 2
        if (myBtn.isPressed()) {
          for (int i = 0; i < 5; i++) {
            loraSerial.println(str_off);
          }
          state = IDLE_STATE;
          
//          Serial.println("IDLE STATE");  
        }
        
//        Serial.println("TRANSMIT STATE");
        vTaskDelay( 300 / portTICK_PERIOD_MS );
        break;
    }
    vTaskDelay( 30 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void ThreadTimer100ms(void *pvParameters )
{
  (void) pvParameters;

  while (true)
  {
    //    Serial.println(countDownConnection);
    if (period_count_off > 0)
    {
      period_count_off = period_count_off - 1;
    }
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}
