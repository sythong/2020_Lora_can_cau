#include "Receive.h"
#include <LiquidCrystal_I2C.h>

typedef enum {IDLE_STATE, RECEIVE_STATE, ALERT_STATE, ERROR_STATE} stateType;
static stateType loraState;

static int errorFlag = 0;
static int receiveFlag = 0;

static unsigned long previousTime = 0;
static unsigned long currentTime = 0;
int idle_state = 0;
int bat_var_int = 0;
float battery_value = 0;

SoftwareSerial loraSerial(3, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void lora_init(void) {
  Serial.begin(9600);
  loraSerial.begin(9600);
  loraState = RECEIVE_STATE;

  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" RECEIVED DEVICE  ");
  delay(1000);
  Serial.println("RECEIVE READY");

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(BUZZ, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
}

void updateLoraReceive()
{
  if (loraSerial.available()) {
    char inChar = (char)loraSerial.read();
    //    Serial.print(inChar);
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  if (stringComplete)
  {
    Serial.print("Done String = ");

    String dataReceive = inputString;
    Serial.println(dataReceive);
    inputString = "";
    stringComplete = false;
    if (dataReceive.indexOf("$1") > -1)
    {
      statusConnection = 1;
      countDownConnection = TIMEOUT_CHECK_CONNECT;
    }
    if (dataReceive.indexOf("$1,ON") > -1)
    {
      do_on();
      lcd.setCursor(13, 1);
      lcd.print("ON ");
    }
    if (dataReceive.indexOf("$1,OFF") > -1)
    {
      do_off();
      Serial.println("off");
      lcd.setCursor(13, 1);
      lcd.print("OFF");
    }
    // get state transmit
    if (dataReceive.indexOf("$1,IDLE") > -1)
    {
      idle_state = 1;
      Serial.println("idle state of transmit");
    }

    if (dataReceive.indexOf("$1BAT,") >= 0)
    {
      String bat_val = dataReceive.substring(6, dataReceive.indexOf(",#"));
      bat_var_int = bat_val.toInt();
      battery_value = (float)bat_var_int * 0.00484;
      Serial.print("value of batery = ");
      Serial.println(battery_value);
    }
  }
}

void updateStateLcd(void) {
  if (statusConnection == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("LOI KET NOI      ");
    do_error();
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    digitalWrite(BUZZ, !digitalRead(BUZZ));
    
    battery_value = 0;
    lcd.setCursor(13, 1);
    lcd.print("ON ");
  }
  else
  {
    digitalWrite(BUZZ, LOW);  // tat chuong
    digitalWrite(RED_LED, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("KET NOI TOT       ");
  }

  if (battery_value >= 4.2)
  {
    lcd.setCursor(0, 1);
    lcd.print("PIN: 100% ");
  }
  if ((battery_value < 4.2) && (battery_value >= 4.1))
  {
    lcd.setCursor(0, 1);
    lcd.print("PIN: 95%  ");
  }
  if ((battery_value >= 4.0) && (battery_value < 4.1))
  {
    lcd.setCursor(0, 1);
    lcd.print("PIN: 84%  ");
  }
  if ((battery_value >= 3.9) && (battery_value < 4.0))
  {
    lcd.setCursor(0, 1);
    lcd.print("PIN: 74%  ");
  }
  if ((battery_value >= 3.8) && (battery_value < 3.9))
  {
    lcd.setCursor(0, 1);
    lcd.print("PIN: 60%  ");
  }
  if ((battery_value >= 3.7) && (battery_value < 3.8))
  {
    lcd.setCursor(0, 1);
    lcd.print("PIN: 51%  ");
  }
  if ((battery_value >= 3.6) && (battery_value < 3.7))
  {
    lcd.setCursor(0, 1);
    lcd.print("PIN: 37%  ");
  }
  if ((battery_value >= 3.5) && (battery_value < 3.6))
  {
    lcd.setCursor(0, 1);
    lcd.print("PIN: 18%  ");
  }
  if ((battery_value >= 3.4) && (battery_value < 3.5))
  {
    lcd.setCursor(0, 1);
    lcd.print("PIN: 9%   ");
  }
  if (battery_value < 3.4)
  {
    lcd.setCursor(0, 1);
    lcd.print("PIN: 0%   ");
  }
}

void do_on(void) {
  digitalWrite(RELAY1, LOW);
  digitalWrite(GREEN_LED, LOW);
}

void do_off(void) {
  digitalWrite(RELAY1, HIGH);
  digitalWrite(GREEN_LED, HIGH);

}

void do_error(void) {
  digitalWrite(RELAY1, LOW);
  //  digitalWrite(BUZZ, HIGH);
  //  digitalWrite(RED_LED, LOW);
}
