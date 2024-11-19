// BLYNK AUTH CREDETIALS
#define BLYNK_TEMPLATE_ID "xxxxxxxxxxxxxxxx"
#define BLYNK_TEMPLATE_NAME "xxxxxxxxxxxxxx"
#define BLYNK_AUTH_TOKEN "xxxxxxxxxxxxxxxxxxx"

// WiFi Credentials
const char* ssid = "xxxxxxx";
const char* password = "xxxxxxxxxx";


// LIBRARIES IMPORT
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>  // Include ESP8266 WiFi library
#include <BlynkSimpleEsp8266.h>  // Include Blynk library for ESP8266
#include "RTClib.h"       // Library for DS3231
#include <DHT.h>          // Library for DHT sensor
#include <Adafruit_BMP280.h> // Library for BMP280
#include <Battery.h> // Library for battery sensing

// OLED display dimensions setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// RTC object
RTC_DS3231 rtc;

// DHT sensor configuration
#define DHTPIN D5 // Connect DHT11 to GPIO14 (D5)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// BMP280 object
Adafruit_BMP280 bmp; // Default I2C address is 0x76 or 0x77

// BatterySense configuration
#define BATTERY_PIN A0   // Analog pin for battery sensing
Battery batt = Battery(3000, 4200, BATTERY_PIN, 10);

// Arrays to store the last 5 readings
#define NUM_READINGS 5
float tempReadings[NUM_READINGS] = {0};
float humReadings[NUM_READINGS] = {0};
float pressReadings[NUM_READINGS] = {0};

void setup() {
  Serial.begin(115200);


  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED Display Not Found"));
    for (;;);
  }

  display.clearDisplay();
  display.display();

  // Loading Display
  LoadingScreen();

  // Initialize RTC
  rtc.begin();

  // Set the RTC to the current system time
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Initialize DHT sensor
  dht.begin();

  // Initialize BMP280
  bmp.begin();
  // Initialize BatterySense
  batt.begin(3300, 1.47, &sigmoidal);

  // Perform sensor tests
  sensorTest();

  // Connect To Wifi
  connectWifi();

  // Initialize Blynk
  if (WiFi.status() == WL_CONNECTED)
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  }

void loop() {
  // Get current time from RTC
  DateTime now = rtc.now();
  if(WiFi.status() != WL_CONNECTED){
   for(int i=0;i<5;i++){
    WiFi.begin(ssid, password);
   }
  }
  // Read sensor data
  for (int i = 0; i < NUM_READINGS; i++) {
    tempReadings[i] = dht.readTemperature();
    humReadings[i] = dht.readHumidity();
    pressReadings[i] = bmp.readPressure()/100.0F;
  }

  // Calculate averages
  float avgTemp = calculateAverage(tempReadings, NUM_READINGS);
  float avgHum = calculateAverage(humReadings, NUM_READINGS);
  float avgPress = calculateAverage(pressReadings, NUM_READINGS);

  // Get battery percentage
  int batteryPercentage = batt.level();

  // Display sensor data on OLED
  displayTimeDateWeather(now, avgTemp, avgHum, avgPress, batteryPercentage);

  // Check Wi-Fi status and send data to Blynk if connected
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
    Blynk.virtualWrite(V0, avgTemp);         // Send temperature to Virtual Pin V0
    Blynk.virtualWrite(V1, avgHum);          // Send humidity to Virtual Pin V1
    Blynk.virtualWrite(V2, avgPress);        // Send pressure to Virtual Pin V2
    Blynk.virtualWrite(V3, batteryPercentage); // Send battery percentage to Virtual Pin V3
  }
  // Delay to prevent excessive updates
  delay(5000); // Update every 60 seconds
}

