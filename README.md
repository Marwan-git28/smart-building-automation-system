# 🏢 Smart Building Automation System

Industrial Building Automation System using ESP32, Modbus RTU Master-Slave, BACnet/IP, Python BAC0, Flask REST API, MQTT, and Real-Time Web Dashboard.

This project demonstrates two Industrial IoT communication protocols:

- Modbus RTU Master-Slave for industrial device communication over RS485.
- BACnet/IP for Building Management System (BMS) monitoring.

The collected data is processed by a Python backend, exposed through a Flask REST API, and displayed on a responsive real-time web dashboard.

---

## 🚀 Features

### Modbus RTU

- ESP32 Modbus RTU Slave
- ESP32 Modbus RTU Master
- RS485 Communication
- MQTT (EMQX Cloud)
- Python MQTT Backend
- Flask REST API
- HTML Web Dashboard

### BACnet/IP

- ESP32 BACnet/IP Device
- Python BAC0 Backend
- Flask REST API
- HTML Web Dashboard
- Remote Access using Ngrok

---

## 🏗️ System Architecture

![BACnet/IP Architecture](images/architecture/BACnetIP.drawio.png)

![Modbus RTU Architecture](images/architecture/ModbusRTU.drawio.png)

---

## 🛠️ Hardware

![BACnet/IP Hardware](images/hardware/hardware_BACnetIP.jpg)

![Modbus RTU Hardware](images/hardware/hardware_modbus_slavemaster.jpg)


- ESP32 x2 (Modbus RTU Master & Slave)
- ESP32 x1 (BACnet/IP)
- RS485 Module x2
- 3 Potentiometers
- 3 LEDs
- Buzzer

---

## 💻 Software

- Arduino IDE
- Python
- BAC0
- Paho MQTT
- Flask
- HTML
- CSS
- JavaScript
- EMQX Cloud

----

### BACnet/IP Arduino Firmware

![BACnet/IP Arduino](images/arduino/bacnet_arduinoIDE_pythonVcode.png)

### Modbus RTU Master-Slave Arduino Firmware

![Modbus RTU Arduino](images/arduino/modbus_master_slave_arduino.png)

### BACnet/IP Python Backend

![BACnet/IP Python](images/python/python_bacnet.png)

### Modbus RTU Python Backend

![Modbus RTU Python](images/python/python_modbus_rtu.png)

## 📊 Web Dashboard

### Smart Building Dashboard (BACnet/IP)

![Smart Building Dashboard](images/dashboard/smart_building_dashboard.png)

### Modbus RTU Dashboard

![Modbus RTU Dashboard](images/dashboard/modbus_Dashboard.png)


## 🎥 Demo Video

### Modbus RTU Demo

https://drive.google.com/....

### BACnet/IP Demo

https://drive.google.com/....


## 📁 Source Code

Arduino/
Python/
Flask/
HTML/
CSS/
JavaScript/


## 👨‍💻 Author

Marwan Saputra

Junior Industrial IoT & Embedded Systems Engineer

GitHub:
https://github.com/Marwan-git28

LinkedIn:
https://www.linkedin.com/in/marwan-saputra-972242415/
