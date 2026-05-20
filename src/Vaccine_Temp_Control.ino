#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ----------- PINS -----------
#define ONE_WIRE_BUS 2
#define PELTIER_PIN 9

// ----------- OBJECTS -----------
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----------- VARIABLES -----------
float tempC;
bool peltierState = false; // OFF initially

void setup() {
  pinMode(PELTIER_PIN, OUTPUT);
  digitalWrite(PELTIER_PIN, LOW);

  sensors.begin();
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Vaccine Ctrl");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read temperature
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);

  // -------- CONTROL LOGIC --------
  if (tempC > 20) {
    peltierState = true;   // ON
  } 
  else if (tempC < 15) {
    peltierState = false;  // OFF
  }

  // Apply control
  if (peltierState) {
    analogWrite(PELTIER_PIN, 255); // FULL ON
  } else {
    analogWrite(PELTIER_PIN, 0);   // OFF
  }

  // -------- SERIAL OUTPUT --------
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" C  | Peltier: ");
  Serial.println(peltierState ? "ON" : "OFF");

  // -------- LCD DISPLAY --------
  lcd.setCursor(0, 0);
  lcd.print("Temp:       ");  
  lcd.setCursor(6, 0);
  lcd.print(tempC);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Status: ");
  if (peltierState) {
    lcd.print("COOLING ");
  } else {
    lcd.print("IDLE    ");
  }

  delay(1000);         //DELAY of 1000s
}