void LoadingScreen() {
  display.clearDisplay();
  // Draw the cloud icon
  display.fillCircle(35, 20, 10, SSD1306_WHITE); // Left part of the cloud
  display.fillCircle(50, 10, 10, SSD1306_WHITE); // Top part of the cloud
  display.fillCircle(65, 20, 10, SSD1306_WHITE); // Right part of the cloud
  display.fillRect(35, 20, 30, 10, SSD1306_WHITE); // Bottom rectangle of the cloud

  // Draw the "Temp Tracker" text in medium size
  display.setTextSize(1.5); // Medium text size
  display.setTextColor(SSD1306_WHITE); // White text
  int16_t x1, y1;
  uint16_t width, height;

  // Center "Temp Tracker" text
  display.getTextBounds("Temp Tracker", 0, 0, &x1, &y1, &width, &height);
  int textX = (SCREEN_WIDTH - width) / 2; // Center horizontally
  display.setCursor(textX, 35); // Position below the cloud icon
  display.println("Temp Tracker");

  // Display the cloud and "Temp Tracker" text first
  display.display();

  // Animate "Loading..." with dots
  for (int i = 0; i < 3; i++) {
    display.setTextSize(1); // Small text size for "Loading..."
    display.getTextBounds("Loading...", 0, 0, &x1, &y1, &width, &height);
    int loadingX = (SCREEN_WIDTH - width) / 2; // Center horizontally
    display.setCursor(loadingX, 55); // Position below "Temp Tracker"
    display.print("Loading");

    // Add dots based on the current iteration
    for (int j = 0; j <= i; j++) {
      display.print(".");
    }

    // Update the display for the current state
    display.display();
    delay(1000); // 1-second delay

    // Clear only the "Loading..." area for the next frame
    display.fillRect(loadingX, 55, SCREEN_WIDTH - loadingX, 10, SSD1306_BLACK);
  }

  // Clear the screen after the animation
  display.clearDisplay();
  display.display();
}


void sensorTest() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Display Test
  display.setCursor(0, 0);
  Serial.print("Display: ");
  display.println("Display: OK");
  Serial.println("OK");
  display.display();
  delay(1000);

  // RTC Test
  display.setCursor(0, 12);
  Serial.print("RTC: ");
  if (rtc.begin()) {
    display.println("RTC: OK");
    Serial.println("OK");
  } else {
    display.println("RTC: Not Found");
    Serial.println("Not Found");
  }
  display.display();
  delay(1000);

  // DHT Test
  display.setCursor(0, 24);
  Serial.print("DHT: ");
  float tempTest = dht.readHumidity(); // Try to read temperature
  if (!isnan(tempTest)) {
    display.println("DHT: OK");
    Serial.println("OK");
  } else {
    display.println("DHT: Not Found");
    Serial.println("Not Found");
  }
  display.display();
  delay(1000);
  // BMP280 Test
  display.setCursor(0, 36);
  Serial.print("BMP280: ");
  if (bmp.begin(0x76)) {
    display.println("BMP280: OK");
    Serial.println("OK");
  } else {
    display.println("BMP280: Not Found");
    Serial.println("Not Found");
  }

  display.display();
  delay(1000);


  // Check for Battery
  display.setCursor(0, 48);
  Serial.print("Battery: ");
  if (batt.level()>0) {
    display.println("Battery: OK");
    Serial.println("OK");
  } else {
    display.println("Battery: N/A");
    Serial.println("Not Found");
  }

  display.display();
  delay(1000); // Show sensor test results for 2 seconds
}


