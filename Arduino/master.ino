#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ModbusRTU.h>

ModbusRTU mb;
HardwareSerial RS485(2);

#define RXD2 16
#define TXD2 17
#define DE_RE 4

//================ WIFI =================
const char* ssid = "TES";
const char* password = "12345678";

//================ MQTT =================
const char* mqtt_server = "bbb5b116.ala.asia-southeast1.emqxsl.com";
const int mqtt_port = 8883;          // Insecure MQTT
const char* mqtt_user = "esp32";
const char* mqtt_pass = "esp32@123";

WiFiClientSecure espClient;
PubSubClient client(espClient);

//================ DATA =================
uint16_t holdingReg[3] = {75, 55, 5};

//=======================================
void setup_wifi() {

  Serial.print("Connecting WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
}

//=======================================
void reconnect() {

  while (!client.connected()) {

    Serial.print("Connecting MQTT...");

    if (client.connect("ESP32_MODBUS", mqtt_user, mqtt_pass)) {

      Serial.println("Connected");

    } else {

      Serial.print("Failed : ");
      Serial.println(client.state());

      delay(2000);
    }
  }
}

//=======================================
void setup() {


espClient.setInsecure();

client.setServer(mqtt_server, mqtt_port);

  Serial.begin(115200);

  RS485.begin(9600, SERIAL_8N1, RXD2, TXD2);

  pinMode(DE_RE, OUTPUT);
  digitalWrite(DE_RE, LOW);

  mb.begin(&RS485, DE_RE);
  mb.slave(1);

  mb.addHreg(0, holdingReg[0]);
  mb.addHreg(1, holdingReg[1]);
  mb.addHreg(2, holdingReg[2]);

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);

  Serial.println("======================");
  Serial.println("MODBUS + MQTT READY");
  Serial.println("======================");
}

//=======================================
void loop() {

  mb.task();

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  mb.Hreg(0, holdingReg[0]);
  mb.Hreg(1, holdingReg[1]);
  mb.Hreg(2, holdingReg[2]);

  static unsigned long last = 0;

  if (millis() - last > 1000) {

    last = millis();

    String payload =
      "{\"Tank Level\":" + String(holdingReg[0]) +
      ",\"Flow Rate\":" + String(holdingReg[1]) +
      ",\"Pressure\":" + String(holdingReg[2]) +
      "}";

    client.publish("industrial/modbus", payload.c_str());

    Serial.println("----------------");
    Serial.print("Tank Level : ");
    Serial.println(holdingReg[0]);

    Serial.print("Flow Rate : ");
    Serial.println(holdingReg[1]);

    Serial.print("Pressure : ");
    Serial.println(holdingReg[2]);

    Serial.print("MQTT Payload : ");
    Serial.println(payload);
  }
}