
#include <WiFi.h> // Libraries 
#include <HTTPClient.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(23, 22, 12, 14, 27, 26); //Configuring the lcd connections


const char* ssid = "Wokwi-GUEST"; //Host
const char* password = ""; //No password require
const String API_key = "650b6a92cfa54e60590705579515619d"; 


const String cities[] = {"Sorocaba", "Ohio"}; //Cities that we want to analyze
const int num_cities = 2; 


String urls[] = {"http://api.openweathermap.org/data/2.5/weather?q=Sorocaba&appid=650b6a92cfa54e60590705579515619d&units=metric",
"http://api.openweathermap.org/data/2.5/weather?q=Ohio&appid=650b6a92cfa54e60590705579515619d&units=metric"}; //url of the api where we going t extract the data 


void setup() {
 Serial.begin(9600);
 lcd.begin(16, 2);
 WiFi.begin(ssid, password); //Initializing wifi
 pinMode(15, OUTPUT);
  Serial.print("Conectando com o WiFi...");
 while (WiFi.status() != WL_CONNECTED) { //Trying to connect 
   delay(500);
   Serial.print(".");
 }
 Serial.println("conectado!"); //Confirmation
  for (int i = 0; i < num_cities; i++) {
   urls[i] = "http://api.openweathermap.org/data/2.5/weather?q=" + cities[i] + "&appid=" + API_key + "&units=metric"; //Acessing the cities in a for loop
 }
}


void loop() {
 for (int i = 0; i < num_cities; i++) {
   if (WiFi.status() == WL_CONNECTED) {
     digitalWrite(15, HIGH); //Light with connect
     HTTPClient http;
     http.begin(urls[i]); //Enter in the link 
     int httpCode = http.GET();
     if (httpCode == HTTP_CODE_OK) {
       String resposta = http.getString(); //Catches the text containing all sorts of data 
       Serial.println(resposta);
       char* ptr = strstr((const char*)resposta.c_str(), "name");
       String cidade = &ptr[7]; //Colects the name of the city
       cidade.replace('"', '\0');
       ptr = strstr((const char*)resposta.c_str(), "temp");
       String temperatura = &ptr[6]; //Collects the temperature
       temperatura.replace(',', '\0'); //Add NULL value
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print(cidade.c_str()); //Printing the name of rhe city
       delay(2000);
       for (int j = 0; j == 0; j++) {
         lcd.clear();
         delay(2000);
         lcd.setCursor(0, 0);
         lcd.printf("Temp: ");
         lcd.print(temperatura.c_str()); //Printing temperature
         lcd.print("\xDF""C"); //Add special characters 
         delay(2000);
         lcd.clear();
         delay(2000);
       }
     }
     http.end();
   } else { //Without wifi connection
     digitalWrite(15, LOW); //Dark to not connected
     WiFi.begin(ssid, password);
     Serial.println("Reconectando ao WiFi...");
     while (WiFi.status() != WL_CONNECTED) {
       delay(500);
     }
   }
   delay(20000);
 }
}