void displayTimeDateWeather(DateTime now, float temperature, float humidity,float pressure, int batteryPercentage) {
  display.clearDisplay();
  // Display time in top left corner with white background
  display.fillRect(0, 0, 128, 12, SSD1306_WHITE); // White background for time
  display.setTextSize(1); // Small text
  display.setTextColor(SSD1306_BLACK);  // Text color black on white background
  display.setCursor(2, 2); // Position at top-left with some padding

  // 12-hour format with AM/PM
  int hour = now.hour();
  String ampm = "AM";

  if (hour >= 12) {
    ampm = "PM";
    if (hour > 12) {
      hour -= 12;  // Convert to 12-hour format
    }
  } else if (hour == 0) {
    hour = 12; // Handle midnight
  }

  if (hour < 10) display.print("0");
  display.print(hour);
  display.print(":");
  if (now.minute() < 10) display.print("0");
  display.print(now.minute());
  display.print(ampm);

  // Display date
  display.setCursor(50, 2);
  display.setTextSize(1);
  display.print(now.day());
  display.print("/");
  display.print(now.month());
  display.print("/");
  // Display only the last two digits of the year
  int yearLastTwoDigits = now.year() % 100;  // Get the last two digits of the year
  display.print(yearLastTwoDigits);

  // Display battery percentage in top-right corner
  display.setCursor(SCREEN_WIDTH - 24, 2);
  display.setTextSize(1);
  display.setTextColor(SSD1306_BLACK);
  display.print(batteryPercentage);
  display.print("%");


  // Display temperature and humidity
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 15);
  if (!isnan(temperature)) {
    display.print("Temp: ");
    display.print(temperature);
    display.print("C");
  } else {
    display.print("Temp: Error");
  }

  display.setCursor(5, 25);
  if (!isnan(humidity)) {
    display.print("Hum: ");
    display.print(humidity);
    display.print("%");
  } else {
    display.print("Hum: Error");
  }

  // Display atmospheric pressure
  display.setCursor(5, 35); // Position below humidity
  if (!isnan(pressure)) {
    display.print("Press: ");
    display.print(pressure);
    display.print(" hPa");
  } else {
    display.print("Press: Error");
  }
  display.setCursor(5, 45); // Position below Pressure
    if (WiFi.status() != WL_CONNECTED) {
    display.print("WIFI: ");
    display.print("Not Connected");
  } else {
    display.print("WIFI: Connected");
  }
  display.display();
}

// Function to calculate the average of an array
  float calculateAverage(float arr[], int len) {
    float sum = 0;
    for (int i = 0; i < len; i++) {
      sum += arr[i];
    }
    return sum / len;
  }


void connectWifi() {
  // Clear the display for connection status
  display.clearDisplay();

  // Display "Connecting WiFi..." text
  display.setTextSize(1); // Small text size
  display.setTextColor(SSD1306_WHITE); // White text
  int16_t x1, y1;
  uint16_t width, height;

  // Center "Connecting WiFi..." text
  display.getTextBounds("Connecting WiFi...", 0, 0, &x1, &y1, &width, &height);
  int textX = (SCREEN_WIDTH - width) / 2; // Center horizontally
  display.setCursor(textX, SCREEN_HEIGHT / 2 - 8); // Center vertically
  display.println("Connecting WiFi...");
  display.display();

  // Attempt to connect to Wi-Fi
  WiFi.begin(ssid, password);
  int attempt = 0; // Keep track of connection attempts

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // Wait 1 second between attempts

    // Update "Connecting WiFi..." text with dots to indicate progress
    display.fillRect(textX, SCREEN_HEIGHT / 2 + 5, SCREEN_WIDTH, 10, SSD1306_BLACK); // Clear dots area
    display.setCursor(textX, SCREEN_HEIGHT / 2 + 5);
    display.print("Attempt ");
    display.print(++attempt);
    display.display();

    // If attempts exceed a threshold, handle failure gracefully
    if (attempt >= 15) { // e.g., give up after 15 attempts
      display.fillRect(0, SCREEN_HEIGHT / 2 + 20, SCREEN_WIDTH, 20, SSD1306_BLACK); // Clear message area
      display.setCursor(textX, SCREEN_HEIGHT / 2 + 20);
      display.print("WiFi Failed");
      display.display();
      return;
    }
  }

  // Once connected, update display to show success
  display.clearDisplay();
  display.setCursor((SCREEN_WIDTH - width) / 2, SCREEN_HEIGHT / 2 - 8);
  display.println("WiFi Connected!");
  display.display();
  delay(2000); // Show success message for 2 seconds
  display.clearDisplay(); // Clear display for the next screen
}
