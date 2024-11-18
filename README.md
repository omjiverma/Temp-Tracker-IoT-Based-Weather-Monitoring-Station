# Temp-Tracker-IoT-Based-Weather-Monitoring-Station
This repository contains the complete documentation, source code, and setup instructions for Temp Tracker, an IoT-based weather monitoring station developed as a minor project for my Master's degree in Embedded Systems.

## Objective  

Create a **smart weather monitoring system** using **NodeMCU (ESP8266)**, equipped with sensors for:  
- **Temperature**  
- **Humidity**  
- **Atmospheric Pressure**  
- **Battery Monitoring**  

The system will:  
1. Display real-time data on an **OLED screen**.  
2. Send the collected data to a **Blynk dashboard** for remote monitoring.  

## Features

- **Real-time Weather Monitoring**:
  - Temperature (from DHT11)
  - Humidity (from DHT11)
  - Atmospheric Pressure (from BMP280)
  
- **Live Data Display**:
  - Data displayed on a 0.94-inch OLED screen.
  - Current time with AM/PM format (using RTC DS3231).
  - Wi-Fi connectivity status.
  - Battery percentage.

- **Blynk Integration**:
  - Sends live weather data (temperature, humidity, pressure, battery) to the Blynk app for remote monitoring.

- **Sensor Testing**:
  - Automated sensor checks (DHT11, BMP280, RTC, and Battery).

## Hardware Requirements

- **NodeMCU (ESP8266)**
- **DHT11 Temperature and Humidity Sensor**
- **BMP280 Barometric Pressure Sensor**
- **RTC DS3231 Real-Time Clock Module**
- **0.94-inch OLED Display (SSD1306)**
- **Li-Ion Battery**
- **TP4056 Module**
- **Resistors - 10k, 4.7k**


## Software Requirements

- Arduino IDE

## Setup Instructions

### 1. Install Libraries
Before you can run the code, you need to install the required libraries in the Arduino IDE. Follow these steps:

1. Open Arduino IDE.
2. Go to **Sketch > Include Library > Manage Libraries...**
3. Search for the following libraries and install them:
   - **Blynk**
   - **ESP8266WIFI**
   - **Adafruit SSD1306**
   - **Adafruit GFX**
   - **DHT sensor library**
   - **Adafruit BMP280**
   - **RTClib**
   - **Battery Sense**

### 2. Hardware Connections

- **DHT11**:
  - Connect **VCC** to **3V3** on NodeMCU.
  - Connect **GND** to **GND**.
  - Connect **Data** to **D5 (GPIO14)** on NodeMCU.

- **BMP280**:
  - Connect **VCC** to **3V3** on NodeMCU.
  - Connect **GND** to **GND**.
  - Connect **SCL** to **D1 (GPIO5)** on NodeMCU.
  - Connect **SDA** to **D2 (GPIO4)** on NodeMCU.

- **RTC DS3231**:
  - Connect **VCC** to **3V3** on NodeMCU.
  - Connect **GND** to **GND**.
  - Connect **SCL** to **D1 (GPIO5)**.
  - Connect **SDA** to **D2 (GPIO4)**.

- **OLED Display (SSD1306)**:
  - Connect **VCC** to **3V3** on NodeMCU.
  - Connect **GND** to **GND**.
  - Connect **SCL** to **D1 (GPIO5)**.
  - Connect **SDA** to **D2 (GPIO4)**.

- **Battery Sensor**:
  - Connect the Voltage divider as shown below to **A0** on NodeMCU for monitoring the battery.
 
    ![image](https://github.com/user-attachments/assets/2b396435-270f-4f8d-aa90-b5ea23f769f1)


### 3. Blynk Web Setup

1. Go to the [Blynk Web Dashboard](https://blynk.cloud/) and log in with your Blynk account.  
2. Create a new project or workspace on the dashboard.  

#### Creating the Device:
3. Add a new device and select **ESP8266** as the hardware type with **Wi-Fi** as the connection method.  
4. After creating the device, Blynk will generate an **Auth Token**,**BLYNK_TEMPLATE_ID**, **BLYNK_TEMPLATE_NAME** for the device. Copy this token.  
5. Replace the `BLYNK_AUTH_TOKEN` in your code with the copied token.  

#### Creating Data Streams:
6. Navigate to the **Data Streams** section and create new streams for the data you want to monitor (e.g., temperature, humidity).  
   - Choose the appropriate stream type (e.g., virtual pins, analog, or digital).  
   - Assign a name, virtual pin, and any required parameters to the stream.  

#### Designing the Dashboard:
7. Go to the **Web Dashboard** tab and start adding widgets for the data streams you created (e.g., gauges, charts, labels).  
   - Configure each widget to link to the corresponding data stream.  
   - Adjust settings like range, units, and update intervals as needed.  
8. Save your dashboard, and it will now display real-time data when your device is online and sending data.  

### 4. Arduino Code Upload

1. Open the **Temp-Tracker.ino** file in Arduino IDE.
2. Replace the following fields in the code:
   - **BLYNK_AUTH_TOKEN**: Paste your Blynk Auth Token here.
   - **BLYNK_TEMPLATE_ID**: Paste your Blynk Template ID here.
   - **BLYNK_TEMPLATE_NAME**: Paste your Blynk Template Name here.
   - **Wi-Fi SSID and Password**: Enter your Wi-Fi credentials (`ssid`, `password`).
3. Connect the NodeMCU to your computer via USB.
4. Select the correct **Board** and **Port** in the **Tools** menu in Arduino IDE.
5. Click **Upload** to upload the code to the NodeMCU.

### 5. Monitor Data

Once the code is uploaded, the NodeMCU will:
- Connect to your Wi-Fi network.
- Start displaying the weather data on the OLED screen.
- Send data to the Blynk app for real-time monitoring.

## Code Explanation

- **Setup**:
  - Initializes the OLED display, RTC, sensors (DHT11, BMP280), and BatterySense.
  - Connects to Wi-Fi and the Blynk server.
  - Runs the `sensorTest()` function to verify that all sensors are working.
  
- **Loop**:
  - Reads sensor data (temperature, humidity, pressure).
  - Calculates the average of the last 5 readings for each sensor.
  - Displays current time, temperature, humidity, pressure, and battery percentage on the OLED.
  - Sends the data to the Blynk app for remote monitoring.
  
- **Loading Screen**:
  - A simple animation that displays a "loading" message during startup.

- **Sensor Test**:
  - Checks the functionality of each sensor and displays the result on the OLED.
## Final Device
![imaage](https://github.com/omjiverma/Temp-Tracker-IoT-Based-Weather-Monitoring-Station/blob/main/final%20Device.png)

## Troubleshooting

- **Wi-Fi Connection Issue**: If the device fails to connect to Wi-Fi, ensure that your credentials are correct, and the signal strength is strong enough.
- **Sensor Issues**: If a sensor isn't responding, check its wiring and ensure it's properly connected.

## Contributing

Feel free to fork the repository and submit pull requests for bug fixes or improvements. Please ensure that your contributions follow the existing code style.


