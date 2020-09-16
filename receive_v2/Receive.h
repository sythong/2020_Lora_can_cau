#ifndef __RECEIVE_H
#define __RECEIVE_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define RELAY1 9
#define RELAY2 8
#define BUZZ 11
#define GREEN_LED 13
#define RED_LED 12
#define TIMEOUT_CHECK_CONNECT 5


extern String lora_data;
extern int statusConnection;
extern int countDownConnection;

extern bool stringComplete; 
extern String inputString;  

extern int is_low_battery;

void lora_init(void);
void updateLoraReceive();
void updateStateLcd(void);
void do_on(void);
void do_off(void);
void do_error(void);

#endif
