#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "FileSystem.h" 
#include "index.h"  

LiquidCrystal_I2C lcd(0x27, 20, 4);
WebServer server(80);

const char *ssid = "Corrida";
const byte buttonPin = 16;
const byte sensorPin = 17;
byte state = LOW;
boolean race = false, arme = false, startRace = false;
const int buttonInterval = 2000, sensorInterval = 2000, lcdInterval = 100;
unsigned long previousButtonMillis = 0, previousSensorMillis = 0, firstLap = 0, lastLap = 0, current,lcdSensorMillis=0;
int inicio = 0, fim = 0;
String textLcd, tempo = "0";


void setup() {
  Serial.begin(9600);
  Serial.println("inicializando");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Inicializando!");
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(sensorPin, INPUT_PULLUP);
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());  
  server.on("/", handleRoot);      
  server.on("/getTime", getTime);
  server.on("/getJson", getJson);
  server.on("/postJson", receiveJson);
  server.on("/delDados", delDados);
  server.begin();                  
  Serial.println("HTTP server started");
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
    tempo = String(res/1000)+","+String(res%1000);
    lcd.print("Tempo ="+ tempo );
  }

}

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void getJson(){
  Serial.println("getJSON");
  if(!readFile(SPIFFS, "/dados.txt")){
    String str = "vazio";
    str.toCharArray(dados,250);
    }
  
   server.send(200, "text/plane", dados);
  };
void getTime() {
  //Serial.println("getTime");
 server.send(200, "text/plane", tempo); 
}
void receiveJson() {
 String str = server.arg("plain");
 char strChar[250];
 str.toCharArray(strChar, 250);
 writeFile(SPIFFS, "/dados.txt", strChar);
 Serial.println("POST body was:\n" + str);
 server.send(200, ("text/plain"), "POST body was:\n" + str);
}
void delDados(){
  Serial.println("Deletando Dados");
  deleteFile(SPIFFS, "/dados.txt");
  server.send(200); 
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
    server.handleClient();
}
