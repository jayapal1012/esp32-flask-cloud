//SD Pin	        ESP32 Pin
//CS	            GPIO 5
//MOSI	          GPIO 23
//MISO	          GPIO 19
//SCK	            GPIO 18
//GND	            GND
//VCC	            5v

//DS18B20 / MAX31850 Pin       	Connect To
//VCC	                          ESP32 3V3
//GND	                          ESP32 GND
//DATA	                        ESP32 GPIO 4
//4.7kΩ Resistor	              Between DATA and 3V3
#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4  // GPIO pin connected to DS18B20

const char* ssid = "BME Lab";
const char* password = "bmelab@123";

// Replace with your Render URL
const char* serverURL = "";
//http://esp32-temperature-iot-4.onrender.com/upload

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Wi-Fi connected.");
  Serial.print("📶 IP Address: ");
  Serial.println(WiFi.localIP());

  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverURL);  // HTTP (no SSL)
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "temperature=" + String(tempC, 2);
    int httpResponseCode = http.POST(postData);

    Serial.print("🌡️ Temp: ");
    Serial.println(tempC);
    Serial.print("📤 POST status: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("⚠️ Wi-Fi disconnected.");
  }

  delay(5000);  // Wait 5 seconds
}
