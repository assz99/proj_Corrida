#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
const byte buttonPin = 3;
const byte sensorPin = 4;
byte state = LOW;
boolean race = false, arme = false, startRace = false;
const int buttonInterval = 2000, sensorInterval = 2000, lcdInterval = 100;
unsigned long previousButtonMillis = 0, previousSensorMillis = 0, firstLap = 0, lastLap = 0, current,lcdSensorMillis=0;
int inicio = 0, fim = 0, tempo = 0, atual = 0;
String textLcd;
void setup() {
  Serial.begin(9600);
  Serial.println("inicializando");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Inicializando!");
  pinMode(buttonPin, INPUT);
  pinMode(sensorPin, INPUT_PULLUP);
  Serial.println("DEBUG");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Desarmado!");
  lcd.setCursor(0, 1);
  lcd.print("Esperando armar!");
}

void readButton() {
  if (millis() - previousButtonMillis >= buttonInterval) {
    if (digitalRead(buttonPin) == LOW) {
      Serial.println("BOTAO PRESsIONADO");
      armar();
      previousButtonMillis = millis();
    }
  }
}

void armar() {
  if (race == false) {
    Serial.print("Tracou armar: ");
    Serial.println(arme);
    if (arme == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Armado!Aguardando");
      firstLap = 0;
      lastLap = 0;
      arme = true;
      race = true;
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Desarmado!");
      arme = false;
    }
  }
}

void readSensor() {
  if (millis() - previousSensorMillis >= sensorInterval) {
    if (digitalRead(sensorPin) == LOW) {
      Serial.println("Sensor Acionado");
      racing();
      previousSensorMillis = millis();
    }
  }
}

void racing() {
  if (startRace == false) {
    Serial.println("Comecando corrida");
    startRace = true;
    firstLap = millis();
    lcd.clear();
  } else {
    Serial.println("Terminando corrida");
    startRace = false;
    race=false;
    arme=false;
    lastLap = millis();
    lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Corrida Finalizada");
    lcd.setCursor(0,0);
    lcd.print("Desarmado!");
    lcd.setCursor(0, 1);
    unsigned long res =(lastLap-firstLap);
    Serial.println("Res="+String(res)+"firstLap="+String(firstLap)+"lastLap="+String(lastLap));
    lcd.print("Tempo ="+ String(res/1000)+","+String(res%1000));
  }

}

void loop() {
  if (race == false) {
    readButton();
  } else {
    readSensor();    
  }

  if(startRace ==true){
    if (millis() - lcdSensorMillis >= lcdInterval) {
      
    current = (millis() - firstLap);
    textLcd = "Tempo:"+String(current/1000)+","+String(current%1000);
    Serial.println(textLcd);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(textLcd);
    lcdSensorMillis = millis();
    }
    
    }
}
