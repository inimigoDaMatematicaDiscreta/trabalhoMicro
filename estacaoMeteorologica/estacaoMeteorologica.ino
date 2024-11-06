#include "DHT.h"
#include "LiquidCrystal.h"
#include <SoftwareSerial.h>

#define DHT_PIN 4           
#define DHTTYPE DHT11

#define RS 12    //MUDANÇA NA ORDEM DOS PINOS PARA QUE O APP (DHT11 with Bluetooth) LEIA OS DADOS
#define EN 9              
#define D4 5
#define D5 6
#define D6 7
#define D7 8              

#define INTERVALO 5000

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
DHT dht(DHT_PIN, DHTTYPE);
SoftwareSerial bluetooth(2, 3); // RX e TX para o HC-06

unsigned long previousMillis = 0; // armazena o último tempo
const long interval = 1000; 

void setup() {
  bluetooth.begin(9600);  
  dht.begin();            


  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("FUNCIONOU INFERNO");
  delay(5000);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);  

  if (isnan(h) || isnan(t) || isnan(f)) {
    bluetooth.println(F("Falha na leitura do sensor"));
    lcd.clear();
    lcd.print("Falha na leitura");
  } 
  else {
    // Criação da string no formato JSON 
    unsigned long currentMillis = millis();  // millis() retorna o tempo em ms que decorreu desde a inicialização/reset do arduino 
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      String str1 =  String(t, 2);  
      String str2 =  String(h, 2);  
      String str3 =  String(f, 2);  
      // Formato JSON (Era isso que faltava para os dados serem recebidos e interpretados pelo app)
      String my_String = "{'data':[{'data1':'"+str1+"','data2':'"+str2+"','data3':'"+str3+"'}]}";
      
      bluetooth.print(my_String);  
      bluetooth.println("");  
    }

    // Exibição no LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Umid: ");
    lcd.print(h);
    lcd.print("%");

    delay(2000);  
  }

  //teste

  delay(INTERVALO);  
}
