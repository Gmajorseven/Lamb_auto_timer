#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

RTC_DS3231 rtc;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int relayPin = 8;

DateTime now;
int on_hour, on_minute, off_hour, off_minute;

void lightOn() {
  digitalWrite(relayPin, HIGH);
  Serial.println("Light on!");
  lcd.setCursor(0, 1);
  lcd.print("Light on!");
}

void lightOff() {
  digitalWrite(relayPin, LOW);
  Serial.println("Light off!");
  lcd.setCursor(0, 1);
  lcd.print("Light off!");
}

void setup() {

  pinMode(relayPin, OUTPUT);

  Serial.begin(57600);
  rtc.begin();
  Wire.begin();

  if(!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    while(1);
  }
  if(rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
  }

 // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  lcd.begin(16,2);
  lcd.clear();

}

void loop() {

  now = rtc.now();
  int curent_hour = now.hour();
  int curent_minute = now.minute();
  int current_month = now.month();
  int current_hour = now.hour();
  int current_minute = now.minute();

  // Set time based on season
  if (current_month >= 4 && current_month <= 9) {
    // Summer (April to September)
    on_hour = 18;
    on_minute = 15;
    off_hour = 5;
    off_minute = 45;
  } else {
    // Winter (October to March)
    on_hour = 17;
    on_minute = 15;
    off_hour = 5;
    off_minute = 50;
  }

  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second());

  int current_time_in_minutes = curent_hour * 60 + curent_minute;
  int on_time_in_minutes = on_hour * 60 + on_minute;
  int off_time_in_munutes = off_hour * 60 + off_minute;

  if(on_time_in_minutes < off_time_in_munutes) {
    if(current_time_in_minutes >= on_time_in_minutes && current_time_in_minutes < off_time_in_munutes) {
      lightOn();
    } else {
      //test
      //lightOn();
      lightOff();
    }
  } else {
    if(current_time_in_minutes >= on_time_in_minutes || current_time_in_minutes < off_time_in_munutes) {
      lightOn();
    } else {
      //test
      //lightOn();
      lightOff();
    }
  }

  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.println(now.second());

  delay(1000);
}
