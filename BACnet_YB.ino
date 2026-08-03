#include <WiFi.h>
#include <WiFiUdp.h>
#include <BACnetLight.h>

const char* ssid = "TES";
const char* password = "12345678";

BACnetLight bacnet;
WiFiUDP bacnetUdp;

// Definisi Pin Sensor / Potensio
const int pinTemp  = 34;
const int pinHum   = 35;
const int pinSmoke = 32;

// Definisi Pin Aktuator (LED & Buzzer)
const int ledPin1   = 25; // Temp Indicator
const int ledPin2   = 26; // Humidity Indicator
const int ledPin3   = 27; // Smoke Indicator
const int buzzerPin = 23; // Central Alert Buzzer

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("\nStarting ESP32 BACnet Automation & Control...");

    // Setup pin aktuator sebagai OUTPUT
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(buzzerPin, OUTPUT);

    // Matikan semua di awal
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(buzzerPin, LOW);

    // Koneksi Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Inisialisasi BACnet
    IPAddress broadcastIP(172, 25, 234, 255); 
    
    if (!bacnet.begin(9999, "ESP32_Smart_Room", broadcastIP, bacnetUdp)) {
        Serial.println("ERROR: BACnet init failed!");
        while (1) delay(1000);
    }

    // Daftarkan 3 Analog Value untuk dikirim ke Python
    bacnet.addAnalogValue(0, "Room_Temperature", 0.0, BACNET_UNITS_DEGREES_CELSIUS);
    bacnet.addAnalogValue(1, "Room_Humidity", 0.0, BACNET_UNITS_PERCENT);
    bacnet.addAnalogValue(2, "Smoke_Level", 0.0, BACNET_UNITS_NO_UNITS);

    Serial.println("BACnet ready & controlling actuators!");
}

void loop() {
    bacnet.loop();

    static unsigned long last = 0;
    if (millis() - last >= 1000) {
        last = millis();

        // 1. Baca nilai sensor dari potensio
        float tempVal  = (analogRead(pinTemp) / 4095.0) * 50.0;    // Skala 0 - 50 °C
        float humVal   = (analogRead(pinHum) / 4095.0) * 100.0;    // Skala 0 - 100 %
        float smokeVal = (analogRead(pinSmoke) / 4095.0) * 1000.0; // Skala 0 - 1000 ppm

        // Kirim data ke BACnet
        bacnet.setValue(BACNET_OBJ_ANALOG_VALUE, 0, tempVal);
        bacnet.setValue(BACNET_OBJ_ANALOG_VALUE, 1, humVal);
        bacnet.setValue(BACNET_OBJ_ANALOG_VALUE, 2, smokeVal);

        // --- LOGIKA KONTROL LED & BUZZER YANG BENAR ---
        bool alertTemp = (tempVal < 20.0 || tempVal > 35.0);   // Abnormal jika di luar 20-35°C
        bool alertHum  = (humVal < 40.0 || humVal > 70.0);     // Abnormal jika di luar 40-70%
        bool alertSmoke = (smokeVal > 400.0);                  // Bahaya jika > 400 ppm

        // LED menyala jika kondisi abnormal/bahaya terpenuhi
        digitalWrite(ledPin1, alertTemp ? HIGH : LOW);
        digitalWrite(ledPin2, alertHum ? HIGH : LOW);
        digitalWrite(ledPin3, alertSmoke ? HIGH : LOW);

        // Buzzer menyala jika ADA SALAH SATU saja yang alert
        if (alertTemp || alertHum || alertSmoke) {
            digitalWrite(buzzerPin, HIGH);
        } else {
            digitalWrite(buzzerPin, LOW);
        }
    }
}