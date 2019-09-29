#include "DHT.h"
//here we use 14 of ESP32 to read data
#define DHTPIN 27
//our sensor is DHT11 type
#define DHTTYPE DHT11
//create an instance of DHT sensor
DHT dht(DHTPIN, DHTTYPE);
#include "BluetoothSerial.h"
 
BluetoothSerial SerialBT;
 
void setup() {
  Serial.begin(115200);
 
  if(!SerialBT.begin("ESP32")){
    Serial.println("An error occurred initializing Bluetooth");
  }
dht.begin();
}
 
void loop() {
  
  while(SerialBT.available()){
    Serial.write(SerialBT.read());
    

  }//use the functions which are supplied by library.
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
Serial.println();
// Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.print("Failed to read from DHT sensor!");
    return;
    }
// print the result to Terminal
Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.println(" *C ");

  SerialBT.println("humidity and temperature |");
  Serial.println("writing to  phone");
  SerialBT.println(h);
  SerialBT.println("|");
  SerialBT.println(t);

  

//we delay a little bit for next read
delay(2000);
}
