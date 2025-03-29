#include <DallasTemperature.h>
#include <OneWire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h> // Include I2C LCD library

#define ONE_WIRE_BUS 18

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

// Define I2C address for your LCD (consult LCD datasheet). SDA and SCL of the LCD is on P21 and P22 respectively.
const int lcd_I2C_address = 0x27; // Replace with your actual address

// Define LCD size (adjust based on your LCD)
LiquidCrystal_I2C lcd(lcd_I2C_address, 16, 2); // Corrected initialization

// Define buzzer pin
const int buzzerPin = 15;

// Define temperature limits
const float upperLimit = 38.0;
const float lowerLimit = 33.5;

// WiFi credentials
const char* ssid = "MAYOWA";
const char* password = "JayMayor";

// Server details
const char* serverAddress = "https://temperature-sensor-project.onrender.com/api/save"; // Replace with your server address

void setup() {
  Serial.begin(9600);
  lcd.init(); // Initialize I2C LCD
  lcd.backlight(); // Turn on backlight (optional)

  sensors.begin();
  sensors.requestTemperatures();
  pinMode(buzzerPin, OUTPUT);

  connectToWiFi();
}

void loop() {
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  delay(1500);
  Serial.println(temperature);

  // Display temperature on LCD
  lcd.clear();
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("°C");

  // Check temperature limits and activate buzzer
  if (temperature > upperLimit || temperature < lowerLimit) {
    lcd.setCursor(0, 1);
    lcd.print("WARNING!");
    digitalWrite(buzzerPin, HIGH); // Activate buzzer
    delay(1500); // Delay to prevent constant activation
  } else {
    digitalWrite(buzzerPin, LOW); // Deactivate buzzer
    delay(1500); // Delay to prevent constant deactivation
  }

  // Send temperature data to server
  sendDataToServer(temperature);
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    lcd.print("Connecting... ");
    delay(1500);
    Serial.print(".");
  }
  lcd.clear();
  lcd.print("Connected");
  Serial.println("Connected");
}

void sendDataToServer(float temperature) {
  HTTPClient http;
  http.begin(serverAddress);
  http.addHeader("Content-Type", "application/json");

  // Prepare the data to send as JSON
  String postData = "{\"temperature\":" + String(temperature) + "}";

  // Send the POST request
  int httpResponseCode = http.POST(postData);

  // Check for a valid response
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    lcd.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    lcd.print(httpResponseCode);
  } else {
    Serial.print("Error code: ");
    lcd.print("Error code: ");
    Serial.println(httpResponseCode);
    lcd.print(httpResponseCode);
    
  }

  // Free resources
  http.end();
}
