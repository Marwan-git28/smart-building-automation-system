#include <ModbusRTU.h>

ModbusRTU mb;

HardwareSerial RS485(2);

#define RXD2 16
#define TXD2 17
#define DE_RE 4

#define SLAVE_ID 1

uint16_t holdingReg[3] = {75, 55, 5};

void setup() {

  Serial.begin(115200);

  RS485.begin(9600, SERIAL_8N1, RXD2, TXD2);

  pinMode(DE_RE, OUTPUT);
  digitalWrite(DE_RE, LOW);

  mb.begin(&RS485, DE_RE);

  mb.slave(SLAVE_ID);

  mb.addHreg(0, holdingReg[0]);
  mb.addHreg(1, holdingReg[1]);
  mb.addHreg(2, holdingReg[2]);

  Serial.println("====================");
  Serial.println("MODBUS SLAVE READY");
  Serial.println("====================");
}

void loop() {

  mb.task();

  mb.Hreg(0, holdingReg[0]);
  mb.Hreg(1, holdingReg[1]);
  mb.Hreg(2, holdingReg[2]);

  static unsigned long lastPrint = 0;

  if (millis() - lastPrint >= 1000) {

    lastPrint = millis();

    Serial.println("----------------");

    Serial.print("Tank Level : ");
    Serial.println(mb.Hreg(0));

    Serial.print("Flow Rate : ");
    Serial.println(mb.Hreg(1));

    Serial.print("Pressure : ");
    Serial.println(mb.Hreg(2));
  }
}